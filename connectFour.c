/*
 * File:   connect.c
 * Author: Daniel Kaichis
 * Purpose: To play a game of connect four with one user against a computer player
 *          or another user. Players will choose a game mode they would like to 
 *          play and then select an uppercase character from A-Z they would like to
 *          represent their pieces on the board. Players will take turns placing
 *          pieces until one player connects 4 pieces in a row vertically, horizontally
 *          or diagonally, or the game board is filled and the game ends in a tie.
 * Status:  DONE
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// horizontal number of columns in the gameboard
const int numColumns = 7;
// vertical number of rows in the gameboard
const int numRows = 6;

/*
 * Purpose:
 *    To set every value of the game board array to O's to represent spaces on the board.
 * Parameters:
 *     gameBoard - the 2D array that stores every value in the game board
 * Returns:
 *      NONE
 * Side-Effects: The game board array is modified to set every index to a default value
 */
void setGameBoard(char gameBoard[numColumns][numRows]) {
    // counter for the index of the rows
    int i = 0;
    // counter for the index of the columns
    int j = 0;
    
    // loop through the rows starting at row 0
    while (i < numRows) {
        // loop through each column and set the default value to 'O'
        for (j = 0; j < numColumns; j++) {
            gameBoard[j][i] = 'O';
        }
        // increment the row counter by 1 to go up a row and continue setting
        // default values to each index.
        i++;
    // end of loop setting default values
    }
}

/*
 * Purpose:
 *    To print the game instructions
 * Parameters:
 *      NONE
 * Returns:
 *      NONE
 * Side-Effects: 
 *      NONE
 */
void printInstructions() {
    // print the game rules and instructions
    printf("Welcome to Connect Four!\n");
    printf("The rules are simple. Drop your pieces into columns trying to get 4\n");
    printf("in a row either vertically, horizontally, or diagonally, but be careful,\n");
    printf("your goal is to connect four before your opponent does so your defense\n");
    printf("must also be considered. Each player will alternate dropping pieces and\n");
    printf("the game board will be printed after every move until the game ends.\n");
    printf("The game ends when one player gets four in a row or the board\n");
    printf("is full in which the game ends in a tie. Don't forget to have fun!\n");
    printf("\n");
}

/*
 * Purpose:
 *      To validate the users inputted game mode
 * Parameters:
 *      gameMode - the variable that stores the users input on what gameMode they want to
 *      play as a character
 * Returns:
 *      gameMode - the validated gameMode character
 * Side-Effects: 
 *      NONE
 */
char validateGamemode(char gameMode) {
    // loop asking for the user to enter a new game mode until their input is a valid game mode
    // u - for user, c - for computer, q - for quit
    while ((gameMode != 'u') && (gameMode != 'c') && (gameMode != 'q')) {
        printf("Your choice of '%c' is invalid.\n", gameMode);
        printf("Would you like to play against another user or the computer?\n");
        printf("Please enter u for user, c for computer, or q to quit: ");
        scanf("%c", &gameMode);
        fgetc(stdin);
    }

    return gameMode;
}

/*
 * Purpose:
 *      To get the user to enter the game mode they would like to play
 * Parameters:
 *      NONE
 * Returns:
 *      gameMode - the game mode the user wants to play
 * Side-Effects:
 */
char selectGamemode() {
    // variable to store the default value for the gameMode
    char gameMode = 'x';
    
    // ask the user to enter the gamemode they would like to play
    printf("Would you like to play against another user or the computer?\n");
    printf("Please enter u for user, c for computer or q to quit: ");
    scanf("%c", &gameMode);
    fgetc(stdin);
    // validate the gamemode
    gameMode = validateGamemode(gameMode);

    return gameMode;
}

/*
 * Purpose:
 *      To validate the character the user wants to use to represent their pieces
 * Parameters:
 *      playerChar - the character to represent the player on the game board
 * Returns:
 *      playerChar - the validated character to represent the player on the game board
 * Side-Effects:
 *      NONE
 */     
char validateChar(char playerChar, int player, const char gameMode, const char* playerPronoun) {
   
    // while the user enters a character that is not uppercase from A - Z, 
    // and is not the default value of 'O', prompt the user to re enter the character
    while ((playerChar < 'A') || (playerChar > 'Z') || (playerChar == 'O')) {
        printf("Your input of '%c' is invalid.\n", playerChar);
        printf("\n");
        printf("Player %d: Please enter an uppercase character you\n", player);
        printf("would like to use to represent %s pieces [A-Z]: ", playerPronoun);
        scanf( "%c", &playerChar);
        fgetc(stdin);
    }
    
    return playerChar;
}

/*
 * Purpose:
 *      To get the user to input a character the user would like to represent their pieces
 * Parameters:
 *      player - the number value given to each player
 *      gameMode - the gameMode the user is playing to determine the pronoun to address the player as
 * Returns:
 *      userChar - the character the user will use to represent their pieces on the board
 * Side-Effects:
 *      NONE
 */
char getPlayerChar(int player, const char gameMode) {
    // set default value for the players character
    char playerChar = 'c';
    // default player pronoun for a real user
    char* playerPronoun = "your";
    
    // if the user is playing against the computer (which is player 2), set the 
    // player pronoun to "the computers" to output an appropriate prompt.
    if ((gameMode == 'c') && (player == 2)) {
        playerPronoun = "the computer's";
        player = 1;
    }
    // ask player to enter
    printf("Player %d: Please enter an uppercase character you\n", player);
    printf("would like to use to represent %s pieces [A-Z]: ", playerPronoun);
    scanf( "%c", &playerChar);
    fgetc(stdin);
    // validate the users character
    playerChar = validateChar(playerChar, player, gameMode, playerPronoun);
    
    // return the validated userChar
    return playerChar;
}

/*
 * Purpose:
 *      To validate the column the user plays their piece in is from 1-7 (inclusive)
 *      because those are the possible columns in the game board
 * Parameters:
 *      columnSlot - the column the user would like to put their piece in
 *      gameBoard - the 2D array of the game board to validate that the
 *      row the user is putting their piece in is not full
 * Returns:
 *      columnSlot - the validated column the user places their pieces in
 * Side-Effects:
 *      NONE - gameboard is const so it is not modified
 */
int validateUserPlacement(int columnSlot, const char gameBoard[][numRows]) {
    // while the users input is not in between 1-7 (the valid columns) prompt 
    // the user to reenter the column they would like to place their pieces.
    while ((columnSlot > 7) || (columnSlot < 1)) {
        printf("Your entry of column %d is inavlid.\n", columnSlot);
        printf("Please enter a new column you would like to drop your piece in [1-7]: ");
        scanf("%d", &columnSlot);
        fgetc(stdin);
    }
    // while they have entered a column that is full (the value is not the default
    // at that index), ask them to choose a new column
    while (gameBoard[columnSlot - 1][5] != 'O') {
        printf("Column %d is full.\n", columnSlot);
        printf("Please enter a new column you would like to drop your piece in [1-7]: ");
        scanf("%d", &columnSlot);
        fgetc(stdin);
    }

    return columnSlot;
}

/*
 * Purpose:
 *      To place the users piece in the column they have selected at the "lowest" available row
 * Parameters:
 *      gameBoard - the 2D array to be modified when the users piece is placed
 *      userChar - the character the user chose to represent their pieces to 
 *      be "dropped" into the gameboard
 *      columnSlot - the column the user wants to play their piece in
 * Returns:
 *      NONE
 * Side-Effects:
 *      The gameboard is modified with the users piece placement
 */
void placepiece( char gameBoard[numColumns][numRows], const char userChar, const int columnSlot) {
    // variable to store the row the users piece will be placed into
    int rowSlot = 0;
    
    // starting from the bottom row (index 0), while the value is not the default
    // and the upper bound has not been reached, the loop will increment up row by row,
    // until it finds an index pair (column and row) in the gameboard that is the default
    // value of 'O'. The column slot is subtracted by 1 because the user is presented
    // with choices from 1-7 while the computers indexes for the columns are 0-6
    while ((gameBoard[columnSlot - 1][rowSlot] != 'O') && (rowSlot < 6)) {
        rowSlot++;
    }
    // set the value at the users column slot and the lowest empty row slot to
    // the character the user chose to represent their pieces.
    gameBoard[columnSlot - 1][rowSlot] = userChar;
}

/*
 * Purpose:
 *      To check if a player has won the game
 * Parameters:
 *      gameBoard - the gameboard array to be checked to see if either player has 4 in a row
 *      userChar - the character of the user to check if that user has 4 in a row
 * Returns:
 *      winGame - the variable storing if a player has won or not
 * Side-Effects:
 *      NONE - the gameboard array is not modified because it is a const
 */         
int checkWinGame(const char gameBoard[numColumns][numRows], const char userChar) {
    // the variable to store if a player has won or not
    int winGame = 0;
    // the counter for the index of the row
    int i = 0;
    // the counter for the index of the column
    int j = 0;
    
    // loop through every index of the game board to check if the player got 4 in a row
    while (i < numRows) {
        // iterate across each column to find the player pieces
        for (j = 0; j < numColumns; j++) {
            if (gameBoard[j][i] == userChar) {
                // if the 3 pieces above a players piece are also its piece, or if
                // the 3 pieces to the right of the players piece (no need to check left because
                // the furthest left game piece will be the fist to be checked by the loop)
                // are also its piece, or if the 3 pieces diagonally right of
                // the players piece are also its piece, or if the 3 pieces 
                // diagonally left of the players piece are also its piece. All these
                // checks also include checks of the constraints to make sure only
                // values within the gameboard are checked. For example it is impossible
                // to win with 4 in a row vertically if the bottom piece is not below
                // the 3rd row from the top.
                if (((gameBoard[j][i + 1] == userChar) && (gameBoard[j][i + 2] == userChar) && (gameBoard[j][i + 3] == userChar) && (i < 3)) || ((gameBoard[j + 1][i] == userChar) && (gameBoard[j + 2][i] == userChar) && (gameBoard[j + 3][i] == userChar) && (j < 4)) || ((gameBoard[j + 1][i + 1] == userChar) && (gameBoard[j + 2][i + 2] == userChar) && (gameBoard[j + 3][i + 3] == userChar) && (j < 4) && (i < 3)) || ((gameBoard[j - 1][i + 1] == userChar) && (gameBoard[j - 2][i + 2] == userChar) && (gameBoard[j - 3][i + 3] == userChar) && (j > 2) && (i < 3))) {
                    // a player has won
                    winGame = 1;
                    break;
                }
            }
        }
        // if the game has been won, break the loop as there is no need to continue
        // checking the array
        if (winGame) {
            break;
        // the game has not been won so continue iterating through the array checking
        // if a player has won
        } else {
            i++;
        }
    // end of while loop. A player did not win
    }

    return winGame;
}

/*
 * Purpose:
 *      For the player to pick a place to play their piece and then place it by changing 
 *      the default value in the game board array to the players chosen character
 * Parameters:
 *      userChar - the character representing the users piece
 *      gameBoard - the game board array
 * Returns:
 *      NONE
 * Side-Effects:
 *      NONE - the gameboard array is modified in the place piece function when it is called
 */
void playerTurn(const char userChar, char gameBoard[numColumns][numRows], const int player) {
    int columnSlot = 0;
    // ask user what column they would like to play their piece into
    printf("\nPlayer %d: Which column would you like to drop your piece into? [1-7]: ", player);
    scanf("%d", &columnSlot);
    fgetc(stdin);
    // validate the users column is within the game board and is not full
    columnSlot = validateUserPlacement(columnSlot, gameBoard);
    // place the users piece at the lowest available slot in their selected column
    placepiece(gameBoard, userChar, columnSlot);
}

/*
 * Purpose:
 *      To check if the computer or player has three in a row and if the computer
 *      can either win or prevent the player from winning
 * Parameters:
 *      gameBoard - the game board array
 *      computerChar - the character representing the computers pieces
 *      checkChar - the character for the player that the programming is checking
 *      if they have 3 in a row
 * Returns:
 *      playColumn - the column the computer played its pieces in to be outputted
 *      to the other player
 * Side-Effects:
 *      The gameBoard array is modified wherever the computer plays its piece to win the game
 *      or to prevent the user from winning. 
 */
int threeInARow(char gameBoard[numColumns][numRows], const char computerChar, const char checkChar) {
    // the counter for the row index
    int i = 0;
    // the counter for the column index
    int j = 0;
    // a default value for the play column that is not a valid column so if 
    // this function does not find a way for the computer to win, the computer turn
    // function will move on to the next function to try to find a good move for the 
    // computer to play
    int playColumn = 8;
    
    // this loop will iterate through the entire array, column by column, row by row,
    // to check if the computer can get 4 in a row to win, or if it can prevent the 
    // player from getting four in a row.
    while (i < numRows){
        // iterate across each column to find the player/computer pieces (the character representing them)
        for (j = 0; j < numColumns; j++) {
            // if the loop gets to a character that is the users/opponent, check if
            // it can win vertically, horizontally, or diagonally
            if (gameBoard[j][i] == checkChar) {
            // for the following a space is defined as available if the slot
            // below it is the bottom of the board or either the player or opponents piece
                
            // for each of the following, constraints are put on the j and i counter
            // to make sure the piece can be played within the gameboard to win
            // or prevent the opponent from winning
                
                // the two pieces directly above the players piece are also the players 
                // piece and the space above that is available for a piece to be played in
                if ((gameBoard[j][i + 1] == checkChar) && (gameBoard[j][i + 2] == checkChar) && (gameBoard[j][i + 3] == 'O') && (i < 3)) {
                    // play winning/defending piece
                    gameBoard[j][i + 3] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j;
                    break;
                    
                // the two pieces to the right of the players piece are also the players
                // piece and the third space to the right is available for a piece to be played in
                } else if ( ((gameBoard[j + 1][i] == checkChar) && (gameBoard[j + 2][i] == checkChar)) && (gameBoard[j + 3][i] == 'O') && ((gameBoard[j + 3][i - 1] != 'O') || (i == 0)) && (j < 5)) {
                    // play winning/defending piece
                    gameBoard[j + 3][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 3;
                    break;
                    
                // the two pieces to the right of the players piece are also the players
                // piece and the first space to the left is available for a piece to be played in
                } else if (((gameBoard[j + 1][i] == checkChar) && (gameBoard[j + 2][i] == checkChar)) && (gameBoard[j - 1][i] == 'O') && ((gameBoard[j - 1][i - 1] != 'O') || (i == 0)) && (j > 0)) {
                    // play winning/defending piece
                    gameBoard[j - 1][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    break;
                    
                // the second and third piece to the right of the players piece are also the players
                // piece and the first space to the right is available for a piece to be played in
                } else if ((gameBoard[j + 1][i] == 'O') && (gameBoard[j + 2][i] == checkChar) && (gameBoard[j + 3][i] == checkChar) && ((gameBoard[j + 1][i - 1] != 'O') || (i == 0)) && (j < 5)) {
                    // play winning/defending piece
                    gameBoard[j + 1][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    break;
                    
                // the first and third piece to the right of the players piece are also the players
                // piece and the second space to the right is available for a piece to be played in
                } else if ((gameBoard[j + 2][i] == 'O') && (gameBoard[j + 1][i] == checkChar) && (gameBoard[j + 3][i] == checkChar) && ((gameBoard[j + 2][i - 1] != 'O') || (i == 0)) && (j < 5)) {
                    // play winning/defending piece
                    gameBoard[j + 2][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 2;
                    break;
                    
                // the two pieces diagonally upward to the right of the players piece are also the players
                // piece and the third space diagonally upward to the right is available for a piece to be played in
                } else if (((gameBoard[j + 1][i + 1] == checkChar) && (gameBoard[j + 2][i + 2] == checkChar)) && (gameBoard[j + 3][i + 3] == 'O') && (gameBoard[j + 3][i + 2] != 'O') && (j < 4) && (i < 3)) {
                    // play winning/defending piece
                    gameBoard[j + 3][i + 3] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 3;
                    break;
                    
                // the two pieces diagonally upward to the right of the players piece are also the players
                // piece and the first space diagonally downward to the left is available for a piece to be played in
                } else if ( ((gameBoard[j + 1][i + 1] == checkChar) && (gameBoard[j + 2][i + 2] == checkChar)) && (gameBoard[j - 1][i - 1] == 'O') && ((gameBoard[j - 1][i - 2] != 'O') || (i == 1)) && (j > 0) && (j < 5) && (i < 4) && (i > 0)) {
                    // play winning/defending piece
                    gameBoard[j - 1][i - 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    break;
                    
                // the second and third pieces diagonally upward to the right of the players piece are also the players
                // piece and the first space diagonally upward to the right is available for a piece to be played in
                } else if ((gameBoard[j + 1][i + 1] == 'O') && (gameBoard[j + 2][i + 2] == checkChar) && (gameBoard[j + 3][i + 3] == checkChar) && (gameBoard[j + 1][i] != 'O') && (j < 4) && (i < 3)) {
                    // play winning/defending piece
                    gameBoard[j + 1][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    break;
                    
                // the first and third pieces diagonally upward to the right of the players piece are also the players
                // piece and the second space diagonally upward to the right is available for a piece to be played in
                } else if ((gameBoard[j + 2][i + 2] == 'O') && (gameBoard[j + 1][i + 1] == checkChar) && (gameBoard[j + 3][i + 3] == checkChar) && (gameBoard[j + 2][i + 1] != 'O') && (j < 4) && (i < 3)) {
                    // play winning/defending piece
                    gameBoard[j + 2][i + 2] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 2;
                    break;
                    
                // the two pieces diagonally upward to the left of the players piece are also the players
                // piece and the third space diagonally upward to the left is available for a piece to be played in
                } else if (((gameBoard[j - 1][i + 1] == checkChar) && (gameBoard[j - 2][i + 2] == checkChar)) && (gameBoard[j - 3][i + 3] == 'O') && (gameBoard[j - 3][i + 2] != 'O') && (j > 2) && (i < 3)) {
                    // play winning/defending piece
                    gameBoard[j - 3][i + 3] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 3;
                    break;
                    
                // the two pieces diagonally upward to the left of the players piece are also the players
                // piece and the first space diagonally downward to the right is available for a piece to be played in
                } else if (((gameBoard[j - 1][i + 1] == checkChar) && (gameBoard[j - 2][i + 2] == checkChar)) && (gameBoard[j + 1][i - 1] == 'O') && ((gameBoard[j + 1][i - 2] != 'O') || (i == 1)) && (j > 1) && (j < 6) && (i > 0) && (i < 4)) {
                    // play winning/defending piece
                    gameBoard[j + 1][i - 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    break;
                    
                // the second and third pieces diagonally upward to the left of the players piece are also the players
                // piece and the first space diagonally upward to the left is available for a piece to be played in
                } else if ((gameBoard[j - 1][i + 1] == 'O') && (gameBoard[j - 2][i + 2] == checkChar) && (gameBoard[j - 3][i + 3] == checkChar) && (gameBoard[j - 1][i] != 'O') && (j > 2) && (i < 3)) {
                    // play winning/defending piece
                    gameBoard[j - 1][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    break;
                    
                // the first and third pieces diagonally upward to the left of the players piece are also the players
                // piece and the second space diagonally upward to the left is available for a piece to be played in
                } else if ((gameBoard[j - 2][i + 2] == 'O') && (gameBoard[j - 1][i + 1] == checkChar) && (gameBoard[j - 3][i + 3] == checkChar) && (gameBoard[j - 2][i + 1] != 'O') && (j > 2) && (i < 3)) {
                    // play winning/defending piece
                    gameBoard[j - 2][i + 2] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 2;
                    break;
                }
            }
        }
        // the playColumn is not the default value, break from the while loop
        // because the computer has made a move.
        if (playColumn != 8) {
            break;
        // the computer has not made a move so increment the row index counter
        // by 1 and continue checking if the computer can play a move to win or
        // prevent the other player from winning.
        } else {
            i++;
        }
    }
    return playColumn;
}

/*
 * Purpose:
 *      To check if the computers move will give the player a winning move and undo
 *      it if it does
 * Parameters:
 *      gameBoard - the array to be modified to check if the player will win if the
 *      computers plays a move and to be modified to undo it to prevent the computer
 *      from losing on that turn
 *      opponentChar - the character representing the pieces of the computers opponent
 *      playColumn - the column the computer played its piece. If the move the computer 
 *      made gives the opponent the win it will be changed back to its default value so
 *      the computer can make a new move.
 * Returns:
 *      The column the computer plays its piece in
 * Side-Effects:
 *      The gameboard array is modified first to check if the computer has given 
 *      the user a winning move and then to undo that move by resetting the values
 *      to the default ('O').
 */
int dontGiveWin(char gameBoard[numColumns][numRows], const char opponentChar, int playColumn) {
    // initiate the variable to store the index of the row above the computers last move
    int rowSlot = 0;
    
    // starting from the bottom row in the column the computer played in, increment
    // the rowSlot up by 1 until a row is found that equals O (the default value)
    // this is the first row value above the computers last move. This loop will only
    // run if the playColumn was modified in another function finding the computers
    // move because there is no need to check if the player can win if the computer
    // has not gone yet.
    while ((gameBoard[playColumn][rowSlot] != 'O') && (rowSlot < 6) && (playColumn != 8)) {
        // increment rowSlot by 1
        rowSlot++;
    }
    // if the rowSlot is within the gameBoard and the playColumn has modified in 
    // a past function, check if by changing the index above the computers last to the
    // other users piece it will let the player win
    if ((rowSlot < 6) && (playColumn != 8)) {
        // change value at the index above the computers last move to the opponents character
        gameBoard[playColumn][rowSlot] = opponentChar;
        // check if the opponent will win with its piece in the index above the computer piece
        // from the previous line of code
        if (checkWinGame(gameBoard, opponentChar)) {
            // if the opponent will win with the last computer move, set the 
            // index that was changed back to the default
            gameBoard[playColumn][rowSlot] = 'O';
            // change the last computers move index back to the default so it can 
            // make a new move
            gameBoard[playColumn][rowSlot - 1] = 'O';
            // set play column back to 8 so the computer can move to the next
            // function in its logic to make a move.
            playColumn = 8;
        } else {
            // the computers move will not give the player a winning move so 
            // change the value at the index that was changed to the opponents piece back
            // to the default
            gameBoard[playColumn][rowSlot] = 'O';
        }
    }   
    
    return playColumn;
}

/*
 * Purpose:
 *      To check if the computer or player can indirectly get 3 pieces in a row 
 *      or prevent the user from playing a trap where they get 3 in a row indirectly.
 *      Overall this will reduce the risk of a user trapping the computer
 *      with two possible moves to win at the same time. While this does not address
 *      every possible trap, it takes action against many of the most common types 
 *      through this defense/offense 
  * Parameters:
 *      gameBoard - the game board array
 *      computerChar - the character representing the computers pieces
 *      checkChar - the character for the player that the programming is checking
 *      if they can get 3 in an indirect line i.e XOOX where playing XXOX could give
 *      the player a winning move
 *      opponentChar - the character representing the pieces of the computers opponent
 * Returns:
 *      playColumn - the column the computer played its pieces in to be outputted
 *      to the other player
 * Side-Effects:
 *      The gameBoard array is modified wherever the computer plays its piece to get
 *      three in a row indirectly or block the player from getting it
 */
int threeTrap(char gameBoard[numColumns][numRows], const char computerChar, const char checkChar, const char opponentChar) {
    // the counter for the row index
    int i = 0;
    // the counter for the column index
    int j = 0;
    // a default value for the play column that is not a valid column so if 
    // this function does not find a way for the computer to win, the computer turn
    // function will move on to the next function to try to find a good move for the 
    // computer to play
    int playColumn = 8;
    
    // this loop will iterate through the entire array, column by column, row by row,
    // to check if the computer can get 3 in a line indirectly, or if it can prevent the 
    // player from getting 3 in a line indirectly.
    while (i < numRows){
        // iterate across each column to find the player/computer pieces (the character representing them)
        for (j = 0; j < numColumns; j++) {
            // if the loop gets to a character that is the users/opponent, check if
            // it can win vertically, horizontally, or diagonally
            if (gameBoard[j][i] == checkChar) {
            // for the following a space is defined as available if the slot
            // below it is the bottom of the board or either the player or opponents piece
                
            // for each of the following, constraints are put on the j and i counter
            // to make sure the computer or player can get 4 in a row in the future 
            // as there is no point playing three in a line to trap/defend if that cannot be expanded
            // to four in the future. The constraints also make sure pieces will be played
            // within the game board
                    
                // the third piece to the right of the players piece is 
                // also the players piece and the two pieces to the right
                // of the players piece are available so play a piece in either slot
                // that will not give the opponent (only if checking computer pieces)
                // the win
                if ((gameBoard[j + 3][i] == checkChar) && (gameBoard[j + 1][i] == 'O') && (gameBoard[j + 2][i] == 'O') && (((gameBoard[j + 1][i - 1] != 'O') && (gameBoard[j + 2][i - 1] != 'O')) || (i == 0)) && (j < 4)) {
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j + 1][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break;
                    // play a piece in the second possible space to block/set a trap
                    } else {
                        // play piece to block or make 3 in a line indirectly
                        gameBoard[j + 2][i] = computerChar;
                        // update play column to the column the computer played in
                        playColumn = j + 2;
                    }
                
                /// the first piece to the right of the players piece is 
                // also the players piece and the first space to the left and the
                // second space to the right of the player are available so play a piece in either slot
                // that will not give the opponent (only if checking computer pieces)
                // the win
                } else if ( (gameBoard[j + 1][i] == checkChar) && (gameBoard[j - 1][i] == 'O') && (gameBoard[j + 2][i] == 'O') && (((gameBoard[j - 1][i - 1] != 'O') && (gameBoard[j + 2][i - 1] != 'O')) || (i == 0)) && (j < 5) && (j > 0)){
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j - 1][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break;
                    // play a piece in the second possible space to block/set a trap
                    } else {
                        // play piece to block or make 3 in a line indirectly
                        gameBoard[j + 3][i] = computerChar;
                        // update play column to the column the computer played in
                        playColumn = j + 3;
                    }  
                
                // the third piece diagonally upward to the right of the players piece is 
                // also the players piece and the two spaces diagonally upward to the right
                // of the players piece are available so play a piece in either slot
                // that will not give the opponent (only if checking computer pieces)
                // the win
                } else if ( (gameBoard[j + 3][i + 3] == checkChar) && (gameBoard[j + 1][i + 1] == 'O') && (gameBoard[j + 2][i + 2] == 'O') && (gameBoard[j + 1][i] != 'O') && (gameBoard[j + 2][i + 1] != 'O') && (j < 4) && (i < 3)){
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j + 1][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break;
                    // play a piece in the second possible space to block/set a trap
                    } else {
                        // play piece to block or make 3 in a line indirectly
                        gameBoard[j + 2][i + 2] = computerChar;
                        // update play column to the column the computer played in
                        playColumn = j + 2;         
                    }        
                
                // the first piece diagonally upward to the right of the players piece
                // is also the players piece and the first space to diagonally downward
                // to the left and the second space upward diagonally to the right are available
                // so play a piece in either slot that will not give the opponent 
                // (only if checking computer pieces) the win
                } else if ( (gameBoard[j + 1][i + 1] == checkChar) && (gameBoard[j - 1][i - 1] == 'O') && (gameBoard[j + 2][i + 2] == 'O') && ((gameBoard[j - 1][i - 2] != 'O') || (i == 1)) && (gameBoard[j + 2][i + 1] != 'O') && (j < 5) && (i < 4) && (j > 0) && (i > 0)) {
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j + 2][i + 2] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 2;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break;
                   // place a piece to block a more direct trap in the second space diagonally
                   // upwards to the right of the players piece
                    } else {
                        // play piece to block or make 3 in a line indirectly
                        gameBoard[j - 1][i - 1] = computerChar;
                        // update play column to the column the computer played in
                        playColumn = j - 1;
                    }             
                
                // the third piece diagonally upward to the left of the players piece is 
                // also the players piece and the two spaces diagonally upward to the left
                // of the players piece are available so play a piece in either slot
                // that will not give the opponent (only if checking computer pieces)
                // the win
                } else if ( (gameBoard[j - 3][i + 3] == checkChar) && (gameBoard[j - 1][i + 1] == 'O') && (gameBoard[j - 2][i + 2] == 'O') && (gameBoard[j - 1][i] != 'O') && (gameBoard[j - 2][i + 1] != 'O') && (j > 2) && (i < 3)){
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j - 1][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break;
                    // play a piece in the second possible space to block/set a trap
                    } else {
                        // play piece to block or make 3 in a line indirectly
                        gameBoard[j - 2][i + 2] = computerChar;
                        // update play column to the column the computer played in
                        playColumn = j - 2;
                    }               
                
                // the first piece diagonally upward to the left of the players piece
                // is also the players piece and the first space to diagonally downward
                // to the right and the second space upward diagonally to the left are available
                // so play a piece in either slot that will not give the opponent 
                // (only if checking computer pieces) the win
                } else if ( (gameBoard[j - 1][i + 1] == checkChar) && (gameBoard[j + 1][i - 1] == 'O') && (gameBoard[j - 2][i + 2] == 'O') && ((gameBoard[j + 1][i - 2] != 'O') || (i == 1)) && (gameBoard[j - 2][i + 1] != 'O') && (j > 1) && (i < 4) && (j < 6) && (i > 0)){
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j - 2][i + 2] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 2;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break; 
                    // the second space diagonally downwards to the left is available
                    // for a piece to be played so play a piece to make/block a trap
                    } else {
                        // play piece to block or make 3 in a line indirectly
                        gameBoard[j + 1][i - 1] = computerChar;
                        // update play column to the column the computer played in
                        playColumn = j + 1;
                    } 
                    
                // the first piece diagonally upward to the right of the players piece
                // is also the players piece and the second space diagonally downward
                // to the left and the first space downward diagonally to the left are available
                // so play a piece that will not give the opponent 
                // (only if checking computer pieces) the win
                } else if ( (gameBoard[j + 1][i + 1] == checkChar) && (gameBoard[j - 1][i - 1] == 'O') && (gameBoard[j - 2][i - 2] == 'O') && ((gameBoard[j - 2][i - 3] != 'O') || (i == 2)) && (j < 6) && (j > 0) && (i < 5) && (i > 1)){
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j - 1][i - 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break; 
                    }

                // the first piece diagonally upward to the left of the players piece
                // is also the players piece and the second space diagonally downward
                // to the right and the first space downward diagonally to the right are available
                // so play a piece that will not give the opponent 
                // (only if checking computer pieces) the win
                } else if ( (gameBoard[j - 1][i + 1] == checkChar) && (gameBoard[j + 1][i - 1] == 'O') && (gameBoard[j + 2][i - 2] == 'O') && ((gameBoard[j + 2][i - 3] != 'O') || (i == 2)) && (j > 0) && (j < 6) && (i < 5) && (i > 1)){
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j + 2][i - 2] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 2;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break; 
                    }
                    
                // the first piece diagonally upward to the right of the players piece
                // is also the players piece and the third space diagonally upward
                // to the right and the second space upward diagonally to the right are available
                // so play a piece that will not give the opponent 
                // (only if checking computer pieces) the win
                } else if ( (gameBoard[j + 1][i + 1] == checkChar) && (gameBoard[j + 3][i + 3] == 'O') && (gameBoard[j + 2][i + 2] == 'O') && (gameBoard[j + 3][i + 2] != 'O') && (j < 4) && (i < 3)) {
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j + 3][i + 3] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 3;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break; 
                    }

                // the first piece diagonally upward to the left of the players piece
                // is also the players piece and the third space diagonally upward
                // to the left and the second space upward diagonally to the left are available
                // so play a piece that will not give the opponent 
                // (only if checking computer pieces) the win
                } else if ( (gameBoard[j - 1][i + 1] == checkChar) && (gameBoard[j - 2][i + 2] == 'O') && (gameBoard[j - 3][i + 3] == 'O') && (gameBoard[j - 3][i + 2] != 'O') && (j > 1) && (i < 4)) {
                    // play piece to block or make 3 in a line indirectly
                    gameBoard[j - 3][i + 3] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 3;
                    // if the computers move gives the user a winning move, undo the 
                    // move and reset the play column to 8 (this can be seen in the function called here)
                    // so the computer will continue to look for good moves
                    // if the computer move does not give the user a winning move, break the
                    // loop and continue the game
                    playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
                    if (playColumn != 8) {
                        break; 
                    }
                }
                
            }
            // if the computers move gives the user a winning move, undo the 
            // move and reset the play column to 8 (this can be seen in the function called here)
            // so the computer will continue to look for good moves
            // if the computer move does not give the user a winning move, break the
            // loop and continue the game
            playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
            if (playColumn != 8) {
                break;
            }
          
        } 
        // the playColumn is not the default value, break from the while loop
        // because the computer has made a move.
        if (playColumn != 8) {
            break;
        // the computer has not made a move so increment the row index counter
        // by 1 and continue checking if the computer can play a move to win or
        // prevent the other player from winning.
        } else {
            i++;
        }
    }
    return playColumn;
}


/*
 * Purpose:
 *      For the computer to play a piece to get three in a row or to prevent 
 *      the user from getting three in a row.
 * Parameters:
 *      gameBoard - the game board array
 *      computerChar - the character for the computer representing its pieces
 *      checkChar - the character for the player that the programming is checking
 *      if they have 2 in a row
 *      opponentChar - the character representing the opponent to check
 *      if the computers move gives it the win
 * Returns:
 *      playColumn - the column the computer played its pieces in to be outputted
 *      to the player
 * Side-Effects:
 *      The gameBoard array is modified wherever the computer plays its piece to get
 *      three in a row or to prevent the user from getting three in a row. 
 */
int twoInARow(char gameBoard[numColumns][numRows], const char computerChar, const char checkChar, const char opponentChar) {
    // the counter for the row index
    int i = 0;
    // the counter for the column index
    int j = 0;
    // a default value for the play column that is not a valid column so if 
    // this function does not find a way for the computer to win, the computer turn
    // function will move on to the next function to try to find a good move for the 
    // computer to play
    int playColumn = 8;

    // this loop will iterate through the entire array, column by column, row by row,
    // to check if the computer can get 3 in a row, or if it can prevent the 
    // player from getting 3 in a row.
    while (i < numRows) {
        // iterate across each column to find the player/computer pieces (the character representing them)
        for (j = 0; j < numColumns; j++) {
        // for the following a space is defined as available if the slot
        // below it is the bottom of the board or either the player or opponents piece
            
        // the constraints (j >, j <, i >, i <) are used to ensure the computer
        // does not play three in a row if that 3 in a row can not be added on to
        // make 4 in a row in the future. The constraints also make sure pieces will be played
        // within the game board
            
            // if the loop gets to a character that is the users/opponent, check if
            // it can get/block 3 in a row vertically, horizontally, or diagonally
            if (gameBoard[j][i] == checkChar) {
                
                // the piece above the players piece is also the players piece and
                // the space above that piece is available for a piece to be played in
                if ((gameBoard[j][i + 1] == checkChar) && (i < 4) && (gameBoard[j][i + 2] == 'O')) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j][i + 2] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j;
                    
                // the first piece to the right of the players piece is also the players piece and
                // the second space to the right is available for a piece to be played in
                } else if ((gameBoard[j + 1][i] == checkChar) && (gameBoard[j + 2][i] == 'O') && (i == 0 || gameBoard[j + 2][i - 1] != 'O') && (j < 4)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j + 2][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 2;
                    
                // the first piece to the right of the players piece is also the players piece and
                // the first space to the left is available for a piece to be played in
                } else if ((gameBoard[j + 1][i] == checkChar) && (gameBoard[j - 1][i] == 'O') && ((gameBoard[j - 1][i - 1] != 'O') || (i == 0)) && (j > 1)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j - 1][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;

                // the second piece to the right of the players piece is also the 
                // players piece and the first space to the right of the players piece
                // is available for a piece to be played in
                } else if ((gameBoard[j + 1][i] == 'O') && (gameBoard[j + 2][i] == checkChar) && ((gameBoard[j + 1][i - 1] != 'O') || (i == 0)) && (j < 4)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j + 1][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    
                // the first piece diagonally upward to the right of the players piece is also 
                // the players piece and the second space upward diagonally right of the players piece
                // is available for a piece to be played in
                } else if ((gameBoard[j + 1][i + 1] == checkChar)  && (gameBoard[j + 2][i + 2] == 'O') && (gameBoard[j + 2][i + 1] != 'O') && (j < 4) && (i < 3)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j + 2][i + 2] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 2;
                    
                // the first piece diagonally upward to the right of the players piece is also 
                // the players piece and the first space downward diagonally left of the players piece
                // is available for a piece to be played in
                } else if ((gameBoard[j + 1][i + 1] == checkChar) && (gameBoard[j - 1][i - 1] == 'O') && ((gameBoard[j - 1][i - 2] != 'O') || (i == 1)) && (j > 0) && (i > 0) && (j < 5) && (i < 4)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j - 1][i - 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    
                // the second piece diagonally upward to the right of the players piece is also 
                // the players piece and the first space upward diagonally right of the players piece
                // is available for a piece to be played in
                } else if ((gameBoard[j + 1][i + 1] == 'O') && (gameBoard[j + 2][i + 2] == checkChar) && (gameBoard[j + 1][i] != 'O') && (j < 5) && (i < 4)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j + 1][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    
                // the first piece diagonally upward to the left of the players piece is also 
                // the players piece and the second space upward diagonally left of the players piece
                // is available for a piece to be played in
                } else if ((gameBoard[j - 1][i + 1] == checkChar) && (gameBoard[j - 2][i + 2] == 'O') && (gameBoard[j - 2][i + 1] != 'O') && (j > 2) && (i < 3)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j - 2][i + 2] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 2;
                    
                // the first piece diagonally upward to the left of the players piece is also 
                // the players piece and the first space downward diagonally right of the players piece
                // is available for a piece to be played in
                } else if ( (gameBoard[j - 1][i + 1] == checkChar) && (gameBoard[j + 1][i - 1] == 'O') && ((gameBoard[j + 1][i - 2] != 'O') || (i == 1)) && (j > 1) && (i < 4) && (j < 6) && (i > 0)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j + 1][i - 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    
                // the second piece diagonally upward to the left of the players piece is also 
                // the players piece and the first space upward diagonally left of the players piece
                // is available for a piece to be played in
                } else if ((gameBoard[j - 1][i + 1] == 'O') && (gameBoard[j - 2][i + 2] == checkChar) && (gameBoard[j - 1][i] != 'O') && (j > 1) && (i < 4)) {
                    // play piece to get 3 in a row or prevent the user from getting
                    // three in a row
                    gameBoard[j - 1][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                }
            }
            // if the computers move does not give the opponent a winning move and a
            // move to block it from winning, break the loop
            
            // if the computers move gives the user a winning move, undo the 
            // move and reset the play column to 8 (this can be seen in the function called here)
            // so the computer will continue to look for good moves
            // if the computer move does not give the user a winning move, break the
            // loop and continue the game
            playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
            if (playColumn != 8) {
                // if the computers move gives the user a move to block it from winning, undo the 
                // move and reset the play column to 8 (this can be seen in the function called here)
                // so the computer will continue to look for good moves
                // if the computer move does not give the user a blocking move, break the
                // loop and continue the game. This aims to eventually force the user
                // to play a piece to give the computer the win late in the game, instead
                // of the computer accidentally giving the user a way to tie by blocking it 
                // from getting 4 in a row. This uses the dont give win function but checks
                // for places the computer can win instead of the player
                playColumn = dontGiveWin(gameBoard, computerChar, playColumn);
                if (playColumn != 8) {
                    break;
                }
            }  
        }
        // the playColumn is not the default value, break from the while loop
        // because the computer has made a move.
        if (playColumn != 8) {
            break;
        // the computer has not made a move so increment the row index counter
        // by 1 and continue checking if the computer can play a move to win or
        // prevent the other player from winning.
        } else {
            i++;
        }
    }

    return playColumn;
}

/*
 * Purpose:
 *      For the computer to place a piece to get 2 in a row
 * Parameters:
 *      gameBoard - the gameBoard array
 *      computerChar - the character representing the computers pieces
 *      opponentChar - the character representing the opponent to check
 *      if the computers move gives it the win
 * Returns:
 *      playColumn - The column the computer played its piece in to be 
 *      outputted to the player
 * Side-Effects:
 *      The gameboard array is modified when the computer makes its move
 */
int connectTwo(char gameBoard[numColumns][numRows], char computerChar, const char opponentChar) {
    // the counter for the row index
    int i = 0;
    // the counter for the column index
    int j = 0;
    // a default value for the play column that is not a valid column so if 
    // this function does not find a way for the computer to win, the computer turn
    // function will move on to the next function to try to find a good move for the 
    // computer to play
    int playColumn = 8;

    // this loop will iterate through the entire array, column by column, row by row,
    // to check if the computer can get 2 in a row
    while (i < numRows) {
        // iterate across each column to find the computer pieces
        for (j = 0; j < numColumns; j++) {
            // if the loop gets to a character that is the users/opponent, check if
            // it can get 2 in a row vertically, horizontally, or diagonally            
            if (gameBoard[j][i] == computerChar) {
            // for the following a space is defined as available if the slot
            // below it is the bottom of the board or either the player or opponents piece  
            
            // constraints are put on the j and i counters for the following to make 
            // sure the piece can be played within the bounds of the game board
                
                // the space directly above the players piece is available for a 
                // piece to be played in
                if ((gameBoard[j][i + 1] == 'O') && (i < 5)) {
                    // place piece for the computer to get 2 in a row
                    gameBoard[j][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j;
                    
                // the space directly right of the players piece is available
                // for a piece to be played in
                } else if ((gameBoard[j + 1][i] == 'O') && ((gameBoard[j + 1][i - 1] != 'O') || (i == 0)) && (j < 6)) {
                    // place piece for the computer to get 2 in a row
                    gameBoard[j + 1][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    
                // the space directly left of the players piece is available
                // for a piece to be played in
                } else if ((gameBoard[j - 1][i] == 'O') && ((gameBoard[j - 1][i - 1] != 'O') || (i == 0)) && (j > 0)) {
                    // place piece for the computer to get 2 in a row
                    gameBoard[j - 1][i] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    
                // the space upward diagonally right of the players piece is available
                // for a piece to be played in
                } else if ((gameBoard[j + 1][i + 1] == 'O') && (gameBoard[j + 1][i] != 'O') && (j < 6) && (i < 5)) {
                    // place piece for the computer to get 2 in a row
                    gameBoard[j + 1][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                    
                // the space downward diagonally left of the players piece is available
                // for a piece to be played in
                } else if ((gameBoard[j - 1][i - 1] == 'O') && ((gameBoard[j - 1][i - 2] != 'O') || (i == 1)) && (j > 0) && (i > 0)) {
                    // place piece for the computer to get 2 in a row
                    gameBoard[j - 1][i - 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    
                // the space upward diagonally left of the players piece is available
                // for a piece to be played in
                } else if ((gameBoard[j - 1][i + 1] == 'O') && (gameBoard[j - 1][i] != 'O') && (j > 0) && (i < 5)) {
                    // place piece for the computer to get 2 in a row
                    gameBoard[j - 1][i + 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j - 1;
                    
                // the space downward diagonally right of the players piece is available
                // for a piece to be played in
                } else if ((gameBoard[j + 1][i - 1] == 'O') && ((gameBoard[j + 1][i - 2] != 'O') || (i == 1)) && (j < 6) && (i > 0)) {
                    // place piece for the computer to get 2 in a row
                    gameBoard[j + 1][i - 1] = computerChar;
                    // update play column to the column the computer played in
                    playColumn = j + 1;
                }
            }
            // if the computers move does not give the opponent a winning move and a
            // move to block it from winning, break the loop
            
            // if the computers move gives the user a winning move, undo the 
            // move and reset the play column to 8 (this can be seen in the function called here)
            // so the computer will continue to look for good moves
            // if the computer move does not give the user a winning move, break the
            // loop and continue the game
            playColumn = dontGiveWin(gameBoard, opponentChar, playColumn);
            if (playColumn != 8) {
                // if the computers move gives the user a move to block it from winning, undo the 
                // move and reset the play column to 8 (this can be seen in the function called here)
                // so the computer will continue to look for good moves
                // if the computer move does not give the user a blocking move, break the
                // loop and continue the game. This aims to eventually force the user
                // to play a piece to give the computer the win late in the game, instead
                // of the computer accidentally giving the user a way to tie by blocking it 
                // from getting 4 in a row. This uses the dont give win function but checks
                // for places the computer can win instead of the player
                playColumn = dontGiveWin(gameBoard, computerChar, playColumn);
                if (playColumn != 8) {
                    break;
                }
            }              
        }
        // the playColumn is not the default value, break from the while loop
        // because the computer has made a move.
        if (playColumn != 8) {
            break;
        // the computer has not made a move so increment the row index counter
        // by 1 and continue checking if the computer can play a move to win or
        // prevent the other player from winning.
        } else {
            i++;
        }
    }

    return playColumn;
}

/*
 * Purpose:
 *      For the computer to play its first two moves
 * Parameters:
 *      gameBoard - the array for the gameboard
 *      computerChar - the character representing the computers pieces
 *      opponentChar - the character representing the users pieces
 *      playColumn - the column the computer plays its piece in
 *      turn - the turn the game is on
 * Returns:
 *      playColumn - the column the computer plays its piece in
 * Side-Effects:
 *      The gameboard array is modified to place the computer pieces
 */
int playFirstTwoMoves(char gameBoard[numColumns][numRows], const char computerChar, const char opponentChar, int playColumn, const int turn) {
        // play first turn in the middle of the board as it is a strong strategic position
    if (turn == 0) {
        // changes the default value of the 3rd column and 0th row to the computers char
        gameBoard[3][0] = computerChar;
        // change the value of the column the computer played to be outputted to the live player
        playColumn = 3;
    }
    // play computer second turn in a good position for the computer
    else {
        // based of the minimax algorithm the following moves were made every 
        // time the second move was simulated so they have been recreated to
        // give the computer a strong first two moves
        
        // NOTE the indexes here are checked as they would be in the array (from 0 - 6)
        // however for my commenting 1 has been added to the index because that is the
        // column value the user of the program would see
        
        // if the user plays their piece in column 2 or 3, the computer plays its
        // piece in column 5
        if ((gameBoard[1][0] == opponentChar) || (gameBoard[2][0] == opponentChar)) {
            gameBoard[5][0] = computerChar;
            playColumn = 5;
        // if the user plays their piece in column 5 or 6, the computer plays its
        // piece in column 2.
        } else if ((gameBoard[4][0] == opponentChar) || (gameBoard[5][0] == opponentChar)) {
            gameBoard[1][0] = computerChar;
            playColumn = 1;
        // if the user plays their piece in column 1 or 7, the computer plays its
        // piece in column 4 above its first piece
        } else if ((gameBoard[0][0] == opponentChar) || (gameBoard[6][0] == opponentChar)) {
            gameBoard[3][1] = computerChar;
            playColumn = 3;
        // if the user plays their piece in column 4 above the computers first piece
        // the computer plays its piece above the user piece in column 4
        } else if (gameBoard[3][1] == opponentChar) {
            gameBoard[3][2] = computerChar;
            playColumn = 3;
        }
    }
    
    return playColumn;
}


/*
 * Purpose:
 *      To play the best move for the computer by calling functions
 *      for the computer to play pieces. It first checks if the computer can get 3
 *      in a row or prevent the opponent from getting 3 and then it checks if the
 *      computer can get two in a row and if none of these are met it will play a random
 *      piece.
 * Parameters:
 *      gameBoard - the gameboard that is modified to make the computers move
 *      computerChar - the character chosen for the computer
 *      opponentChar - the live players chosen character
 * Returns:
 *      playColumn - the column the computer plays its piece in
 * Side-Effects: 
 *      The gameboard array is modified in functions called by this one
 */
int playBestMove(char gameBoard[numColumns][numRows], const char computerChar, const char opponentChar) {
    // initiate the variable for the column the computer plays its piece in
    // as a column number outside the array so the gameboard is not modified unless
    // the computer finds a move to play within the gameboard where this will be reassigned
    int playColumn = 8;
    // the row the computer plays its piece in which is needed if the computer
    // picks a random column to play in
    int rowSlot = 0;
    
    // the column the computer plays has not been modified from 
    // the function calls in computer turn function that come before this function call
    if (playColumn == 8) {
        // check if the computer can get a set of three in a row in an indirect
        // line to try to trap the user player. Eg something like this XOXX where 
        // x is the computer piece
        playColumn = threeTrap(gameBoard, computerChar, computerChar, opponentChar);
    }
    if (playColumn == 8) {
        // check if the computer has 2 in a row (or 2 with an empty 
        // space in the middle), play a piece to get 3 in a row
        playColumn = twoInARow(gameBoard, computerChar, computerChar, opponentChar);
    }
    if (playColumn == 8) {
        // check if the player can get a set of three in a row in an indirect
        // line to try to trap the computer and play a move to block it. Eg something like this X_OX where 
        // O is the computer piece that has blocked the player from a indirect 3 in a line trap
        playColumn = threeTrap(gameBoard, computerChar, opponentChar, opponentChar);
    }
    // the playColumn was not modified by the previous function call
    if (playColumn == 8) {
        // check if the user has 2 in a row (or 2 with an empty 
        // space in the middle), play a piece to prevent them from getting 3 in a row
        playColumn = twoInARow(gameBoard, computerChar, opponentChar, opponentChar);
    }
    // the playColumn was not modified by the previous function call
    if (playColumn == 8) {
        // place a piece to give the computer two in a row.
        playColumn = connectTwo(gameBoard, computerChar, opponentChar);
    }

    return playColumn;
}

/*
 * Purpose:
 *      For the computer to play a random move
 * Parameters:
 *      gameBoard - the gameboard that is modified for the computer to play a random move
 *      computerChar - the character chosen for the computer
 * Returns:
 *      playColumn - the column the computer plays its piece in
 * Side-Effects: 
 *      The gameboard array is modified to play the computer piece
 */
int playRandomMove(char gameBoard[numColumns][numRows], const char computerChar) {
    // initiate play column variable to make a random move
    int playColumn = 0; 
    
    // pick a random number from 1 - 7. The range is from 1-7 and not 0-6 because
    // the place piece function takes a integer that the user enters and subtracts one to 
    // get the actual column index
    playColumn = ((rand() % (7 - 1 + 1)) + 1);
    // if the column is not full, place the computers piece
    if (gameBoard[playColumn][5] == 'O') {
        // place the computers piece in the column it randomly chose
        placepiece(gameBoard, computerChar, playColumn);     
    } else {
        // while the column that is randomly chosen is full, choose a new column
        while (gameBoard[playColumn][5] != 'O') {
            // uses random function algorithm (upper limit - lower limit + 1) + lower limit
            // to pick a column. This will repeat until the column chosen is not full so
            // a piece can be played
            playColumn = (rand() % (7 - 1 + 1)) + 1;
        }
        // place the computers piece in the column it randomly chose
        placepiece(gameBoard, computerChar, playColumn); 
    }

    return playColumn;
}

/*
 * Purpose:
 *      For the computer to play a move
 * Parameters:
 *      computerChar - the character representing the computers pieces
 *      gameBoard - the array for the game board
 *      opponentChar - the character representing the live players pieces (the computers opponent)
 *      turn - the turn the game is on
 * Returns:
 *      NONE
 * Side-Effects:
 *      The gameboard array is modified in functions called by this one
 */
void computerTurn(char computerChar, char gameBoard[numColumns][numRows], char opponentChar, int turn) {
    // set playColumn to 8 meaning computer has not played a move (1-7)
    int playColumn = 8;
    
    if (turn <= 2) {
        // play the computers first two moves to give the computer a strong start
        playColumn = playFirstTwoMoves(gameBoard, computerChar, opponentChar, playColumn, turn);
    // after the first two turns, resort to a pattern recognition approach
    // instead of hardcoding every single possible move
    } else {
        // if the computer has 3 pieces in a row, check if the computer can play
        // a piece to win and play it if it can
        playColumn = threeInARow(gameBoard, computerChar, computerChar);
        
        // if the computer has not made a move yet, move on to next step
        // of computer logic
        if (playColumn == 8) {
        // if the opponent has 3 pieces in a row, check if the computer can play
        // a piece to defend and if it can play it
            playColumn = threeInARow(gameBoard, computerChar, opponentChar);
        }
        
        // if the computer has not made a move yet, move on to next step
        // of computer logic
        if (playColumn == 8) {
            // call the play best move function to play the best move for the computer
            playColumn = playBestMove(gameBoard, computerChar, opponentChar);
        }
        
        // if the computer has not made a move yet, move on to next step
        // of computer logic
        if (playColumn == 8) {
            // if the computer has not made a move yet, pick a random column to play
            // a move. At this point it is likely the computer has no move it can make
            // that does not let the player win
            playColumn = playRandomMove(gameBoard, computerChar);
        }
    }
     
    // print a message telling the user where the computer played. 1 is added
    // to the column because the gameboard indexes are 0 - 6 but the players
    // play on a board with columns 1 - 7
    printf("\nThe computer plays its piece in column %d\n", playColumn + 1);

}

/*
 * Purpose:
 *      To print every value at every index in the game board array
 * Parameters:
 *      gameBoard - the gameboard array
 * Returns:
 *      NONE
 * Side-Effects:
 *      NONE - gameboard is const as it is only printed and no indexes are modified
 */
void printGameboard(const char gameBoard[numColumns][numRows]) {
    // Counter for the rows. start at the last index so the gameboard is printed
    // the right way for the user
    int i = numRows - 1;
    // counter for the columns
    int j = 0;

    // print the gameboard going across each column index incrementing by 1 and
    // then going down row by row, incrementing the row counter down by 1 each cycle of the loop
    printf("_____________________________\n");
    while (i >= 0) {
        printf("|");
        for (j = 0; j < numColumns; j++) {
            printf(" %c |",gameBoard[j][i]);
        }
        printf("\n");
        i--;
    }
    printf("-----------------------------\n");
    // print labels for the columns to make it easier to read
    printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");


}

/*
 * Purpose:
 *      To print a message to the winning player
 * Parameters:
 *      player - the player number
 *      gameMode - the game mode the user is playing
 * Returns:
 *      NONE
 * Side-Effects:
 *      NONE
 */
void printWinMessage(int player, char gameMode) {
    // if the gamemode is user v computer and the player is #2 (the computer)
    // print message saying the computer won
    if ((gameMode == 'c') && (player == 2)){
        printf("\nToo bad. The computer won!\n");
    // the game was not against the computer
    } else {
        printf("\nCongratulations player %d. You won!\n", player);
        printf("\n");
    }
}

/*
 * Purpose:
 *      To print a message if the game ends in a tie
 * Parameters:
 *      NONE
 * Returns:
 *      NONE
 * Side-Effects:
 *      NONE
 */
void printTieMessage() {
    printf("Good Effort! You Tied.\n");
    printf("\n");
}

/*
 * Purpose:
 *      To play a game of user v user/computer connect 4
 * Parameters:
 *      gameBoard - the array of the gameboard
 *      userChar - the first players character
 *      playerTwoChar - the second players/computers character
 *      gameMode - the gamemode 
 *      turn - the turn the game is on
 * Returns:
 *      NONE
 * Side-Effects:
 *      The gameboard array is modified in functions called by this one
 */
void playGame(char gameBoard[numColumns][numRows], const char userChar, const char playerTwoChar, const char gameMode, int turn) {
    // variable to store which player is playing first
    int firstPlayer = 1;
    // variable to store which player is playing second
    int secondPlayer = 2;
    // variable to store the character of the player playing first
    char firstPlayerChar = userChar;
    // variable to store the character of the player playing second
    char secondPlayerChar = playerTwoChar;
    // variable to store if a player has won the game
    int winGame = 0;
    
    if (gameMode == 'c') {
        // the computer is player 2 however it always plays first so this variable
        // is updated to reflect this
        firstPlayer = 2;
        // the user is always player one but it plays second in a game against the computer
        // so this variable is updated to reflect this
        secondPlayer = 1;
        // the computers character is playerTwoChar however the computer is the first
        // player in this game mode so this variable is updated to reflect this
        firstPlayerChar = playerTwoChar;
        // the players character is userChar however the user player is the second
        // player so this variable is updated to reflect this
        secondPlayerChar = userChar;
    }
    
    // while the game board is not full (when the number of turns is less than the
    // size of the board), alternate player and computer taking turns
    while (turn < (numRows * numColumns)) {
        // if the user is playing against the computer the first move of every set
        // of two moves is the computer move
        if (gameMode == 'c') {
            computerTurn(playerTwoChar, gameBoard, userChar, turn);
        // user is playing another user so the first user makes the first of each
        // set of two moves (player 1 goes then player 2 and so on)
        } else {
            playerTurn(userChar, gameBoard, 1);
        }
        // print the gameBoard after each turn
        printGameboard(gameBoard);
        
        // if the first player has won, print a message to them and break the game
        // loop to end the current game
        winGame = checkWinGame(gameBoard, firstPlayerChar);
        if (winGame) {
            printWinMessage(firstPlayer, gameMode);
            break;
        }
        // it is the second players turn. No matter the gamemode a player will
        // be making a move here but depending on the mode this player will either
        // be player 1 as the live user against the computer or player two as a live
        // player against a live user who is player one
        playerTurn(secondPlayerChar, gameBoard, secondPlayer);
        // print gameBoard after each move
        printGameboard(gameBoard);
        
        // if the second player has won, print a message to them and break the game
        // loop to end the current game
        winGame = checkWinGame(gameBoard, secondPlayerChar);
        if (winGame) {
            printWinMessage(secondPlayer, gameMode);
            break;
        }
        // two turns have been played (1 each) so increment turn counter by 2
        turn += 2;

        // if the board is full, print a message telling the players they tied
        if (turn == numRows * numColumns) {
            printTieMessage();
        }
    }
}

/*
 * Purpose:
 *      To print a message if the user wants to quit
 * Parameters:
 *      NONE
 * Returns:
 *      NONE
 * Side-Effects:
 *      NONE
 */
void printQuitMessage() {
    printf("Thank you for playing connect four!\n");
    printf("Have a nice day!!");
}


int main(int argc, char** argv) {
    // array for the game board
    char gameBoard[numColumns][numRows];
    // default value for the gamemode
    char gameMode = 'x';
    // intitiate variable to store what turn the game is on
    int turn = 0;
    // variable to store the number of games the user played so the instructions
    // are only printed at the beginning of the first game
    int gameCount = 0;
    // initiate variable to store the character to represent the users pieces
    char userChar = 'u';
    // // initiate variable to store the character to represent the second players pieces
    char playerTwoChar = 'x';
    
    // seed random number generator
    srand(time(NULL));
    
    // play game until user wants to quit
    while (1) {
        // set turn to 0 at the beginning of each game
        turn = 0;
        // set every value of each index of the game board to 'O'
        setGameBoard(gameBoard);
        //print instructions if the user is playing their first game
        if (gameCount == 0){
            printInstructions();
            gameCount++;
        }
        // get user to choose the gamemode they would like to play
        gameMode = selectGamemode();
        printf("\n");
        // if user wants to quit, print message and quit game
        if (gameMode == 'q') {
            printQuitMessage();
            break;
        }
        // get the user to choose a character to represent their pieces
        userChar = getPlayerChar(1, gameMode);
        printf("\n");
        // get the second player to choose a character to represent their pieces
        // and if the gameMode is c get the first player to choose a character for the computer
        playerTwoChar = getPlayerChar(2, gameMode);
        // while the computer/second player chooses a character that is already 
        // in use, get them to re-choose their character
        while (playerTwoChar == userChar) {
            printf("The character '%c' is already in use.\n", playerTwoChar);
            playerTwoChar = getPlayerChar(2, gameMode);
        }
        printf("\n");
        // play a full game of connect four, either as a player v player match
        // or a computer v player match
        playGame(gameBoard, userChar, playerTwoChar, gameMode, turn);  
    }

    return (EXIT_SUCCESS);
}
