#pragma once
#include <stdbool.h>		//bool defenition
#include "Othello_const.h"  //Constans defenition

#define MAXSIZE UD_MAX_POS			// Set the maximum size of the board
#define MINSIZE UD_MIN_POS			// Set the minimum size of the board

//typedef enum { false, true } bool;
typedef enum color {
	empty,
	black,
	white,
	gameOver
} color_t;
typedef enum available_for {
	none,
	for_black,
	for_white,
	for_both
} possible_t;

typedef struct state_t {
	color_t color;
	possible_t possible;
} state_tdef;

typedef enum mode {
	check,
	execute
} mode_t;

typedef enum gameStatus {
	stoped,
	started
} status_t;

void		mainCicleOfGame(); //00 Main cicle 
void		initStart(state_tdef*** board, int row, int col, int* mainMoveCounter, color_t* player, color_t* computer, color_t* curentMove, int order); //01 Initialisation start condition
void		resizeBoard(state_tdef*** board, int n, int m); //02 Create board requested size
void		eraseBoard(state_tdef** board, int n, int m);//03 Delite board and free mamory
struct state_t cellInst(void); //04 Create cell instance
void		outputBoard(state_tdef** board, int row, int col); //05 Print board
void		printBoard(state_tdef** board, int row, int col, int mainMoveCounter, int* score); //06  Print board in console
void		writeCell(state_tdef** board, int row, int col, int x, int y, color_t player); //06 write value to cell
int			movePossibilities(state_tdef** board, int row, int col, color_t curentMove); //07count of posible moves and fill array of posible cell
int			flipAround(state_tdef** board, int row, int col, int x, int y, color_t curentMove, mode_t mode); //08handling a fliping of opponent disks
int			Hint(state_tdef** board, int row, int col, color_t curentMove);	//09found of most pragmatic step
void		pcMove(state_tdef** board, int row, int col, color_t computer, int* rowSel, int* colSel);	//10simulation of computer move
void		waitFor(unsigned int secs); //11dalay
int			CalñSore(state_tdef** board, int row, int col, color_t curentMove); //12Calculate the score for AI
void		mainScore(int* score, state_tdef** board, int row, int col, color_t player, color_t computer); //13Calculate the curent score

