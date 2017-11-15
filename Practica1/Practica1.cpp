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
struct bullet {
	int x;
	int dir;	//1 == right / 0 == left
};

struct pj {
	int x;
	int lives;
	int score;
};

struct mushroom {
	int x;
	int points;

};

struct enemy {
	int x;
	int dir;		//1 == right / 0 == left
};

//Functions Declaration
void checkEnemyColission(std::list<enemy> *enemy_list, pj &main_pj);
void checkMushroomColission(std::list<mushroom> *mush_list, pj &main_pj);
void checkBulletColission(std::list<bullet> *bullet_list, std::list<enemy> *enemy_list);
void moveBullets(std::list<bullet> *bullet_list, int world_size);
void moveEnemies(std::list<enemy> *enemy_list);
void sortBulletList(std::list<bullet> *bullet_list);
void sortEnemyList(std::list<enemy> *enemy_list);
void sortMushroomList(std::list<mushroom> *mush_list);
void paintWorld(pj &main_pj, std::list<enemy> *enemy_list, std::list<bullet> *bullet_list, std::list<mushroom> *mush_list, int size);
void paintWeather(int world_size);
void consoleCursorGoToXY(int x, int y);
void hidecursor();

int main(){

	//var
	const unsigned int        world_size     = 80;
	pj                        main_pj        = { 40, 3 };
	int                       key	         = 0;
	bool                      end_game       = false;
	const int                 enemy_prob     = 25;
	const int                 mush_prob      = 80;
	const unsigned int        mush_points    = 100;
	std::list<mushroom>	      mush_list;
	std::list<enemy>	      enemy_list;
	std::list<bullet>		  bullet_list;
	bullet                    new_bullet;

	srand(static_cast<unsigned int>(time(NULL)));

	hidecursor();

	printf("\n");
	printf("\n"); 
	printf("\n");
	printf("\n");
	printf("\n");

	while (key != ESC_KEY && !end_game) {

		//Lectura
		Sleep(50);
		if (_kbhit()) {
			key = _getch();
			switch (key) {
				case J_KEY:
					if (main_pj.x > 0) {
						main_pj.x--;
					}
					break;
				case K_KEY:
					if (main_pj.x < world_size - 1) {
						main_pj.x++;
					}
					break;
				case L_KEY:
					new_bullet = { main_pj.x, 1 };
					bullet_list.push_front(new_bullet);
					break;
				case H_KEY:
					new_bullet = { main_pj.x, 0 };
					bullet_list.push_front(new_bullet);
					break;
			}
		}

		//Bullet
		moveBullets(&bullet_list, world_size);

		//Enemy
		int rand_enemy = rand() % enemy_prob + 1;

		if (rand_enemy == 1) {
			rand_enemy = rand() % enemy_prob + 1;
			int pos_n_enemy = (rand_enemy % 2) ? world_size - 1 : 0;
			enemy n_enemy = { pos_n_enemy, !(rand_enemy % 2) };
			enemy_list.push_front(n_enemy);
		}
		
		moveEnemies(&enemy_list);

		//Mushroom
		int mush_rand = rand() % mush_prob + 1;

		if (mush_rand == 1) {
			int pos_n_mush = rand() % world_size - 1;
			mushroom n_mush = { pos_n_mush, mush_points};
			mush_list.push_front(n_mush);
		}

		//Collisions
		checkBulletColission(&bullet_list, &enemy_list);
		checkEnemyColission(&enemy_list, main_pj);
		checkMushroomColission(&mush_list, main_pj);

		//Sort
		sortBulletList(&bullet_list);
		sortEnemyList(&enemy_list);
		sortMushroomList(&mush_list);

		//Check End Game
		if (!main_pj.lives) {
			end_game = true;
		}

		//Pintado
		if (end_game) {
			printf("\n\nYOU LOSE!!!!!!!!");
		}
		else {
			paintWorld(main_pj, &enemy_list, &bullet_list, &mush_list, world_size);	
		}
	}

	getchar();
    return 0;
}

void checkEnemyColission(std::list<enemy> *enemy_list, pj &main_pj) {
	for (auto it = enemy_list->begin(); it != enemy_list->end();) {
		if (main_pj.x + 1 == it->x || main_pj.x - 1 == it->x) {
			it = enemy_list->erase(it);
			main_pj.lives--;
		}
		else {
			it++;
		}
	}
}

void checkMushroomColission(std::list<mushroom> *mush_list, pj &main_pj) {
	for (auto it = mush_list->begin(); it != mush_list->end();) {
		if (main_pj.x + 1 == it->x || main_pj.x - 1 == it->x) {
			main_pj.score += it->points;
			it = mush_list->erase(it);
		}
		else {
			it++;
		}
	}
}

void checkBulletColission(std::list<bullet> *bullet_list, std::list<enemy> *enemy_list) {
	for (auto it_bullet = bullet_list->begin(); it_bullet != bullet_list->end();) {
		bool bullet_destroyed = false;
		for (auto it_enemy = enemy_list->begin(); it_enemy != enemy_list->end() && !bullet_destroyed;) {
			if (it_bullet->x + 1 == it_enemy->x || it_bullet->x - 1 == it_enemy->x || it_bullet->x == it_enemy->x) {
				it_enemy = enemy_list->erase(it_enemy);
				it_bullet = bullet_list->erase(it_bullet);
				bullet_destroyed = true;
			}
			else {
				it_enemy++;
			}
		}
		if (!bullet_destroyed) {
			it_bullet++;
		}
	}
}

void moveBullets(std::list<bullet> *bullet_list, int world_size) {
	for (auto it_bullet = bullet_list->begin(); it_bullet != bullet_list->end();) {
		if (it_bullet->dir) {
			it_bullet->x += 1;
		}
		else {
			it_bullet->x -= 1;
		}

		if (it_bullet->x < 0 || it_bullet->x >= world_size) {
			it_bullet = bullet_list->erase(it_bullet);
		}
		else {
			it_bullet++;
		}
	}
}

void moveEnemies(std::list<enemy> *enemy_list) {
	for (auto it_enemy = enemy_list->begin(); it_enemy != enemy_list->end(); it_enemy++) {
		if (it_enemy->dir) {
			it_enemy->x += 1;
		}
		else {
			it_enemy->x -= 1;
		}
	}
}

void sortBulletList(std::list<bullet> *bullet_list) {
	bullet_list->sort([](const bullet & a, const bullet & b) { return a.x < b.x; });
}

void sortEnemyList(std::list<enemy> *enemy_list) {
	enemy_list->sort([](const enemy & a, const enemy & b) { return a.x < b.x; });
}

void sortMushroomList(std::list<mushroom> *mush_list) {
	mush_list->sort([](const mushroom & a, const mushroom & b) { return a.x < b.x; });
}

void paintWorld(pj &main_pj, std::list<enemy> *enemy_list, std::list<bullet> *bullet_list, std::list<mushroom> *mush_list, int world_size) {
	
	auto it_enemy  = enemy_list->begin();
	auto it_bullet = bullet_list->begin();
	auto it_mush   = mush_list->begin();

	bool done_enemy   = it_enemy  == enemy_list->end();
	bool done_bullet  = it_bullet == bullet_list->end();
	bool done_mush    = it_mush   == mush_list->end();

	paintWeather(world_size);

	printf("\r");
	printf("%i ", main_pj.lives);
	for (int i = 0; i < world_size; i++) {
		if (i == main_pj.x) {
			printf("%c", 'X');
		}
		else if (!done_bullet && i == it_bullet->x) {
			if (it_bullet->dir) {
				printf("%c", '>');
			}
			else {
				printf("%c", '<');
			}
			it_bullet++;
			done_bullet = it_bullet == bullet_list->end();
		}
		else if (!done_enemy && i == it_enemy->x) {
			printf("%c", '@');
			it_enemy++;
			done_enemy = it_enemy == enemy_list->end();
		}
		else if (!done_mush && i == it_mush->x) {
			printf("%c", '*');
			it_mush++;
			done_mush = it_mush == mush_list->end();
		}
		else {
			printf("%c", '-');
		}
	}
	printf(" %i", main_pj.score);
}

void paintWeather(int world_size) {

	int height = 4;
	int max_height = height + 5;
	
	for (; height < max_height; height++) {
		consoleCursorGoToXY(0, height);
		printf("\r");
		printf("  ");
		for (int j = 0; j < world_size; j++) {
			
			if (rand() % 2 + 1 == 1 ) {
				printf("/");
			}
			else {
				printf(" ");
			}
		}
	}

	consoleCursorGoToXY(0, max_height);
	printf("\r");
	printf("  ");
	for (int i = 0; i < world_size; i++) {
		printf(" ");
	}
}

void consoleCursorGoToXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidecursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}







