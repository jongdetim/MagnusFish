import numpy as np
import chess
import pandas as pd
import struct
import tqdm
import argparse
from typing import List, Tuple

SQUARE_NUM = 64
PIECE_NUM = 10  # All piece types except kings (5 types * 2 colors)
KING_SQUARE_NUM = 64
MATE_SCORE = 10000

# Feature index calculation constants
FEATURE_DIMENSIONS = (KING_SQUARE_NUM, PIECE_NUM, SQUARE_NUM)

def get_halfkp_indices(board: chess.Board) -> Tuple[List[int], List[int]]:
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

def save_features_binary(output_file: str, 
                         fens: List[str], 
                         white_features: List[List[int]], 
                         black_features: List[List[int]], 
                         scores: List[float]):
    with open(output_file, 'wb') as f:
        # Write header: number of positions
        f.write(struct.pack('i', len(fens)))
        
        # Write each position
        for i in range(len(fens)):
            # Write FEN
            fen_bytes = fens[i].encode('utf-8')
            f.write(struct.pack('i', len(fen_bytes)))
            f.write(fen_bytes)
            
            # Write score
            f.write(struct.pack('f', scores[i]))
            
            # Write white features
            f.write(struct.pack('i', len(white_features[i])))
            f.write(struct.pack(f'{len(white_features[i])}i', *white_features[i]))
            
            # Write black features
            f.write(struct.pack('i', len(black_features[i])))
            f.write(struct.pack(f'{len(black_features[i])}i', *black_features[i]))

def read_features_binary(input_file: str) -> Tuple[List[str], List[List[int]], List[List[int]], List[float]]:
    fens = []
    white_features = []
    black_features = []
    scores = []
    
    with open(input_file, 'rb') as f:
        # Read number of positions
        num_positions = struct.unpack('i', f.read(4))[0]
        
        for _ in range(num_positions):
            # Read FEN
            fen_length = struct.unpack('i', f.read(4))[0]
            fen = f.read(fen_length).decode('utf-8')
            fens.append(fen)
            
            # Read score
            score = struct.unpack('f', f.read(4))[0]
            scores.append(score)
            
            # Read white features
            num_white_features = struct.unpack('i', f.read(4))[0]
            white_feature_data = f.read(num_white_features * 4)
            white_feature_list = list(struct.unpack(f'{num_white_features}i', white_feature_data))
            white_features.append(white_feature_list)
            
            # Read black features
            num_black_features = struct.unpack('i', f.read(4))[0]
            black_feature_data = f.read(num_black_features * 4)
            black_feature_list = list(struct.unpack(f'{num_black_features}i', black_feature_data))
            black_features.append(black_feature_list)
    
    return fens, white_features, black_features, scores

def process_kaggle_dataset(input_file: str, output_file: str, max_positions: int = None):
    print(f"Reading dataset from {input_file}")
    df = pd.read_csv(input_file)
    
    # Validate dataset format - should have 'FEN' and 'Evaluation' columns
    required_columns = ['FEN', 'Evaluation']
    for col in required_columns:
        if col not in df.columns:
            raise ValueError(f"Dataset missing required column: {col}")
    
    # maximum positions to process
    if max_positions is not None:
        df = df.head(max_positions)
    
    num_positions = len(df)
    print(f"Processing {num_positions} positions")
    
    fens = []
    white_features_list = []
    black_features_list = []
    scores = []
    
    # Process positions in dataset
    for idx, row in tqdm.tqdm(df.iterrows(), total=num_positions):
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
                continue
                
            fens.append(fen)
            white_features_list.append(white_features)
            black_features_list.append(black_features)
            scores.append(score)
            
        except Exception as e:
            print(f"Error processing position {idx} (FEN: {fen}): {str(e)}")
    
    print(f"Successfully processed {len(fens)} valid positions")
    
    # Save features to binary file
    print(f"Saving features to {output_file}")
    save_features_binary(output_file, fens, white_features_list, black_features_list, scores)
    print(f"Data saved successfully")
    
    # Print statz
    feature_counts = [len(features) for features in white_features_list]
    print(f"Average features per position: {np.mean(feature_counts):.2f}")
    print(f"Min features: {min(feature_counts)}, Max features: {max(feature_counts)}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process chess positions and convert to NNUE features')
    subparsers = parser.add_subparsers(dest='command')
    
    process_parser = subparsers.add_parser('process', help='Process dataset')
    process_parser.add_argument('--input', type=str, required=True, help='Input CSV file (dataset)')
    process_parser.add_argument('--output', type=str, required=True, help='Output binary file for features')
    process_parser.add_argument('--max_positions', type=int, help='Maximum number of positions to process')
    
    args = parser.parse_args()
    
    if args.command == 'process':
        process_kaggle_dataset(args.input, args.output, args.max_positions)
    else:
        parser.print_help()
