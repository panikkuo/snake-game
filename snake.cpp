#include <iostream>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

struct Point {
    int x, y;
    bool operator == (const Point& other) {
        return (x == other.x) && (y == other.y);
    }
    void operator = (const Point& other) {
        x = other.x;
        y = other.y;
    }
};

struct Segment : Point {
    int x, y;
    enum Type {Head = 0, Body = 1, EatedBody = 2} segment_type;
};

struct Fruit : Point {
    void generate(int X, int Y) {
        x = rand() % (X - 1) + 1;
        y = rand() % (Y - 1) + 1;
    }
};

enum eDirection {
    STOP, LEFT, RIGHT, UP, DOWN
};

const int fieldWidth = 21;
const int fieldHeight = 21;

bool gameOver = false;

vector<Segment> snake;
eDirection direction;
Fruit fruit;
int score;

void Setup() {
    Segment head;
    head.x = fieldHeight / 2 + 1;
    head.y = fieldHeight / 2 + 1;
    head.segment_type = Segment::Head;
    snake.push_back(head);

    fruit.generate(fieldWidth, fieldHeight);
}

bool isSnake(int x, int y) {
    for (int i = 0; i < snake.size(); i++) 
        if (snake[i].x == x && snake[i].y == y)
            return true;
    
    return false;
}

char getSnakeSymbol(int x, int y) {
    for (int i = 0; i < snake.size(); i++) 
        if (snake[i].x == x && snake[i].y == y) {
            switch (snake[i].segment_type)
            {
                case Segment::Head:
                    return '0';
                case Segment::Body:
                    return 'o';
                case Segment::EatedBody:
                    return 'O';
            }
        }
    
    return 0;
} 

void Draw() {
    system("cls");

    for (int i = 0; i <= fieldWidth; i++)
        cout << "#";
    cout << endl;

    for (int y = 1; y <= fieldHeight - 1; y++) {
        for (int x = 0; x <= fieldWidth; x++) {
            if (x == 0 || x == fieldWidth) cout << "#";
            else if (x == fruit.x && y == fruit.y) cout << "F";
            else if (isSnake(x, y)) cout << getSnakeSymbol(x, y);
            else cout << ' ';
        }
        cout << endl;
    }

    for (int i = 0; i <= fieldWidth; i++)
        cout << "#";
    cout << endl;

    cout << "Your score : " << score;
    cout << endl; 
}

void Input() {
    if (_kbhit()) {
        switch (_getch())
        {
        case 'w':
            if (direction != DOWN) direction = UP;
            break;
        case 'a':
            if (direction != RIGHT) direction = LEFT;
            break;
        case 's':
            if (direction != UP) direction = DOWN;
            break;
        case 'd':
            if (direction != LEFT) direction = RIGHT;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}

bool isGameOver() {
    if (snake[0].x == 0 || snake[0].x == fieldWidth ||
        snake[0].y == 0 || snake[0].y == fieldHeight) {
            return true;
        }
    
    for (int i = 1; i < snake.size(); i++)
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
            return true;
    
    return false;
}

void Logic() {
    Segment newSegment;
    newSegment.x = snake[snake.size() - 1].x;
    newSegment.y = snake[snake.size() - 1].y;
    newSegment.segment_type = Segment::Body;

    snake[0].segment_type = Segment::Body;
    for (int i = snake.size() - 1; i > 0; i--) {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
        snake[i].segment_type = snake[i - 1].segment_type;
    }

    snake[0].segment_type = Segment::Head;
    
    switch (direction)
	{
		case LEFT:
            snake[0].x--;
			break;
		case RIGHT:
            snake[0].x++;
			break;
		case UP:
            snake[0].y--;
			break;
		case DOWN:
			snake[0].y++;
    }

    if (isGameOver()) gameOver = true;

    if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
        score += 100;
        if (snake.size() == 1)
            snake.push_back(newSegment);
        else 
            snake[1].segment_type = Segment::EatedBody;
        fruit.generate(fieldWidth, fieldHeight);
    }

    if (snake[snake.size() - 1].segment_type == Segment::EatedBody) {
        snake[snake.size() - 1].segment_type = Segment::Body;
        snake.push_back(newSegment);
    }
}

int main() {
    srand(time(NULL));
    Setup();
    while (!gameOver) {
        Sleep(150);
        Draw();
        Input();
        Logic();
    }
    system("pause");
}


//field
//0----21
//#1-20#