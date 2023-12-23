//
// Eyal Shpater 319041166
// This is a TAKI game!
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NAME_LENGTH 20
#define NUM_OF_STARTING_CARDS 4
#define NUM_OF_COLORS 4
#define FIRST_INDEX 0
#define SECOND_INDEX 1
#define ONE_CARD "1"
#define TWO_CARD "2"
#define THREE_CARD "3"
#define FOUR_CARD "4"
#define FIVE_CARD "5"
#define SIX_CARD "6"
#define SEVEN_CARD "7"
#define EIGHT_CARD "8"
#define NINE_CARD "9"
#define PLUS_CARD "+"
#define CHANGE_DIRECTION_CARD "<->"
#define STOP_CARD " STOP"
#define COLOR_CARD "COLOR"
#define TAKI_CARD " TAKI"
#define RED_COLOR 'R'
#define BLUE_COLOR 'B'
#define GREEN_COLOR 'G'
#define YELLOW_COLOR 'Y'
#define NO_COLOR ' '
#define PLUS_CARD_ID 9
#define STOP_CARD_ID 11
#define COLOR_CARD_ID 12
#define CARD_WIDTH 9
#define CARD_LENGTH 6
#define FIRST 1
#define CARD_TYPE_ROW 3
#define CARD_COLOR_ROW 4
#define MIDDLE 5
#define NO_CARDS 0
#define TAKE_CARD_FROM_DECK 0
#define NUM_OF_TOTAL_TYPES_OF_CARDS 14
#define GARBAGE_INDEX -1
#define COLS 2
#define NOT_TAKI false
#define TWO_PLAYERS 2

typedef struct Card {
    char color;
    int typeID;
}card;

typedef struct Player {
    char name[MAX_NAME_LENGTH];
    card* cards;
    int numberOfCards; // logic size of cards array
    int physSize; // physical size of cards array
} player;

typedef struct Game {
    card upperCard;
    int counter[NUM_OF_TOTAL_TYPES_OF_CARDS][COLS];
    int playerIndex;
    int numOfPlayers;
    int winnerIndex;
    bool exit;
    bool changeDirection;
}game;

/**************** main functions ******************/
void printWelcomeScreen();
void readNumberOfPlayers(game* pUtility);
player* getPlayers(game* pUtility);
void playerTurn(player* pPlayer, game* utility);
void printWinnerName(char name[]);
void initializeGameUtility(game* pUtility);
void printStatisticTable(int arr[][COLS], int size);

/**************** game functions ******************/
void readPlayerName(char name[], int num);
void handOutStartingCards(card* cards, game* pUtility);
void readPlayerChoice(int numOfCards, int* pChoice);
void readPlayerChoiceForTAKI(int numOfCards, int* pChoice);
void getCardFromTheDeck(player* p, game* pUtility);
void playSpecialCard(player* pPlayer, game* pUtility, int cardIndex);

/**************** card functions ******************/
card getNewCard(int type, game* utility);
void printCard(card c);
void removeCardFromDeck(card* arr, int* pSize, int cardIndex);
void stopCardAction(player* pPlayer, game* pUtility, int cardIndex);
void plusCardAction(player* pPlayer, game* pUtility, int cardIndex);
void colorCardAction(player* pPlayer, game* pUtility, int cardIndex);
void changeDirectionAction(player* pPlayer, game* pUtility, int cardIndex);
void takiCardAction(player* pPlayer, game* pUtility, int cardIndex);

////// Auxiliary functions for card functions //////
void getCardColor(char* pColor, int num);
int getRandomNumberForCardType();
int getRandomNumberForUpperCardType();
int getRandomNumberForCardColor();
void printCardTypeLine(int type);
void printCardColorLine(char c);
void printEmptyLine();
void printFullLine();
void printHalfLine(int stop, int starCondition);
void printPlayerCards(player p);
void swapCards(card* a, card* b);
char* convertCardIdToString(int id);
int getAndCheckColorChoice();
void printColorCardMenu();
card* cardRealloc(card* c, int size, int newSize);

/********* general auxiliary functions ***********/
void allocateCardArray(player* p);
void checkMalloc(void* p);
bool isCardValid(card upper, card check, bool isTaki);
bool isChoiceNumberValid(int num, int lim);
void checkIfChoiceIsValid(int* pChoice, int numberOfCards, card upperCard, card* arr, bool isTaki);
void freeAllPlayersCardsArray(player* pArr, int size);
void sortCounterArray(int counter[][COLS], int size);
void merge(int arr1[][COLS], int size1, int arr2[][COLS], int size2, int res[][COLS]);
void copyArr(int src[][COLS], int dest[][COLS], int size);
void printHeadOfTheTable();
void initializeTable(int table[][COLS], int size);

void main()
{
    srand(time(NULL));

    player* gamePlayers;
    game gameUtility;

    initializeGameUtility(&gameUtility);

    printWelcomeScreen();
    readNumberOfPlayers(&gameUtility);
    gamePlayers = getPlayers(&gameUtility);


    while (!gameUtility.exit) // exit update in playTurn function.
    {
        if (gameUtility.playerIndex >= gameUtility.numOfPlayers)
            gameUtility.playerIndex = FIRST_INDEX;
        else if (gameUtility.playerIndex < FIRST_INDEX)
            gameUtility.playerIndex = gameUtility.numOfPlayers - 1;

        gameUtility.winnerIndex = gameUtility.playerIndex;
        playerTurn(&gamePlayers[gameUtility.playerIndex], &gameUtility);

        if (gameUtility.changeDirection)
            gameUtility.playerIndex--;
        else
            gameUtility.playerIndex++;
    }

    printWinnerName(gamePlayers[gameUtility.winnerIndex].name);

    sortCounterArray(gameUtility.counter, NUM_OF_TOTAL_TYPES_OF_CARDS);
    printStatisticTable(gameUtility.counter, NUM_OF_TOTAL_TYPES_OF_CARDS);

    freeAllPlayersCardsArray(gamePlayers, gameUtility.numOfPlayers);
}


/***************************** main functions *******************************/

// This function prints the welcome screen of the game.
void printWelcomeScreen()
{
    printf("************  Welcome to TAKI game !!! ***********\n");
}

// This function scans the number of the players in the game.
void readNumberOfPlayers(game* pUtility)
{
    printf("Please enter the number of players:\n");
    scanf("%d", &pUtility->numOfPlayers);
}

// This function scans the players names and hands out 4 cards for each player.
player* getPlayers(game* pUtility)
{
    int i, size = pUtility->numOfPlayers;
    player* p;

    p = (player*)malloc(size * sizeof(player));
    checkMalloc(p);

    for (i = FIRST_INDEX; i < size; i++)
    {
        readPlayerName(p[i].name, i + 1);
        allocateCardArray(&p[i]);
        handOutStartingCards(p[i].cards, pUtility);
    }

    return p;
}

// This function is the turn of one player.
void playerTurn(player* pPlayer, game* pUtility)
{
    int choice;

    printf("Upper card:\n");
    printCard(pUtility->upperCard);

    printPlayerCards(*pPlayer);
    readPlayerChoice(pPlayer->numberOfCards, &choice);
    checkIfChoiceIsValid(&choice, pPlayer->numberOfCards, pUtility->upperCard, pPlayer->cards, NOT_TAKI);

    if (choice == TAKE_CARD_FROM_DECK)
        getCardFromTheDeck(pPlayer, pUtility);
    else if (pPlayer->cards[choice - 1].typeID >= PLUS_CARD_ID) // if it's a special card
        playSpecialCard(pPlayer, pUtility, (choice - 1));
    else
    {
        pUtility->upperCard = (*pPlayer).cards[choice - 1];
        removeCardFromDeck((*pPlayer).cards, &(*pPlayer).numberOfCards, choice - 1);
    }

    if ((*pPlayer).numberOfCards == NO_CARDS)
    {
        if ((pUtility->upperCard.typeID == PLUS_CARD_ID) || (pUtility->numOfPlayers == TWO_PLAYERS && pUtility->upperCard.typeID == STOP_CARD_ID))
        {
            pPlayer->cards[FIRST_INDEX] = getNewCard(getRandomNumberForCardType(), pUtility);
            pPlayer->numberOfCards = FIRST;
        }
        else
            pUtility->exit = true;
    }
}

// This function gets a name and prints it as the winner of the game.
void printWinnerName(char name[])
{
    printf("The winner is... %s! Congratulations!\n", name);
}

// This function initialize the game utility.
void initializeGameUtility(game* pUtility)
{
    initializeTable(pUtility->counter, NUM_OF_TOTAL_TYPES_OF_CARDS);
    pUtility->upperCard = getNewCard(getRandomNumberForUpperCardType(), pUtility);
    pUtility->changeDirection = false;
    pUtility->exit = false;
    pUtility->playerIndex = FIRST_INDEX;
}

// This function gets a two dim array and prints it.
// Assumption: the array is sorted in ascending.
void printStatisticTable(int arr[][COLS], int size)
{
    int i, len;

    printHeadOfTheTable();

    for (i = size - 1; i >= FIRST_INDEX; i--)
    {
        char* type = convertCardIdToString(arr[i][FIRST_INDEX]);
        len = strlen(type);

        switch (len) {
        case 1:
            printf("   %-3s %s%5d\n", type, "|", arr[i][SECOND_INDEX]);
            break;
        case 3:
            printf("  %-4s %s%5d\n", type, "|", arr[i][SECOND_INDEX]);
            break;
        default:
            printf("%-5s  %s%5d\n", type, "|", arr[i][SECOND_INDEX]);
            break;

        }
    }
}

/*************************** game functions *******************************/

// This function gets the player name from the user and return it via an output string.
void readPlayerName(char name[], int num)
{
    printf("Please enter the first name of player #%d\n", num);
    scanf("%s", name);
}

// This function hands out the starting cards for a player.
void handOutStartingCards(card* cards, game* pUtility)
{
    int i;

    for (i = FIRST_INDEX; i < NUM_OF_STARTING_CARDS; i++)
        cards[i] = getNewCard(getRandomNumberForCardType(), pUtility);
}

// This function scans a player choice for card during his turn.
// The choice saves in an output variable.
void readPlayerChoice(int numOfCards, int* pChoice)
{
    printf("Please enter %d if you want to take a card from the deck\n", TAKE_CARD_FROM_DECK);
    printf("or %d-%d if you want to put one of your cards in the middle:\n", FIRST, numOfCards);
    scanf("%d", pChoice);
}

// This function scans a player choice for card during a TAKI "waterfall".
// The choice saves in an output variable.
void readPlayerChoiceForTAKI(int numOfCards, int* pChoice)
{
    printf("Please enter %d if you want to finish your turn\n", TAKE_CARD_FROM_DECK);
    printf("or %d-%d if you want to put one of your cards in the middle:\n", FIRST, numOfCards);
    scanf("%d", pChoice);
}

// This function create a new card for the player and update his number of cards.
void getCardFromTheDeck(player* p, game* pUtility)
{
    if (p->numberOfCards == p->physSize)
    {
        p->cards = cardRealloc(p->cards, p->numberOfCards, (p->numberOfCards) * 2); // cardRealloc function includes an allocation check.
        p->physSize = (p->numberOfCards) * 2;
    }
    p->cards[p->numberOfCards] = getNewCard(getRandomNumberForCardType(), pUtility);
    (p->numberOfCards)++;
}

// This function is used when the player wants to play with a special card (not numbers).
// The function converts the card type id to the "special power action" of the card.
void playSpecialCard(player* pPlayer, game* pUtility, int cardIndex)
{
    int type;

    if (cardIndex == GARBAGE_INDEX) // if it's an action from TAKI card action
        type = pUtility->upperCard.typeID;
    else
        type = pPlayer->cards[cardIndex].typeID;

    switch (type)
    {
    case 9: // PLUS CARD
        plusCardAction(pPlayer, pUtility, cardIndex);
        break;
    case 10: // CHANGE DIRECTION CARD
        changeDirectionAction(pPlayer, pUtility, cardIndex);
        break;
    case 11: //STOP CARD
        stopCardAction(pPlayer, pUtility, cardIndex);
        break;
    case 12: //COLOR CARD
        colorCardAction(pPlayer, pUtility, cardIndex);
        break;
    case 13: // TAKI CARD
        takiCardAction(pPlayer, pUtility, cardIndex);
        break;
    }
}


/*************************** card functions ******************************/


// The function makes a new card and returns it.
card getNewCard(int type, game* utility)
{
    card c;
    c.typeID = type;

    if (c.typeID != COLOR_CARD_ID) // give the card a color if it's not a COLOR card
        getCardColor(&c.color, getRandomNumberForCardColor());
    else
        c.color = NO_COLOR;

    utility->counter[type][SECOND_INDEX]++;

    return c;
}

// This function gets a card and prints it.
void printCard(card c)
{
    int i;

    for (i = FIRST; i <= CARD_LENGTH; i++)
    {
        if (i == FIRST || i == CARD_LENGTH)
            printFullLine();
        else if (i == CARD_TYPE_ROW)
            printCardTypeLine(c.typeID);
        else if (i == CARD_COLOR_ROW)
            printCardColorLine(c.color);
        else
            printEmptyLine();

        printf("\n");
    }
    printf("\n");
}

// This function gets the player's cards array and its size.
// The function delete the wanted card from the access of the player
void removeCardFromDeck(card* arr, int* pSize, int cardIndex)
{
    swapCards((arr + cardIndex), (arr + (*pSize) - 1));
    (*pSize)--;
}

// This function is the action of the "PLUS" card (+).
void plusCardAction(player* pPlayer, game* pUtility, int cardIndex)
{
    if (cardIndex != GARBAGE_INDEX) // if it's not an action from TAKI card action
    {
        pUtility->upperCard = (*pPlayer).cards[cardIndex];
        removeCardFromDeck((*pPlayer).cards, &(*pPlayer).numberOfCards, cardIndex);
    }

    if (!(pUtility->changeDirection))
        (pUtility->playerIndex)--;
    else
        (pUtility->playerIndex)++;
}

// This function is the action of the "Change Direction" card (<->).
void changeDirectionAction(player* pPlayer, game* pUtility, int cardIndex)
{
    if (cardIndex != GARBAGE_INDEX) // if it's not an action from TAKI card action
    {
        pUtility->upperCard = pPlayer->cards[cardIndex];
        removeCardFromDeck(pPlayer->cards, &(pPlayer->numberOfCards), cardIndex);
    }

    pUtility->changeDirection = !(pUtility->changeDirection);
}

// This function is the action of the "STOP" card.
void stopCardAction(player* pPlayer, game* pUtility, int cardIndex)
{
    if (cardIndex != GARBAGE_INDEX) // if it's not an action from TAKI card action
    {
        pUtility->upperCard = pPlayer->cards[cardIndex];
        removeCardFromDeck(pPlayer->cards, &(pPlayer->numberOfCards), cardIndex);
    }

    if (pUtility->playerIndex == pUtility->numOfPlayers - 1) // If it's the last player in the array
    {
        if (!((pUtility->changeDirection)))
            pUtility->playerIndex = FIRST_INDEX;
        else
            pUtility->playerIndex--;
    }
    else if (pUtility->playerIndex == FIRST_INDEX) // If it's the first player in the array
    {
        if (pUtility->changeDirection)
            pUtility->playerIndex = pUtility->numOfPlayers - 1;
        else
            pUtility->playerIndex++;
    }
    else // If it's a player from the middle of the array
    {
        if (!(pUtility->changeDirection))
            pUtility->playerIndex++;
        else
            pUtility->playerIndex--;
    }

}

// This function is the action of the "COLOR" card.
void colorCardAction(player* pPlayer, game* pUtility, int cardIndex)
{
    int colorID;

    printColorCardMenu();
    colorID = getAndCheckColorChoice();

    getCardColor(&pUtility->upperCard.color, colorID);
    pUtility->upperCard.typeID = COLOR_CARD_ID;

    if (cardIndex != GARBAGE_INDEX) // if it's not an action from TAKI card action
        removeCardFromDeck((*pPlayer).cards, &(*pPlayer).numberOfCards, cardIndex);
}

// This function is the action of the "TAKI" card.
void takiCardAction(player* pPlayer, game* pUtility, int cardIndex)
{
    int choice = cardIndex + 1;

    while (choice != TAKE_CARD_FROM_DECK && pPlayer->numberOfCards > NO_CARDS)
    {
        pUtility->upperCard = pPlayer->cards[choice - 1];
        removeCardFromDeck(pPlayer->cards, &(pPlayer->numberOfCards), choice - 1);

        printf("Upper card:\n");
        printCard(pUtility->upperCard);
        printPlayerCards(*pPlayer);
        readPlayerChoiceForTAKI(pPlayer->numberOfCards, &choice);
        checkIfChoiceIsValid(&choice, pPlayer->numberOfCards, pUtility->upperCard, pPlayer->cards, !NOT_TAKI);

        if (choice == TAKE_CARD_FROM_DECK)
        {
            if (pUtility->upperCard.typeID >= PLUS_CARD_ID && pUtility->upperCard.typeID < COLOR_CARD_ID)
                playSpecialCard(pPlayer, pUtility, GARBAGE_INDEX);
        }
        else if (pPlayer->cards[choice - 1].typeID == COLOR_CARD_ID)
        {
            colorCardAction(pPlayer, pUtility, choice - 1);
            choice = TAKE_CARD_FROM_DECK;
        }
        else if (pPlayer->numberOfCards - 1 == NO_CARDS)
        {
            // if this is the last card of the player, and it's a PLUS card or there are 2 players, and it's a STOP card
            if (pPlayer->cards[FIRST_INDEX].typeID == PLUS_CARD_ID || (pUtility->numOfPlayers == TWO_PLAYERS && pPlayer->cards[FIRST_INDEX].typeID == STOP_CARD_ID))
            {
                removeCardFromDeck(pPlayer->cards, &(pPlayer->numberOfCards), FIRST_INDEX);
                getCardFromTheDeck(pPlayer, pUtility);
                choice = TAKE_CARD_FROM_DECK;
            }
            else
            {
                choice = TAKE_CARD_FROM_DECK;
                pUtility->exit = true;
            }
        }
    }
}

////// Auxiliary functions for card functions //////

// This function gets an integer number and return a color by the number with an output char.
void getCardColor(char* pColor, int num)
{
    switch (num) {
    case 0:
        *pColor = YELLOW_COLOR;
        break;
    case 1:
        *pColor = RED_COLOR;
        break;
    case 2:
        *pColor = BLUE_COLOR;
        break;
    case 3:
        *pColor = GREEN_COLOR;
        break;
    }
}

// This function returns a random number between 0 and 13 (include) (for general card type)
int getRandomNumberForCardType()
{
    return (rand() % 14);
}

// This function returns a random number between 0 and 8 (include) (for upper card type)
int getRandomNumberForUpperCardType()
{
    return rand() % 9;
}

// This function returns a random number between  0 and 3 (include) (for card color)
int getRandomNumberForCardColor()
{
    return rand() % 4;
}

// This function gets two cards and exchange the values of the two.
void swapCards(card* a, card* b)
{
    card temp = *a;
    *a = *b;
    *b = temp;
}

// This function gets a card id (a number between 0 - 13)
// and return a string with the card type it represent.
// Assumption: The integer number the function gets is only a number between 0 and 13 include.
char* convertCardIdToString(int id)
{
    switch (id) {
    case 0:
        return ONE_CARD;
    case 1:
        return TWO_CARD;
    case 2:
        return THREE_CARD;
    case 3:
        return FOUR_CARD;
    case 4:
        return FIVE_CARD;
    case 5:
        return SIX_CARD;
    case 6:
        return SEVEN_CARD;
    case 7:
        return EIGHT_CARD;
    case 8:
        return NINE_CARD;
    case 9:
        return PLUS_CARD;
    case 10:
        return CHANGE_DIRECTION_CARD;
    case 11:
        return STOP_CARD;
    case 12:
        return COLOR_CARD;
    case 13:
        return TAKI_CARD;
    }
}

// This function is an auxiliary function to the "COLOR" card action function.
// The function prints a color menu to the player to choose the color he wants.
void printColorCardMenu()
{
    printf("Please enter your color choice:\n");
    printf("1 - Yellow\n");
    printf("2 - Red\n");
    printf("3 - Blue\n");
    printf("4 - Green\n");
}

// This function is an auxiliary function to the "COLOR" card action function.
// The function scan a color choice from the player and keep scanning if the choice is not valid.
int getAndCheckColorChoice()
{
    int colorID;

    scanf("%d", &colorID);

    while (colorID < FIRST || colorID > NUM_OF_COLORS)
    {
        printf("Invalid color! Try again.\n");
        printColorCardMenu();
        scanf("%d", &colorID);
    }

    return colorID - 1; // match choice to card ID
}

// This function gets a card array and its logic size.
// The function allocate a new size card array.
// If the allocation succeed, the function copy the data from the old array to the new one.
// After the copying the function free the old array allocation.
// If the allocation fails, the program finishes.
card* cardRealloc(card* c, int size, int newSize)
{
    int i;
    card* newArr = (card*)malloc(sizeof(card) * newSize);

    checkMalloc(newArr); // if the malloc function fails, the program finishes.

    for (i = 0; i < size && i < newSize; i++)
        newArr[i] = c[i];

    free(c);

    return newArr;
}


///////////// print functions for card /////////////

// This function prints the line of the type in the card.
void printCardTypeLine(int type)
{
    char* cardType = convertCardIdToString(type);

    int len = strlen(cardType);
    int half = (CARD_WIDTH - len) / 2;

    printHalfLine(half, FIRST);
    printf("%s", cardType);
    printHalfLine(half, half);
}

// This function prints the line of the color in the card.
void printCardColorLine(char c)
{
    int i;

    for (i = FIRST; i <= CARD_WIDTH; i++)
    {
        if (i == FIRST || i == CARD_WIDTH)
            printf("*");
        else if (i == MIDDLE)
            printf("%c", c);
        else
            printf(" ");

    }
}

// This function prints an empty line in the card.
void printEmptyLine()
{
    int i;

    for (i = FIRST; i <= CARD_WIDTH; i++)
    {
        if (i == FIRST || i == CARD_WIDTH)
            printf("*");
        else
            printf(" ");
    }
}

// This function prints a half line of a card.
// The function gets a stop integer- which is the stop condition for the for loop (the half of the card).
// The function gets a starCondition- which is in which side to print the star.
void printHalfLine(int stop, int starCondition)
{
    int i;

    for (i = FIRST; i <= stop; i++) {
        if (i == starCondition)
            printf("*");
        else
            printf(" ");
    }
}

// This function prints a line full of * in the card.
void printFullLine()
{
    int i;
    for (i = FIRST; i <= CARD_WIDTH; i++)
        printf("*");
}

// This function gets a player and prints his cards.
void printPlayerCards(player p)
{
    int i;

    printf("%s's turn:\n", p.name);

    for (i = FIRST_INDEX; i < p.numberOfCards; i++)
    {
        printf("Card #%d\n", i + 1);
        printCard(p.cards[i]);
    }
}


/*********************** general auxiliary functions ***********************/

// This function gets a pointer to a player and allocates a card array to the player.
// The function is an auxiliary function to the getPlayers function.
// The size of the allocated array is the number of the starting cards.
void allocateCardArray(player* p)
{
    p->cards = (card*)malloc(sizeof(card) * NUM_OF_STARTING_CARDS);
    checkMalloc(p);
    p->numberOfCards = NUM_OF_STARTING_CARDS;
    p->physSize = NUM_OF_STARTING_CARDS;
}

// This function gets a pointer p to an allocated memory,
// and check if the malloc function allocated the memory successfully.
// If the malloc function failed (p = NULL), the program finishes.
void checkMalloc(void* p)
{
    if (p == NULL)
    {
        printf("Error! Memory allocation faild!\n");
        exit(1);
    }
}

// This function gets two cards and compare their values.
// The function returns true if the card types are equals or the colors are equals, else false.
bool isCardValid(card upper, card check, bool isTaki)
{
    if (!isTaki)
        return (upper.color == check.color || upper.typeID == check.typeID || check.typeID == COLOR_CARD_ID);
    else
        return (upper.color == check.color || check.typeID == COLOR_CARD_ID);
}

// This function gets a number and a limit number.
// The function returns true if the number between one and the limit (include), else false.
bool isChoiceNumberValid(int num, int lim)
{
    return (num >= 1 && num <= lim);
}

// This function checks if the player's choice for a card is valid.
// If the choice is invalid, the function will keep scan a choice.
void checkIfChoiceIsValid(int* pChoice, int numberOfCards, card upperCard, card* arr, bool isTaki)
{
    while ((*pChoice != TAKE_CARD_FROM_DECK) && ((!isChoiceNumberValid(*pChoice, numberOfCards)) || (!isCardValid(upperCard, arr[(*pChoice) - 1], isTaki))))
    {
        printf("Invalid card! Try again.\n");
        readPlayerChoice(numberOfCards, pChoice);
    }
}

// This function gets a player array and its size.
// The function free each player's allocated card array,
// and free the player allocated array.
void freeAllPlayersCardsArray(player* pArr, int size)
{
    int i;

    for (i = FIRST_INDEX; i < size; i++)
    {
        free(pArr[i].cards);
    }

    free(pArr);
}

// This function gets a two dim counter array, and sorts it.
// Assumption: the size of the cols is COLS, the size of rows is NUM_OF_TOTAL_TYPES_OF_CARDS
void sortCounterArray(int counter[][COLS], int size)
{
    if (size <= FIRST)
        return;
    else
    {
        int(*tempArr)[COLS] = malloc(size * COLS * sizeof(int));

        sortCounterArray(counter, size / 2);
        sortCounterArray(counter + size / 2, size - size / 2);

        merge(counter, size / 2, counter + size / 2, size - size / 2, tempArr);
        copyArr(tempArr, counter, size);

        free(tempArr);
    }
}

// This function gets 2 sorted arrays and return a merge sorted array.
// Assumption: the size of the cols is 2.
void merge(int arr1[][COLS], int size1, int arr2[][COLS], int size2, int res[][COLS])
{
    int ind1 = FIRST_INDEX, ind2 = FIRST_INDEX, writeIndex = FIRST_INDEX;

    while (ind1 < size1 && ind2 < size2)
    {
        if (arr1[ind1][SECOND_INDEX] < arr2[ind2][SECOND_INDEX])
        {
            res[writeIndex][FIRST_INDEX] = arr1[ind1][FIRST_INDEX];
            res[writeIndex][SECOND_INDEX] = arr1[ind1][SECOND_INDEX];
            ind1++;
        }
        else
        {
            res[writeIndex][FIRST_INDEX] = arr2[ind2][FIRST_INDEX];
            res[writeIndex][SECOND_INDEX] = arr2[ind2][SECOND_INDEX];
            ind2++;
        }

        writeIndex++;
    }

    while (ind1 < size1)
    {
        res[writeIndex][FIRST_INDEX] = arr1[ind1][FIRST_INDEX];
        res[writeIndex][SECOND_INDEX] = arr1[ind1][SECOND_INDEX];
        ind1++;
        writeIndex++;
    }

    while (ind2 < size2)
    {
        res[writeIndex][FIRST_INDEX] = arr2[ind2][FIRST_INDEX];
        res[writeIndex][SECOND_INDEX] = arr2[ind2][SECOND_INDEX];
        ind2++;
        writeIndex++;
    }
}

// This function copy src two dim array to dest two dim array.
// Assumption: the size of the cols is 2.
void copyArr(int src[][COLS], int dest[][COLS], int size)
{
    int i;

    for (i = FIRST_INDEX; i < size; i++)
    {
        dest[i][FIRST_INDEX] = src[i][FIRST_INDEX];
        dest[i][SECOND_INDEX] = src[i][SECOND_INDEX];
    }
}

// This function prints the head of the game statistics table.
void printHeadOfTheTable()
{
    printf("************ Game Statistics ************\n");
    printf("Card # | Frequency\n");
    printf("__________________\n");
}

// This function gets a two dim array and initialize it.
// The first col (0) the type ID col.
// The second col (1) is a counter col.
// Assumption: The size of the cols of the array is 2.
void initializeTable(int table[][COLS], int size)
{
    int i;

    for (i = FIRST_INDEX; i < size; i++)
    {
        table[i][FIRST_INDEX] = i;
        table[i][SECOND_INDEX] = NO_CARDS;
    }
}
