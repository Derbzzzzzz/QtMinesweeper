#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>
#include <QtUiTools/QUiLoader>

// Initial Setup of Application
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    QFrame* frame = ui->frame;
    frame->setFixedSize(500, 500);

    QLabel* endLabel = ui->endLabel;

    QVBoxLayout* layout = new QVBoxLayout(ui->page_3);
    layout->addWidget(endLabel);
    layout->addWidget(frame);
    layout->setAlignment(Qt::AlignCenter);

    ui->page_3->setLayout(layout);

    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setBoardUI(QGridLayout* grid, int size)
{
    while( grid->count() ) { QWidget* widget = grid->itemAt(0)->widget(); if( widget ) { grid->removeWidget(widget); delete widget; } }
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);
    grid->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    grid->setHorizontalSpacing(0);

    ui->flag->setCheckState(Qt::Unchecked);

    QLabel* endLabel = ui->endLabel;
    endLabel->raise();
    endLabel->setVisible(false);


    // Create Buttons for game
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            CustomButton *button = new CustomButton(i, j, this);
            button->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            button->setContentsMargins(0, 0, 0, 0);
            QFont f( "Arial", 12, QFont::Bold);
            button->setFont( f);
            button->setStyleSheet("margin: 0; padding: 0; background-color: white; color: black");

            grid->addWidget(button, i, j);
            connect(button, &CustomButton::clicked, [=](){
                handleClick(grid, button, i, j);     // Call the function which uses i and j here
            });
        }
    }

    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);
}

// Create Labels for when button is clicked
QLabel* MainWindow::createLabel(int i, int j)
{
    QLabel * label = new QLabel(this);
    label->setAlignment(Qt::AlignCenter);
    QFont f( "Arial", 12, QFont::Bold);
    label->setFont( f);
    char val = aBoard->getVal(i, j);
    if(val == '0') { label->setStyleSheet("color: white;");}
    else if(val == '1') { label->setStyleSheet("color: blue;");}
    else if(val == '2') { label->setStyleSheet("color: green;");}
    else if(val == '3') { label->setStyleSheet("color: red;");}
    else if(val == '4') { label->setStyleSheet("color: purple;");}
    else  { label->setStyleSheet("color: darkRed");}
    QString string = QString(val);
    label->setText(string);
    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    return label;
}

// Clear Board on win / lose to show complete grid
void MainWindow::clearBoard(QGridLayout* grid)
{
    for (int i = 0; i < aBoard->getSize(); i++)
    {
        for (int j = 0; j < aBoard->getSize(); j++)
        {
            QWidget *widget = grid->itemAtPosition(i, j)->widget();

            auto customButton = qobject_cast<CustomButton*>(widget);
            if (nullptr != customButton){
                revealSquare(grid, customButton, i, j);
            }
        }
    }
}

void MainWindow::revealSquare(QGridLayout* grid, CustomButton * button, int i, int j)
{
    QLabel * label = createLabel(i, j);
    label->setMinimumHeight(button->height());
    label->setMinimumWidth(button->width());
    grid->removeWidget(button);
    grid->addWidget(label, i, j);
    uBoard->setVal(i, j, aBoard->getVal(i, j));
}

// recursive dfs search (done when square with 0 neighboring bombs is clicked)
void MainWindow::recSearch(QGridLayout* grid, int x, int y, int size)
{
    if (x < 0 or y < 0 or x >= size or y >= size or uBoard->getVal(x, y) != '-')
    {
        return;
    }

    int val = aBoard->getVal(x, y);
    if (val != '0')
    {
        QWidget *widget = grid->itemAtPosition(x, y)->widget();

        auto customButton = qobject_cast<CustomButton*>(widget);
        if (nullptr != customButton){
            revealSquare(grid, customButton, x, y);
        }
        return;
    }


    QWidget *widget = grid->itemAtPosition(x, y)->widget();

    auto customButton = qobject_cast<CustomButton*>(widget);
    if (nullptr != customButton){
        revealSquare(grid, customButton, x, y);
    }

    recSearch(grid, x + 1, y + 1, size);
    recSearch(grid, x + 1, y, size);
    recSearch(grid, x + 1, y - 1, size);
    recSearch(grid, x, y + 1, size);
    recSearch(grid, x, y - 1, size);
    recSearch(grid, x - 1, y + 1, size);
    recSearch(grid, x - 1, y, size);
    recSearch(grid, x - 1, y - 1, size);
}

// handles button clicks (both flagging and revealing squares)
void MainWindow::handleClick(QGridLayout* grid, CustomButton * button, int i, int j)
{

    if(ui->flag->isChecked())
    {
        if(button->text() == "")
        {
            button->setText("B");
        } else
        {
            button->setText("");
        }
    }
    else if(aBoard->getVal(i, j) == BOMB)
    {
        clearBoard(grid);
        QLabel* endLabel = ui->endLabel;
        QString string = QString("You Lost!");
        endLabel->setText(string);
        endLabel->setVisible(true);
    } else
    {
        recSearch(grid, i, j, aBoard->getSize());
        if(uBoard->hasWon())
        {
            clearBoard(grid);
            QLabel* endLabel = ui->endLabel;
            QString string = QString("You Won!");
            endLabel->setText(string);
            endLabel->setVisible(true);
        }
    }
}

// Start Game and intitialize new board objects
void MainWindow::startGame(int size, int bombs)
{
    aBoard = new actualBoard(size, bombs);
    uBoard = new userBoard(size, bombs);

    ui->stackedWidget->setCurrentIndex(2);
    setBoardUI(ui->gridLay, size);
}


void MainWindow::on_btnNewGame_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_btnBeginner_clicked()
{
    startGame(10, 10);
}


void MainWindow::on_btnBack_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


// free boards from game and return to home screen
void MainWindow::on_btnQuit_clicked()
{
    aBoard->free();
    uBoard->free();
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_btnIntermediate_clicked()
{
    startGame(15, 20);
}


void MainWindow::on_btnAdvanced_clicked()
{
    startGame(20, 40);
}

