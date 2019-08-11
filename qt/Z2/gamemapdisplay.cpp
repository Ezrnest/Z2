#include "gamemapdisplay.h"
#include "ui_gamemapdisplay.h"
#include <QPainter>
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
        break;
    }
    case TilePreview::Mountain: {
        p.fillRect(cordX,cordY,tileWidth,tileHeight,Qt::darkGray);
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
    pt.fillRect(0,0,displayWidth,displayHeight,Qt::darkGreen);
    double tileWidth = (double)displayWidth / mw;
    double tileHeight = (double)displayHeight / mh;
    int tw = (int) tileWidth;
    int th = (int) tileHeight;
    for(int x=0;x<mw;x++){
        for(int y = 0 ; y < mh ; y++){
            int cordX = gameCordToViewCordX(x,tileWidth,mw);
            int cordY = gameCordToViewCordY(y,tileHeight,mh);
            paintTile(pt,mp->getTile(x,y),cordX,cordY,tw,th);
        }
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




