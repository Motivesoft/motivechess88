#include "board.h"

//#define square(file,rank) (rank << 4) + file
//#define empty(board,file,rank) onboard(file,rank) && (board->squares[square(file,rank)] == 0)
//#define offboard(file,rank) square(file,rank) & 0x88
//#define onboard(file,rank) !offboard(file,rank)
#define addmove(moves,startIndex,endIndex) \
{ \
    ( *moves )[ 0 ]++; \
    ( *moves )[ ( *moves )[ 0 ] ] = ( startIndex << 8 ) | endIndex; \
}

#define ONE_RANK 16
#define TWO_RANKS 32

unsigned char pieceChar( Piece piece );
unsigned short indexof( unsigned short file, unsigned short rank )
{
    return ( rank << 4 ) + file;
}
unsigned short fileof( unsigned short index )
{
    return index & 0b1111;
}
unsigned short rankof( unsigned short index )
{
    return ( index >> 4 ) & 0b1111;
}
bool offboard( unsigned short index )
{
    return index & 0x88;
}
bool onboard( unsigned short index )
{
    return !offboard( index );
}
Piece pieceat( Board* board, unsigned short index )
{
    return board->squares[ index ];
}
bool empty( Board* board, unsigned short index )
{
    return board->squares[ index ] == no_piece;
}
bool whitepiece( Board* board, unsigned short index )
{
    Piece piece = pieceat( board, index );
    return ( piece >= white_pawn && piece <= white_king );
}
bool blackpiece( Board* board, unsigned short index )
{
    Piece piece = pieceat( board, index );
    return ( piece >= black_pawn && piece <= black_king );
}

Piece whitePieces[] =
{
    white_pawn,
    white_knight,
    white_bishop,
    white_rook,
    white_queen,
    white_king
};

Piece blackPieces[] =
{
    black_pawn,
    black_knight,
    black_bishop,
    black_rook,
    black_queen,
    black_king
};

void board_populateBoard( Board* board )
{
    board->squares[ A1 ] = white_rook;
    board->squares[ H1 ] = white_rook;
    board->squares[ A8 ] = black_rook;
    board->squares[ H8 ] = black_rook;

    board->squares[ B1 ] = white_knight;
    board->squares[ G1 ] = white_knight;
    board->squares[ B8 ] = black_knight;
    board->squares[ G8 ] = black_knight;

    board->squares[ C1 ] = white_bishop;
    board->squares[ F1 ] = white_bishop;
    board->squares[ C8 ] = black_bishop;
    board->squares[ F8 ] = black_bishop;

    board->squares[ D1 ] = white_queen;
    board->squares[ D8 ] = black_queen;

    board->squares[ E1 ] = white_king;
    board->squares[ E8 ] = black_king;

    board->squares[ A2 ] = white_pawn;
    board->squares[ B2 ] = white_pawn;
    board->squares[ C2 ] = white_pawn;
    board->squares[ D2 ] = white_pawn;
    board->squares[ E2 ] = white_pawn;
    board->squares[ F2 ] = white_pawn;
    board->squares[ G2 ] = white_pawn;
    board->squares[ H2 ] = white_pawn;
    board->squares[ A7 ] = black_pawn;
    board->squares[ B7 ] = black_pawn;
    board->squares[ C7 ] = black_pawn;
    board->squares[ D7 ] = black_pawn;
    board->squares[ E7 ] = black_pawn;
    board->squares[ F7 ] = black_pawn;
    board->squares[ G7 ] = black_pawn;
    board->squares[ H7 ] = black_pawn;

    for ( unsigned short rank = 2; rank < 6; rank++ )
    {
        for ( unsigned short file = 0; file < 8; file++ )
        {
            board->squares[ indexof( file, rank ) ] = no_piece;
        }
    }

    for ( unsigned short rank = 0; rank < 8; rank++ )
    {
        for ( unsigned short file = 8; file < 16; file++ )
        {
            board->squares[ indexof( file, rank ) ] = unused_piece;
        }
    }

    board->whiteToPlay = true;
    board->epIndex = 0;
    board->castling = Kside | Qside | kside | qside;
}

typedef bool ( *PieceTest )( Board* board, unsigned short index );

void board_getSlidingMoves( Board* board, Move( *moves )[ 256 ], unsigned short index, unsigned short offsetCount, unsigned short offsets[] )
{
    PieceTest friendPiece;
    PieceTest enemyPiece;

    if ( board->whiteToPlay )
    {
        friendPiece = &whitepiece;
        enemyPiece = &blackpiece;
    }
    else
    {
        friendPiece = &blackpiece;
        enemyPiece = &whitepiece;
    }

    for ( unsigned short loop = 0; loop < offsetCount; loop++ )
    {
        unsigned short destination = index;
        for ( unsigned short iteration = 0; iteration < 7; iteration++ )
        {
            destination += offsets[ loop ];
            if ( offboard( destination ) )
            {
                break;
            }

            // Blocker?
            if ( (*friendPiece)( board, destination ) )
            {
                break;
            }

            addmove( moves, index, destination );

            // Capture?
            if ( (*enemyPiece)( board, destination ) )
            {
                break;
            }
        }
    }
}

void board_getMoves( Board* board, Move( *moves )[ 256 ] )
{
    static unsigned short knightOffsets[] = { -TWO_RANKS - 1, -TWO_RANKS + 1, -ONE_RANK - 2, -ONE_RANK + 2, ONE_RANK - 2, ONE_RANK + 2, TWO_RANKS - 1, TWO_RANKS + 1 };
    static unsigned short bishopOffsets[] = { -ONE_RANK - 1, -ONE_RANK + 1, ONE_RANK - 1, ONE_RANK + 1 };
    static unsigned short rookOffsets[] = { -ONE_RANK, -1, 1, ONE_RANK };
    static unsigned short kingOffsets[] = { -ONE_RANK - 1, -ONE_RANK, -ONE_RANK + 1, -1, 1, ONE_RANK - 1, ONE_RANK, ONE_RANK + 1 };

    PieceTest friendPiece;
    PieceTest enemyPiece;

    unsigned short oneRank;
    unsigned short twoRanks;
    unsigned short pawnHomeRank;

    if ( board->whiteToPlay )
    {
        friendPiece = &whitepiece;
        enemyPiece = &blackpiece;

        oneRank = ONE_RANK;
        twoRanks = TWO_RANKS;

        pawnHomeRank = 1;
    }
    else
    {
        friendPiece = &blackpiece;
        enemyPiece = &whitepiece;

        oneRank = -ONE_RANK;
        twoRanks = -TWO_RANKS;

        pawnHomeRank = 6;
    }

    for ( unsigned short rank = 0; rank < 8; rank++ )
    {
        for ( unsigned short file = 0; file < 8; file++ )
        {
            unsigned short index = indexof( file, rank );

            if ( friendPiece( board, index ) )
            {
                Piece piece = board->squares[ index ];

                switch ( piece & 0b0111 ) // colorless
                {
                    case pawn:
                        // One step
                        if ( empty( board, index + oneRank ) )
                        {
                            addmove( moves, index, index + oneRank );

                            // Two step
                            if ( rank == pawnHomeRank && empty( board, index + twoRanks ) )
                            {
                                addmove( moves, index, index + twoRanks );
                            }
                        }

                        // Capture
                        if ( ( *enemyPiece )( board, index + oneRank - 1 ) || ( board->epIndex == index - 1 ) )
                        {
                            addmove( moves, index, index + oneRank - 1 );
                        }

                        if ( ( *enemyPiece )( board, index + oneRank + 1 ) || ( board->epIndex == index + 1 ) )
                        {
                            addmove( moves, index, index + oneRank + 1 );
                        }

                        break;

                    case knight:
                        for ( unsigned short loop = 0; loop < 8; loop++ )
                        {
                            unsigned short destination = index + knightOffsets[ loop ];
                            if ( offboard( destination ) )
                            {
                                continue;
                            }

                            if ( !( *friendPiece )( board, destination ) )
                            {
                                addmove( moves, index, destination );
                            }
                        }
                        break;

                    case bishop:
                        board_getSlidingMoves( board, moves, index, 4, bishopOffsets );
                        break;

                    case rook:
                        board_getSlidingMoves( board, moves, index, 4, rookOffsets );
                        break;

                    case queen:
                        board_getSlidingMoves( board, moves, index, 8, kingOffsets );
                        break;

                    case king:
                        for ( unsigned short loop = 0; loop < 8; loop++ )
                        {
                            unsigned short destination = index + kingOffsets[ loop ];
                            if ( offboard( destination ) )
                            {
                                continue;
                            }

                            if ( !( *friendPiece )( board, destination ) )
                            {
                                addmove( moves, index, destination );
                            }
                        }

                        // Castling
                        if ( board->whiteToPlay )
                        {
                            if ( board->castling & Kside )
                            {
                                if ( empty( board, F1 ) && empty( board, G1 ) )
                                {
                                    addmove( moves, index, G1 );
                                }
                            }
                            if ( board->castling & Qside )
                            {
                                if ( empty( board, B1 ) && empty( board, C1 ) && empty( board, D1 ) )
                                {
                                    addmove( moves, index, C1 );
                                }
                            }
                        }
                        else
                        {
                            if ( board->castling & kside )
                            {
                                if ( empty( board, F8 ) && empty( board, G8 ) )
                                {
                                    addmove( moves, index, G8 );
                                }
                            }
                            if ( board->castling & qside )
                            {
                                if ( empty( board, B8 ) && empty( board, C8 ) && empty( board, D8 ) )
                                {
                                    addmove( moves, index, C8 );
                                }
                            }
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    }

    printf( "%d moves found\n", ( *moves )[ 0 ] );
    for ( unsigned short loop = 0; loop < ( *moves )[ 0 ]; loop++ )
    {
        Move move = ( *moves )[ loop + 1 ];
        unsigned char from = ( move >> 8 ) & 0xff;
        unsigned char to = move & 0xff;
        printf( "%c%c%c%c\n", 'a' + fileof( from ), '1' + rankof( from ), 'a' + fileof( to ), '1' + rankof( to ) );
    }
}

void board_printBoard( Board* board )
{
    for ( unsigned short rowIndex = 0; rowIndex < 8; rowIndex++ )
    {
        unsigned short rank = 7 - rowIndex;

        for ( unsigned short file = 0; file < 16; file++ )
        {
            unsigned short index = indexof( file, rank );
            if ( offboard( index ) )
            {
                printf( " - " );
            }
            else
            {
                printf( " %c ", pieceChar( board->squares[ index ] ) );
            }
        }
        printf( "\n" );
    }
}

unsigned char pieceChar( Piece piece )
{
    switch ( piece )
    {
        case no_piece:
            return ' ';

        case white_pawn:
            return 'P';

        case white_knight:
            return 'N';

        case white_bishop:
            return 'B';

        case white_rook:
            return 'R';

        case white_queen:
            return 'Q';

        case white_king:
            return 'K';

        case unused_piece:
            return '-';

        case black_pawn:
            return 'p';

        case black_knight:
            return 'n';

        case black_bishop:
            return 'b';

        case black_rook:
            return 'r';

        case black_queen:
            return 'q';

        case black_king:
            return 'k';

        default:
            return 'X';
    }
}
