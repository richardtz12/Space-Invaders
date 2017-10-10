#include "mylib.h"
#include "DMA.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void waitForVblank() {
	while(SCANLINECOUNTER > 160)
		;
	while(SCANLINECOUNTER<160)
		;
}

void setPixel(int r, int c, u16 color) {
  videoBuffer[240*r+c] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r=0; r<height; r++) {
	DMA[3].src = &color;
	DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
	DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void drawImage3(int r, int c, int width, int height, const u16* image) {

  for (int a = 0; a < height; a++) {
		DMA[3].src = (unsigned short*)(image + a * width);
		DMA[3].dst = videoBuffer + a * 240 + ( r * 240 + c);
		DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | (width);	
	}
}

//reverse the image from top to bottom
// void drawImage3(int r, int c, int width, int height, const u16* image) {

//   for (int a = 0; a < height; a++) {
// 		DMA[3].src = (unsigned short*)(image + (height - a - 1) * width);
// 		DMA[3].dst = videoBuffer + a * 240 + ( r * 240 + c);
// 		DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | (width);	
// 	}
// }

void clearScreen() {
	drawRect(0, 0, 160, 240, 0x00000);
}
