#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>     
#include <time.h>     

#define ESC_KEY 27
#define J_KEY 'j'
#define K_KEY 'k'
#define L_KEY 'l'
#define H_KEY 'h'

int main(){
	//var
	const unsigned int        size           = 80;
	unsigned int              x_pos          = 40;
	int                       left_bullet    = -1;
	unsigned int              right_bullet   = size + 1;
	int                       enemy_pos      = -1;
	int                       key	         = 0;
	bool                      end_game       = false;
	const unsigned int        points_mush    = 100;
	int                       mush_pos       = -1;
	unsigned int              score          = 0;
	unsigned int              lives          = 3;

	srand(time(NULL));

	while (key != ESC_KEY && !end_game) {
		//Lectura
		Sleep(100);
		if (_kbhit()) {
			key = _getch();
			switch (key) {
				case J_KEY:
					if (x_pos > 0) {
						x_pos--;
						if (enemy_pos == x_pos) {
							if (lives > 0) {
								lives--;
								enemy_pos = -1;
							}
							else {
								end_game = true;
							}
						}
						if (x_pos == mush_pos) {
							mush_pos = -1;
							score += points_mush;
						}
					}
					break;
				case K_KEY:
					if (x_pos < size - 1) {
						x_pos++;
						if (enemy_pos == x_pos) {
							if (lives > 0) {
								lives--;
								enemy_pos = -1;
							}
							else {
								end_game = true;
							}
						}
						if (x_pos == mush_pos) {
							mush_pos = -1;
							score += points_mush;
						}
					}
					break;
				case L_KEY:
					if (left_bullet < 0) {
						if (right_bullet == size + 1) {
							right_bullet = x_pos + 1;
						}
					}
					break;
				case H_KEY:
					if (right_bullet == size + 1) {
						if (left_bullet == -1) {
							left_bullet = x_pos - 1;
						}
					}
					break;
			}
		}

		//Bullet
		if (right_bullet < size + 1) {
			right_bullet++;
			if (enemy_pos == right_bullet) {
				enemy_pos = -1;
			}
		}

		if (left_bullet >= 0) {
			left_bullet--;
			if (enemy_pos == left_bullet) {
				enemy_pos = -1;
			}
		}

		//Enemy
		if (enemy_pos != -1) {
			if (enemy_pos > x_pos) {
				enemy_pos--;
			} 
			else {
				enemy_pos++;
			}
			if (enemy_pos == right_bullet || enemy_pos == left_bullet) {
				enemy_pos = -1;
			}
		}
		else {
			int pos_random = rand() % 2 - 1 ;

			if (pos_random) {
				enemy_pos = size - 1;
			} 
			else {
				enemy_pos = 0;
			}
		}

		if (enemy_pos == x_pos) {
			if (lives > 0) {
				lives--;
				enemy_pos = -1;
			}
			else {
				end_game = true;
			}
			
		}

		//Mushroom
		if (mush_pos == -1) {
			int mush_rand = rand() % 70 + 1;
			if (mush_rand == 4) {
				mush_pos = x_pos;
				while (mush_pos == x_pos) {
					mush_pos = rand() % size;
				}
			}
		}
		

		//Pintado
		if (end_game) {
			printf("\n\nYOU LOSE!!!!!!!!");
		} 
		else {
			printf("\r");
			printf("%i ", lives);
			for (int i = 0; i < size; i++) {
				if (i == x_pos) {
					printf("%c", 'X');
				} 
				else if (i == right_bullet) {
					printf("%c", '>');
				}
				else if (i == left_bullet) {
					printf("%c", '<');
				}
				else if (i == enemy_pos) {
					printf("%c", '@');
				}
				else if (i == mush_pos) {
					printf("%c", '*');
				}
				else {
					printf("%c", '-');
				}
			}
			printf(" %i", score);
		}
	}
	getchar();
    return 0;
}

