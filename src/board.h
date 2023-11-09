#pragma once

#include "stdafx.h"

typedef enum // Set the values of some of these where the values are not consecutive due to 0x88
{
    A1 = 0x00, B1, C1, D1, E1, F1, G1, H1,
    A2 = 0x10, B2, C2, D2, E2, F2, G2, H2,
    A3 = 0x20, B3, C3, D3, E3, F3, G3, H3,
    A4 = 0x30, B4, C4, D4, E4, F4, G4, H4,
    A5 = 0x40, B5, C5, D5, E5, F5, G5, H5,
    A6 = 0x50, B6, C6, D6, E6, F6, G6, H6,
    A7 = 0x60, B7, C7, D7, E7, F7, G7, H7,
    A8 = 0x70, B8, C8, D8, E8, F8, G8, H8,
} Square;

typedef enum
{
    pawn = 1,
    knight,
    bishop,
    rook,
    queen,
    king
} ColorlessPiece;

typedef enum
{
    no_piece = 0x00,
    white_pawn = no_piece + pawn,
    white_knight = no_piece + knight,
    white_bishop = no_piece + bishop,
    white_rook = no_piece + rook,
    white_queen = no_piece + queen,
    white_king = no_piece + king,
    unused_piece = 0x08, // To make sure that black and white pieces are exactly 0x08 apart
    black_pawn = unused_piece + pawn,
    black_knight = unused_piece + knight,
    black_bishop = unused_piece + bishop,
    black_rook = unused_piece + rook,
    black_queen = unused_piece + queen,
    black_king = unused_piece + king,
} Piece;

#define piece_color_mask 0b1000
#define white_piece_color 0b0000
#define black_piece_color 0b1000

// TODO review whether these are useful
Piece whitePieces[];
Piece blackPieces[];

typedef struct
{
    Piece squares[ 128 ]; // 8x8 x2
    bool whiteToPlay;
    unsigned short epIndex;
    // TODO also needs clocks, castling
} Board;

// This needs to carry more than the from/to (which have to be 2x 4-bits each). It also gets the promotion piece and maybe other stuff
typedef unsigned int Move;

// TODO extend this to take a FEN string
void board_populateBoard( Board* board );

// TODO pass in a collection object
void board_getMoves( Board* board, Move (*moves)[256]);

void board_printBoard( Board* board );

