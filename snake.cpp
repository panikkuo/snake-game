#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

class SnakeGame {
private:
	struct point {
		int x, y;
		bool operator == (const point& other) const {
			return (x == other.x && y == other.y);
		}
		void operator = (const point& other) {
			x = other.x;
			y = other.y;
		}
		int TailType;
	};

	enum eDirection {
		STOP = 0, LEFT, RIGHT, UP, DOWN
	};
	bool gameOver;
	int fieldWidth, fieldHeight;
	point snakeCoord, fruitCoord;
	vector<point> tailCoords;
	int score;
	eDirection snakeDirection;
	void Setup() {
		fieldWidth = 20;
		fieldHeight = 20;
		gameOver = false;
		snakeDirection = STOP;
		snakeCoord.x = fieldWidth / 2 + 1;
		snakeCoord.y = fieldHeight / 2 + 1;
		fruitCoord.x = rand() % fieldWidth + 1;
		fruitCoord.y = rand() % fieldHeight + 1;
		score = 0;
	}
	bool isSnakeTail(int x, int y, int& TailType) {
		for (const auto& tail : tailCoords) {
			if (tail.x == x && tail.y == y) {
				TailType = tail.TailType;
				return true;
			}
		}
		return false;
	}
	void Draw() {
		system("cls"); // очистка поля
		for (int i = 0; i < fieldWidth + 2; i++)
			cout << "#";
		cout << endl;

		for (int y = 0; y < fieldHeight + 1; y++) {
			for (int x = 0; x < fieldWidth + 2; x++) {
				int TailType = -1;
				if (x == 0 || x == fieldWidth + 1) cout << "#";
				else if (y == snakeCoord.y && x == snakeCoord.x) cout << "0";
				else if (y == fruitCoord.y && x == fruitCoord.x) cout << "F";
				else if (isSnakeTail(x, y, TailType)) {
					if (TailType == 0) cout << "o";
					else cout << "O";
				}
				else cout << " ";
			}
			cout << endl;
		}
		for (int i = 0; i < fieldWidth + 2; i++)
			cout << "#";
		cout << endl;
		cout << "SCORE : " << score << endl;
	}
	void Input() {
		if (_kbhit()) {
			switch (_getch())
			{
			case 'a':
				if (snakeDirection != RIGHT) snakeDirection = LEFT;
				break;
			case 'w':
				if (snakeDirection != DOWN) snakeDirection = UP;
				break;
			case 's':
				if (snakeDirection != UP) snakeDirection = DOWN;
				break;
			case 'd':
				if (snakeDirection != LEFT) snakeDirection = RIGHT;
				break;
			case 'x':
				gameOver = true;
				break;
			}
		}
	}
	bool isGameOver() {
		bool result = false;
		if (snakeCoord.x == 0 || snakeCoord.x == fieldWidth + 1
			|| snakeCoord.y == -1 || snakeCoord.y == fieldHeight + 1) result = true;
		else {
			int countTail = tailCoords.size();
			for (int i = 0; i < countTail; i++)
				if (snakeCoord == tailCoords[i]) {
					result = true;
					break;
				}
		}
		return result;
	}
	void Logic() {
		point newTailCoord;
		if (tailCoords.size() == 0) newTailCoord = snakeCoord;
		else newTailCoord = tailCoords[tailCoords.size() - 1];

		if (tailCoords.size() != 0) {
			for (int i = tailCoords.size() - 1; i >= 1; i--) {
				tailCoords[i] = tailCoords[i - 1];
			}
			tailCoords[0] = snakeCoord;
			for (int i = tailCoords.size() - 1; i >= 1; i--) {
				tailCoords[i].TailType = tailCoords[i - 1].TailType;
			}
			tailCoords[0].TailType = 0;
			if (tailCoords.size() != 0 && tailCoords[tailCoords.size() - 1].TailType == 1) {
				newTailCoord.TailType = 0; 
				tailCoords.push_back(newTailCoord);
				tailCoords[tailCoords.size() - 2].TailType = 0;
			}
			for (int i = tailCoords.size() - 1; i >= 1; i--) {
				tailCoords[i].TailType = tailCoords[i - 1].TailType;
			}
			tailCoords[0].TailType = 0;
		}

		switch (snakeDirection)
		{
		case LEFT:
			snakeCoord.x--;
			break;
		case RIGHT:
			snakeCoord.x++;
			break;
		case UP:
			snakeCoord.y--;
			break;
		case DOWN:
			snakeCoord.y++;
			break;
		}
		if (isGameOver()) gameOver = true;

		if (snakeCoord == fruitCoord) {
			score += 10;
			fruitCoord.x = rand() % fieldWidth + 1;
			fruitCoord.y = rand() % fieldHeight + 1;
			if (tailCoords.size() != 0)
				tailCoords[0].TailType = 1;
			else {
				newTailCoord.TailType = 0;
				tailCoords.push_back(newTailCoord);
			}
		}
		if (tailCoords.size() != 0 && tailCoords[tailCoords.size() - 1].TailType == 1) {
			newTailCoord.TailType = 0;
			tailCoords.push_back(newTailCoord);
			tailCoords[tailCoords.size() - 2].TailType = 0;
		}

	}
public:
	void StartSnakeGame() {
		srand(time(NULL));
		Setup();
		while (!gameOver) {
			Sleep(50);
			Draw(); Draw(); Draw();
			Input();
			Logic();
		}
		system("pause");
	}
};

int main() {
	SnakeGame play;
	play.StartSnakeGame();
	return 0;
}