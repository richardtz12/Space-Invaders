
#include "mylib.h"
#include <stdlib.h>
#include "spaceinvaders.h" 
#include <stdbool.h>
#include "gameoverscreen.h"
#include "tank20.h"
#include "victoryscreen.h"
#include <stdio.h>
#include "text.h"
#include <string.h>

int main() {
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	//initializes the colors
	int colors[] = { RED, BLUE, GREEN, YELLOW, CYAN, MAGENTA, WHITE };

	//initializes the Aliens
	ALIEN alienArray[NALIENS];
	int nAliens = sizeof(alienArray) / sizeof(alienArray[0]);

	for (int i = 0; i < nAliens; i++) {
		alienArray[i].row = 10;
		alienArray[i].col = 20 + 10 * i;
		alienArray[i].color = colors[1];
		alienArray[i].size = 5;
		alienArray[i].alive = true;
		alienArray[i].forward = true;
		if ((i % 2) == 0) {
			alienArray[i].alienType = HOVERER;
		}
		else {
			alienArray[i].alienType = CHARGER;
		}
	}

	//initializes the playerShip
	SHIP playerShip;

	playerShip.row = 140;
	playerShip.col = 120;
	playerShip.sizeOfShip = 20;
	playerShip.color = colors[2];
	playerShip.lives = 5;

	//initializes the bullets
	BULLET tank_bullets[TANKBULLET];

	int tankBulletSize = 2;

	for (int i=0; i<TANKBULLET; i++) {
    	tank_bullets[i].row = playerShip.row;
    	tank_bullets[i].col = playerShip.col;
    	tank_bullets[i].fired = false;
    	tank_bullets[i].alive = false;
  	}

	//initializes variables for the loop
	int state = 1;
	bool screenCleared = false;
	int aliensLeft = 20;
	while(1) { // Game Loop
		if (state == 1) {
			if (screenCleared == false) {
				clearScreen();
				screenCleared = true;
			}
			drawImage3(0,0,240,160, spaceinvaders);
			if (KEY_DOWN_NOW(BUTTON_START)) {
				clearScreen();
				state = 2;

			//reinitalizes everything since the game needs to refresh when I press start
				int nAliens = sizeof(alienArray) / sizeof(alienArray[0]);
					for (int i = 0; i < nAliens; i++) {
						alienArray[i].row = 10;
						alienArray[i].col = 10 + 10 * i;
						alienArray[i].color = colors[1];
						alienArray[i].size = 5;
						alienArray[i].alive = true;
						alienArray[i].forward = true;
						if ((i % 2) == 0) {
							alienArray[i].alienType = HOVERER;
						}
						else {
							alienArray[i].alienType = CHARGER;
						}
					}
				playerShip.lives = 5;
				playerShip.row = 140;
				playerShip.col = 120;
				aliensLeft = 20;

				for (int i=0; i<TANKBULLET; i++) {
    				tank_bullets[i].row = playerShip.row;
    				tank_bullets[i].col = playerShip.col;
    				tank_bullets[i].fired = false;
    				tank_bullets[i].alive = false;
  				}
			}
		}

		//goes back to the title screen
		if (KEY_DOWN_NOW(BUTTON_SELECT)) {
			state = 1;
			screenCleared = false;
		}

		if (state == 2) {

			//draws the playerLives, Level, and Aliens Remaining text at the top of the screen and updates them
			char playerLives[9];
    		sprintf(playerLives, "Lives: %02d", playerShip.lives);
    		drawRect(0, 120, 10, 15, BLACK);
    		drawString(0, 80, playerLives, WHITE);

    		char aliensLeftDisplay[10];
    		sprintf(aliensLeftDisplay, "Aliens: %02d", aliensLeft);
    		drawRect(0, 228, 10, 12, BLACK);
    		drawString(0,180, aliensLeftDisplay, WHITE);

    		drawString(0,0, "Level 1", GREEN);

    		//goes to the appropriate end game screen based on the conditional
			if (playerShip.lives == 0) {
				state = 3;
			}
			if (aliensLeft == 0) {
				state = 4;
			}
			int oldrow = playerShip.row;
			int oldcol = playerShip.col;

			//fires a bullet
			if (KEY_DOWN_NOW(BUTTON_A)) {
				for (int i = 0; i < TANKBULLET; i ++) {
					if (tank_bullets[i].fired == false) {
						tank_bullets[i].fired = true;
						tank_bullets[i].alive = true;
    					tank_bullets[i].row = playerShip.row;
    					tank_bullets[i].col = playerShip.col + playerShip.sizeOfShip / 3;
    					i = TANKBULLET;
					}
				}
			}

			//navigates the ship
			if(KEY_DOWN_NOW(BUTTON_UP)) {
				playerShip.row--;
				if(playerShip.row < 20) {
					playerShip.row = 20;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_DOWN)) {
				playerShip.row++;
				if(playerShip.row > 140) {
					playerShip.row = 140;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_LEFT)) {
				playerShip.col--;
				if(playerShip.col <0) {
					playerShip.col = 0;
				}
			}
			if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
				playerShip.col++;
				if(playerShip.col>220) {
					playerShip.col = 220;
				}
			}

			//handles alien charger movement, they go down until they hit the bottom of the screen and then go up and repeat
			for (int i = 0; i < nAliens; i++) {		
				if (alienArray[i].alive == false) {
					continue;
				}		
				if ((alienArray[i].alienType == CHARGER) && (rand() % 2 == 0)) {
					if (alienArray[i].row >= 150 || alienArray[i].row == 0) {
						alienArray[i].forward = !(alienArray[i].forward);
					}
					if (alienArray[i].forward == true) {
						int oldAlienRow = alienArray[i].row;
						alienArray[i].row++;
						drawRect(oldAlienRow, alienArray[i].col, alienArray[i].size, alienArray[i].size, BLACK);
						drawRect(alienArray[i].row, alienArray[i].col, alienArray[i].size, alienArray[i].size, alienArray[i].color);
					}
					else if (alienArray[i].forward == false) {
						int oldAlienRow = alienArray[i].row;
						alienArray[i].row--;
						drawRect(oldAlienRow, alienArray[i].col, alienArray[i].size, alienArray[i].size, BLACK);
						drawRect(alienArray[i].row, alienArray[i].col, alienArray[i].size, alienArray[i].size, alienArray[i].color);
					}
				}
			// collision between aliens and playership
				if (((alienArray[i].row) <= (playerShip.row + playerShip.sizeOfShip)) && (alienArray[i].row + alienArray[i].size >= playerShip.row) 
					&& (alienArray[i].col <= (playerShip.col + playerShip.sizeOfShip)) && (alienArray[i].col + alienArray[i].size >= playerShip.col)) {
					drawRect(alienArray[i].row, alienArray[i].col, alienArray[i].size, alienArray[i].size, BLACK);
					if (alienArray[i].alive == true) {
						alienArray[i].alive = false;
						playerShip.lives--;
						aliensLeft--;
					}
				}

				//collision between tank bullets and the aliens
				for (int j = 0; j < TANKBULLET; j++) {
					if (((alienArray[i].row) <= (tank_bullets[j].row + tankBulletSize)) && (alienArray[i].row + alienArray[i].size >= tank_bullets[j].row) 
						&& (alienArray[i].col <= (tank_bullets[j].col + tankBulletSize)) && (alienArray[i].col + alienArray[i].size >= tank_bullets[j].col)) {
						drawRect(alienArray[i].row, alienArray[i].col, alienArray[i].size, alienArray[i].size, BLACK);
						if (alienArray[i].alive == true && tank_bullets[j].fired == true) {
							aliensLeft = aliensLeft - 1;
							alienArray[i].alive = false;
						}
					}	
				}
			}

			//controls whether a tank bullet at an index in the array is "alive" and can fire it again if it isn't alive
			for (int i=0; i<TANKBULLET; i++) {
				if (tank_bullets[i].row < -10) {
					tank_bullets[i].alive=false;
					tank_bullets[i].fired = false;
			}
				if (tank_bullets[i].fired == true && tank_bullets[i].alive == true) {
					drawRect(tank_bullets[i].row, tank_bullets[i].col, 5, 5, BLACK);
		    		tank_bullets[i].row--;
		    		drawRect(tank_bullets[i].row, tank_bullets[i].col, tankBulletSize, tankBulletSize, RED);
	    		}
  			}

			waitForVblank();

			//draws the aliens
			for (int i = 0; i < nAliens; i++) {
				if (alienArray[i].alive == true) {
					drawRect(alienArray[i].row, alienArray[i].col, alienArray[i].size, alienArray[i].size, alienArray[i].color);
				}
			}

			//draws the playership
			struct playerShip *sp = &playerShip;
			drawRect(oldrow, oldcol, sp->sizeOfShip, sp->sizeOfShip, BLACK);
			drawImage3(playerShip.row,playerShip.col, playerShip.sizeOfShip, playerShip.sizeOfShip, tank20);
		}

		//shows the gameoverscreen
		if (state == 3) {
			if (screenCleared == false) {
				clearScreen();
				screenCleared = true;
			}
			drawImage3(0,0,240,160, gameoverscreen);
		}

		//shows the victoryscreen
		if (state == 4) {
			if (screenCleared == false) {
				clearScreen();
				screenCleared = true;
			}
			drawImage3(0,0,240,160, victoryscreen);
		}
	} // Game Loop
}
