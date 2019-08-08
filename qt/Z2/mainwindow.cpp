#include "gamewindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config/MapRepository.h"
#include "world/GameInitSetting.h"

#include <QMessageBox>
#include "core/LocalClient.h"
#include <core/Server.h>
#include <core/RemoteClient.h>
#include <core/RemoteServerProxy.h>
using namespace z2;


QComboBox* getPlayerTypeComboBox(bool localGame){
    QComboBox *comBox = new QComboBox();
    comBox->addItem(QObject::tr("本地玩家"));
    comBox->addItem(QObject::tr("电脑"));
    if(!localGame){
        comBox->addItem(QObject::tr("局域网玩家"));
    }
    comBox->addItem(QObject::tr("关闭"));
    comBox->setEnabled(true);
    return comBox;
}

PlayerType getPlayerTypeByIndex(int idx){
    switch(idx){
    case 0: return PlayerType::LOCAL_PLAYER;
    case 1: return PlayerType::BOT_PLAYER;
    case 2: return PlayerType::REMOTE_PLAYER;
    default: return PlayerType::BOT_PLAYER;
    }
}
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
//    ui->tableWidget->setC
    connect(this,SIGNAL(notifyStartServerGame()),this,SLOT(startServerGame()),Qt::BlockingQueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_btnMultiple_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    localGame = false;
    initGameLobby();
}

void MainWindow::on_btnSingle_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    localGame = true;
    initGameLobby();
}

void MainWindow::on_btnJoin_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::on_stackedWidget_currentChanged(int arg1)
{

}

void initMapComboBox(QComboBox* box){
    //TODO add from GameMapRepository
    auto& maps = z2::MapRepository::instance().getMaps();
    for(auto& m : maps){
        box->addItem(QString::fromStdString(m->getName()));
    }
    box->setCurrentIndex(0);
}

void MainWindow::initGameLobby()
{
    auto comBox = ui->cmbMap;
    initMapComboBox(comBox);

}

void MainWindow::loadMap(const std::shared_ptr<z2::GameMap> &map)
{
    auto table = ui->tablePlayer;
    table->clearContents();
    int rowCount = map->getMaxPlayerCount();
    table->setRowCount(rowCount);
    for(int i=0;i<rowCount;i++){
        auto comboBox = getPlayerTypeComboBox(this->localGame);
        if(i!=0){
            comboBox->setCurrentIndex(1);
        }
        table->setCellWidget(i,0,comboBox);


        auto item = new QTableWidgetItem(QString::number(i));
        table->setItem(i,1,item);
        // playerId

        item = new QTableWidgetItem(QString::number(i));
        table->setItem(i,2,item);
        //group id

        item = new QTableWidgetItem(QString::number(i));
        table->setItem(i,3,item);
        //pos id
    }
}


void MainWindow::on_cmbMap_currentIndexChanged(int index)
{
    auto& maps = z2::MapRepository::instance().getMaps();
    currentMap = maps[index];
    loadMap(currentMap);
}



GameInitSetting loadSettingFromTable(const shared_ptr<GameMap>& map, QTableWidget* table){
    int rowCount = table->rowCount();
    vector<PlayerSetting> players;
    for(int i=0;i<rowCount;i++){
        QComboBox* box = dynamic_cast<QComboBox*>(table->cellWidget(i,0));
        if(box == nullptr){
            continue;
        }
        int idx = box->currentIndex();
        if(idx == box->count()-1){ // closed
            continue;
        }
        PlayerType type = getPlayerTypeByIndex(idx);
        int playerId = table->item(i,1)->text().toInt();
        int groupId = table->item(i,2)->text().toInt();
        int posId = table->item(i,3)->text().toInt();
        players.push_back(PlayerSetting(playerId,posId,groupId,type));
    }
    return GameInitSetting(players,map);
}


void MainWindow::on_btnStartGame_clicked()
{
    auto table = ui->tablePlayer;
    auto setting = loadSettingFromTable(currentMap,table);
    if(!setting.isValidLocalSetting(localGame)){
        QString title= "游戏设置不正确";
        QString context = "游戏设置不正确";
        QMessageBox::warning(this,title,context);
        return;
    }
    if(localGame){
        startLocalGame(setting);
    }else{
        startOnlineGameServer(setting);
    }
}


void MainWindow::startLocalGame(z2::GameInitSetting &setting)
{
    cout << "Starting local game!" << endl;
    auto pair = setting.buildLocalGame();
    auto& server = pair.first;
    shared_ptr<LocalClient>& client = pair.second;
    GameWindow* gw = new GameWindow(this);
    shared_ptr<Client> c = static_pointer_cast<Client>(client);
    gw->setClient(c);
    gw->setServer(server);
    gw->show();
    server->startGame();
}



const int port = 23456;

void MainWindow::startOnlineGameServer(GameInitSetting &setting)
{
    cout << "Starting online game!" << endl;
    auto players = setting.getPlayers();
    vector<PlayerType> ps;
    for(auto& p : players){
        ps.push_back(p.type);
    }
    onlineLobby.reset(new Lobby(ps,port,setting.buildWorld()));
    auto listener =  [this](Lobby& lobby,int id){
        if(lobby.isGameReady()){
            emit notifyStartServerGame();
        }
        //TODO add updates
        return;
    };
    onlineLobby->setOnPlayerConnected(listener);
    ui->lblAddress->setText(QString::fromStdString(onlineLobby->getAddressInfo()));
    ui->stackedWidget->setCurrentIndex(2);
    update();
}

void MainWindow::startOnlineGameClient(QString address,int id)
{
    string add = address.toStdString();
    shared_ptr<RemoteClient> rc(new RemoteClient(id));
    shared_ptr<RemoteServerProxy> proxy(new RemoteServerProxy(rc));
    rc->setServerProxy(proxy);

    GameWindow* gw = new GameWindow(this);
    rc->setView(gw->getGui());
    gw->setClient(rc);

    if(!RemoteServerProxy::tryConnect(proxy,"127.0.0.1", port)){
        cout << "failure, return";
        QString title= "连接失败";
        QString context = "连接失败";
        QMessageBox::warning(this,title,context);
        gw->deleteLater();
        return;
    }

    gw->show();
}


void MainWindow::on_btnCancel_clicked()
{
    if(onlineLobby){
        onlineLobby->closeLobby();
        onlineLobby.reset();
    }
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::startServerGame()
{
    GameWindow* gw = new GameWindow(this);
    cout << "Starting game!" << endl;
    auto server = onlineLobby->startGame(gw->getGui(),1000*60);
    cout << "Starting game 2!" << endl;
    gw->setServer(server);
    gw->show();
//    server->startGame();
}

void MainWindow::on_pushButton_clicked()
{
    QString strId = ui->textInputId->text();
    startOnlineGameClient(ui->textInputAddress->text(),strId.toInt());
}

