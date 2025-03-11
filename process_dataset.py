import os
import numpy as np
import chess
import pandas as pd
import struct
import gzip
import tqdm
import argparse
from typing import List, Tuple
import torch

SQUARE_NUM = 64
PIECE_NUM = 10  # All piece types except kings (5 types * 2 colors)
KING_SQUARE_NUM = 64
MATE_SCORE = 10000

# Feature index calculation constants
FEATURE_DIMENSIONS = (KING_SQUARE_NUM, PIECE_NUM, SQUARE_NUM)

def get_halfkp_indices(board):
    white_features = []
    black_features = []
    
    # Find king positions
    white_king_sq = board.king(chess.WHITE)
    black_king_sq = board.king(chess.BLACK)
    
    if white_king_sq is None or black_king_sq is None:
        # Invalid position without kings
        return [], []
    
    # Map pieces to indices (0-9)
    # Order: pawn, knight, bishop, rook, queen for each color
    piece_idx_map = {
        chess.PAWN: 0,
        chess.KNIGHT: 1,
        chess.BISHOP: 2,
        chess.ROOK: 3,
        chess.QUEEN: 4
    }
    
    # Process each square on the board
    for sq in range(64):
        piece = board.piece_at(sq)
        if piece is None or piece.piece_type == chess.KING:
            continue  # Skip empty squares and kings
        
        piece_type = piece.piece_type
        color = piece.color
        
        # Get base piece index (0-4 for white pieces, 5-9 for black pieces)
        if color == chess.WHITE:
            piece_idx = piece_idx_map[piece_type]
        else:
            piece_idx = piece_idx_map[piece_type] + 5
        
        # White perspective feature
        white_feature_idx = white_king_sq * PIECE_NUM * SQUARE_NUM + piece_idx * SQUARE_NUM + sq
        white_features.append(white_feature_idx)
        
        # Black perspective feature
        # Mirror the square and king square (flip rank)
        mirror_sq = (7 - sq // 8) * 8 + (sq % 8)
        mirror_king_sq = (7 - black_king_sq // 8) * 8 + (black_king_sq % 8)
        
        # Mirror piece color (white pieces are black from black's perspective)
        if color == chess.WHITE:
            mirror_piece_idx = piece_idx_map[piece_type] + 5  # Make it a black piece
        else:
            mirror_piece_idx = piece_idx_map[piece_type]  # Make it a white piece
        
        black_feature_idx = mirror_king_sq * PIECE_NUM * SQUARE_NUM + mirror_piece_idx * SQUARE_NUM + mirror_sq
        black_features.append(black_feature_idx)
    
    return white_features, black_features

def write_chunk_features_torch(dir_name, chunk_idx, fens, white_features, black_features, scores):
    total_features = KING_SQUARE_NUM * PIECE_NUM * SQUARE_NUM
    num_rows = len(fens)
    
    assert(num_rows == len(scores))
    # Prepare data for this chunk
    chunk_scores = torch.tensor([scores[i] for i in range(len(scores))], dtype=torch.float32)

    # Create white perspective sparse tensors
    white_indices = []
    white_values = []
    for i in range(len(fens)):
        for feature_idx in white_features[i]:
            white_indices.append([i, feature_idx])
            white_values.append(1)

    print(min(white_indices))
    print(max(white_indices))
    
    if white_indices:
        white_indices_tensor = torch.tensor(white_indices, dtype=torch.int32).t()
        white_values_tensor = torch.tensor(white_values, dtype=torch.bool)
        white_sparse = torch.sparse_coo_tensor(
            white_indices_tensor, 
            white_values_tensor,
            size=(num_rows, total_features)
        )
    else:
        # Empty tensor if no features
        white_sparse = torch.sparse_coo_tensor(
            torch.zeros((2, 0), dtype=torch.int32),
            torch.zeros(0, dtype=torch.bool),
            size=(num_rows, total_features)
        )
    
    # Create black perspective sparse tensors
    black_indices = []
    black_values = []
    for i in range(num_rows):
        for feature_idx in black_features[i]:
            black_indices.append([i, feature_idx])
            black_values.append(1)
    
    if black_indices:
        black_indices_tensor = torch.tensor(black_indices, dtype=torch.int32).t()
        black_values_tensor = torch.tensor(black_values, dtype=torch.bool)
        black_sparse = torch.sparse_coo_tensor(
            black_indices_tensor, 
            black_values_tensor,
            size=(num_rows, total_features)
        )
    else:
        # Empty tensor if no features
        black_sparse = torch.sparse_coo_tensor(
            torch.zeros((2, 0), dtype=torch.int32),
            torch.zeros(0, dtype=torch.bool),
            size=(num_rows, total_features)
        )
    
    # Save this chunk
    chunk_file = f"{dir_name}/chunk_{chunk_idx + 1}.pt.gz"
    with gzip.open(chunk_file, 'wb') as f:
        torch.save({
            'white_features': white_sparse,
            'black_features': black_sparse,
            'scores': chunk_scores
        }, f, _use_new_zipfile_serialization=True, pickle_protocol=4)


    # torch.save({
    #     'white_features': white_sparse,
    #     'black_features': black_sparse,
    #     'scores': chunk_scores
    # }, chunk_file, _use_new_zipfile_serialization=True, pickle_protocol=4)
    
    print(f"Saved chunk {chunk_idx+1}/{130}")
    

def process_kaggle_dataset(input_file, output_dir, max_positions = None, chunk_size = 1000000):
    if not os.path.exists(output_dir):
        os.mkdir(output_dir, exist_ok=True)
    
    with open(input_file, 'r') as f:
        print(f"Reading dataset from {input_file}")
    
        total_processed = 0
        for chunk_idx, chunk in enumerate(pd.read_csv(input_file, chunksize=chunk_size)):
            if max_positions and total_processed >= max_positions:
                break

            print(f"Processing chunk {chunk_idx + 1}/{130}")

            # Validate dataset format - should have 'FEN' and 'Evaluation' columns
            required_columns = ['FEN', 'Evaluation']
            for col in required_columns:
                if col not in chunk.columns:
                    raise ValueError(f"Dataset missing required column: {col}")
            
            fens = []
            white_features_list = []
            black_features_list = []
            scores = []
            
            # Process positions in dataset
            for row_idx, row in tqdm.tqdm(chunk.iterrows(), total=len(chunk)):
                if max_positions and total_processed >= max_positions:
                    break
                fen = row['FEN']
                # Convert centipawn evaluation to float value (pawn = 1.0)
                eval_str = row['Evaluation']
                if eval_str[0] == '#':
                    # Checkmate - convert to mate score
                    score = (MATE_SCORE if eval_str[1] == '+' else -MATE_SCORE) / 100.0
                else:
                    score = float(eval_str) / 100.0
                
                # Parse FEN
                try:
                    board = chess.Board(fen)
                    white_features, black_features = get_halfkp_indices(board)
                    
                    # Skip positions with no features (invalid positions)
                    if len(white_features) == 0 or len(black_features) == 0:
                        print("FOUND INVALID POSITION IN CHUNK")
                        continue
                        
                    fens.append(fen)
                    white_features_list.append(white_features)
                    black_features_list.append(black_features)
                    scores.append(score)
                    
                except Exception as e:
                    print(f"Error processing position {row_idx} (FEN: {fen}): {str(e)}")

                total_processed += 1
            
            
            # Save features as tensors
            write_chunk_features_torch(output_dir, chunk_idx, fens, white_features_list, black_features_list, scores)
            print(f"Successfully processed chunk {chunk_idx + 1} with {len(fens)} positions")
    
        # # Save metadata
        # metadata = {
        #     'total_positions': len(fens),
        #     'feature_dimensions': FEATURE_DIMENSIONS,
        #     'total_features': total_features
        # }
        
        # metadata_file = "metadata.pt"
        # torch.save(metadata, metadata_file)
        # print(f"Saved metadata to {metadata_file}")
    
        # # Print statz
        # feature_counts = [len(features) for features in white_features_list]
        # print(f"Average features per position: {np.mean(feature_counts):.2f}")
        # print(f"Min features: {min(feature_counts)}, Max features: {max(feature_counts)}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process chess positions and convert to NNUE features')

    parser.add_argument('--input', type=str, required=True, help='Input CSV file (dataset)')
    parser.add_argument('--output_dir', type=str, required=True, help='Output directory for features')
    parser.add_argument('--max_positions', type=int, help='Maximum number of positions to process')
    
    args = parser.parse_args()
    process_kaggle_dataset(args.input, args.output_dir, args.max_positions)
    # parser.print_help()

