# import numpy as np
import chess
import pandas as pd
import tqdm
import argparse
import torch
import os

# Feature dimensions
SQUARE_NUM = 64
PIECE_NUM = 10  # All piece types except kings (5 types * 2 colors)
KING_SQUARE_NUM = 64
MATE_SCORE = 10000

# Add auxiliary feature counts
CASTLING_FEATURES = 4  # KQkq
EN_PASSANT_FEATURES = 8  # One per file
SIDE_TO_MOVE_FEATURE = 1
FIFTY_MOVE_FEATURE = 1

# Base HalfKP feature count
HALFKP_FEATURE_COUNT = KING_SQUARE_NUM * PIECE_NUM * SQUARE_NUM
# Auxiliary feature count
AUX_FEATURE_COUNT = CASTLING_FEATURES + EN_PASSANT_FEATURES + SIDE_TO_MOVE_FEATURE + FIFTY_MOVE_FEATURE
# Total feature count
TOTAL_FEATURE_COUNT = HALFKP_FEATURE_COUNT + AUX_FEATURE_COUNT

# [Previous imports and constants remain unchanged]

def get_features(board):
    white_features = []
    black_features = []
    
    # Find king positions
    white_king_sq = board.king(chess.WHITE)
    black_king_sq = board.king(chess.BLACK)
    
    if white_king_sq is None or black_king_sq is None:
        # Invalid position without kings
        return [], []
    
    # Map pieces to indices (0-9)
    piece_idx_map = {
        chess.PAWN: 0,
        chess.KNIGHT: 1,
        chess.BISHOP: 2,
        chess.ROOK: 3,
        chess.QUEEN: 4
    }
    
    # Process each square for HalfKP features
    for sq in range(64):
        piece = board.piece_at(sq)
        if piece is None or piece.piece_type == chess.KING:
            continue
        
        piece_type = piece.piece_type
        color = piece.color
        
        # Base piece index
        if color == chess.WHITE:
            piece_idx = piece_idx_map[piece_type]
        else:
            piece_idx = piece_idx_map[piece_type] + 5
        
        # White perspective HalfKP feature
        white_feature_idx = white_king_sq * PIECE_NUM * SQUARE_NUM + piece_idx * SQUARE_NUM + sq
        white_features.append(white_feature_idx)
        
        # Black perspective HalfKP feature (mirrored)
        mirror_sq = (7 - sq // 8) * 8 + (sq % 8)
        mirror_king_sq = (7 - black_king_sq // 8) * 8 + (black_king_sq % 8)
        if color == chess.WHITE:
            mirror_piece_idx = piece_idx_map[piece_type] + 5  # Black piece
        else:
            mirror_piece_idx = piece_idx_map[piece_type]      # White piece
        
        black_feature_idx = mirror_king_sq * PIECE_NUM * SQUARE_NUM + mirror_piece_idx * SQUARE_NUM + mirror_sq
        black_features.append(black_feature_idx)
    
    # Add auxiliary features (same for both perspectives)
    aux_base_idx = HALFKP_FEATURE_COUNT
    
    # 1. Castling rights (4 features: KQkq)
    if board.has_kingside_castling_rights(chess.WHITE):
        white_features.append(aux_base_idx + 0)
        black_features.append(aux_base_idx + 0)
    if board.has_queenside_castling_rights(chess.WHITE):
        white_features.append(aux_base_idx + 1)
        black_features.append(aux_base_idx + 1)
    if board.has_kingside_castling_rights(chess.BLACK):
        white_features.append(aux_base_idx + 2)
        black_features.append(aux_base_idx + 2)
    if board.has_queenside_castling_rights(chess.BLACK):
        white_features.append(aux_base_idx + 3)
        black_features.append(aux_base_idx + 3)
    
    # 2. En passant targets (8 features, one per file)
    ep_base_idx = aux_base_idx + CASTLING_FEATURES
    if board.ep_square is not None:
        file_idx = board.ep_square % 8
        white_features.append(ep_base_idx + file_idx)
        black_features.append(ep_base_idx + file_idx)
    
    # 3. Side to move (1 feature)
    stm_idx = ep_base_idx + EN_PASSANT_FEATURES
    if board.turn == chess.WHITE:
        white_features.append(stm_idx)
        black_features.append(stm_idx)
    
    # 4. Fifty-move rule counter (1 feature)
    fifty_idx = stm_idx + SIDE_TO_MOVE_FEATURE
    if board.halfmove_clock >= 40:
        white_features.append(fifty_idx)
        black_features.append(fifty_idx)
    
    return white_features, black_features

def write_chunk_features_torch(dir_name, chunk_idx, fens, white_features, black_features, scores):
    """
    Save extracted features to torch sparse tensors
    
    Args:
        dir_name: Directory to save the tensors
        chunk_idx: Current chunk index
        fens: List of FEN strings for positions
        white_features: List of feature indices from white's perspective (incl. auxiliary)
        black_features: List of feature indices from black's perspective (incl. auxiliary)
        scores: List of evaluation scores
    """
    os.makedirs(dir_name, exist_ok=True)
    
    num_rows = len(fens)
    assert num_rows == len(scores)
    
    # Prepare scores tensor
    chunk_scores = torch.tensor(scores, dtype=torch.float32)

    # Create white perspective sparse tensors
    white_indices = []
    white_values = []
    for i in range(num_rows):
        for feature_idx in white_features[i]:
            white_indices.append([i, feature_idx])
            white_values.append(1)
    
    if white_indices:
        white_indices_tensor = torch.tensor(white_indices, dtype=torch.int).t()
        white_values_tensor = torch.tensor(white_values, dtype=torch.bool)
        white_sparse = torch.sparse_coo_tensor(
            white_indices_tensor, 
            white_values_tensor,
            size=(num_rows, TOTAL_FEATURE_COUNT)
        )
    else:
        white_sparse = torch.sparse_coo_tensor(
            torch.zeros((2, 0), dtype=torch.int),
            torch.zeros(0, dtype=torch.bool),
            size=(num_rows, TOTAL_FEATURE_COUNT)
        )
    
    # Create black perspective sparse tensors
    black_indices = []
    black_values = []
    for i in range(num_rows):
        for feature_idx in black_features[i]:
            black_indices.append([i, feature_idx])
            black_values.append(1)
    
    if black_indices:
        black_indices_tensor = torch.tensor(black_indices, dtype=torch.int).t()
        black_values_tensor = torch.tensor(black_values, dtype=torch.bool)
        black_sparse = torch.sparse_coo_tensor(
            black_indices_tensor, 
            black_values_tensor,
            size=(num_rows, TOTAL_FEATURE_COUNT)
        )
    else:
        black_sparse = torch.sparse_coo_tensor(
            torch.zeros((2, 0), dtype=torch.int),
            torch.zeros(0, dtype=torch.bool),
            size=(num_rows, TOTAL_FEATURE_COUNT)
        )
    
    # Save this chunk
    chunk_file = f"{dir_name}/chunk_{chunk_idx + 1}.pt"
    torch.save({
        'white_features': white_sparse,
        'black_features': black_sparse,
        'scores': chunk_scores
    }, chunk_file)
    
    print(f"Saved chunk {chunk_idx+1} to {chunk_file}")

def process_kaggle_dataset(input_file, output_dir, max_positions=None, chunk_size=100000):
    """
    Process a dataset of chess positions and extract NNUE features
    
    Args:
        input_file: CSV file with FEN and Evaluation columns
        output_dir: Directory to save the processed features
        max_positions: Maximum number of positions to process
        chunk_size: Number of positions per chunk
    """
    with open(input_file, 'r') as f:
        print(f"Reading dataset from {input_file}")
    
    total_processed = 0
    for chunk_idx, chunk in enumerate(pd.read_csv(input_file, chunksize=chunk_size)):
        if max_positions and total_processed >= max_positions:
            break

        print(f"Processing chunk {chunk_idx + 1}")

        required_columns = ['FEN', 'Evaluation']
        for col in required_columns:
            if col not in chunk.columns:
                raise ValueError(f"Dataset missing required column: {col}")
        
        fens = []
        white_features_list = []
        black_features_list = []
        scores = []
        
        for row_idx, row in tqdm.tqdm(chunk.iterrows(), total=len(chunk)):
            if max_positions and total_processed >= max_positions:
                break
            fen = row['FEN']
            eval_str = row['Evaluation']
            if eval_str[0] == '#':
                score = (MATE_SCORE if eval_str[1] == '+' else -MATE_SCORE) / 100.0
            else:
                score = float(eval_str) / 100.0
            
            try:
                board = chess.Board(fen)
                white_features, black_features = get_features(board)
                
                # invalid position (this no longer works now that we append auxiliary features)
                if len(white_features) == 0 or len(black_features) == 0:
                    continue
                    
                fens.append(fen)
                white_features_list.append(white_features)
                black_features_list.append(black_features)
                scores.append(score)
                
            except Exception as e:
                print(f"Error processing position {row_idx} (FEN: {fen}): {str(e)}")

            total_processed += 1
        
        print(f"Successfully processed chunk {chunk_idx + 1} with {len(fens)} positions")
        write_chunk_features_torch(output_dir, chunk_idx, fens, white_features_list, black_features_list, scores)
    
    metadata = {
        'total_positions': total_processed,
        'feature_dimensions': {
            'halfkp': HALFKP_FEATURE_COUNT,
            'castling': CASTLING_FEATURES,
            'en_passant': EN_PASSANT_FEATURES,
            'side_to_move': SIDE_TO_MOVE_FEATURE,
            'fifty_move': FIFTY_MOVE_FEATURE,
            'total': TOTAL_FEATURE_COUNT
        }
    }
    
    metadata_file = f"{output_dir}/metadata.pt"
    torch.save(metadata, metadata_file)
    print(f"Saved metadata to {metadata_file}")
    print(f"Total positions processed: {total_processed}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Process chess positions and convert to NNUE features with auxiliary data')
    parser.add_argument('--input', type=str, required=True, help='Input CSV file (dataset)')
    parser.add_argument('--output_dir', type=str, required=True, help='Output directory for features')
    parser.add_argument('--max_positions', type=int, help='Maximum number of positions to process')
    parser.add_argument('--chunk_size', type=int, default=100000, help='Positions per chunk')
    
    args = parser.parse_args()
    process_kaggle_dataset(args.input, args.output_dir, args.max_positions, args.chunk_size)