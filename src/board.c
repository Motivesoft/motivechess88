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
}

void board_getMoves( Board* board, Move( *moves )[ 256 ] )
{
    if ( board->whiteToPlay )
    {
        for ( unsigned short rank = 0; rank < 8; rank++ )
        {
            for ( unsigned short file = 0; file < 8; file++ )
            {
                unsigned short index = indexof( file, rank );
                Piece piece = board->squares[ index ];

                switch ( piece )
                {
                    case white_pawn:
                        // One step
                        if ( empty( board, index + ONE_RANK ) )
                        {
                            addmove( moves, index, index + ONE_RANK );

                            // Two step
                            if ( rank == 1 && empty( board, index + TWO_RANKS ) )
                            {
                                addmove( moves, index, index + TWO_RANKS );
                            }
                        }

                        // Capture
                        if ( blackpiece( board, index + ONE_RANK - 1 ) || ( board->epIndex == index - 1 ) )
                        {
                            addmove( moves, index, index + ONE_RANK - 1 );
                        }

                        if ( blackpiece( board, index + ONE_RANK + 1 ) || ( board->epIndex == index + 1 ) )
                        {
                            addmove( moves, index, index + ONE_RANK + 1 );
                        }

                        break;

                    case white_knight:
                        unsigned short offsets[] = { -TWO_RANKS - 1, -TWO_RANKS + 1, -ONE_RANK - 2, -ONE_RANK + 2, ONE_RANK - 2, ONE_RANK + 2, TWO_RANKS - 1, TWO_RANKS + 1 };
                        for ( unsigned short loop = 0; loop < 8; loop++ )
                        {
                            unsigned short destination = index + offsets[ loop ];
                            if ( offboard( destination ) )
                            {
                                continue;
                            }

                            if ( !whitepiece( board, destination ) )
                            {
                                addmove( moves, index, destination );
                            }
                        }
                        break;

                    case white_bishop:
                        break;

                    case white_rook:
                        break;

                    case white_queen:
                        break;

                    case white_king:
                        break;

                    default:
                        break;
                }
            }
        }
    }
    else // if ( !board->whiteToPlay )
    {
        for ( unsigned short rank = 0; rank < 8; rank++ )
        {
            for ( unsigned short file = 0; file < 8; file++ )
            {
                Piece piece = board->squares[ indexof( file, rank ) ];

                switch ( piece )
                {
                    case black_pawn:
                        break;

                    case black_knight:
                        break;

                    case black_bishop:
                        break;

                    case black_rook:
                        break;

                    case black_queen:
                        break;

                    case black_king:
                        break;

                    default:
                        break;
                }
            }
        }
    }

    printf( "%d moves found", ( *moves )[ 0 ] );
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
