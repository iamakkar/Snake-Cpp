#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <thread>

using namespace std;

const int width = 20, height = 20;
char grid[width + 2][height + 3];
bool gameOver = false;

enum direction
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class snake
{
public:
    int i;
    int j;
    int tailLength;
    enum direction dir;
    snake()
    {
        i = 10;
        j = 10;
        tailLength = 1;
        dir = UP;
    }
};

class fruit
{
public:
    int i;
    int j;
    void set_fruit(int x, int y)
    {
        this->i = x;
        this->j = y;
    }
};

//DRAW THE AREA
void Draw(int x, int y, int fx, int fy)
{
    system("clear");
    for (int i = 0; i < width + 2; i++)
    {
        for (int j = 0; j < height + 3; j++)
        {
            if ((j == 0 || j == 21 || i == 0 || i == 21) && j != width + 2)
            {
                grid[i][j] = 'X';
            }
            else if (j == height + 2 && i != 21)
            {
                grid[i][j] = '\n';
            }
            else
            {
                grid[i][j] = ' ';
            }
        }
    }

    grid[x][y] = '*';
    grid[fx][fy] = '^';

    for (int i = 0; i < width + 2; i++)
    {
        for (int j = 0; j < height + 3; j++)
        {
            cout << grid[i][j];
        }
    }
}

enum direction Movement()
{
    enum direction dir;
    //cout << '1';
    char c;
    c = getchar(); //removed while loop here so that flow could come out of the function
    if (c == 27)
    {
        c = getchar();
        if (c == 91)
        {
            c = getchar();
            switch (c)
            {
            case 65:
                dir = UP;
                //cout << c;
                break;
            case 66:
                dir = DOWN;
                //cout << c;
                break;
            case 67:
                dir = RIGHT;
                //cout << c;
                break;
            case 68:
                dir = LEFT;
                //cout << c;
                break;
            }
        }
    }
    return dir;
}

void Logic(direction dir, int &x, int &y) //call by reference here
{
    switch (dir)
    {
    case UP:
        x--;
        break;
    case DOWN:
        x++;
        break;
    case LEFT:
        y--;
        break;
    case RIGHT:
        y++;
        break;
    }
}

int main()
{
    system("stty cbreak"); //enter key (\n) or any whitespace character no longer required to enter inputs
    system("stty -echo");  //do not echo input to screen (can't see what you type as input)
    snake sn;
    fruit fr;
    fr.set_fruit(rand() % 20, rand() % 20);
    while (!gameOver)
    {
        Draw(sn.i, sn.j, fr.i, fr.j);
        sn.dir = Movement();
        Logic(sn.dir, sn.i, sn.j);
    }

    return 0;
}
