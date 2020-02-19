#include "MainWindow.hpp"

#include "ui_MainWindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <map>
#include "FrontBoard.hpp"

constexpr auto humanString = "Human";
static const std::map<std::string, StrategyType> strategyToStringMapping = {{humanString, StrategyType::Human},
                                                                            {"AI", StrategyType::Ai}};

MainWindow::MainWindow(const GameState& gameState, QWidget* parent)
    : QMainWindow(parent), gameState(gameState), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    frontBoard = new FrontBoard(gameState);

    connect(frontBoard, &FrontBoard::signalMoveDone, [this](const Move& move) { emit signalMoveDone(move); });

    connect(ui->start, &QPushButton::clicked, [this]() { emit signalStartNewGame(); });
    ui->frontBoard->layout()->addWidget(frontBoard);

    for (const auto& strategy : strategyToStringMapping)
    {
        ui->blackPlayerStrategy->addItem(strategy.first.c_str());
        ui->whitePlayerStrategy->addItem(strategy.first.c_str());
        ui->whitePlayerStrategy->setCurrentText(humanString);
    }

    connect(ui->loadAI, &QPushButton::clicked, [this]() {
        const auto loadedFile = QFileDialog::getOpenFileName(nullptr, "Saved AI file");
        if (!loadedFile.isEmpty())
        {
            emit signalLoadMetrics(loadedFile.toStdString());
        }
    });

    setWindowTitle("Genetic checkers");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::repaint()
{
    frontBoard->repaint();
    QMainWindow::repaint();
}

void MainWindow::syncTemporaryState()
{
    frontBoard->syncTemporaryState();
}

void MainWindow::blockPawnMoves(FigureColor color, bool block)
{
    frontBoard->blockPawnMove(color, block);
}

void MainWindow::showGameResultStatement(GameResult gameResult)
{
    QMessageBox messageBox(QMessageBox::Icon::NoIcon, QString("Game finished"), QString(""));
    switch (gameResult)
    {
        case GameResult::Draw:
            messageBox.setText("Draw!");
            break;
        case GameResult::BlackWin:
            messageBox.setText("Black win!");
            break;
        case GameResult::WhiteWin:
            messageBox.setText("White win!");
            break;
        default:
            throw std::runtime_error("Error! No winner found!");
    }
    messageBox.exec();
}

StrategyType MainWindow::getWhiteStrategy() const
{
    return strategyToStringMapping.at(ui->whitePlayerStrategy->currentText().toStdString());
}

StrategyType MainWindow::getBlackStrategy() const
{
    return strategyToStringMapping.at(ui->blackPlayerStrategy->currentText().toStdString());
}

unsigned int MainWindow::minimaxDeep() const
{
    return ui->minimaxDeep->value();
}

void MainWindow::pushInfo(const QString& info)
{
    ui->info->append(info);
}
