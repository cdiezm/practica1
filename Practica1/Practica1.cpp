#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>     
#include <time.h>     
#include <list>

#define ESC_KEY 27
#define J_KEY 'j'
#define K_KEY 'k'
#define L_KEY 'l'
#define H_KEY 'h'

//Entities
struct bala {
	int x;
	int dir;
};

struct pj {
	int x;
	int lives;
};

//meter puntos dentro de pj?????

int main(){
	//var
	const unsigned int        size           = 80;
	pj                        main_pj        = { 40, 3 };
//	int                       pj_pos         = 40;
//	int                       left_bullet    = -1;
//	int                       right_bullet   = size + 1;
//	int                       enemy_pos      = -1;
	int                       key	         = 0;
	bool                      end_game       = false;
	const unsigned int        points_mush    = 100;
	int                       mush_pos       = -1;
	unsigned int              score          = 0;
//	unsigned int              lives          = 3;
	std::list<int>			  mush_list;
	std::list<int>			  enemy_list;
	std::list<bala>			  bullet_list;

	srand(time(NULL));

	printf("\n");
	printf("\n"); 
	printf("\n");
	printf("\n");
	printf("\n");

	while (key != ESC_KEY && !end_game) {
		//Lectura
		Sleep(100);
		if (_kbhit()) {
			key = _getch();
			switch (key) {
				case J_KEY:
					if (main_pj.x > 0) {
						main_pj.x--;
						if (enemy_pos == main_pj.x) {
							if (main_pj.lives > 0) {
								main_pj.lives--;
								enemy_pos = -1;
							}
							else {
								end_game = true;
							}
						}
						if (main_pj.x == mush_pos) {
							mush_pos = -1;
							score += points_mush;
						}
					}
					break;
				case K_KEY:
					if (main_pj.x < size - 1) {
						main_pj.x++;
						if (enemy_pos == main_pj.x) {
							if (main_pj.lives > 0) {
								main_pj.lives--;
								enemy_pos = -1;
							}
							else {
								end_game = true;
							}
						}
						if (pj_pos == mush_pos) {
							mush_pos = -1;
							score += points_mush;
						}
					}
					break;
				case L_KEY:
					if (left_bullet < 0) {
						if (right_bullet == size + 1) {
							right_bullet = main_pj.x + 1;
						}
					}
					break;
				case H_KEY:
					if (right_bullet == size + 1) {
						if (left_bullet == -1) {
							left_bullet = main_pj.x - 1;
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
			if (enemy_pos > main_pj.x) {
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

		if (enemy_pos == main_pj.x) {
			if (main_pj.lives > 0) {
				main_pj.lives--;
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
				mush_pos = main_pj.x;
				while (mush_pos == main_pj.x) {
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
			printf("%i ", main_pj.lives);
			for (int i = 0; i < size; i++) {
				if (i == main_pj.x) {
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

void checkEnemyColission(std::list<int> *enemy_list, pj &main_pj) {
	for (auto it = enemy_list->begin(); it != enemy_list->end();) {
		if (main_pj.x + 1 == *it || main_pj.x - 1 == *it) {
			enemy_list->erase(it);
			main_pj.lives--;
		}
		else {
			it++;
		}
	}
}

void checkMushroomColission(std::list<int> *mush_list, pj &main_pj) {
	for (auto it = mush_list->begin(); it != mush_list->end();) {
		if (main_pj.x + 1 == *it || main_pj.x - 1 == *it) {
			mush_list->erase(it);
			//main_pj.lives--;
		}
		else {
			it++;
		}
	}
}



