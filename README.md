# TAKI Card Game
### Game programmed in C. 
![Taki_Cards_Game_Logo](https://user-images.githubusercontent.com/104992892/188504607-fa0156de-c1f6-4205-bc23-e20fb591c0e3.png)
  
> The game was made using memory allocation, pointers, sorting algorythems and by using C random function.
<br />

> This project is my first project in my studies, in an introductory course to computer science. Therefore, I chose to leave it as I submitted it. It was written before I learned about the option to split it into files, so it does not include file separation, and all the code is written in a single file.

# Game Rules:
Each player follows the preceding card, laid on the deck, with a card of the same color or figure.  
The object of the game is to discard all the cards in your hand.
  
      
## Special cards:
* **Stop** - The next player loses their turn.
* **Plus** - Forces the user to play again. If the player cannot play another card he must draw.
* **Switch Direction** - Reverses the direction of the play.
* **Switch Color** - Allows the user to determine the color to be played by the next player.
* **TAKI** – Allows a player to follow with all the cards of the same color as the TAKI card.

     
### Some Notes:
1.  The **TAKI** card will allow the play to drop any card that is the same color as the TAKI card,  
    until he choses to draw a new card from the deck, or he finished all of his cards and won the game.
    
2. The **STOP** card will skip the next player's turn,  
   the next turn will be of the player that is after that player.

3. The **SWITCH DIRECTION** card will switch the direction of the turns,  
   the player that player before the current player will get the next turn.
   
4. The **PLUS** card forces the user to play again.  
   If the player cannot play another card he must draw a new card.
