Othello game project.

Description: 
	Original Othello is a strategy board game for two players (Black and White), played on board size as you wish.
	The players take turns. All the opponent's disks lying between new disks and existing player's disks are flipping. 
	Available only moves which opponent's disks flipping. In case there are haven't available moves turn next.
	The player with the most number of discs on the board at the end of the game wins. 


Presentation:
https://www.slideshare.net/NikolaiBelokonenko/othellofinalpptx

Video:
https://youtu.be/LPPWa-vr8nY

Curent version 1.0.0.
Ñhanges: 
	1. Renamed modules, functions and variables to meaningful name and according to next convention:
		Modules		- Pascale_Snake_Case
		function	- PascaleCase
		variables	- camelCase
		constants	- SCREAMING_SNAKE_CASE
	2. Reduce dependence between staff of project, and separate staff by modules according they functions and levels. 
	3. Transfer to a separate files typedefs, structures, constants and global variables.
	4. Include libraries and external modules only where it's needed.
	5. removed duplicate code.
	6. Fixed bug with draw ending game when game start.
	7. Fixed bug with "endles" animation after end of the game.
	8. Improve computer game algorithm.