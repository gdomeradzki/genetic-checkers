#pragma once

#include <QMainWindow>
#include "PawnState.hpp"
#include "Types.hpp"
namespace Ui
{
class MainWindow;
}

class FrontBoard;
class GameState;

enum class StrategyType
{
    Human,
    Ai
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const GameState&, QWidget* parent = nullptr);
    ~MainWindow() override;

    void repaint();
    void syncTemporaryState();
    void blockPawnMoves(FigureColor color, bool block);
    void showGameResultStatement(GameResult);
    StrategyType getWhiteStrategy() const;
    StrategyType getBlackStrategy() const;

    unsigned int minimaxDeep() const;
    void pushInfo(const QString& info);
signals:

    void signalMoveDone(const Move&);
    void signalStartNewGame();
    void signalLoadMetrics(const std::string&);

private:
    FrontBoard* frontBoard;
    const GameState& gameState;
    Ui::MainWindow* ui;
};
