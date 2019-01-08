#include <iostream>
#include <windows.h>
#include <vector>

#define SZ_1 18
#define SZ_2 32

using namespace std;
char tmp_map[SZ_1][SZ_2];

char map[SZ_1][SZ_2] = {
	"+#############################+",
	"|*        *     *            *|",
	"|  **  *  *  *  *  * ***** *  |",
	"|     **  *  *     *       ** |",
	"|  ***    *  * ***** ***** *  |",
	"|  *   *     *           * * *|",
	"|  *   *  *******    *** * *  |",
	"|  *  ***       ****  *  *    |",
	"|                        *** *|",
	"| *  **  ***** *****         *|",
	"| **    **         **  ***    |",
	"|      **           ** ****  *|",
	"|  ***  **         **         |",
	"|        ** ********  **** ***|",
	"|  *         *     *    **    |",
	"|  ****** **   **     * ***** |",
	"|          *****   *  *       |",
	"+#############################+"
};

void ShowMap(){	
	for (int i = 0; i < SZ_1; i++) {
		printf("%s\n", map[i]);
	}
}

void defeat(){
	printf( "****   *****  *****  *****      *     *******\n"
			"*   *  *      *      *         * *       *\n"
			"*   *  *****  *****  *****    *****      *\n"
			"*   *  *      *      *       *     *     *\n"
			"****   *****  *      *****  *       *    *\n");
}

void ShowStart(){
  printf("*           *  *****  *       ****    ****      *   *      *****  \n"
	     " *         *   *      *      *    *  *    *    * * * *     *      \n"
	     "  *   *   *    *****  *      *       *    *   *   *   *    *****  \n"
	     "   * * * *     *      *      *    *  *    *  *         *   *      \n"
	     "    *   *      *****  *****   ****    ****  *           *  *****  \n\n"
	         
	     "                       *******   ****\n"
	     "                          *     *    *\n"
	     "                          *     *    *\n"
	     "                          *     *    *\n"
	     "                          *      ****\n\n"
	                            
	        "    ****      *      ****      *   *          *      *   *\n"
			"    *   *    * *    *    *    * * * *        * *     **  *\n"
			"    ****    *****   *        *   *   *      *****    * * *\n"
			"    *      *     *  *    *  *         *    *     *   *  **\n"
			"    *     *       *  ****  *           *  *       *  *   *\n");
	
}


void gotoxy(short x, short y) //переміщує курсор в задану точку
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };

	SetConsoleCursorPosition(hStdout, position);
}

class entity { 
public:
	entity(int x, int y) {
		this->x = x;
		this->y = y;
	}

	void move_x(int p) {
		if (map[y][x + p] == ' ') x += p;
	}

	void move_y(int p) {
		if (map[y + p][x] == ' ') y += p;
	}

	void move(int p, int q) {
		x += p;
		y += q;
	}

	int get_x() { return x; }
	int get_y() { return y; }

	void draw(char p) {
		map[x][y] = p;
		gotoxy(x, y); printf("%c", p);
	}

private:
	int x;
	int y;
};

struct walk {
	short walk_count;
	short x;
	short y;
	short back;
};

struct target {
	short x;
	short y;
};

vector<target> walk_queue;

vector<walk> BFSArray;

void AddArray(int x, int y, int wc, int back) {
	if (tmp_map[y][x] == ' ' || tmp_map[y][x] == '.') {
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back(tmp);
	}
}

void FindPath(int sx, int sy, int x, int y) { // пошук дороги монстра до пекмена
	memcpy(tmp_map, map, sizeof(map));
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back(tmp);

	int i = 0;
	while (i < BFSArray.size()) {
		if (BFSArray[i].x == x && BFSArray[i].y == y) {
			walk_queue.clear();
			target tmp2;
			while (BFSArray[i].walk_count != 0) {
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_queue.push_back(tmp2);

				i = BFSArray[i].back;
			}

			break;
		}

		AddArray(BFSArray[i].x + 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x - 1, BFSArray[i].y, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y + 1, BFSArray[i].walk_count + 1, i);
		AddArray(BFSArray[i].x, BFSArray[i].y - 1, BFSArray[i].walk_count + 1, i);

		i++;
	}

	BFSArray.clear();
}


int main()
{
	bool playing = true;
	while(playing){
	
	bool running = true;
	int x = 29; // початкові коорд пекмена
	int y = 16; 
	int old_x; // старі коорд пекмена
	int old_y; 

	int ex = 1; //координати ворога
	int ey = 1;

	int pts = 0;
	char choise;
	char diffi;
    
    system("cls");
    ShowStart();
    printf("\nChoiсe difficulty:");
	printf("\nH -> Hard\nN -> Normal\nE -> Easy\n\nInput : ");

	int speedmod = 3;

	cin >> diffi;

	if (diffi == 'N') {
		speedmod = 2;		
	}
	else if (diffi == 'H') {
		speedmod = 1;
	}

	system("cls");
	ShowMap();
	
	gotoxy(x, y); cout << "H";

	int frame = 0;

	FindPath(ex, ey, x, y);

	while (running) {
		gotoxy(x, y); cout << " ";

		old_x = x;
		old_y = y;

		if (GetAsyncKeyState(VK_UP)) {
			if (map[y - 1][x] == '.') { y--; pts++; }
			else
				if (map[y - 1][x] == ' ') y--;
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			if (map[y + 1][x] == '.') { y++; pts++; }
			else
				if (map[y + 1][x] == ' ') y++;
		}
		if (GetAsyncKeyState(VK_LEFT)) {
			if (map[y][x - 1] == '.') { x--; pts++; }
			else
				if (map[y][x - 1] == ' ') x--;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			if (map[y][x + 1] == '.') { x++; pts++; }
			else
				if (map[y][x + 1] == ' ') x++;
		}

		if (old_x != x || old_y != y) {
			FindPath(ex, ey, x, y);
		}

		gotoxy(x, y); cout << "C";

		map[ey][ex] = '.';
		gotoxy(ex, ey); cout << ".";

		if (frame % speedmod == 0 && walk_queue.size() != 0) {
			ex = walk_queue.back().x;
			ey = walk_queue.back().y;
			walk_queue.pop_back();
		}

		gotoxy(ex, ey); cout << "M";

		if (ex == x && ey == y) {
			break;
		}


		gotoxy(32, 18);
		gotoxy(32, 1); cout << pts;
		Sleep(100);
		frame++;
	}

	system("cls");
	defeat();
	
	 printf("\nYour score is %d\n", pts);
	 printf("If you want to play again enter A\n"); 
	 printf("If you want to exit enter C\n");
	 printf("Your choise -> ");
	 cin >> choise;
	 	if(choise == 'A')	continue; // A повертаємося в початок циклу 
	 	
	 	if(choise == 'C')	return 0; // C виходимо з циклу 
	
}
	
	system("pause");
	return 0;
}
