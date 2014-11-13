#include "TicTacToe2D.h"
#include <stdlib.h>
#include <string.h>

#define FALSE 0
#define TRUE 1

Player playerList[MAX_PLAYERS];
int numOfPlayers = 0;


void playerMenu(){
	char idString[7];
	system("cls");
	printf("\n--------PLAYER MENU--------\n");
	printf("Enter playerID or \"New\" to play (0 to exit): ");
	scanf("%s", idString);
	if (strcmp(idString, "0") == 0){
		printPlayerList();
		printf("Press Enter to exit. Bye!\n");
		_getch();
		exit(0);
	}
	if (strcmp(idString, "New") == 0){
		addNewPlayer();
		currentPlayerIndex = numOfPlayers - 1;
	}
	else{
		int playerIndex = getPlayerIndex(idString);
		if (playerIndex == -1){
			printf("PlayerID not found!!");
			_getch();
			playerMenu();
			return;
		}
		currentPlayerIndex = playerIndex;
		printf("Welcome %s\n", playerList[playerIndex].name);
		printf("You have won %d games\n", playerList[playerIndex].numOfWins);
	}
	printf("Press Enter to continue\n");
	_getch();
	menu();
}

void addNewPlayer(){
	char name[20];	//same as char *name = malloc(20 * sizeof(char));
	void generateId(char*, char*);	//initialise generateId function
	int checkUniqueness(char*);	//initialise checkUniqueness function
	char id[7];	//same as char *id = malloc(7 * sizeof(char));

	if (numOfPlayers == MAX_PLAYERS){
		/*insert your own code (1)*/
		playerMenu();	//start playerMenu function
	}
	else{
		printf("Enter your name: ");
		//scanf("%s", name);	//gets player's name as string, however any string after \w, \n, \t, etc in the stream, will be removed
		fflush(stdin);	//flushes out the buffer
		fgets(name, strlen(name), stdin);	//gets player's name as string, however any string after \w, \n, \t, etc in the stream, will be stored
		generateId(name, id); //prints unique ID

		/*insert your own code here (2)
		This code segment should call to checkUniqueness() to check
		for the uniqueness of the generated ID
		*/


		if (checkUniqueness(id) == TRUE){	//check if id is unique,
			char *tempID = (char *)malloc((1 + strlen(id))*sizeof(char));	//tempID (allocate memory of 8 (7 + 1), 7 for id and 1 for null termintor)
			strcpy(tempID, id);	//copy id to tempID
			tempID[6] = '\0';	//add a null terminator to index 6 of tempID
			playerList[numOfPlayers].id = tempID;	//insert tempID to playerList as player's id

			char *tempName = (char *)malloc((1 + strlen(name))*sizeof(char));	//tempName (allocate memory of length of name + 1, 1 for null termintor)
			strcpy(tempName, name);	//copy name to tempName
			tempName[strlen(name)+1] = '\0';	//add a null terminator to last index of tempName
			playerList[numOfPlayers].name = tempName;	//insert tempName into playerList as player's name

			playerList[numOfPlayers].numOfWins = 0;	//add 0 into playerList as player's numOfWins
			numOfPlayers++;	//increment numOfPlayers by 1
		}

	}


}


void generateId(char *name, char *id){
	/*insert your own code here (3)*/

	int temp = strlen(name) - 2;	//length of name - 2

	for (int i = 0; i < strlen(name); i++){
		if (name[i] == ' '){	//checks if index of name has a blank space
			temp = i - 1;	//temp = index of name - 1
			break; //break from loop
		}
	}

	
	strncpy(id, name, 3); //get first 3 letter of name and insert into id
	id[3] = '\0';	//add null termintor to id[3]

	char *twoInt = getRandom2DigitNumber();	//generate 2 digit number and add assign to pointer
	strcat(id, twoInt);	//concatenate the 3 letters and twoInt 

	char lastLetter = name[temp];	//get the value of the last letter of name
	char lastLetterTemp[2];	//lastLetterTemp array of 2, index 1 for last letter of name and index 2 for null terminator
	sprintf(lastLetterTemp, "%c", lastLetter);	//convert lastLetter into lastLetterTemp
	strcat(id, lastLetterTemp);	//concatenate id with lastLetterTemp
	id[6] = '\0';	//close up the string with a null terminator
	
	printf("Your id: %s\n", id);	//display id

}



int checkUniqueness(char * idString){
	int i;
	if (numOfPlayers == 0)
		return TRUE;
	for (i = 0; i < numOfPlayers; i++){
		if (strcmp(idString, playerList[i].id) == 0)	//compares string idString and player's id taken from playerList
			return FALSE;	//return FALSE if idString matches with playersList[i].id (in other words, check if idString already exists in playerList array)
	}
	return TRUE;
}

int getPlayerIndex(char *idString){
	/*insert your own code here (4)*/
		int count = 0;
		int tempIndex = 0;
	
		for (int i = 0; i < numOfPlayers; i++){
			if (strcmp(idString, playerList[i].id) == 0){	//compares string idString and player's id taken from playerList
				tempIndex = i;	//assign the player's index to tempIndex 
				count++;	//increment count by 1
			}
		}
	
		if (count > 0){	//check if count contains value greater than 0
			return tempIndex;	//return the player's index
		}
		else{	//if count contains value lesser then or equal to 0
			return -1;	//return -1
		}
}

void updateNumberOfWins(int index){
	playerList[index].numOfWins++;
}

/*function displays all player's id, player's name, player's numOfWins*/
void printPlayerList(){
	/*insert your code here (5)*/
	printf("%-15s %-22s %-5s\n", "PlayerID", "Name", "NumberOWins");
	if (numOfPlayers > 0){
			for (int i = 0; i < numOfPlayers; i++){
				printf("%-15s", playerList[i].id);
				printf("%s", playerList[i].name);
				printf("%d\n", playerList[i].numOfWins);
			}
	}	
}

/*function returns pointer to a char (string)*/
char* getRandom2DigitNumber(){
	int n1 = rand() % 10 + 0;	//randomise from 0 to 9
	int n2 = rand() % 10 + 0;	//randomise from 0 to 9

	char n1ToChar[2];	//initialise n1ToChar as array of 2
	sprintf(n1ToChar, "%d", n1);	//convert int n1 to string n1ToChar
	char n2ToChar[2];	//initialise n2ToChar as array of 2
	sprintf(n2ToChar, "%d", n2);	//convert int n2 to string n2ToChar

	char *twoDigitNum;
	twoDigitNum = (char *)malloc((3)*sizeof(char));	//memory allocation of 3
	strcpy(twoDigitNum, n1ToChar);	//adds n1ToChar to memory 
	strcat(twoDigitNum, n2ToChar);	//concatenate n2ToChar with whatever is in memory
	twoDigitNum[2] = '\0';	//close string with null terminator

	return twoDigitNum;	//return string
}