#include "gamewindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config/MapRepository.h"
#include "config/GameConfiguration.h"
#include "world/GameInitSetting.h"
#include "SoundRepository.h"
#include "gameutil.h"
#include "playercolor.h"
#include <QDialog>
#include <QMediaPlaylist>
#include <QMessageBox>
#include <QSoundEffect>
#include "core/LocalClient.h"
#include <core/Server.h>
#include <core/RemoteClient.h>
#include <core/RemoteServerProxy.h>
#include <QFileDialog>

using namespace z2;

void setupTable(QTableWidget* table){
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
}

void setupComboBoxInt(QComboBox* x,int maxNumber){
    x->clear();
    for(int i=1;i<=maxNumber;i++){
        x->addItem(QString::number(i));
    }
    x->setCurrentIndex(maxNumber-1);
}

QComboBox* getPlayerGroupComboBox(QWidget* parent,int idx, const shared_ptr<GameMap>& map){
    int maxGroup = map->getMaxPlayerCount();
    QComboBox *comBox = new QComboBox(parent);
    comBox->addItem("-");
    for(int i=0;i<maxGroup;i++){
        comBox->addItem(QString::number(i));
    }

    comBox->setCurrentIndex(idx+1);
    return comBox;
}


int getPlayerGroupFromComboBox(QWidget* w){
    QComboBox* box = dynamic_cast<QComboBox*>(w);
    if(!box || box->currentIndex() == 0){
        return -1;
    }else{
        return box->currentIndex()-1;
    }
}

QComboBox* getPlayerPosComboBox(QWidget* parent, int idx, const shared_ptr<GameMap>& map){
    int maxGroup = map->getMaxPlayerCount();
    QComboBox *comBox = new QComboBox(parent);
    comBox->addItem("随机");
    for(int i=1;i<=maxGroup;i++){
        comBox->addItem(QString::number(i));
    }
    comBox->setCurrentIndex(0);
    return comBox;
}

int getPlayerPosFromComboBox(QWidget* w){
    QComboBox* box = dynamic_cast<QComboBox*>(w);
    if(!box || box->currentIndex() == 0){
        return PlayerSetting::RANDOM_POS;
    }else{
        return box->currentIndex()-1;
    }
}


QComboBox* getPlayerTypeComboBox(QWidget* parent,bool localGame){
    QComboBox *comBox = new QComboBox(parent);
    comBox->addItem(QObject::tr("本地玩家"));
    comBox->addItem(QObject::tr("电脑"));
    if(!localGame){
        comBox->addItem(QObject::tr("局域网玩家"));
    }
//    comBox->addItem(QObject::tr("关闭"));
    comBox->setEnabled(true);

    return comBox;
}

QComboBox* getPlayerColorComboBox(QWidget* parent){
    QComboBox* comBox = new QComboBox();
    for(auto& s : PlayerColor::getColorNames()){
        comBox->addItem(s);
    }
    comBox->setEnabled(true);
    return comBox;
}

QComboBox* getBotDifficultyComboBox(QWidget* parent){
    QComboBox* comBox = new QComboBox();
    comBox->addItem("无");
    comBox->addItem("简单");
    comBox->addItem("中等");
    comBox->addItem("困难");
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
    setupTable(ui->tablePlayer);
    SoundRepository::instance().playMainMenuBGM();
    connect(this,SIGNAL(notifyStartServerGame()),this,SLOT(startServerGame()),Qt::AutoConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_btnMultiple_clicked()
{
    SoundRepository::instance().playClick();
    localGame = false;
    loadSavedMap = false;
    initGameLobby();
}

void MainWindow::on_btnSingle_clicked()
{
    SoundRepository::instance().playClick();
    localGame = true;
    loadSavedMap = false;
    initGameLobby();
}

void MainWindow::on_btnJoin_clicked()
{
    SoundRepository::instance().playClick();
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::on_stackedWidget_currentChanged(int arg1)
{

}

void initMapComboBox(QComboBox* box){
    box->clear();
    auto& maps = z2::MapRepository::instance().getMaps();
    for(auto& m : maps){
        box->addItem(QString::fromStdString(m->getName()));
    }
    box->setCurrentIndex(0);
}

void MainWindow::initGameLobby()
{
    ui->stackedWidget->setCurrentIndex(1);
    auto comBox = ui->cmbMap;
    initMapComboBox(comBox);
}

void MainWindow::refreshGameLobby()
{
    auto comBox = ui->cmbMap;
    if(loadSavedMap){
        comBox->hide();
        ui->lblMapName->show();
        ui->lblMapName->setText(QString::fromStdString(currentMap->getName()));
    }else{
        comBox->show();
        ui->lblMapName->hide();
    }
    ui->mapDisplay->setGameMap(currentMap);
}

void MainWindow::loadMap(const std::shared_ptr<z2::GameMap> &map)
{
    currentMap = map;
    auto table = ui->tablePlayer;
    table->clearContents();
    int rowCount = map->getMaxPlayerCount();
    table->setRowCount(rowCount);
    for(int i=0;i<rowCount;i++){


        auto item = new QTableWidgetItem(QString::number(i));
        table->setItem(i,0,item);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        // playerId

        auto comboBox = getPlayerTypeComboBox(table,this->localGame);
        if(i!=0){
            comboBox->setCurrentIndex(1);
        }
        table->setCellWidget(i,1,comboBox);

        comboBox = getPlayerGroupComboBox(table,i,map);
        table->setCellWidget(i,2,comboBox);
        //group id

        comboBox = getPlayerPosComboBox(table,i,map);
        table->setCellWidget(i,3,comboBox);
        //pos id

        comboBox = getPlayerColorComboBox(table);
        if(i < comboBox->count()){
            comboBox->setCurrentIndex(i);
        }
        table->setCellWidget(i,4,comboBox);
        //color

        comboBox = getBotDifficultyComboBox(table);
        table->setCellWidget(i,5,comboBox);
    }
    setupComboBoxInt(ui->comboBoxPlayerNumber,map->getMaxPlayerCount());
    refreshGameLobby();
}

int MainWindow::getPlayerCount()
{
    return ui->comboBoxPlayerNumber->currentIndex() + 1;
}


void MainWindow::on_cmbMap_currentIndexChanged(int index)
{
    auto& maps = z2::MapRepository::instance().getMaps();
    if(index < 0 || index >= maps.size()){
        return;
    }
    currentMap = maps[index];
    loadMap(currentMap);
}



GameInitSetting MainWindow::loadGameSettings(){
    auto& map = currentMap;
    auto table = ui->tablePlayer;
    int playerCount = getPlayerCount();

    vector<PlayerSetting> players;
    for(int i=0;i<playerCount;i++){
        int playerId = table->item(i,0)->text().toInt();
        QComboBox* box = dynamic_cast<QComboBox*>(table->cellWidget(i,1));
        if(box == nullptr){
            continue;
        }
        int idx = box->currentIndex();

        PlayerType type = getPlayerTypeByIndex(idx);

        int groupId = getPlayerGroupFromComboBox(table->cellWidget(i,2));
        int posId = getPlayerPosFromComboBox(table->cellWidget(i,3));
        box = dynamic_cast<QComboBox*>(table->cellWidget(i,4));
        if(box == nullptr){
            continue;
        }
        int colorCode = box->currentIndex();
        box = dynamic_cast<QComboBox*>(table->cellWidget(i,5));
        BotDifficulty diff = (BotDifficulty)box->currentIndex();
        PlayerSetting ps(playerId,posId,groupId,type);
        ps.colorCode = colorCode;
        ps.diff = diff;
        players.push_back(ps);
    }
    return GameInitSetting(players,map);
}


void MainWindow::on_btnStartGame_clicked()
{
    SoundRepository::instance().playClick();
    auto setting = loadGameSettings();
    if(!setting.isValidLocalSetting(localGame)){
        QString title= "游戏设置不正确";
        QString context = "游戏设置不正确\n"
                          "位置不能重复";
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

    server->startGame();
    beforeStartingGame(gw);
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
    onlineLobby.reset(new Lobby(port,setting));
    auto listener =  [this](Lobby& lobby,int id){
        if(lobby.isGameReady()){
            emit notifyStartServerGame();
        }
        //TODO add updates
        return;
    };
    onlineLobby->setOnPlayerConnected(listener);
    onlineLobby->openLobby();
    if(!onlineLobby || onlineLobby->isGameReady()){ //game already started
        return;
    }
    ui->lblHostName->setText(QString::fromStdString(onlineLobby->getHostNameInfo()));
    ui->lblAddress->setText(QString::fromStdString(onlineLobby->getAddressInfo()));
    ui->stackedWidget->setCurrentIndex(2);
    update();
}

void MainWindow::startServerGame()
{
    GameWindow* gw = new GameWindow(this);
    //    cout << "Starting game!" << endl;
    auto server = onlineLobby->startGame(gw->getGui(),1000*60);
    //    cout << "Starting game 2!" << endl;
    gw->setServer(server);
    gw->setLobby(onlineLobby);
    onlineLobby.reset();
    ui->stackedWidget->setCurrentIndex(0);

    beforeStartingGame(gw);
    //    server->startGame();
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

    if(!RemoteServerProxy::tryConnect(proxy,add, port)){
        QString title= "连接失败";
        QString context = "连接失败";
        QMessageBox::warning(this,title,context);
        gw->deleteLater();
        return;
    }

    beforeStartingGame(gw);
}

void MainWindow::initSettingPage()
{
    ui->stackedWidget->setCurrentIndex(4);
    auto& gc = GameConfiguration::instance();
    const string& playerName = gc.getPlayerName();
    ui->textInputPlayerName->setText(QString::fromStdString(playerName));
    auto& prop = gc.getProp();
    ui->sliderBGM->setValue(prop.getInt("volumnBGM",100));
    ui->sliderSoundEffect->setValue(prop.getInt("volumnEff",100));
}

void MainWindow::saveGameSetting()
{
    QString playerName = ui->textInputPlayerName->text();
    if(playerName.contains(' ')){
        QMessageBox::warning(this,"警告","用户名中不能含有空格!");
        return;
    }
    auto& gc = GameConfiguration::instance();
    auto& prop = gc.getProp();
    prop.set("playerName",playerName.toStdString());
    prop.setInt("volumnBGM",ui->sliderBGM->value());
    prop.setInt("volumnEff",ui->sliderSoundEffect->value());
    SoundRepository::instance().setVolumn(ui->sliderBGM->value(),ui->sliderSoundEffect->value());
    gc.saveProp();
    QMessageBox::information(this,"信息","保存成功");
}

void MainWindow::beforeStartingGame(GameWindow *gw)
{
    SoundRepository::instance().playInGameBGM();
//    this->setVisible(false);
    ui->stackedWidget->setCurrentIndex(0);
    gw->show();
}

void MainWindow::afterGameEnded()
{
    SoundRepository::instance().playMainMenuBGM();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showEvent(QShowEvent *e)
{
    QMainWindow::showEvent(e);
    this->update();
}


void MainWindow::on_btnCancel_clicked()
{
    if(onlineLobby){
        onlineLobby->closeLobby();
        cout << "Closed" << endl;
        onlineLobby.reset();
        cout << "Reseted" << endl;
    }
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_pushButton_clicked()
{
    SoundRepository::instance().playClick();
    QString strId = ui->textInputId->text();
    startOnlineGameClient(ui->textInputAddress->text(),strId.toInt());
}


void MainWindow::on_pushButton_2_clicked()
{
    SoundRepository::instance().playClick();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    SoundRepository::instance().playClick();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_4_clicked()
{
    auto map = GameUtil::loadGame(this);
    if(!map){
        loadSavedMap = false;
        refreshGameLobby();
        return;
    }
    loadSavedMap = true;
    loadMap(map);
}

void MainWindow::on_btnSetting_clicked()
{
    SoundRepository::instance().playClick();
    initSettingPage();
}

void MainWindow::on_btnCancel_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnCancel_3_clicked()
{
    saveGameSetting();
}



void MainWindow::on_btnExitGame_clicked()
{
    this->close();
}

void MainWindow::on_comboBoxPlayerNumber_currentIndexChanged(int index)
{
    auto table = ui->tablePlayer;
    int number = min(index+1,table->rowCount());
    int column = table->columnCount();
    for(int i=0;i<number;i++){
        table->showRow(i);
//        for(int j =0;j<column;j++){
//            auto item = table->item(i,j);
//            }
//            item->setFlags(item->flags() & (Qt::ItemIsEnabled));
//        }
    }
    for(int i=number;i<table->rowCount();i++){
//        for(int j =0;j<column;j++){
//            auto item = table->item(i,j);
//            item->setFlags(item->flags() & (~Qt::ItemIsEnabled));
//        }
        table->hideRow(i);
    }
}
