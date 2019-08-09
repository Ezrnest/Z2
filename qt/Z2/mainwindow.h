#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <memory>

#include <world/GameMap.h>
#include "world/GameInitSetting.h"
#include "core/Lobby.h"
namespace Ui {
class MainWindow;
}

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

private:
    Ui::MainWindow *ui;

    bool localGame = false;

    shared_ptr<z2::GameMap> currentMap;

    shared_ptr<z2::Lobby> onlineLobby;

    void initGameLobby();

    void loadMap(const std::shared_ptr<z2::GameMap>& map);

    void startLocalGame(z2::GameInitSetting& setting);

    void startOnlineGameServer(z2::GameInitSetting& setting);

    void startOnlineGameClient(QString address,int id);

signals:
    void notifyStartServerGame();
};

#endif // MAINWINDOW_H
