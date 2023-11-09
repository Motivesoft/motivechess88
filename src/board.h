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
    empty = 0x00,
    white_pawn,
    white_knight,
    white_bishop,
    white_rook,
    white_queen,
    white_king,
    unused = 0x08, // To make sure that black and white pieces are exactly 0x08 apart
    black_pawn,
    black_knight,
    black_bishop,
    black_rook,
    black_queen,
    black_king,
} Piece;

Piece whitePieces[];

Piece blackPieces[];

typedef struct
{
    unsigned char squares[ 128 ]; // 8x8 x2
    bool whiteToPlay;

} Board;

// TODO extend this to take a FEN string
void board_populateBoard( Board* board );

// TODO pass in a collection object
void board_getMoves( Board* board );

void board_printBoard( Board* board );
