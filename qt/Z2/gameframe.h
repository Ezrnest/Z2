#ifndef GAMEFRAME_H
#define GAMEFRAME_H

#include <QFrame>
#include <entity/ConstructionBase.h>
#include <world/World.h>
#include "world/Point.h"
#include "ImageRepository.h"
#include <ctime>
namespace Ui {
class GameFrame;
}

class GameWindow;

class GameFrame : public QFrame
{
    Q_OBJECT

public:
    explicit GameFrame(QWidget *parent = nullptr);
    ~GameFrame();


    void paintEvent(QPaintEvent* event) override;

    void setWindow(GameWindow* win);
    friend class GameWindow;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;


private:
    z2::ImageRepository& repo = z2::ImageRepository::instance();

    Ui::GameFrame *ui;
    GameWindow* win;
    z2::Point selPos;
    QTransform trans;

    QPoint clickedPos;
    clock_t clickedTime;
    int dragState = 0;
    QPoint gameCordToViewCord(const z2::Point& p);

    z2::Point viewCordToGameCord(const QPoint& p);


    void paintTile(QPainter &painter, QBrush &brush, QRect &rect,z2::World& world, z2::Tile &t, z2::Player& p);

    void paintTiles(QPainter &painter, const shared_ptr<z2::World> &world);

    void paintHighlightedTile(QPainter& painter, const shared_ptr<z2::World> &world );

    void rightClickedOn(z2::Point& p);


    void paintTerrainTextureLost(QPainter &painter, QBrush &brush, QRect &rect, z2::Tile &t);
    void paintTerrain(QPainter &painter, QBrush &brush, QRect &rect, z2::Tile &t);

    void paintResource(QPainter &painter, QBrush &brush, QRect &rect, z2::Tile &t);

    void paintEntity(QPainter &painter, QBrush &brush, QRect &rect,z2::World& world, z2::Tile &t, bool grey);

    void paintConstructionBaseIndicator(QPainter& painter, z2::World &world);

    QRectF getDisplayRect();

    void zoom(bool in,int ,int);

    void translate(double dx,double dy);

    void fitToSize();

    void makeCenter(const z2::Point& p);

    void makeCenterConstructionBase();
    void scaleWithPivot(QTransform &trans, double multiplier, double pivotX, double pivotY);
};

#endif // GAMEFRAME_H
