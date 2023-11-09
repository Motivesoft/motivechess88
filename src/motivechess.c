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

    board_getMoves( &board );
}
