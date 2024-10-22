#include "gamemapdisplay.h"
#include "ui_gamemapdisplay.h"
#include <QPainter>
#include <algorithm>
GameMapDisplay::GameMapDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameMapDisplay)
{
    ui->setupUi(this);
}

GameMapDisplay::~GameMapDisplay()
{
    delete ui;
}

void GameMapDisplay::paintTile(QPainter &p, TilePreview tp, int cordX, int cordY, int tileWidth, int tileHeight)
{
    switch (tp){
    case TilePreview::Plain: {
        p.fillRect(cordX,cordY,tileWidth,tileHeight,Qt::darkGreen);
        break;
    }
    case TilePreview::Mountain: {
        p.fillRect(cordX,cordY,tileWidth,tileHeight,Qt::darkGray);
        break;
    }
    case TilePreview::Water: {
        p.fillRect(cordX,cordY,tileWidth,tileHeight,Qt::blue);
        break;
    }
    case TilePreview::Mine: {
        p.fillRect(cordX,cordY,tileWidth,tileHeight,Qt::yellow);
        break;
    }
    case TilePreview::Gem: {
        p.fillRect(cordX,cordY,tileWidth,tileHeight,Qt::cyan);
        break;
    }
    case TilePreview::BornPoint: {
        p.fillRect(cordX,cordY,tileWidth,tileHeight,Qt::black);
        break;
    }
    case TilePreview::Building: {
        p.setBrush(Qt::gray);
        p.drawEllipse(cordX,cordY,tileWidth,tileHeight);
        break;
    }
    case TilePreview::BattleUnit: {
        p.setBrush(Qt::red);
        p.drawEllipse(cordX,cordY,tileWidth,tileHeight);
        break;
    }
    case TilePreview::EcoUnit: {
        p.setBrush(Qt::blue);
        p.drawEllipse(cordX,cordY,tileWidth,tileHeight);
        break;
    }
    case TilePreview::GameUnit: {
        p.setBrush(Qt::darkBlue);
        p.drawEllipse(cordX,cordY,tileWidth,tileHeight);
        break;
    }
    }
}


void GameMapDisplay::paintEvent(QPaintEvent *event)
{
    if(!mp){
        return;
    }
    QPainter pt(this);

    int mw = mp->getWidth();
    int mh = mp->getHeight();
    int displayWidth = width();
    int displayHeight = height();
    pt.fillRect(0,0,displayWidth,displayHeight,Qt::blue);
    double tileSize =std::min((double)displayWidth / mw,(double)displayHeight / mh);
    int ts = (int) tileSize;
    for(int x=0;x<mw;x++){
        for(int y = 0 ; y < mh ; y++){
            int cordX = gameCordToViewCordX(x,tileSize,mw);
            int cordY = gameCordToViewCordY(y,tileSize,mh);
            paintTile(pt,mp->getTile(x,y),cordX,cordY,ts,ts);
        }
    }
    auto& bps = mp->getBornPoints();
    QPen pen;
    pen.setColor(Qt::white);
    pt.setPen(pen);
    for(size_t i=0;i<bps.size();i++){
        auto& bp = bps[i];
        int cordX = gameCordToViewCordX(bp.x,tileSize,mw);
        int cordY = gameCordToViewCordY(bp.y,tileSize,mh);
        pt.drawText(cordX,cordY,ts,ts, Qt::AlignCenter,QString::number(i+1));
    }
}

void GameMapDisplay::setGameMap(const shared_ptr<GameMap> gm)
{
    if(gm){
        mp = gm->getPreview();
    }
    update();
}

int GameMapDisplay::gameCordToViewCordX(int x, double tileWidth, int mw)
{
    return (int)(x * tileWidth);
}

int GameMapDisplay::gameCordToViewCordY(int y, double tileHeight, int mh)
{
    return (int)( (mh - y - 1) * tileHeight);
}




