#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QtUiTools/QUiLoader>
#include "ui_mainwindow.h"

class CustomButton : public QPushButton
{
    Q_OBJECT

public:
    CustomButton(int row, int column, QWidget *parent = nullptr)
        : QPushButton(parent), row(row), column(column)
    {
        // Additional initialization if needed
    }

    int getRow() const { return row; }
    int getColumn() const { return column; }

private:
    int row;
    int column;
};

#include <board.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_btnNewGame_clicked();

    void on_btnBeginner_clicked();

    void on_btnBack_clicked();

    void on_btnQuit_clicked();

    void on_btnIntermediate_clicked();

    void on_btnAdvanced_clicked();

private:
    Ui::MainWindow *ui;

    actualBoard * aBoard;
    userBoard * uBoard;

    void recSearch(QGridLayout* grid, int x, int y, int size);
    void clearBoard(QGridLayout* grid);
    void revealSquare(QGridLayout* grid, CustomButton * button, int i, int j);
    void handleClick(QGridLayout* grid, CustomButton * button, int i, int j);
    void startGame(int size, int bombs);
    void setBoardUI(QGridLayout* grid, int size);
    QLabel* createLabel(int i, int j);
};

#endif // MAINWINDOW_H
