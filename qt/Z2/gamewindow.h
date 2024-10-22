#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <core/Server.h>
#include <event/InGamePlayerEvent.h>
#include <event/EntityEvent.h>
#include <event/StateEvent.h>
#include <event/PlayersWon.h>
#include "world/World.h"
#include "core/GameGui.h"
#include "core/Lobby.h"
#include "mainwindow.h"
#include <QMediaPlayer>
namespace Ui {
class GameWindow;
}

class GameWindow;
class GameFrame;
const static int TILE_SIZE = 64;
const static int HALF_TILE_SIZE = TILE_SIZE / 2;
const static int PLAYER_COLOR_BLOCK_SIZE = 8;
using namespace std;
using namespace z2;


class QtGui : public GameGui{
    GameWindow* window;
public:
    QtGui(GameWindow* gw);

    virtual void update() override;

    virtual void onPlayerTurnStarted(int playerId) override;

    virtual void onPlayerTurnFinished(int playerId) override;

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
    explicit GameWindow(MainWindow *parent = nullptr);
    ~GameWindow() override;

    void setClient(const shared_ptr<Client>& client);
    void setServer(const shared_ptr<Server>& server);

    void setLobby(const shared_ptr<Lobby>& lobby);

    friend class QtGui;
    friend class GameFrame;


    void refreshSelection(bool playerClicked = false);

    void clickedOnOwnedEntity(const shared_ptr<Entity>& en);

    void refreshPlayerInfo();

    void refreshTurnInfo();

public slots:
    /**
     * Refreshes the game gui;
     * @brief refreshAll
     */
    void refreshAll();

    void gameStarted();

    void showGameEnded();


    void dealWithGameEvent(const shared_ptr<GameEvent> &event);

    void showGameWin(const shared_ptr<PlayersWon> &event);

    void showPlayerQuit(int playerId);
private slots:
    void on_btnPerform_clicked();

    void on_btnEndTurn_clicked();

    void on_btnBuy_clicked();

    void on_btnResearch_clicked();

    void on_actionPlayerInfo_triggered();

    void on_btnMenu_clicked();

    void on_btnExitGame_clicked();

    void on_btnMenuCancel_clicked();

    void on_btnSaveGame_clicked();

    void on_tableResearch_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_tableBuy_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_btnMenuCancel_2_clicked();

    void on_btnDiplomacy_clicked();

private:
//    using GameEventProcessor = function<bool(const GameEventPtr&)>;
    Ui::GameWindow *ui;
    MainWindow* mainWindow;
    shared_ptr<QtGui> gui;
    shared_ptr<Entity> selectedEntity;
    shared_ptr<Server> server;

    shared_ptr<Lobby> lobby;

    enum GameState{
        NOT_STARTED,
        RUNNING,
        ENDING,
        ENDED
    };

    QRect viewport;
    GameState gameState = NOT_STARTED;


    shared_ptr<World> getWorld();

    shared_ptr<Client>& getClient();

    bool shouldDrawUI();

    void dealWithStateEvent(const shared_ptr<StateEvent>& event);

    void dealWithInGamePlayerEvent(const shared_ptr<InGamePlayerEvent> &event);

    void dealWithEntityEvent(const shared_ptr<EntityEvent>& event);

    bool isEntityVisible(const shared_ptr<Entity>& en);

    void processEntityPerform(const shared_ptr<EntityEvent>& event);

    void processEntityMove(const shared_ptr<EEntityMoved>& event);

    void processEntityCreated(const shared_ptr<EntityEvent>& event);

    void processEntityAttacking(const shared_ptr<EEntityDamaging>& event);

    void processEntityDied(const shared_ptr<EntityEvent>& event);

    void processPlayerResearch(const shared_ptr<TechResearchedEvent>& event);

    void arrangeUi();

    void adjustPosToBorder(QWidget* t, int x,bool toLeft, int y, bool toTop);

    void adjustPosMid(QWidget* t, bool xMid, bool yMid);

    void adjustWidgetSize(QWidget* t, bool xExpanding, bool yExpanding);

    int getPlayerId();

    bool isCurrentTurn();

    Point& getSelectedPos();


    void closeEvent(QCloseEvent* event) override;

    void refreshEntityProps(shared_ptr<Entity>& en, World& w);

    void refreshContruction(shared_ptr<Entity>& en, World& w, Point& p);

    void refreshTechnology(World& w);

    void refreshPerformAbility(shared_ptr<Entity>& en, World& w, Point& p);

    void refreshTileInfo(bool entityInfo,World& w,Point& pos);

    void showMenuPlayerList();

    void saveGame();

    void exitGame();

    void showPlayerDefeated(const shared_ptr<PlayerEvent>& event);

protected:
    void keyPressEvent(QKeyEvent* event) override;

public:
    shared_ptr<QtGui> getGui();

    void resizeEvent(QResizeEvent* event) override;

signals:
    void notifyRefreshAll();

    void notifyGameStarted();

    void notifyGameEnded();

    void notifyPlayerQuit(int pid);

    void notifyGameEvent(const shared_ptr<GameEvent> &event);
};



#endif // GAMEWINDOW_H
