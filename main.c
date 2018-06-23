#include<stdio.h> 
#include<string.h> 
#include<conio.h>
#include <windows.h>
#include <stdlib.h> 

/*
* Constants
*/

#define NUMBER_OF_CARDS_IN_HAND 13
#define MAX_TURN 13
#define MAX_PLAYERS 10
#define GAME_STORE_FILE "gameStr.dat"
#define GAME_STORE_URL ""//game data will be saved in the same folder that the game run, add directory here if want another(need to create folder as well).
#define FILES_EXTENSION ".dat"
#define END_ARRAY "99999"
#define NAME_STR_MAX 30
#define MAX_NUMBER_FILES 99



//program variables
FILE *fptr;


/* Jose Ignacio Retamal ,Procedural Programing Assigment, December 2017.
*	"War" Cards game, high card game.
*	Suits:
*	H = hear , D = diamonds, C = clubs, S = spade
*/


/*
*  Structs
*  4 Struct : Game(Basic Structure with all game data), Turn(data for 1 turn),
*  Player(Data for 1 player), Card(represent 1 single card)
*/
//card
typedef struct
{
	int value;
	char suit;
}Card;

//player
typedef struct
{
	int number;
	int score;
	Card hand[NUMBER_OF_CARDS_IN_HAND];
}Player;
//turn
typedef struct
{
	int turnPoint;
	int winner;
	Card turnCardsTemp[NUMBER_OF_CARDS_IN_HAND];
	Card turnCardsPerm[NUMBER_OF_CARDS_IN_HAND];
}Turn;
//game
typedef struct
{
	char name[NAME_STR_MAX];
	int turnNum; //stage of the game
	int pointsOnHold;
	int numberOfPlayers;
	Turn turns[MAX_TURN];
	Player players[MAX_PLAYERS];
}Game;
/*************************End Structs**********************************/

/*
* Function Declaration
* The order of the the declaratin of functions shows how the program was develop.
* Before the function "playGame" game was done in main.
*/

void createDeck(Card deck[156]);
void displayCard(Card card);
void dealCards(Player players[], Card fullDeck[], int numOfPlayers);
void displayPlayerCards(Player player);
void displayCardsTurn(Card cards[]);
int calculateWinner(Card turnCardsTemp[], int numOfPlayers);
int calculatePoint(Card turnCardsTemp[], int numOfPlayers);
void createNewGame(Game *game, char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX]);
void playGame(Game *game);
void displayGameStats(Game game);
void displayAllPlayersPoints(Game game);
void saveGame(Game game);
void loadGame(Game *game, char gameNameParameter[NAME_STR_MAX]);
void clearConsole();
void loadGameNames(char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX]);
void displayGameNames(char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX]);
int getNamesStringsLength(char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX]);
void addGameName(char gameName[NAME_STR_MAX]);
int checkIfNameExist(char gameName[NAME_STR_MAX], char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX]);
void displayPointsOrdered(Game game);
void shuffleCards(Card fullDeck[], Card sDeck[]);
int checkCards(Player players[], int numOfPlayers);

/*************************End Function Declaration**********************************/

/*
* Program Body
* gui functions : main(), playGame(), createGame().
*/
void main()
{
	//program variables
	//game structur
	Game game;
	Game *gamePointer = &game;
	//files names
	char gameNameStrings[MAX_NUMBER_FILES][NAME_STR_MAX];
	//loop and boolean and other
	int choice, choice1, choice2;
	int isChoiceGood = 0, isChoiceGood1 = 0, isChoicegood2 = 0;
	int gameNameStringLength;
	int isGameSaved = 1;

	//init gameNameStrings, 99999 means last
	strcpy(gameNameStrings[0], END_ARRAY);

	//init game
	game.numberOfPlayers = 0;
	gamePointer->turnNum = 0;

	loadGameNames(gameNameStrings);

	while (isChoiceGood == 0)
	{
		//clear screen
		system("@cls||clear");

		printf("*==================================================================================================================*\n");
		printf("|                               %30s                                                     |\n", "WAR");
		printf("*==================================================================================================================*\n");
		printf("PLease Select an Option:\n");
		printf(" 1 -> New Game.\n 2 -> Load Game.\n 3 -> Exit.\n");

		scanf("%d", &choice);
		clearConsole();

		if (choice < 1 || choice>3)
		{
			puts(" please select a valid options.");
		}
		else
		{
			switch (choice)
			{
			case 1: //new game
				createNewGame(gamePointer, gameNameStrings);
				playGame(gamePointer);
				break;
			case 2://load game
				//clear screen
				system("@cls||clear");
				//reset choice check
				isChoiceGood1 = 0;
				//load game
				//select game
				gameNameStringLength = getNamesStringsLength(gameNameStrings);
				isChoicegood2 = 0;

				loadGameNames(gameNameStrings);

				while (isChoicegood2 == 0)
				{
					printf("PLease Select A game or 0 to exit:\n");
					displayGameNames(gameNameStrings);
					choice2 = 0;//maybe fix a bug
					scanf("%d", &choice2);
					clearConsole();
					gameNameStringLength = getNamesStringsLength(gameNameStrings);

					if ((choice2>0 && choice2<=gameNameStringLength))
					{
						puts(gameNameStrings[choice2 - 1]);
						loadGame(gamePointer, gameNameStrings[choice2 - 1]);
						isChoicegood2 = 1;
						isGameSaved = 1;

						
					}
					else if (choice2 != 0)
					{
						puts("Wrong choice please try again.");
						isChoicegood2 = 0;
					}
					else
					{
						isChoicegood2 = 1;
						isGameSaved = 0;
					}

				}//while (isChoicegood2 == 0)
				if (isGameSaved == 1)
				{
					if (game.turnNum <= 12)
					{
						//display game stats
						displayGameStats(*gamePointer);
						//input choice
						while (isChoiceGood1 == 0)
						{
							printf("PLease Select A Choise:\n");
							printf(" 1 -> Continue Game(Next Round).\n 2 -> OutputGame Status.\n 3 -> Exit.\n");
							scanf("%d", &choice1);
							clearConsole();

							if (choice1 < 1 || choice1>3)
							{
								puts("please select a valid options.");
							}
							else
							{
								switch (choice1)
								{
								case 1:
									//load game
									isChoiceGood1 = 1;
									playGame(gamePointer);
									break;
								case 2:
									//display game stats wil aoutput to file later...
									displayGameStats(*gamePointer);
									break;
								case 3:
									//exit sub meni
									isChoiceGood1 = 1;
									break;
								}
							}
						}

					}
					else//game is over
					{
						printf("PLease Select A Choise:\n");
						displayGameStats(*gamePointer);
						printf("*==================================================================================================================*\n");
						printf("|                               %30s                                                     |\n", "Game Over");
						printf("|------------------------------------------------------------------------------------------------------------------|\n");
						displayPointsOrdered(*gamePointer);
						printf("*------------------------------------------------------------------------------------------------------------------*\n");

						printf("Any character -> Exit.\n");
						scanf("%d", &choice1);
						clearConsole();
					}//(game.turnNum <= 12)
				}//if (isGameSaved == 1)
				break;

			case 3://exit
				isChoiceGood = 1;
				break;
			}//switch (choice)
		}//if (choice < 1 || choice>3)
	}//while (isChoiceGood == 0)
}//main()

void displayCard(Card card)
{
	//if A
	if (card.value == 14)
	{
		printf("%2c-%1c", 'A', card.suit);
	}//normal card from 2 to 10
	else if (card.value > 1 && card.value <= 10)
	{
		printf("%2d-%1c", card.value, card.suit);
	}
	else//other cards J,Q,K
	{
		switch (card.value)
		{
		case 11:
			printf("%2c-%1c", 'J', card.suit);
			break;

		case 12:
			printf("%2c-%1c", 'Q', card.suit);
			break;
		case 13:
			printf("%2c-%1c", 'K', card.suit);
			break;
		}//switch (card.value)
	}//if (card.value == 14)
}//void displayCard(Card card)

void createDeck(Card deck[156])
{
	Card subDeck[52];
	int counter = 0;

	printf("Creating deck.");//prompt

	//create and fill subDeck
	for (int j = 1; j <= 4; j++)
	{
		for (int i = 2; i <= 14; i++)
		{
			Card card1;
			card1.value = i;

			if (j == 1)card1.suit = 'H';
			else if (j == 2) card1.suit = 'D';
			else if (j == 3) card1.suit = 'C';
			else if (j == 4) card1.suit = 'S';

			subDeck[counter++] = card1;
			printf(".");//prompt
		}
	}
	//fild deck using subDeck
	int j = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 52; k++)
		{
			deck[j++] = subDeck[k];
			printf(".");//prompt
		}
	}
	printf("\n");
}//createDeck(Card deck[156])

void dealCards(Player players[], Card fullDeck[], int numOfPlayers)
{
	int player = 0, hand = 0, i;//loop variables
	Card shuffledDeck[156];
	//create shuffledDeck from the fullDeck
	shuffleCards(fullDeck, shuffledDeck);

	printf("Dealing cards.");//prompt
	//deal cards to players
	for (i = 0; i < numOfPlayers * NUMBER_OF_CARDS_IN_HAND; i++)
	{
		if (player == numOfPlayers)
		{
			player = 0;
			hand++;
		}
		if (hand == NUMBER_OF_CARDS_IN_HAND)hand = 0;
		players[player].hand[hand] = shuffledDeck[i];

		player++;

	}//for (i = 0; i < numOfPlayers * NUMBER_OF_CARDS_IN_HAND; i++)

	printf("\n");

}//dealCards(Player players[], Card fullDeck[], int numOfPlayers)

void displayPlayerCards(Player player)
{
	int cardNum;
	for (cardNum = 0; cardNum < NUMBER_OF_CARDS_IN_HAND; cardNum++)
	{
		if (player.hand[cardNum].value > 1 && player.hand[cardNum].value < 15)
		{
			printf("| %4d -> ", cardNum + 2);
			displayCard(player.hand[cardNum]);
			printf("                                                                                                     |\n");
		}//if
	}//for
}//displayPlayerCards(Player player)

void displayCardsTurn(Card cards[])
{
	int i = 0, playerNumber = 0;
	//display card untl null card -1 or max of cards.
	while (cards[i].value != -1 && i < NUMBER_OF_CARDS_IN_HAND)
	{
		playerNumber = i + 1;
		printf("|Player %2d plays: ", playerNumber);
		displayCard(cards[i]);
		printf("                                                                                             |\n");

		i++;
	}//while (cards[i].value != -1 && i < NUMBER_OF_CARDS_IN_HAND)
}// displayCardsTurn(Card cards[])

int calculateWinner(Card turnCardsTemp[], int numOfPlayers)
{
	int i = 0, j, foundEquals = 0;
	//start assuming tat first card is the winner
	int winner = -1;
	int winnerValue = 0;

	//remove invalid moves
	for (i = 0; i < numOfPlayers; i++)
	{
		foundEquals = 0;
		for (j = i + 1; j < numOfPlayers; j++)
		{
			if (turnCardsTemp[i].value == turnCardsTemp[j].value)
			{
				turnCardsTemp[j].value = -1;
				foundEquals = 1;
			}
		}//for (j = i + 1; j < numOfPlayers; j++)
		if (foundEquals == 1)
		{
			turnCardsTemp[i].value = -1;
		}
	}//for (i = 0; i < numOfPlayers; i++)

	//look for higher
	for (i = 0; i < numOfPlayers; i++)
	{
		if (turnCardsTemp[i].value > winnerValue)
		{
			winnerValue = turnCardsTemp[i].value;
			winner = i;
		}
	}//for (i = 0; i < numOfPlayers; i++)
	//return highest card position,-1 if there is no winner
	return winner;
}//calculateWinner(Card turnCardsTemp[], int numOfPlayers)

int calculatePoint(Card turnCardsTemp[], int numOfPlayers)
{
	int i, points = 0;
	for (i = 0; i < numOfPlayers; i++)
	{
		points += turnCardsTemp[i].value;
	}
	return points;
}

void createNewGame(Game *game, char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX])
{
	char gameName[100];
	Card nullCard;
	nullCard.suit = 'X';
	nullCard.value = -1;
	int i = 0, j;
	int isDealGood = 0;
	int temporal = 0;
	//char temporalString[100];
	Card deckIn[156];
	int isValidInput = 0, isValidInput1 = 0;

	printf("*==================================================================================================================*\n");
	printf("|                               %30s                                                     |\n", "NEW GAME");
	printf("*==================================================================================================================*\n");
	//input basics of the new game
	//name
	isValidInput = 0;
	while (isValidInput == 0)
	{
		printf(" Please give  name to new game(max 30 characteres, no space, only letter and/or numbers):\n");
		scanf("%s", gameName);
		clearConsole();

		j = 0;
		isValidInput1 = 1;
		while (gameName[j] != '\0')
		{
			//check is is number or upper case letter, or lower case leter
			if ((gameName[j] >= 48 && gameName[j] <= 57) || (gameName[j] >= 65 && gameName[j] <= 90) || (gameName[j] >= 97 && gameName[j] <= 122))
			{
				j++;
				if (j == NAME_STR_MAX)//to long
				{
					isValidInput1 = 0;
					puts(" Invalid name please try again, max 30 characters");
					break;
				}
			}
			else
			{
				isValidInput1 = 0;
				puts(" Invalid name please try again, only number or letters");
				break;
			}//if ((gameName[j] ....

		}//end while (gameName[j] = !'\0')



		if (isValidInput1 == 1)
		{
			//check if name is new
			if (checkIfNameExist(gameName, gameNamestrings) != -1)
			{
				puts(" Invalid name please try again, game name exists please try another name");
			}
			else
			{


				isValidInput = 1;
				strcpy(game->name, gameName);
			}
		}
	}//end while (isValidInput == 0)
	//end name input

	//number of players
	isValidInput = 0;
	while (isValidInput == 0)
	{
		printf(" Please enter the number of player(min 2 ,max 10):\n");
		scanf("%d", &temporal);
		clearConsole();
		if (temporal < 2 || temporal>10)
		{
			puts(" Invalid number of players, please try again.");
		}
		else
		{
			game->numberOfPlayers = temporal;
			isValidInput = 1;
		}

	}//while (isValidInput == 0)

	 //init score to hold
	game->pointsOnHold = 0;

	//initiate scores 
	for (i = 0; i < game->numberOfPlayers; i++)
	{
		game->players[i].score = 0;
		game->players[i].number = i + 1;
	}

	//init first turn card to null for no show
	for (i = 0; i < MAX_TURN; i++)
	{
		game->turns[i].turnCardsTemp[0] = nullCard;
	}
	//start at turn 0
	game->turnNum = 0;


	//deal cards
	while (isDealGood == 0)
	{
		createDeck(deckIn);
		dealCards(game->players, deckIn, game->numberOfPlayers);
		isDealGood = checkCards(game->players, game->numberOfPlayers);
	}//while (isDealGood == 0)
}//createNewGame(Game *game, Card deck[])

void displayAllPlayersPoints(Game game)
{
	int numberOfPlayers;
	int i;
	numberOfPlayers = game.numberOfPlayers;

	for (i = 0; i < numberOfPlayers; i++)
	{
		printf("| player %2d = %3d points.                                                                                          |\n", i + 1, game.players[i].score);
	}
}

void displayGameStats(Game game)
{
	printf("\n\n*==================================================================================================================*\n");
	printf("|                               %30s Stats                                               |\n", game.name);
	printf("|==================================================================================================================|\n");
	printf("| Number of players: %2d                                                                                            |\n", game.numberOfPlayers);
	printf("| Actual Turn: %2d                                                                                                  |\n", game.turnNum);
	printf("| Rolled Over Points: %3d                                                                                          |\n", game.pointsOnHold);
	printf("|==================================================================================================================|\n");
	printf("| Scores:                                                                                                          |\n");
	displayAllPlayersPoints(game);
	printf("*==================================================================================================================*\n");
}//displayGameStats(Game game)

void playGame(Game *game)
{
	//null card
	Card nullCard;
	//loop variales 
	int player;
	int isValidMove = 0;
	int gameIsOn = 1;
	//input varialve
	int move = 0;
	//temp
	int tempTurn = 0;
	int scorePlayer = 0;
	//init null card
	nullCard.suit = 'X';
	nullCard.value = -1;


	while (/*game->turnNum <= (MAX_TURN +1) &&*/ gameIsOn == 1)//while for each turn
	{
		///*************************************************************************************************************************
		tempTurn = game->turnNum;
		//each player game
		for (player = 0; player < game->numberOfPlayers; player++)
		{
			///*************************************************************************************************************************************
			//clear screen
			system("@cls||clear");


			printf("*==================================================================================================================*\n");
			printf("|                               %30s                                                     |\n", game->name);
			printf("|------------------------------------------------------------------------------------------------------------------|\n");
			printf("| Player : %2d                                                                                                      |\n", player + 1);
			printf("|------------------------------------------------------------------------------------------------------------------|\n");
			printf("| Score from last turn: %6d                                                                                     |\n", game->pointsOnHold);
			printf("|------------------------------------------------------------------------------------------------------------------|\n");

			//show turn and player playing
			printf("| Turn   : %2d                                                                                                      |\n", tempTurn);

			printf("|------------------------------------------------------------------------------------------------------------------|\n");

			//display the game played for others
			printf("| Actual Game :                                                                                                    |\n");
			displayCardsTurn(game->turns[tempTurn].turnCardsTemp);
			printf("|------------------------------------------------------------------------------------------------------------------|\n");
			//display cards played
			printf("| Your Cards :                                                                                                     |\n");
			displayPlayerCards(game->players[player]);
			//printf("%d",game->players[player].hand[0].value);
			printf("*==================================================================================================================*\n");

			//play card
			//prompt user and read input
			isValidMove = 0;
			while (isValidMove == 0)
			{
				puts("Please Select a card to play");
				scanf("%d", &move);
				clearConsole();

				move -= 2;

				if (move < 0 || move>12)//there is 2 diference
				{
					puts("Inalid input please try again.");
				}
				else if (game->players[player].hand[move].value == -1)
				{
					puts("Inalid input please try again.");
				}
				else
				{
					isValidMove = 1;
				}
			}

			//take card and put it into turnCards
			game->turns[tempTurn].turnCardsTemp[player] = game->players[player].hand[move];
			game->turns[tempTurn].turnCardsPerm[player] = game->players[player].hand[move];
			game->players[player].hand[move].value = -1;

			if (player != game->numberOfPlayers)
			{
				game->turns[tempTurn].turnCardsTemp[player + 1] = nullCard;
				game->turns[tempTurn].turnCardsPerm[player + 1] = nullCard;
			}
			///*************************************************************************************************************************************
		}//for - end turns

		//show results after turn is dones

		//clear screen
		system("@cls||clear");


		printf("*==================================================================================================================*\n");

		printf("| Turn   : %2d  done                                                                                                |\n", game->turnNum + 1);
		printf("|------------------------------------------------------------------------------------------------------------------|\n");
		printf("| Game was:                                                                                                        |\n");
		printf("|------------------------------------------------------------------------------------------------------------------|\n");
		displayCardsTurn(game->turns[tempTurn].turnCardsPerm);
		printf("|------------------------------------------------------------------------------------------------------------------|\n");


		//get total point and display it
		game->turns[tempTurn].turnPoint = calculatePoint(game->turns[tempTurn].turnCardsTemp, game->numberOfPlayers);

		//calculate winner
		game->turns[tempTurn].winner = calculateWinner(game->turns[tempTurn].turnCardsTemp, game->numberOfPlayers);
		if (game->turns[tempTurn].winner != -1)
		{
			printf("|Winner: player %2d                                                                                                 |\n", game->turns[tempTurn].winner + 1);
		}
		else
		{
			printf("| Game was a Draw:                                                                                                 |\n");
		}
		printf("|------------------------------------------------------------------------------------------------------------------|\n");

		//point in game
		printf("|Points this round      :  %4d                                                                                    |\n", game->turns[tempTurn].turnPoint);
		printf("|Points from last round :  %4d                                                                                    |\n", game->pointsOnHold);
		printf("|Total point in play    :  %4d                                                                                    |\n", game->turns[tempTurn].turnPoint + game->pointsOnHold);

		//check if there is winner, if not hold point
		if (game->turns[game->turnNum].winner != -1)
		{

			//there is a winner
			//there is a winner we add point on hold
			scorePlayer = game->turns[game->turnNum].turnPoint;
			scorePlayer += game->pointsOnHold;

			//pointOnHold have ben use so make it 0
			game->pointsOnHold = 0;

			//give points
			game->players[game->turns[tempTurn].winner].score += scorePlayer;

			//pronmt 
			printf("|player %2d wins points  :  %4d                                                                                    |\n", game->turns[tempTurn].winner + 1, scorePlayer);
		}
		else
		{
			//there is not winner
			//add turn point to point on hold
			game->pointsOnHold += game->turns[tempTurn].turnPoint;
			printf("|Carry on Points       :   %4d                                                                                    |\n", game->pointsOnHold);

		}

		printf("|------------------------------------------------------------------------------------------------------------------|\n");
		printf("| Scores :                                                                                                         |\n");
		displayAllPlayersPoints(*game);
		printf("*==================================================================================================================*\n");

		//go to next turn;
		//end of turn selection
		//
		if (game->turnNum < MAX_TURN - 1)
		{
			isValidMove = 0;
			while (isValidMove == 0)
			{
				//prompt user for what to do
				printf("\n Please enter :\n 1 -> continue the game(no save).\n 2 -> continue and save the game.\n 3 -> exit and save game\n 4 -> exit game with out save.\n");
				scanf("%d", &move);
				clearConsole();

				if (move >= 1 && move <= 4)
				{
					isValidMove = 1;
					switch (move)
					{
					case 1:
						game->turnNum++;
						break;
					case 2:
						game->turnNum++;
						saveGame(*game);
						break;
					case 3:
						game->turnNum++;
						saveGame(*game);
						gameIsOn = 0;
						break;
					case 4:
						game->turnNum++;
						gameIsOn = 0;
						break;

					}//switch (move)
				}
				else
				{
					isValidMove = 0;
					puts("Inalid input please try again.");
				}//if (move < 1 || move>4)

			}//end while (isValidMove == 0)
		}//if turn
		else if (game->turnNum == 12)
		{
			//game is over
			printf("*==================================================================================================================*\n");
			printf("|                               %30s                                                     |\n", "Game Over");
			printf("|------------------------------------------------------------------------------------------------------------------|\n");
			displayPointsOrdered(*game);
			printf("*------------------------------------------------------------------------------------------------------------------*\n");
			isValidMove = 0;
			while (isValidMove == 0)
			{
				//prompt user for what to do
				printf("\n Please enter :\n 1 -> save and exit.\n 2 -> exit with out save.\n");
				scanf("%d", &move);
				clearConsole();

				printf("\n move: -%d-\n", move);
				if (move < 1 || move>2)//there is 2 diference
				{
					puts("Inalid input please try again.");
				}
				else
				{

					isValidMove = 1;
					switch (move)
					{
					case 1:
						game->turnNum++;
						saveGame(*game);
						gameIsOn = 0;
						break;
					case 2:
						game->turnNum++;
						gameIsOn = 0;
						break;
					}
				}

			}//end while (isValidMove == 0)
		}

	}

	///*************************************************************************************************************************
}

void saveGame(Game game)
{
	int turnNum = game.turnNum;
	int numberOfPlayers = game.numberOfPlayers;
	int i, j;
	char file[100];
	int isGameNameSaved;
	char gameNameStrings[MAX_NUMBER_FILES][NAME_STR_MAX];
	//open file

	//compose file
	strcpy(file, GAME_STORE_URL);
	strcat(file, game.name);
	strcat(file, FILES_EXTENSION);

	loadGameNames(gameNameStrings);
	isGameNameSaved = checkIfNameExist(game.name, gameNameStrings);
	if (isGameNameSaved == -1)
	{
		//add name to file
		addGameName(game.name);
	}
	fptr = fopen(file, "w");
	if (fptr == NULL) {
		printf("Error opening file ! \n");
	}
	//simple variables
	//name
	fprintf(fptr, "%s\n", game.name);
	//number of players
	fprintf(fptr, "%d\n", game.numberOfPlayers);
	//turn number
	fprintf(fptr, "%d\n", game.turnNum);
	//point on hold
	fprintf(fptr, "%d\n", game.pointsOnHold);

	//turns
	for (i = 0; i <= turnNum; i++)
	{
		//turn point
		fprintf(fptr, "%d\n", game.turns[i].turnPoint);
		//turn winner
		fprintf(fptr, "%d\n", game.turns[i].winner);
		//will only save all the array with all the cards
		//save turns
		for (j = 0; j < NUMBER_OF_CARDS_IN_HAND; j++)
		{
			fprintf(fptr, " %d %c ", game.turns[i].turnCardsPerm[j].value, game.turns[i].turnCardsPerm[j].suit);
		}
		fprintf(fptr, "\n");
	}
	//save players
	for (i = 0; i < numberOfPlayers; i++)
	{
		//player number
		fprintf(fptr, "%d\n", game.players[i].number);
		//score
		fprintf(fptr, "%d\n", game.players[i].score);
		//card in hand
		for (j = 0; j < NUMBER_OF_CARDS_IN_HAND; j++)
		{
			fprintf(fptr, " %d %c ", game.players[i].hand[j].value, game.players[i].hand[j].suit);
		}
		fprintf(fptr, "\n");
	}

	//close file
	fclose(fptr);
}

void loadGame(Game *game, char gameNameParameter[NAME_STR_MAX])
{

	FILE *cfPtr;

	int numberOfPlayers, turnNumber, pointsOnHold, turnPoint, turnWinner, value, playerNumber, playerScore, i, j;
	char suit;
	char file[100], gameName[NAME_STR_MAX];
	Card nullCard;
	nullCard.suit = 'X';
	nullCard.value = -1;

	//info
	printf("\nLoading game ");

	//compose file
	strcpy(file, GAME_STORE_URL);
	strcat(file, gameNameParameter);
	strcat(file, FILES_EXTENSION);
	puts(file);
	//open file to read
	if ((cfPtr = fopen(file, "r")) == NULL) { printf("File could not be opened\n"); } /* end if */

	//read name
	fscanf(cfPtr, "%s", gameName);
	/*printf("\n%s\n", gameName);*/
	strcpy(game->name, gameName);

	//info
	printf(" \"%s\" .", gameName);

	//number of players
	fscanf(cfPtr, "%d", &numberOfPlayers);
	/*printf("\n%d\n", numberOfPlayers);*/
	game->numberOfPlayers = numberOfPlayers;

	//read turn number
	fscanf(cfPtr, "%d", &turnNumber);
	/*printf("\n%d\n", turnNumber);*/
	game->turnNum = turnNumber;

	//read point on hold
	fscanf(cfPtr, "%d", &pointsOnHold);
	//printf("\n%d\n", pointsOnHold);
	game->pointsOnHold = pointsOnHold;

	//read turns
	for (i = 0; i <= turnNumber; i++)
	{
		//info
		printf(".");

		/*printf("\nTry read\n");*/
		//turn points 
		fscanf(cfPtr, "%d", &turnPoint);
		/*printf("\n%d\n", turnPoint);*/
		game->turns[i].turnPoint = turnPoint;

		//turn winner
		fscanf(cfPtr, "%d", &turnWinner);
		/*printf("\n%d\n", turnWinner);*/
		game->turns[i].winner = turnWinner;
		/*printf("\n%d\n", game->turns[i].winner);*/
		//all card play
		for (j = 0; j < NUMBER_OF_CARDS_IN_HAND; j++)
		{
			fscanf(cfPtr, "%d %c", &value, &suit);
			game->turns[i].turnCardsPerm[j].value = value;
			game->turns[i].turnCardsPerm[j].suit = suit;

			//info
			printf(".");
		}//for (j = 0; j < NUMBER_OF_CARDS_IN_HAND; j++)
	}//for (i = 0; i <= turnNumber; i++)

	//read players
	for (i = 0; i < numberOfPlayers; i++)
	{
		//info
		printf(".");

		//player number
		fscanf(cfPtr, "%d", &playerNumber);
		/*printf("\n%d\n", playerNumber);*/
		game->players[i].number = playerNumber;

		//score
		fscanf(cfPtr, "%d", &playerScore);
		/*printf("\n%d\n", playerScore);*/
		game->players[i].score = playerScore;

		//card in hand
		for (j = 0; j < NUMBER_OF_CARDS_IN_HAND; j++)
		{
			fscanf(cfPtr, "%d %c", &value, &suit);

			game->players[i].hand[j].value = value;
			game->players[i].hand[j].suit = suit;

			//info
			printf(".");

		}
	}
	//init first turn card to null for no show
	for (i = 0; i < MAX_TURN; i++)
	{
		game->turns[i].turnCardsTemp[0] = nullCard;
	}

	//info
	printf(".Game Loaded Successfully.\n");
	fclose(cfPtr);
}// loadGame(Game *game, char gameNameParameter[NAME_STR_MAX])

void addGameName(char gameName[NAME_STR_MAX])
{
	FILE *cfPtr;
	char file[100];

	//compose file name
	strcpy(file, GAME_STORE_URL);
	strcat(file, GAME_STORE_FILE);

	//open file
	if ((cfPtr = fopen(file, "a")) == NULL)
	{
		//file not open
	}
	else
	{
		fprintf(cfPtr, "\n%s", gameName);
	}

	fclose(cfPtr);
}//addGameName(char gameName[NAME_STR_MAX])

void loadGameNames(char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX])
{
	FILE *cfPtr;
	char file[100];
	char fileLine[NAME_STR_MAX];
	int fileLineCounter = 0;

	//compose file name
	strcpy(file, GAME_STORE_URL);
	strcat(file, GAME_STORE_FILE);

	//open file
	if ((cfPtr = fopen(file, "r")) == NULL)
	{
		//create file
		cfPtr = fopen(file, "w");
		gameNamestrings[0][0] = '\0';
		fprintf(cfPtr, "%s", END_ARRAY);
		strcpy(gameNamestrings[0], END_ARRAY);

	}
	else
	{
		while (!feof(cfPtr))
		{
			fscanf(cfPtr, "%s", fileLine);
			if (strcmp(fileLine, END_ARRAY) == 0)
			{
				gameNamestrings[0][0] = '\0';
				strcpy(gameNamestrings[0], END_ARRAY);
			}
			else
			{
				strcpy(gameNamestrings[fileLineCounter], fileLine);
				fileLineCounter++;
			}
		}//while (!feof(cfPtr))
		strcpy(gameNamestrings[fileLineCounter], END_ARRAY);

	}//if ((cfPtr = fopen(file, "r")) == NULL)
	fclose(cfPtr);
}//loadGameNames(char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX])

void displayGameNames(char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX])
{
	int i = 0;

	if (strcmp(gameNamestrings[0], END_ARRAY) == 0)
	{
		puts(" No saved games.");
	}
	else
	{
		while (strcmp(gameNamestrings[i], END_ARRAY) != 0)
		{
			printf(" %d -> %s\n", i + 1, gameNamestrings[i]);
			i++;
		}
	}//if (strcmp(gameNamestrings[0], END_ARRAY) == 0)
}//displayGameNames(char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX])

int getNamesStringsLength(char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX])
{
	int length = 0;
	while (strcmp(gameNamestrings[length], END_ARRAY) != 0 || length == 99)
	{
		length++;
	}
	return length;
}

int checkIfNameExist(char gameName[NAME_STR_MAX], char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX])
{
	int i = 0;
	int found = -1;
	while (strcmp(gameNamestrings[i], END_ARRAY) != 0)
	{
		if (strcmp(gameNamestrings[i], gameName) == 0)
		{
			found = i;
			return found;
		}
		i++;
	}
	return found;
}//checkIfNameExist(char gameName[NAME_STR_MAX], char gameNamestrings[MAX_NUMBER_FILES][NAME_STR_MAX])

void clearConsole()
{
	while (getchar() != '\n');
}

void displayPointsOrdered(Game game)
{
	int i, j;
	Player temp;
	Player playerSorted[MAX_PLAYERS];

	//copy array of players
	for (i = 0; i < MAX_PLAYERS; i++)
	{
		playerSorted[i] = game.players[i];

	}

	//sort array
	for (i = 0; i < game.numberOfPlayers; i++)
	{
		for (j = 0; j <  game.numberOfPlayers + 1; j++)
		{
			if (playerSorted[j].score < playerSorted[j + 1].score)
			{
				temp = playerSorted[j];
				playerSorted[j] = playerSorted[j + 1];
				playerSorted[j + 1] = temp;
			}
		}
	}
	//display
	for (i = 0; i < game.numberOfPlayers; i++)
	{
		printf("|Place %2d = Player %2d, points = %4d                                                                               |\n", i + 1, playerSorted[i].number, playerSorted[i].score);
	}
}

void shuffleCards(Card fullDeck1[], Card sDeck[])
{
	int randomPosition, numOfCardLeft, i;
	srand(time(NULL)); /* seeds with the time of day */

	numOfCardLeft = 156;
	for (i = 0; i < 156; i++)
	{
		//get random position
		randomPosition = rand() % numOfCardLeft;
		//add randon position to new deck
		sDeck[i] = fullDeck1[randomPosition];
		//remplace card used with last
		fullDeck1[randomPosition] = fullDeck1[numOfCardLeft - 1];
		//make the deck 1 smaller
		numOfCardLeft--;
	}//for (i = 0; i < 156; i++)
}//shuffleCards(Card fullDeck1[], Card sDeck[])

int checkCards(Player players[], int numberOfPlayers)
{
	int isGood = 1;//1 true 0 false
	int found, suitsToCheck, playersCount, card;
	char suits[] = { 'H','D','C','S' };

	for (suitsToCheck = 0; suitsToCheck < 4; suitsToCheck++)
	{
		for (playersCount = 0; playersCount < numberOfPlayers; playersCount++)
		{
			found = 0;
			for (card = 0; card < NUMBER_OF_CARDS_IN_HAND; card++)
			{
				if (players[playersCount].hand[card].suit == suits[suitsToCheck])
				{
					found = 1;
					break;
				}
			}//for (card = 0; card < NUMBER_OF_CARDS_IN_HAND; card++)

			if (found == 1)
			{
				//have this sout so keep looking
			}
			else
			{
				//1 mising
				isGood = 0;
				return isGood;
			}
		}//for (playersCount = 0; playersCount < numberOfPlayers; playersCount++)
	}//for (suitsToCheck = 0; suitsToCheck < 4; suitsToCheck++)
	return isGood;
}//checkCards(Player players[], int numberOfPlayers)

