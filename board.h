#ifndef BOARD_H
#define BOARD_H

#endif // BOARD_H


#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>

#define BEGINNER 1
#define INTERMEDIATE 2
#define ADVANCED 3
#define QUIT 4

#define NOQUIT 0

#define BOMB '@'

using namespace std;

class Board
{
protected:
    int m_size;
    char* array;

    int index(int x, int y) const { return x * m_size + y; };

public:
    Board(int size)
    {
        m_size = size;
        array = new char[size * size]();
    }

    void free()
    {
        delete [] array;
    }

    int getSize()
    {
        return m_size;
    }

    char getVal(int x, int y) const { return array[index(x, y)]; };

    void setVal(int x, int y, char val) { array[index(x, y)] = val; };


};

class actualBoard : public Board
{
private:
    void placeBombs(int bombs)
    {
        unsigned int time_ui = (unsigned int)(time(NULL));
        srand(time_ui);

        while (bombs > 0)
        {
            int x = rand() % m_size;
            int y = rand() % m_size;

            if (getVal(x, y) != BOMB)
            {
                setVal(x, y, BOMB);
                bombs--;
            }
        }
    }

    void setBoard()
    {
        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                setVal(i, j, '0');
            }
        }
    }

    void setBombCounts()
    {
        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                if (getVal(i, j) != BOMB)
                {
                    setVal(i, j, getBombs(i, j) + '0');
                }
            }
        }
    }

public:
    actualBoard(int size, int bombs) : Board(size)
    {
        setBoard();
        placeBombs(bombs);
        setBombCounts();
    }

    int getBombs(int x, int y)
    {
        int closeBombs = 0;
        bool top = (x == 0);
        bool bottom = (x == m_size - 1);
        bool left = (y == 0);
        bool right = (y == m_size - 1);

        // Bottom Left
        if (!bottom and !left and getVal(x + 1, y - 1) == BOMB) { closeBombs++; };

        // Left
        if (!left and getVal(x, y - 1) == BOMB) { closeBombs++; };

        // Top Left
        if (!top and !left and getVal(x - 1, y - 1) == BOMB) { closeBombs++; };

        // Top
        if (!top and getVal(x - 1, y) == BOMB) { closeBombs++; };

        // Top Right
        if (!top and !right and getVal(x - 1, y + 1) == BOMB) { closeBombs++; };

        // Right
        if (!right and getVal(x, y + 1) == BOMB) { closeBombs++; };

        // Bottom Right
        if (!bottom and !right and getVal(x + 1, y + 1) == BOMB) { closeBombs++; };

        // Bottom
        if (!bottom and getVal(x + 1, y) == BOMB) { closeBombs++; };

        return closeBombs;

    }
};

class userBoard : public Board
{
private:
    int safeSquares;
    void setBoard()
    {
        for (int i = 0; i < m_size; i++)
        {
            for (int j = 0; j < m_size; j++)
            {
                Board::setVal(i, j, '-');
            }
        }
    }
public:
    userBoard(int size, int bombs) : Board(size)
    {
        setBoard();
        safeSquares = m_size * m_size - bombs;
    }

    void setVal(int x, int y, char val)
    {
        Board::setVal(x, y, val);
        safeSquares--;
    }

    bool hasWon()
    {
        return safeSquares == 0;
    }
};

