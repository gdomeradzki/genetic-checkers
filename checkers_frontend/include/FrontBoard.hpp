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
    virtual ~FrontBoard();

    bool event(QEvent* e);

    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
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
