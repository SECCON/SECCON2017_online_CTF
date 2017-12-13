//
//  Candy.c
//  Lazenca.0x0
//
//  Created by JEONG YOUNGMIN on 2017. 10. 30..
//

#include "Global.h"
#include "Board.h"

void setBoard(){
    printf("Please enter a comment for candy.\n");
    board = malloc(BOARDSIZE);
    UserInput(board,BOARDSIZE);
}
