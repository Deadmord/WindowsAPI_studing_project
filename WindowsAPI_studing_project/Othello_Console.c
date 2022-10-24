#include <stdio.h>		//standard input/output
#include <stdlib.h>		//Memory allocation
#include <string.h>		//string
#include <time.h>		//use in random and dalay
#include <conio.h>		//use in random
#include <assert.h>		//assert for debuging

#include <locale.h>		//for show which coding use

#include <windows.h>	//functions in the Windows API
#include <wchar.h>		//for work with whide char

#include "Othello_Console.h" //my lib

/* TODO: Avoid global variables.
 * TODO: Try to avoid passing your structure (40 bytes + padding)
 * without pointer. */
//struct state_t** board = NULL;
//assert(row_num >= row && row_num <= row);

int main(void)
{
	while (1) {
		mainCicleOfGame();
		_getch(); //validation before exit
		exit(1);
	}
    return 0;
}

void mainCicleOfGame() {
	struct state_t** board = NULL; //declare of board (as pointer to pointer to struct).

	/*
	board = (state_tdef**)malloc(1 * sizeof(state_tdef*)); //First memory allocation, jast as a example, not obligatory in our code
	for (int i = 0; i < 1; i++) {
		board[i] = (state_tdef*)malloc(1 * sizeof(state_tdef));
	}
	*/
	

	char accept;
	char inputX[3];
	char inputY[3];
	int order = 0;
	int row = 0, col = 0; //size of board
	int x = 0, y = 0; //coordinats each cell

	color_t player = 0;
	color_t computer = 0;
	color_t curentMove = 0;

	int mainMoveCounter = 0;
	int crossCount = 0;

	int score[2] = { 0 };
	

	/*
	int c;
	char* l = setlocale(LC_ALL, "");
	if (l == NULL) {
		printf("Locale not set\n");
	}
	else {
		printf("Locale set to %s\n", l);
	}
	printf("%ls\n", L"s:\\яшертыHello°™");
	*/

	printf("============================= Othello ============================\n");
	printf("REVERSI\n");
	printf("Black (X) move first, white (O) move second\nSelect a square for your move by typing a digit for the row\nand a letter for the column with no spaces between.\nGood luck! Press Enter to start.\n");
	scanf_s("%c", &accept, 1);

	while (row < MINSIZE || row > MAXSIZE || col < MINSIZE || col > MAXSIZE) {
		printf("Select the board sizes. \nEnter number of row. ");
		scanf_s("%d", &row);
		printf("Enter number of colomn. ");
		scanf_s("%d", &col);
	}

	while (order != 1 && order != 2) {
		printf("Choose a color. Press 1 to go black or 2 to go white. ");
		scanf_s("%d", &order);
	}

	initStart( &board, row, col, &mainMoveCounter, &player, &computer, &curentMove, order);

	//outputBoard( board, row, col);
	movePossibilities(board, row, col, curentMove);
	mainScore(score, board, row, col, player, computer);
	printBoard(board, row, col, mainMoveCounter, score);

	for (mainMoveCounter; mainMoveCounter < (row * col) && crossCount < 2;) //count the steps and check the possibility of moving
	{
		if (curentMove == player) // checking the order of move player/PC
		{
			if (movePossibilities(board, row, col, curentMove)) //if at least one move is available
			{
				//outputBoard(board, row, col); //Dalite it!!!!!!!!!!!-----!!!!!!!!

				while (1)
				{
					printf("Please enter your move. row number: ");
					scanf_s("%s", &inputX, 3);
					printf("Colomn number: ");
					scanf_s("%s", &inputY, 3);

					sscanf_s(inputX, "%d", &x);
					x--;

					y = inputY[0];			
					if (y >= 'a')
					{
						y = y - 'a' + 1;
					}
					else
					{
						y = y - 'A' + 1;
					}
					y--;//Convert the column index entered by the user into the column index of a two-dimensional array

					if (x >= 0 && y >= 0 && x < MAXSIZE && y < MAXSIZE && (board[x][y].possible == player || board[x][y].possible == for_both))//Judge whether the input is correct
					{
						writeCell(board, row, col, x, y, curentMove);
						mainMoveCounter++;
						break;
					}
					else
					{
						printf("Not a valid move, try again.\n");
					}
				}
				system("cls"); //!!!!!!!!!!!!!!!!!!!!!!!***************!!!!!!!!!!!!!!!
				movePossibilities(board, row, col, computer);
				mainScore(score, board, row, col, player, computer);
				printBoard(board, row, col, mainMoveCounter, score);//Update the board and display according to the position input by the user
			}
			else if (++crossCount < 2)
			{
				printf("No avalible moves, PC GO.\n");
			}
			else
			{
				printf("Both have no moves available, GAME OVER.\n");
			}
			curentMove = computer;
		}
		else //PC move handler
		{
			if (movePossibilities(board, row, col, curentMove))
			{
				//printf("Waiting for opponent.");
				waitFor(2);
				Sleep(1000);
				system("cls");

				crossCount = 0;
				pcMove(board, row, col, computer, &x, &y);
				writeCell(board, row, col, x, y, computer);
				mainMoveCounter++;
				movePossibilities(board, row, col, player);
				mainScore(score, board, row, col, player, computer);
				printBoard(board, row, col, mainMoveCounter, score);

			}
			else
			{
				if (++crossCount < 2)
				{
					printf("No avalible moves, You GO.\n");
				}
				else
				{
					printf("Both have no moves available, GAME OVER.\n");
				}
			}
			curentMove = player;
		}
	}

	mainScore(score, board, row, col, player, computer);
	printf("Final score:\n");
	printf("AI: %d\nPlayer: %d\n", score[1], score[0]);
	_getch();

	eraseBoard(board, row, col);
};


void initStart(state_tdef*** board, int row, int col, int* mainMoveCounter, color_t* player, color_t* computer, color_t* curentMove, int order) {
	/* //Old way
	state_tdef** boardLocal = *board;	//here we use ppp to board keep available in scope above, due to we will change a address of **board in "resizeBoard" function below  
	resizeBoard(&boardLocal, row, col); //prepare new gamebord
	*board = boardLocal; //replacing old with a new array
	*/

	//New way
	resizeBoard(board, row, col); //prepare new gamebord: allocate mamory
	state_tdef** boardLocal = *board; //define "local" variable to pointer on board


	for (int i = 0; i < row; i++) {	//reset to NULL allocated memory
		for (int j = 0; j < col; j++) {
 			boardLocal[i][j] = cellInst();
		}
	}

	writeCell(boardLocal, row, col, (row / 2 - 1), (col / 2 - 1), black); //set initial disks to the centr of board
	writeCell(boardLocal, row, col, (row / 2), (col / 2), black);
	writeCell(boardLocal, row, col, (row / 2 - 1), (col / 2), white);
	writeCell(boardLocal, row, col, (row / 2), (col / 2 - 1), white);
	*mainMoveCounter = 4;										//preset the game-counter
	*curentMove = black;
	
	if (order == 1) {
		*player = black;
		*computer = white;
	}
	else
	{
		*player = white;
		*computer = black;
	}
}

void resizeBoard(state_tdef*** board, int n, int m)
{

	/* TODO: Handle reallocations errors. */
	if (!(*board = (state_tdef**)realloc(*board, n * sizeof(state_tdef*)))) {
		printf("Error: can't allocate memory");
		_getch();
		exit(2);
	}
	else {
		for (int i = 0; i < n; i++) {
			if (!((*board)[i] = (state_tdef*)malloc(m * sizeof(state_tdef)))) {
				printf("Error: can't allocate memory");
				_getch();
				exit(2);
			}
		}
	}
	//*board1 = realloc(*board1, n * sizeof(state_tdef*)); //realloc dont work with pointer send to function
	//for (int i = 0; i < n; i++) {
	//	(*board1)[i] = (state_tdef*)malloc(m * sizeof(state_tdef));
	//}

	

}

void eraseBoard(state_tdef** board, int n, int m)
{
	for (int i = n - 1; i > 0; i--) {
		free((board)[i]);
	}
	free(board);
	
}

struct state_t cellInst(void)
{
	struct state_t thisdata;
	thisdata.color = empty;
	thisdata.possible = none;
	thisdata.last_move = 0;

	//thisdata.color = rand()%3;
	//printf("%d \n", thisdata.color);

	return thisdata;
}

void outputBoard(state_tdef** board, int row, int col)
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//printf("%d \t", field[i*col+j].color);
			printf("%d ", board[i][j].color);
		}
		printf("\n");
	}
	printf("\n");
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//printf("%d \t", field[i*col+j].color);
			printf("%d ", board[i][j].possible);
		}
		printf("\n");
	}
}

void printBoard(state_tdef** board, int row, int col, int mainMoveCounter, int* score)
{	
	printf("\n    ");
	for (int j = 0; j < col; j++)//Display the horizontal logo of the board
	{
		printf("%c  ", j + 'a');
	}
	for (int i = 0; i < row; i++)
	{
		printf("\n  ");
		for (int j = 0; j < col; j++)
		{
			printf("+--");
			if (j == (col - 1))
				printf("+\n");
		}
		printf("%d", i + 1);
		if (i < 9) {
			printf(" ");
		}
		for (int j = 0; j < col; j++)
		{
			if (board[i][j].color == black)//Display the corresponding mark according to the value in the array
				printf("|X ");
			else if (board[i][j].color == white)
				printf("|O ");
			else if (board[i][j].possible != none)
				printf("|. ");
			else
				printf("|  ");

			if (j == (col - 1)){
				printf("|");
				if (i == 0) printf("\t\tCounter:  %d", mainMoveCounter);
				if (i == 2) printf("\t\tAI score: %d", score[1]);
				if (i == 3) printf("\t\tPlayer:   %d", score[0]);
			}
				
		}
		if (i == (row - 1))
		{
			printf("\n  ");
			for (int j = 0; j < col; j++)//display separator
			{
				printf("+--");
				if (j == (col - 1))
					printf("+\n");
			}
		}
	}
	printf("\n");
}

void writeCell(state_tdef** board, int row, int col, int x, int y, color_t curentMove)
{
	if (board[x][y].color) {
		printf("Position %d %d already been taken, try again\n", x, y);
	}
	else {
		board[x][y].color = curentMove;
		flipAround(board, row, col, x, y, curentMove, execute);
	}
}

int movePossibilities(state_tdef** board, int row, int col, color_t curentMove) {
	
	int move = 0;					// Counter of moves
	int opponent = 0;
	if (curentMove == 1) {
		opponent = 2;
	}
	else if (curentMove == 2) {
		opponent = 1;
	}
	else {
		printf("Incorect color of current move");
		_getch();
		return 0;
	}

	for (int i = 0; i < row; i++)	
	{
		for (int j = 0; j < col; j++)
		{
			board[i][j].possible = 0;	//reset to NULL the field of possibilities
		}
	}
	for (int i = 0; i < row; i++)	//checking each cell of board
	{
		for (int j = 0; j < col; j++)
		{
			if (board[i][j].color != empty)					//checking the cell color
				continue;				//if the cell is occupied then take next cell.
			else if (flipAround(board, row, col, i, j, curentMove, check) != 0) {
				board[i][j].possible = curentMove; //mark it cell as available to move for current player
				move++;
			}
		}
	}
	return move;
}

int flipAround(state_tdef** board, int row, int col, int x, int y, color_t curentMove, mode_t mode) {
	int rowDir, colDir, xNext, yNext = 0;	//Direction and coordinate of oponents disks
	int move = 0;					// Counter of moves
	color_t opponent = 0;
	if (curentMove == 1) {
		opponent = 2;
	}
	else if (curentMove == 2) {
		opponent = 1;
	}
	else {
		printf("Incorect color of current move");
		_getch();
		return 0;
	}

	for (rowDir = -1; rowDir <= 1; rowDir++)		//go through each direction
	{
		for (colDir = -1; colDir <= 1; colDir++)	//checking the cells around
		{

			if (x + rowDir < 0 || x + rowDir >= row		//Determine whether it is out of bounds 
				|| y + colDir < 0 || y + colDir >= col
				|| (rowDir == 0 && colDir == 0))		//exclude the center
			{
				continue; //to the next direction
			}
			else if (board[x + rowDir][y + colDir].color == opponent) // find opponents disks around
			{
				xNext = x + rowDir;	//coordinate of 
				yNext = y + colDir; //curent opponents disk
				while (1)		//checking each direction which start from opponent disk
				{
					xNext += rowDir; //coordinate of 
					yNext += colDir; //next cell in this direction
					if (xNext < 0 || xNext >= row || yNext < 0 || yNext >= col) //if it is out of bounds 
					{
						break; //break the loop
					}
					if (board[xNext][yNext].color == empty)
					{
						break;
					}
					if (board[xNext][yNext].color == curentMove)
					{	
						if (mode == execute) {
							while (board[xNext -= rowDir][yNext -= colDir].color == opponent)//Find the current position
							{
								board[xNext][yNext].color = curentMove;
							}
						}
						move++;
						break;
					}
				}
			}
		}
	}
	return move;
}

int Hint(state_tdef** board, int row, int col, color_t curentMove)//Prompt the best way
{
	int rowX, colY;
	int maxscore = 0;
	int score = 0;

	struct state_t** boardTry = NULL;	//объявление дополнительной доски, не забыть освободить память
	resizeBoard(&boardTry, row, col);

	color_t opponent = empty;
	if (curentMove == black) {			//определение цвета компьютера и игрока
		opponent = white;
	}
	else if (curentMove == white) {
		opponent = black;
	}
	else {
		printf("Incorect color of current move");
		_getch();
		return 0;
	}

	for (rowX = 0; rowX < row; rowX++)
		for (colY = 0; colY < col; colY++)
		{										//проверяем по всему массиву
			if (!board[rowX][colY].possible)				//из будущих возможных ходов игрока
				continue;
			for (int i = 0; i < row; i++)
				for (int j = 0; j < col; j++)
				{
					boardTry[i][j] = board[i][j]; //копируем текущее состояние доски
				}
			writeCell(boardTry, row, col, rowX, colY, curentMove);	//Делаем ход из возможных в тестовой доске
			score = CalсSore(boardTry, row, col, curentMove);		//расчитываем количество очков противника.
			if (maxscore < score)
				maxscore = score;
		}
	eraseBoard(boardTry, row, col);
	return maxscore;
}

void pcMove(state_tdef** board, int row, int col, color_t computer, int* rowSel, int* colSel)
{
	int rowX, colY, rowXtry, colYtry;	//координаты
	int score = 0, minscore = 100;		//счетчики результата

	struct state_t** boardTry = NULL;	//объявление дополнительной доски, не забыть освободить память
	resizeBoard(&boardTry, row, col);

	color_t opponent = empty;
	if (computer == black) {			//определение цвета компьютера и игрока
		opponent = white;
	}
	else if (computer == white) {
		opponent = black;
	}
	else {
		printf("Incorect color of current move");
		_getch();
		exit(3);
	}


	for (rowX = 0; rowX < row; rowX++)
	{
		for (colY = 0; colY < col; colY++)
		{
			if (board[rowX][colY].possible == none) // для всех возможных ходов 
			{
				continue;
			}
			for (int i = 0; i < row; i++) //копируем текущее состояние доски
			{
				for (int j = 0; j < col; j++)
				{
					boardTry[i][j] = board[i][j];// тут копируем
				}
			}
			writeCell(boardTry, row, col, rowX, colY, computer); //Делаем ход из возможных в тестовой доске
			movePossibilities(boardTry, row, col, opponent);	//получаем таблицу ходов которые стали доступны для игрока
			score = Hint(boardTry, row, col, opponent);			//Хинт, нужно разобрать как работает.
			if ((rowX == 0 && colY == 0) || (rowX == 0 && colY == (col - 1) || rowX == (row - 1) && colY == 0 || rowX == (row - 1) && colY == (col - 1))) //проверяем, занимаем ли мы угол.
			{
				minscore = score; //сохраняем результат,координаты хода и выходим
				rowXtry = rowX;
				colYtry = colY;
			}
			else if (row == 0 || row == MAXSIZE - 1 || col == 0 || col == MAXSIZE - 1) //проверяем, занимаем ли мы край поля.
			{
				minscore = score; //сохраняем результат,координаты хода и выходим
				rowXtry = rowX;
				colYtry = colY;
			}
			else if (score < minscore) // иначе проверяем меньше ли мы получили очков
			{
				minscore = score; //сохраняем результат,координаты хода и выходим
				rowXtry = rowX;
				colYtry = colY;
			}
		}
	}
	eraseBoard(boardTry, row, col);
	//writeCell(board, row, col, rowXtry, colYtry, computer);
	*rowSel = rowXtry;
	*colSel = colYtry;
}

void waitFor(unsigned int secs) {
	unsigned int retTime = (unsigned int)time(0) + secs;   // Get finishing time.
	while (time(0) < retTime);               // Loop until it arrives.
}

int CalсSore(state_tdef** board, int row, int col, color_t curentMove)	//Calculate the score
{
	int score = 0;
	int rowX, colY;

	color_t opponent = empty;
	if (curentMove == black) {			//определение цвета компьютера и игрока
		opponent = white;
	}
	else if (curentMove == white) {
		opponent = black;
	}
	else {
		printf("Incorect color of current move");
		_getch();
		return 0;
	}

	for (rowX = 0; rowX < row; rowX++)
		for (colY = 0; colY < col; colY++)
		{
			score = score - (board[rowX][colY].color == opponent);
			score = score + (board[rowX][colY].color == curentMove);
		}
	return score;
}

void mainScore(int* score, state_tdef** board, int row, int col, color_t player, color_t computer)
{
	score[1] = score[0] = 0;
	for (int i = 0; i < row; i++)//Start calculating the score
	{
		for (int j = 0; j < col; j++)
		{
			score[0] = score[0] + (board[i][j].color == player);
			score[1] = score[1] + (board[i][j].color == computer);
		}
	}
}



