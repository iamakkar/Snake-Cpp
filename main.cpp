#include <iostream>
#include <cstdlib>
#ifndef _WIN32
#include <ncurses.h>
#endif
// #ifdef _WIN32
// #include <conio.h>
// #endif
#include <string.h>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

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
    std::vector<std::pair<int, int> > cord;
    enum direction dir;
    snake()
    {
        std::pair<int, int> head(10, 10);
        cord.push_back(head);
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
void Draw(int fx, int fy, snake &sn)
{
    // system("clear");
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
    grid[fx][fy] = '^';
    for (int i = 0; i < sn.cord.size(); i++)
    {
        grid[sn.cord[i].first][sn.cord[i].second] = '*';
    }

    for (int i = 0; i < width + 2; i++)
    {
        for (int j = 0; j < height + 3; j++)
        {
            printw("%c", grid[i][j]);
        }
    }
}

void Movement(direction &dir)
{
    int x = getch();
    if (x == KEY_LEFT && dir != RIGHT)
        dir = LEFT;
    else if (x == KEY_RIGHT && dir != LEFT)
        dir = RIGHT;
    else if (x == KEY_UP && dir != DOWN)
        dir = UP;
    else if (x == KEY_DOWN && dir != UP)
        dir = DOWN;
    clear();
}

void Logic(direction dir, std::vector<std::pair<int, int> > &v) //call by reference here
{
    // set first element to position determined by keyboard input
    // set all other elements to position of previous element
    for (int i = v.size() - 1; i > 0; i--)
    {
        v[i].first = v[i - 1].first;
        v[i].second = v[i - 1].second;
    }
    int &x = v[0].first, &y = v[0].second;
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

inline bool check_fruit(int x, int y, int fx, int fy)
{
    return (x == fx && y == fy);
}

void after()
{
    std::cout << "\e[31m";
    std::cout << "  ▄████  ▄▄▄       ███▄ ▄███▓▓█████     ▒█████   ██▒   █▓▓█████  ██▀███  \n";
    std::cout << " ██▒ ▀█▒▒████▄    ▓██▒▀█▀ ██▒▓█   ▀    ▒██▒  ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒\n";
    std::cout << "▒██░▄▄▄░▒██  ▀█▄  ▓██    ▓██░▒███      ▒██░  ██▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒" << std::endl;
    std::cout << "░▓█  ██▓░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄    ▒██   ██░  ▒██ █░░▒▓█  ▄ ▒██▀▀█▄  " << std::endl;
    std::cout << "░▒▓███▀▒ ▓█   ▓██▒▒██▒   ░██▒░▒████▒   ░ ████▓▒░   ▒▀█░  ░▒████▒░██▓ ▒██▒" << std::endl;
    std::cout << " ░▒   ▒  ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░   ░ ▒░▒░▒░    ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░" << std::endl;
    std::cout << "  ░   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░     ░ ▒ ▒░    ░ ░░   ░ ░  ░  ░▒ ░ ▒░" << std::endl;
    std::cout << "░ ░   ░   ░   ▒   ░      ░      ░      ░ ░ ░ ▒       ░░     ░     ░░   ░ " << std::endl;
    std::cout << "      ░       ░  ░       ░      ░  ░       ░ ░        ░     ░  ░   ░     " << std::endl;
    std::cout << "                                                     ░                   \e[0m" << std::endl;
    system("stty echo");
    exit(1);
}

void GameOver(snake &sn)
{
    int row = sn.cord[0].first, col = sn.cord[0].second;
    if (row <= 0 || col <= 0 || row > 20 || col > 20)
        gameOver = true;
    std::map<std::pair<int, int>, bool> m;
    std::pair<int, int> head = sn.cord[0];
    for (int i = 1; i < sn.cord.size(); i++)
    {
        if (head == sn.cord[i])
        {
            gameOver = true;
            break;
        }
    }
}

int main()
{
    initscr(); /*initialises curses in the terminal*/

    cbreak();             /*Do not require enter key for input*/
    keypad(stdscr, TRUE); /*allows for input like arrow keys, F1, F2 etc.*/
    noecho();             /*Do not echo input to screen*/
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    curs_set(FALSE);
    refresh();

    snake sn;
    fruit fr;
    fr.set_fruit(rand() % 20 + 1, rand() % 20 + 1);
    while (!gameOver)
    {
        Movement(sn.dir);
        Logic(sn.dir, sn.cord);
        if (check_fruit(sn.cord[0].first, sn.cord[0].second, fr.i, fr.j))
        {
            fr.set_fruit(rand() % 20 + 1, rand() % 20 + 1);
            std::pair<int, int> temp(sn.cord[0].first + 1, sn.cord[0].second);
            sn.cord.push_back(temp);
        }
        GameOver(sn);
        Draw(fr.i, fr.j, sn);
        napms(200); // microsecond delay
        refresh();  // echoes to main terminal screen
    }
    after();
    endwin(); //end ncurses

    return 0;
}

//g++ -std=c++11 same.cpp -o run -lncurses
//./run
