#pragma once

#include <QMainWindow>
#include <memory>
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

    MainWindow(MainWindow&&) = delete;
    MainWindow(const MainWindow&) = delete;

    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;

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
    std::unique_ptr<FrontBoard> frontBoard;
    const GameState& gameState;
    Ui::MainWindow* ui;
};
