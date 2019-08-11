#ifndef GAMEMAPDISPLAY_H
#define GAMEMAPDISPLAY_H

#include <QWidget>

#include <memory>
#include <world/GameMap.h>
#include <world/MapPreview.h>

using namespace z2;
namespace Ui {
class GameMapDisplay;
}

class GameMapDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit GameMapDisplay(QWidget *parent = nullptr);
    ~GameMapDisplay();
    void paintEvent(QPaintEvent* event) override;

    void setGameMap(const shared_ptr<GameMap> gm);

private:
    Ui::GameMapDisplay *ui;
    shared_ptr<MapPreview> mp;

    inline static int gameCordToViewCordX(int x, double tileWidth, int mw);
    inline static int gameCordToViewCordY(int y, double tileHeight, int mh);

    void paintTile(QPainter& p,TilePreview tp, int cordX, int cordY, int tileWidth, int tileHeight );


};

#endif // GAMEMAPDISPLAY_H
