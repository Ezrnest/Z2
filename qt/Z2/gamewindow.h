#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <core/Server.h>
#include "world/World.h"
#include "core/GameGui.h"

namespace Ui {
class GameWindow;
}

class GameWindow;
class GameFrame;
const static int TILE_SIZE = 64;
using namespace std;
using namespace z2;


class QtGui : public GameGui{
    GameWindow* window;
public:
    QtGui(GameWindow* gw);

    virtual void update() override;

    virtual void onPlayerTurnStarted(int playerId) override;

    virtual void onGameStarted() override;

    virtual void onPlayerWin(int playerId) override;

    virtual void onGameStopped() override ;

    virtual void onEvent(const shared_ptr<GameEvent> &event)override;

    friend class GameWindow;
    friend class GameFrame;
};

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow() override;

    void setClient(const shared_ptr<Client>& client);
    void setServer(const shared_ptr<Server>& server);

    friend class QtGui;
    friend class GameFrame;

    /**
     * Refreshes the game gui;
     * @brief refreshAll
     */
    void refreshAll();
    void refreshSelection();

    void refreshPlayerInfo();

    void refreshTurnInfo();

private slots:
    void on_btnPerform_clicked();

    void on_btnEndTurn_clicked();

    void on_btnBuy_clicked();

    void on_btnResearch_clicked();

private:
    Ui::GameWindow *ui;
    shared_ptr<QtGui> gui;
    shared_ptr<Entity> selectedEntity;
    shared_ptr<Server> server;
    QRect viewport;

    shared_ptr<World> getWorld();

    shared_ptr<Client> getClient();

    Point& getSelectedPos();


    void refreshContruction(shared_ptr<Entity>& en, World& w, Point& p);

    void refreshTechnology(World& w);

    void refreshPerformAbility(shared_ptr<Entity>& en, World& w, Point& p);

    void refreshTileInfo(bool entityInfo,World& w,Point& pos);

public:
    shared_ptr<QtGui> getGui();
};



#endif // GAMEWINDOW_H
