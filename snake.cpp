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
    start_color();
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_RED, COLOR_RED);
    init_pair(5, COLOR_WHITE, COLOR_WHITE);

    for (int i = 0; i < sn.cord.size(); i++)
    {
        grid[sn.cord[i].first][sn.cord[i].second] = '*';
    }

    for (int i = 0; i < width + 2; i++)
    {
        for (int j = 0; j < height + 3; j++)
        {
            if (grid[i][j] == '*') attron(COLOR_PAIR(3));
            else if (grid[i][j] == '^') attron(COLOR_PAIR(4));
            else if (grid[i][j] == 'X') attron(COLOR_PAIR(5));
            printw(" %c", grid[i][j]);
            attroff(COLOR_PAIR(3));
            attroff(COLOR_PAIR(4));
            attroff(COLOR_PAIR(5));
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
// ifndef _WIN32
void after(snake &sn)
{
    erase();
    start_color();

    init_pair(2, COLOR_BLACK, COLOR_RED);

    attron(COLOR_PAIR(2));
    printw("   __ _  __ _ _ __ ___   ___    _____   _____ _ __ \n");
    printw("  / _` |/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|\n");
    printw(" | (_| | (_| | | | | | |  __/ | (_) \\ V /  __/ |   \n");
    printw("  \\__, |\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|   \n");
    printw("   __/ |                                           \n");
    printw("  |___/                                            \n");
    attroff(COLOR_PAIR(2));

    printw("YOUR SCORE IS: %d\n\n", sn.cord.size() - 1);
    printw("PRESS ANY KEY TO EXIT !!");

    nodelay(stdscr, FALSE);
    getch();
}
// endif

void about()
{
    erase();
    printw(" __        _______ _     ____ ___  __  __ _____   _____ ___    ____  _   _    _    _  _______ \n");
    printw(" \\ \\      / / ____| |   / ___/ _ \\|  \\/  | ____| |_   _/ _ \\  / ___|| \\ | |  / \\  | |/ / ____|   \n");
    printw("  \\ \\ /\\ / /|  _| | |  | |  | | | | |\\/| |  _|     | || | | | \\___ \\|  \\| | / _ \\ | ' /|  _|     \n");
    printw("   \\ V  V / | |___| |__| |__| |_| | |  | | |___    | || |_| |  ___) | |\\  |/ ___ \\| . \\| |___    \n");
    printw("    \\_/\\_/  |_____|_____\\____\\___/|_|  |_|_____|   |_| \\___/  |____/|_| \\_/_/   \\_\\_|\\_\\_____|   \n");
    printw("                                                                                                 \n");
    printw("PRESS ANY KEY TO CONTINUE !!");
    nodelay(stdscr, FALSE);
    getch();
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
    // ifndef _WIN32
    initscr(); /*initialises curses in the terminal*/
    about();

    cbreak();             /*Do not require enter key for input*/
    keypad(stdscr, TRUE); /*allows for input like arrow keys, F1, F2 etc.*/
    noecho();             /*Do not echo input to screen*/
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    curs_set(FALSE);
    refresh();
    // endif

    // ifdef _WIN32
    // some conio function
    // endif

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

    attron(COLOR_PAIR(1));

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
    after(sn);
    attroff(COLOR_PAIR(1));
    endwin(); //end ncurses

    return 0;
}

//g++ -std=c++11 same.cpp -o run -lncurses
//./run
