// motivechess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// This project will be C, rather than C++, to minimize bloat and encourage inventiveness

#include "board.h"

int main()
{
    printf( "Hello World!\n" );

    Board board;
    board_populateBoard( &board );
    board_printBoard( &board );

    Move moves[ 256 ];
    moves[ 0 ] = 0; // index 0 will be the count of elements in the list

    board_getMoves( &board, &moves );
}
