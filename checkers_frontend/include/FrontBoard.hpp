#pragma once

#include <set>

#include <QDragEnterEvent>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QWidget>
#include "GameState.hpp"

namespace Ui
{
class FrontBoard;
}

class FrontBoard : public QWidget
{
    Q_OBJECT

    using Space = int;

public:
    explicit FrontBoard(const GameState&, QWidget* parent = nullptr);
    ~FrontBoard() override;

    bool event(QEvent* e) override;

    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void blockPawnMove(FigureColor color, bool block);

    void syncTemporaryState();
    const GameState& getGameState() const;

signals:
    void signalMoveDone(const Move&);

private:
    void drawPawn(FigureColor color, FigureType type, Position, int space, QPainter* painter);
    QPoint getPointByPosition(const Position&) const;
    Position getPositionByPoint(const QPoint&) const;

    Space getCurrentSpace() const;

    Move m_dragMove;

    std::set<FigureColor> m_blockedPlayers;

    const GameState& m_state;
    GameState m_temporaryDrawnState;

    Ui::FrontBoard* m_ui;
};
