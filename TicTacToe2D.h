#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#define EMPTY 2
#define X_VALUE 3
#define O_VALUE 5
#define BOARD_SIZE 10
#define GAME_SIZE 3
#define MAX_PLAYERS 10

void menu();
void move_to(int n);
void start_game();
void check_draw();
void draw_board();
void player_turn();
void put_X_O(char, int , int );
void initialize_screen();
int isWinning(int);
int getPossibleWinningPos();
int getWinningPos(int);
int convert_pos_to_row(int);
int convert_pos_to_col(int);
int convert_row_col_to_pos(int, int);
void initialize_game_board();
void addNewPlayer();
int getPlayerIndex(char*);
char* getRandom2DigitNumber();
void playerMenu();
void updateNumberOfWins(int);
void printPlayerList();

/*
Lab8 & Lab9
name and id is not fixed size, 
length of name and id depends on memory allocation

Lab6
playerID and name are fixed size,
example, if name only allows 5 characters,
if user's input of name is more then 5 characters,
the input might be invalid
*/
struct playerStruct{
	char *name;
	char *id;
	int numOfWins;

};

typedef struct playerStruct Player;

int currentPlayerIndex;

