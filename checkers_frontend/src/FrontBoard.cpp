#include "FrontBoard.hpp"
#include "ui_FrontBoard.h"

#include <QDebug>
#include <QDrag>
#include <QMimeData>

namespace
{
constexpr auto whitePawnImage = ":/pawn_white.png";
constexpr auto blackPawnImage = ":/pawn_black.png";
constexpr auto whitePawnSpecialImage = ":/pawn_white_special.png";
constexpr auto blackPawnSpecialImage = ":/pawn_black_special.png";

} // namespace

FrontBoard::FrontBoard(const GameState& gameState, QWidget* parent)
    : QWidget(parent), m_state(gameState), m_temporaryDrawnState(gameState), m_ui(new Ui::FrontBoard)
{
    setStyleSheet("background-color:blue;");
    setAcceptDrops(true);
}

FrontBoard::~FrontBoard()
{
    delete m_ui;
}

bool FrontBoard::event(QEvent* e)
{
    return QWidget::event(e);
}

void FrontBoard::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    const auto space = getCurrentSpace();
    for (int i = 0; i < boardSize + 1; i++)
    {
        const QPoint hLineBegin(0, i * space);
        const QPoint hLineEnd(space * boardSize, i * space);

        const QPoint vLineBegin(i * space, 0);
        const QPoint vLineEnd(i * space, space * boardSize);

        painter.drawLine(hLineBegin, hLineEnd);
        painter.drawLine(vLineBegin, vLineEnd);
    }

    auto allPawns = m_temporaryDrawnState.pawns(FigureColor::White);
    const auto blackPawns = m_temporaryDrawnState.pawns(FigureColor::Black);
    allPawns.insert(allPawns.end(), blackPawns.begin(), blackPawns.end());
    for (const auto& pawn : allPawns)
    {
        drawPawn(pawn.state.color, pawn.state.type, pawn.position, space, &painter);
    }

    for (int row = 0; row < boardSize; row++)
    {
        for (int col = 0; col < boardSize; col++)
        {
            const auto point = getPointByPosition({row, col});
            painter.drawText(point.x(), point.y(), space, space, Qt::AlignLeft, QString("%1x%2").arg(col).arg(row));
        }
    }
}

void FrontBoard::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        const auto positionClicked = getPositionByPoint(event->pos());
        if (m_temporaryDrawnState.isFree(positionClicked)) return;
        const auto pawn = m_temporaryDrawnState.pawnAtPosition(positionClicked);
        if (m_blockedPlayers.find(pawn.color) != m_blockedPlayers.end()) return;

        if (m_dragMove.empty())
        {
            m_temporaryDrawnState = m_state;
            m_dragMove.push_back(positionClicked);
        }
        else
        {
            if (!(m_dragMove.back() == positionClicked))
            {
                syncTemporaryState();
                return;
            }
        }
        QDrag* drag = new QDrag(this);
        QPixmap map = pawn.color == FigureColor::White ? QPixmap(whitePawnImage) : QPixmap(":/pawn_black.png");
        drag->setPixmap(map);
        QMimeData* mimeData = new QMimeData();
        mimeData->setImageData(map);
        drag->setMimeData(mimeData);
        drag->exec();
    }
}

void FrontBoard::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->source() == this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    else
    {
        event->acceptProposedAction();
    }
}

void FrontBoard::dropEvent(QDropEvent* event)
{
    const auto position = getPositionByPoint(event->pos());
    if (!GameState::isValid(position)) return;
    m_temporaryDrawnState.movePawn(m_dragMove.back(), position);
    m_dragMove.push_back(position);
    emit signalMoveDone(m_dragMove);
    repaint();
}

void FrontBoard::blockPawnMove(FigureColor color, bool block)
{
    if (block)
    {
        m_blockedPlayers.insert(color);
    }
    else
    {
        m_blockedPlayers.erase(color);
    }
}

void FrontBoard::syncTemporaryState()
{
    m_dragMove.clear();
    m_temporaryDrawnState = m_state;
    repaint();
}

const GameState& FrontBoard::getGameState() const
{
    return m_state;
}

void FrontBoard::drawPawn(FigureColor color, FigureType type, Position position, int space, QPainter* painter)
{
    QPixmap pawn;
    if (color == FigureColor::White && type == FigureType::Pawn)
    {
        pawn = QPixmap(whitePawnImage);
    }
    else if (color == FigureColor::White && type == FigureType::King)
    {
        pawn = QPixmap(whitePawnSpecialImage);
    }
    else if (color == FigureColor::Black && type == FigureType::Pawn)
    {
        pawn = QPixmap(blackPawnImage);
    }
    else if (color == FigureColor::Black && type == FigureType::King)
    {
        pawn = QPixmap(blackPawnSpecialImage);
    }
    else
    {
        throw std::runtime_error("Not type of pawn reconized !");
    }
    const auto point = getPointByPosition(position);
    constexpr auto imageAlignment = 6;
    const QRect rect(
        point.x() + imageAlignment, point.y() + imageAlignment, space - imageAlignment * 2, space - imageAlignment * 2);
    painter->drawPixmap(rect, pawn);
}

QPoint FrontBoard::getPointByPosition(const Position& position) const
{
    const auto space = getCurrentSpace();
    return QPoint{position.col * space, (boardSize - 1 - position.row) * space};
}

Position FrontBoard::getPositionByPoint(const QPoint& point) const
{
    const auto space = getCurrentSpace();
    return Position{boardSize - 1 - point.y() / space, point.x() / space};
}

FrontBoard::Space FrontBoard::getCurrentSpace() const
{
    return qMin(size().height(), size().width()) / boardSize;
}
