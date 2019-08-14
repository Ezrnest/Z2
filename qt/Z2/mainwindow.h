#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QTableWidget>
#include <memory>

#include <world/GameMap.h>
#include "world/GameInitSetting.h"
#include "core/Lobby.h"
namespace Ui {
class MainWindow;
}

void setupTable(QTableWidget* table);
class GameWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_btnMultiple_clicked();

    void on_btnSingle_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_cmbMap_currentIndexChanged(int index);

    void on_btnStartGame_clicked();

    void on_btnCancel_clicked();

    void startServerGame();

    void on_pushButton_clicked();

    void on_btnJoin_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_btnSetting_clicked();

    void on_btnCancel_2_clicked();

    void on_btnCancel_3_clicked();

    void on_btnExitGame_clicked();

private:
    Ui::MainWindow *ui;

    bool localGame = false;

    bool loadSavedMap = false;

    shared_ptr<z2::GameMap> currentMap;

    shared_ptr<z2::Lobby> onlineLobby;

    QMediaPlaylist* playListMainMenu;

    bool gameRunning = false;


    void initGameLobby();

    void refreshGameLobby();

    void loadMap(const std::shared_ptr<z2::GameMap>& map);

    void startLocalGame(z2::GameInitSetting& setting);

    void startOnlineGameServer(z2::GameInitSetting& setting);

    void startOnlineGameClient(QString address,int id);


    void initSettingPage();

    void saveGameSetting();

    void beforeStartingGame(GameWindow* gw);
public:
    void afterGameEnded();

    void showEvent(QShowEvent *e) override;

signals:
    void notifyStartServerGame();
};

#endif // MAINWINDOW_H
