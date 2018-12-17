

	      ===========================================================================
	   ==============================	          ==================================
	===============================     CRAZY FIGHT     ===================================
	   ==============================  		  ==================================
	      ===========================================================================



Author : 		Aurélien BLAISE

Last version date : 	16 december 2018
Release version : 	1.0

Version	:		Windows




********** DESCRIPTION **********

	This program has been developped as part of a project to carry out at home.
This project was given in a course "Programmation avancée" in a class In the 2nd year of Bachelor in Science computer
in the Faculty of Sciences and Technologies, in the Université de Lorraine in France.

	This project has been coded in C with the SDL librairy in order to handle the graphic part.
It has been developped with the Windows operating system and the integrated developement environnement named Code Blocks.
Therefore, the version the most steady is for sure the version for Windows.
However, a Linux (Ubuntu) version has been developped as well with a Makefile to be able to compile the project
and run it.


	Crazy Fight is a local multiplayer 2D platform game (2 people so far but easily programmable for more).
It's a shooter game, players can use a weapon to kill the others. 
Both players have to fight against each other until one player's death. You can die by having no longer health points.
If you leap into the void, you will die.
The last remaining player wins the round. To win one game, you need to win 3 rounds.

	Each player can shoot at everyone and each one deals the same amount of damage with the weapon.
Each player starts off the game with 100 health points. Each bullet which hits a player causes him 10 damage.


	Moreover, there is a level editor which allows to build and design your own map to play on it.
You just need to right clik on the tileset window on the tile you want to use. Then, get back to the main window and
right click again on the location you want to position the new tile. The latter is going to replace the old tile.
With the left click, you can delete any tile you want.




********** KEYS **********

- 'escape' key :		Get back to the menu wherever you are (in the level editor or the game)


\\\\\ GAME /////

- Player 1 :
	Move leftward : 	left arrow
	Move rightward : 	right arrow
	Jump : 			up arrow
	Bend down : 		Down arrow
	Fire : 			'p' key

- Player 2 :
	Move leftward :		'q' key
	Move rightward :	'd' key
	Jump : 			'z' key
	Bend down : 		's' key
	Fire : 			'v' key

-> Can Jump upward, leftward or rightward. It depends if you move or not at the same time you start jumping.


\\\\\ LEVEL EDITOR /////

- 's' key : 			save the current design of the map in the file "/ressources/level_design_map_perso"
