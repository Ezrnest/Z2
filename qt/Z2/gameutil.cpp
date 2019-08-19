#include "gameutil.h"
#include "config/GameConfiguration.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <world/GameMap.h>
GameUtil::GameUtil()
{

}

QString generateGameSaveName(const shared_ptr<World> &world){
    QDateTime curDateTime = QDateTime::currentDateTime();
    QString suffix = curDateTime.toString("yyyy-MM-dd_HH_mm");
    QString mapName = QString::fromStdString(world->getMapName());
    QString result = mapName + "_" + suffix + ".save";
    return result;
}

void GameUtil::saveGame(QWidget* parent,const shared_ptr<World> &world)
{
    string s = z2::GameConfiguration::instance().getSaveDir().getPath();
    QString recommandedFileName = QString::fromStdString(s) + "\\" + generateGameSaveName(world);
    QString path = QFileDialog::getSaveFileName(parent,QObject::tr("保存游戏"), recommandedFileName,"*.save");
    if(path.isEmpty()){
        return;
    }
    if(!path.endsWith(".save")){
        path += ".save";
    }
    bool re = GameMap::saveWorldTo(path.toStdString(),"",world);
    if(!re){
        QMessageBox::warning(parent,QObject::tr("警告"),QObject::tr("保存游戏失败！"));
    }
}

shared_ptr<GameMap> GameUtil::loadGame(QWidget *parent)
{
    string s = z2::GameConfiguration::instance().getSaveDir().getPath();
    QString path = QFileDialog::getOpenFileName(parent,QObject::tr("加载存档"),QString::fromStdString(s),"*.save");
    if(path.isEmpty()){
        return shared_ptr<GameMap>();
    }
    auto map = GameMap::loadWorldFrom(path.toStdString());
    if(!map){
        QMessageBox::warning(parent,QObject::tr("警告"),QObject::tr("加载游戏失败！"));
        return shared_ptr<GameMap>();
    }
    return map;
}
