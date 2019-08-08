#include "gameframe.h"
#include "ui_gameframe.h"
#include "gamewindow.h"
#include "core/messages/UnitAttack.h"
#include "core/messages/UnitMove.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPen>
using namespace z2;
const static QSize qTileSize(TILE_SIZE,TILE_SIZE);
GameFrame::GameFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::GameFrame)
{
    ui->setupUi(this);
    trans.scale(1.5,1.5);
}

GameFrame::~GameFrame()
{
    delete ui;
}

void paintLines(QPainter& painter, const shared_ptr<World>& world){
    int wH = world->getHeight();
    int wW = world->getWidth();
    int xEnd = wW * TILE_SIZE;
    int yEnd = wH * TILE_SIZE;
    QPen pen;
    pen.setWidth(2);
    painter.setPen(pen);
    for(int i=0;i<=wH;i++){
        int y = i*TILE_SIZE;
        painter.drawLine(0,y,xEnd,y);
    }
    for(int i=0;i<=wW;i++){
        int x = i*TILE_SIZE;
        painter.drawLine(x,0,x,yEnd);
    }
}

QPoint GameFrame::gameCordToViewCord(const Point &p)
{
    auto w = win->getWorld();
    int y = w->getHeight() - p.y - 1;
    int x = p.x;
    return QPoint(x * TILE_SIZE, y * TILE_SIZE);
}

Point GameFrame::viewCordToGameCord(const QPoint &p)
{
    auto w = win->getWorld();
    auto invt = trans.inverted();
    auto pReal = invt.map(p);
    int x = pReal.x() / TILE_SIZE;
    int y = w->getHeight() -  (pReal.y() / TILE_SIZE) - 1;
    return Point(x,y);
}




void GameFrame::paintTiles(QPainter& painter, const shared_ptr<World>& world){
    int wH = world->getHeight();
    int wW = world->getWidth();
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for(int x=0;x<wW;x++){
        for(int y=0;y<wH;y++){
            Tile& t = world->getTile(x,y);
            QPoint ul = gameCordToViewCord(Point(x,y));
            switch (t.getTerrain()) {
            case Terrain::PLAIN:{
                brush.setColor(Qt::darkGreen);
                break;
            }
            case Terrain::MOUNTAIN:{
                brush.setColor(Qt::gray);
                break;
            }
            default:{
                brush.setColor(Qt::black);
                break;
            }
            }
            auto rect = QRect(ul,qTileSize);
            painter.fillRect(rect,brush);

            auto en = t.getEntity();
            if(en){
                painter.drawText(rect,QString::fromStdString(en->getEntityName()));
            }
        }
    }
}

void GameFrame::paintHighlightedTile(QPainter &painter, const shared_ptr<World> &world)
{
    if(!world->isInside(selPos)){
        return;
    }
    QPoint ul = gameCordToViewCord(selPos);
    auto rect = QRect(ul,qTileSize);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::red);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawRect(rect);
}




void GameFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setTransform(trans);
//    painter.setWindow(QRect(20,20,int(percent * 631),int(percent *551)));
    auto world = win->getWorld();
    paintTiles(painter,world);
    paintLines(painter,world);
    paintHighlightedTile(painter,world);
}

void GameFrame::setWindow(GameWindow *w)
{
    win = w;
}



void GameFrame::mousePressEvent(QMouseEvent *event)
{
//    cout << "Event" << endl;
    switch(event->button()){
    case Qt::MouseButton::LeftButton:{
        auto pos = event->pos();
        clickedPos = pos;
        dragState = 1;
        break;
    }
    case Qt::MouseButton::RightButton:{
        auto pos = event->pos();
        auto pGame = viewCordToGameCord(pos);
        rightClickedOn(pGame);
//        win->update();
        break;
    }

    }
}

void GameFrame::mouseMoveEvent(QMouseEvent *event)
{
    if(dragState == 0){
        return;
    }
    dragState = 2;
    auto pos = event->pos();
    int dx = pos.x() - clickedPos.x();
    int dy = pos.y() - clickedPos.y();
    clickedPos = pos;
    trans.translate(dx,dy);
    update();
}

void GameFrame::mouseReleaseEvent(QMouseEvent *event)
{
    switch(event->button()){
    case Qt::MouseButton::LeftButton:{
        if(dragState != 2){
            auto pos = event->pos();
            selPos = viewCordToGameCord(pos);
            cout << "Selected pos: " << selPos.x << ' ' << selPos.y << endl;
            win->refreshSelection();
            update();
        }
        break;
    }
    }
    dragState = 0;
}

void GameFrame::rightClickedOn(Point &p)
{
    auto world = win->getWorld();
    auto client = win->getClient();
    auto en = world->getEntity(selPos);
    bool operated = false;
    if(world->canAttack(selPos,p,client->getPlayerId())){
        shared_ptr<UnitAttack> msg(new UnitAttack(selPos,p));
        client->sendMessageToServer(msg);
        operated = true;
    }else if (world->canMove(selPos,p,client->getPlayerId())) {
        shared_ptr<UnitMove> msg(new UnitMove(selPos,p));
        client->sendMessageToServer(msg);
        operated = true;
    }
    if(operated){
        if(en){
            selPos = en->getPos();
        }
        win->refreshSelection();
//        win->refreshAll();
    }

}

void GameFrame::zoom(bool in)
{
    double mul;
    if(in){
        mul = 1.25;

    }else{
        mul = 0.8;
    }
    trans.scale(mul,mul);
    update();
}



void GameFrame::wheelEvent(QWheelEvent *event)
{
    this->zoom(event->delta() > 0);
}


