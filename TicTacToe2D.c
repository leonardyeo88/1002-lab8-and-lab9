#include "TicTacToe2D.h"

COORD coord = { 0, 0 }; // this is global variable. Note: center of axis is set to the top left corner of the screen
HANDLE h;
int board[GAME_SIZE][GAME_SIZE];


int turn = 1;
int player, comp;


void gotoxy(int x, int y)
{
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void main()
{
	initialize_screen();
	playerMenu();
	_getch();

}

void menu()
{
	int choice;
	system("cls");
	initialize_game_board();
	printf("\n--------TIC-TAC-TOE MENU--------");
	printf("\n1 : Play with X");
	printf("\n2 : Play with O");
	printf("\n3 : Exit");
	printf("\nEnter your choice:>");
	scanf("%d", &choice);
	turn = 1;
	switch (choice)
	{
	case 1:
		player = 1;
		comp = 0;
		player_turn();
		break;
	case 2:
		player = 0;
		comp = 1;
		start_game();
		break;
	case 3:
		printPlayerList();
		printf("Press Enter to exit. Bye!\n");
		_getch();
		exit(1);
	default:
		menu();
	}
}

void player_turn()
{
	int pos;

	check_draw();
	draw_board();
	gotoxy(30, 18);
	printf("Your Turn :> ");
	scanf("%d", &pos);
	if (pos != 0){
		if (board[convert_pos_to_row(pos)][convert_pos_to_col(pos)] != EMPTY)
			player_turn();

		move_to(pos);
		draw_board();

		if (isWinning(player))
		{
			gotoxy(30, 20);
			SetConsoleTextAttribute(h, FOREGROUND_RED);
			printf("Player Wins\n");
			updateNumberOfWins(currentPlayerIndex);
			SetConsoleTextAttribute(h, 7);
			_getch();
			playerMenu();
			//exit(0);
		}
		else
			start_game();
	}
	else{
		printPlayerList();
		printf("Press Enter to exit. Bye!\n");
		_getch();
		exit(0);

	}
}


void initialize_game_board(){
	int i, j;
	for (i = 0; i < GAME_SIZE; i++)
		for (j = 0; j < GAME_SIZE; j++)
			board[i][j] = EMPTY;
}


void initialize_screen(){
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD wOldColorAttrs;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	/*
	* First save the current color information
	*/
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
}

void start_game()
{
	// p==1 then X   p==0  then  O
	check_draw();
	move_to(getPossibleWinningPos());
	draw_board();

	if (isWinning(comp))
	{
		gotoxy(30, 20);
		SetConsoleTextAttribute(h, FOREGROUND_BLUE);
		printf("Computer wins\n");
		SetConsoleTextAttribute(h, 7); //7 - WHITE
		_getch();
		playerMenu();
	}
	else
		player_turn();
}

void check_draw()
{
	if (turn > GAME_SIZE*GAME_SIZE)
	{
		gotoxy(30, 20);
		SetConsoleTextAttribute(h, 14);
		printf("Game Draw\n");
		SetConsoleTextAttribute(h, 7); //7 - WHITE
		_getch();
		playerMenu();
	}
}

int isWinning(int p)
{
	// p==1 then X   p==0  then  O
	int i, j;
	int win_value;

	if (p == 1)
		win_value = X_VALUE * X_VALUE * X_VALUE;
	else
		win_value = O_VALUE * O_VALUE * O_VALUE;

	int product_diag1 = 1, product_diag2 = 1;
	for (i = 0; i < GAME_SIZE; i++){ //row check
		int product_row = 1, product_col = 1;
		for (j = 0; j < GAME_SIZE; j++){
			product_row *= board[i][j];
			product_col *= board[j][i];
		}
		if (product_row == win_value || product_col == win_value)
			return TRUE;
		product_diag1 *= board[i][i];
		product_diag2 *= board[i][GAME_SIZE - i - 1];
	}
	if (product_diag1 == win_value || product_diag2 == win_value)
		return TRUE;

	return FALSE;
}


int getPossibleWinningPos()
{
	int make2();
	int make4();

	int pos = getWinningPos(comp);
	if (pos)
		return pos;

	pos = getWinningPos(player);
	if (pos)
		return pos;

	pos = make2();
	if (pos)
		return pos;

	pos = make4();
	if (pos)
		return pos;

	return FALSE;
}

/*
*	when the function is called, it loops through the cells indicated below:
*
*	i=0. 1st iteration:	product_row = 1 * value of (0,0), product_column = 1 * value of (0,0)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|	(0,0) empty?			|							|							|
*		emp_col_index = 0			
*		emp_row_index = 0
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*
*
*	i=0. 2nd iteration:	product_row = product_row * value of (0,1), product_column = product_column * value of (1,0)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|	(0,1) empty?			|							|
*									emp_col_index = 1
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|	(1,0) empty? return 4	|							|							|
*		emp_row_index = 1
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*
*
*	i=0. 3rd iteration:	product_row = product_row * value of (0,2), product_column = product_column * value of (2,0)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|		(0,2) empty?		|
*																	emp_col_index = 2
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*		
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|	(2,0) empty?			|							|							|
*		emp_row_index = 2
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*	
*	----------------------------------------------------------------------------------------------------------------------------
*
*	i=1. 1st iteration:	product_row = 1 * value of (1,0), product_column = 1 * value of (0,1)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|		(1,2) empty?		|							|
*										emp_col_index = 0
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|		(0,1) empty?		|							|							|
*			emp_row_index = 0														
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*										
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*

*
*	i=1 2nd iteration:	product_row = product_row * value of (1,1), product_column = product_column * value of (1,1)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*																	
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|		(1,1) empty?		|							|
*										(1,1) empty?
*										emp_row_index = 1
*										emp_col_index = 1
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*		
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*
*
*	i=1. 3rd iteration:	product_row = product_row * value of (1,2), product_column = product_column * value of (2,1)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*																	
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|		(1,2) empty?		|
*																	emp_col_index = 2
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|		(2,1) empty?		|							|
*										emp_row_index = 2
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*
*	------------------------------------------------------------------------------------------------------------------------------
*
*	i=2. 1st iteration:	product_row = 1 * value of (2,0), product_column = 1 * value of (0,2)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|		(0,2) empty?		|
*																	emp_row_index = 2
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*																	
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|		(2,0) empty?		|							|							|
*			emp_col_index = 0							
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*
*
*	i=2. 2ns iteration:	product_row = product_row * value of (2,1), product_column = product_column * value of (1,2)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*																	
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|		(1,2) empty?		|
*																	emp_row_index = 1
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|		(2,1) empty?		|							|
*										emp_col_index = 1
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*
*	i=2. 3rd iteration:	product_row = product_row * value of (2,2), product_column = product_column * value of (2,2)
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*																	
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|		(2,2) empty?		|
*																	(2,2) empty?
*																	emp_row_index = 2
*																	emp_col_index = 2
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*
*/
int getWinningPos(int p){
	int i;
	int win_value;
	int product_row = 1, product_column = 1, product_diag1 = 1, product_diag2 = 1, j;

	if (p == 1)
		win_value = X_VALUE * X_VALUE * EMPTY;	//(3*3*2 = 18)
	else
		win_value = O_VALUE * O_VALUE * EMPTY;	//(5*5*2 = 50)
	int emp_diag_index1, emp_diag_index2;
	//row & column && diagonal check
	for (i = 0; i < GAME_SIZE; i++){
		int emp_col_index, emp_row_index;
		product_column = 1;
		product_row = 1;
		for (j = 0; j < GAME_SIZE; j++){
			product_row *= board[i][j];
			if (board[i][j] == EMPTY)
				emp_col_index = j;
			product_column *= board[j][i];
			if (board[j][i] == EMPTY)
				emp_row_index = j;
		}
		if (product_row == win_value)
			return convert_row_col_to_pos(i, emp_col_index);
		if (product_column == win_value)
			return convert_row_col_to_pos(emp_row_index, i);

		/*check diagonal*/
		/*
		*	1st iteration
		*	i=0, product_diag1 = product_diag1 * value of (0,0), product_diag2 = product_diag2 * value of (0,2)
		*	
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|		(0,0) empty?		|							|		(0,2) empty?		|
		*			emp_diag_index1 = 0										emp_diag_index2 = 0
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|							|							|							|
		*																	
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|							|							|							|
		*																
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*
		*
		*	2nd iteration
		*	i=1, product_diag1 = product_diag1 * value of (1,1), product_diag2 = product_diag2 * value of (1,1)
		*
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|							|							|							|
		*												
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|							|		(0,0) empty?		|							|
		*										(0,0) empty?
		*										emp_diag_index2 = 0
		*										emp_diag_index1 = 0	
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|							|							|							|
		*
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*
		*
		*
		*	3rd iteration
		*	i=2, product_diag1 = product_diag1 * value of (2,2), product_diag2 = product_diag2 * value of (2,0)
		*
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|							|							|							|
		*
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|							|							|							|
		*										
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*	|		(2,2) empty?		|							|		(2,0) empty?		|
		*			emp_diag_index1 = 2										emp_diag_index2 = 2
		*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
		*
		*/
		product_diag1 *= board[i][i];
		if (board[i][i] == EMPTY)
			emp_diag_index1 = i;
		product_diag2 *= board[i][GAME_SIZE - i - 1];
		if (board[i][GAME_SIZE - i - 1] == EMPTY)
			emp_diag_index2 = i;
	}

	if (product_diag1 == win_value)
		return convert_row_col_to_pos(emp_diag_index1, emp_diag_index1);
	if (product_diag2 == win_value)
		return convert_row_col_to_pos(emp_diag_index2, GAME_SIZE - emp_diag_index2 - 1);
	return FALSE;
}

/*
*	when the function is called, it loops through the cells indicated below:
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|	(0,1) empty? return 2	|							|	
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
*	|	(1,0) empty? return 4	|	(1,1) empty? return 5	|	(1,2) empty? return 6	|	
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
*	|							|	(2,1) empty? return 8	|							|	
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
*
*	returns the position of the cell if the program checks if the cell is empty
*/
int make2()
{
	int i;
	for (i = 0; i < GAME_SIZE; i++){
		if (board[1][i] == EMPTY)
			return (GAME_SIZE + i + 1);
		if (board[i][1] == EMPTY)
			return GAME_SIZE*i + 2;
	}
	return 0;
}



/*
*	when the function is called, it loops through the cells indicated below:
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|	(0,0) empty? return 1	|							|	(0,2) empty? return 3	|
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|							|							|							|
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*	|	(2,0) empty? return 7	|							|	(2,2) empty? return 9	|
*	_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _
*
*	returns the position of the cell if the program checks if the cell is empty
*/
int make4()
{
	int i = 0;
	while (i < GAME_SIZE){
		if (board[i][0] == EMPTY)
			return (i*GAME_SIZE + 1);
		if (board[i][GAME_SIZE - 1] == EMPTY)
			return ((i*GAME_SIZE + GAME_SIZE));
		i += 2;
	}
	return FALSE;
}

void move_to(int n)
{
	int i = convert_pos_to_row(n), j = convert_pos_to_col(n);
	if (turn % 2)
		board[i][j] = X_VALUE;
	else
		board[i][j] = O_VALUE;
	turn++;
}

void draw_board()
{
	int j;
	SetConsoleTextAttribute(h, 4); // 4 - LIGHT RED

	for (j = 9; j<17; j++)
	{
		gotoxy(35, j);
		printf("|       |");

	}
	gotoxy(28, 11);
	printf("-----------------------");
	gotoxy(28, 14);
	printf("-----------------------");
	char string[2];
	for (j = 1; j < 10; j++){
		_itoa(j, string, 10);
		put_X_O(string[0], convert_pos_to_row(j), convert_pos_to_col(j));
	}
	SetConsoleTextAttribute(h, 7);

	for (int i = 0; i < GAME_SIZE; i++)
		for (j = 0; j < GAME_SIZE; j++)
			if (board[i][j] == X_VALUE)
				put_X_O('X', i, j);
			else if (board[i][j] == O_VALUE)
				put_X_O('O', i, j);
}

int convert_pos_to_row(int n){
	return (n - 1) / GAME_SIZE;
}
int convert_pos_to_col(int n){
	return (n - 1) % GAME_SIZE;
}

int convert_row_col_to_pos(int row, int col){
	return row*GAME_SIZE + col + 1;
}

void put_X_O(char ch, int i, int j)
{
	int x = 31, y = 10;

	x = x + (8 * j);
	y = y + (3 * i);

	gotoxy(x, y);
	printf("%c", ch);
}