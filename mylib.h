#include <stdbool.h>

typedef unsigned short u16;

#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define OFFSET(r, c, rowlen) ((c) + (rowlen)*(r)) 
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31, 0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)
#define YELLOW COLOR(31, 31, 0)
#define BLACK 0

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

#define SCANLINECOUNTER *(volatile short *)0x4000006

//initializes the number of aliens and number of bullets in the bullet system.
#define NALIENS 20
#define TANKBULLET 10

typedef struct bullet {
	int row;
	int col;
	int size;
	int speed;
	bool fired;
	bool alive;
	unsigned short color;
} BULLET;

typedef struct alien {
	int row;
	int col;
    enum ALIENTYPE {HOVERER, CHARGER, SHOOTER} alienType;
	int size;
	bool forward;
	bool alive;
	unsigned short color;
	BULLET bullet;
} ALIEN; 

typedef struct playerShip {
	int row;
	int col;
	int lives;
	int sizeOfShip;
	BULLET bullet;
	unsigned short color;
} SHIP;

void waitForVblank();
void drawRect(int row, int col, int height, int width, unsigned short color);
void drawImage3(int r, int c, int width, int height, const u16* image);
void clearScreen();
void setPixel(int r, int c, u16 color);