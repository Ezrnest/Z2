#include "gameutil.h"
#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include "entity/Farmer.h"
#include <entity/ConstructionBase.h>
#include "config/EntityRepository.h"
#include "config/TechRepository.h"
#include "config/GameConfiguration.h"
#include "core/messages/UnitBuy.h"
#include "core/messages/TechResearch.h"
#include "core/messages/EntityPerform.h"
#include "core/messages/PlayerMessage.h"
#include "event/StateEvent.h"
#include "event/PlayersWon.h"
#include "event/InGamePlayerEvent.h"
#include "world/Technology.h"
#include "core/messages/ControlMessage.h"
#include "mainwindow.h"
#include <QFileDialog>
#include "SoundRepository.h"

GameWindow::GameWindow(MainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    gui = shared_ptr<QtGui>(new QtGui(this));
    //    ui->frEntityInfo->
    mainWindow = parent;




    ui->gameFrame->setWindow(this);
    ui->frMenu->setVisible(false);
    ui->frPlayerList->setVisible(false);

    QPalette background;
    background.setColor(QPalette::Background,Qt::white);
    ui->frTurnInfo->setPalette(background);
    ui->frTurnInfo->setAutoFillBackground(true);


    setupTable(ui->tableBuy);
    setupTable(ui->tableResearch);
    setupTable(ui->tablePlayerList);


    connect(this,SIGNAL(notifyRefreshAll()),this,SLOT(refreshAll()),Qt::AutoConnection);
    connect(this,SIGNAL(notifyGameStarted()),this,SLOT(gameStarted()),Qt::AutoConnection);
    connect(this,SIGNAL(notifyGameEvent(const shared_ptr<GameEvent>&)),this,SLOT(dealWithGameEvent(const shared_ptr<GameEvent>&)),Qt::AutoConnection);
    connect(this,SIGNAL(notifyGameEnded()),this,SLOT(showGameEnded()),Qt::AutoConnection);
    connect(this,SIGNAL(notifyPlayerQuit(int)),this,SLOT(showPlayerQuit(int)),Qt::AutoConnection);
}

GameWindow::~GameWindow()
{
    delete ui;
}





void GameWindow::setClient(const shared_ptr<Client> &client)
{
    gui->setControllerAndView(client);
    client->setView(gui);
}

void GameWindow::setServer(const shared_ptr<Server> &s)
{
    server =s;
}

void GameWindow::setLobby(const shared_ptr<Lobby> &lb)
{
    lobby = lb;
}

void GameWindow::refreshAll()
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    refreshSelection();
    refreshPlayerInfo();
    refreshTurnInfo();
    update();
}

void GameWindow::gameStarted()
{
    ui->labelGameInfo->hide();
    ui->gameFrame->makeCenterConstructionBase();
    gameState = GameState::RUNNING;
    refreshAll();
}

void GameWindow::showGameEnded()
{
    if(gameState != RUNNING){
        return;
    }
    gameState = ENDING;
    //    update();
    QString title= tr("游戏结束");
    QString detail = tr("有玩家退出或者失去连接，游戏结束");
    QMessageBox::warning(this,title,detail);
    exitGame();
}

void GameWindow::dealWithGameEvent(const shared_ptr<GameEvent> &event)
{
    refreshAll();
    switch (event->getType()) {
    case EventType::STATE_EVENT:{
        dealWithStateEvent(static_pointer_cast<StateEvent>(event));
        break;
    }
    case EventType::IN_GAME_EVENT:{
        if(dynamic_pointer_cast<InGamePlayerEvent>(event)){
            dealWithInGamePlayerEvent(static_pointer_cast<InGamePlayerEvent>(event));
        }else if(dynamic_pointer_cast<EntityEvent>(event)){
            dealWithEntityEvent(static_pointer_cast<EntityEvent>(event));
        }

        break;
    }
    }


}

void GameWindow::showGameWin(const shared_ptr<PlayersWon> &event)
{
    QString title= tr("游戏结束");
    QString context = tr("游戏结束，玩家 %1 获胜");
    QStringList playerNames;
    auto w = getWorld();
    for(int id : event->getPlayerIds()){
        playerNames.append(QString::fromStdString(w->getPlayer(id).getName()));
    }
    QMessageBox::information(this,title,context.arg(playerNames.join(", ")));
    gameState = ENDED;
    close();
    deleteLater();
    if(mainWindow){
        mainWindow->afterGameEnded();
    }
}

void GameWindow::showPlayerQuit(int playerId)
{
    QString title= tr("信息");
    QString context = tr("玩家 %1 退出了游戏");
    auto w = getWorld();
    auto& pName = w->getPlayer(playerId).getName();
    QMessageBox::information(this,title,context.arg(QString::fromStdString(pName)));
}




shared_ptr<World> GameWindow::getWorld()
{
    return gui->client->getWorld();
}

shared_ptr<Client>& GameWindow::getClient()
{
    return gui->client;
}

void GameWindow::dealWithStateEvent(const shared_ptr<StateEvent> &event)
{
    switch(event->getSType()){
    case StateEventType::PlayersWon:{
        auto we = dynamic_pointer_cast<PlayersWon>(event);
        if(we){
            showGameWin(we);
        }
        break;
    }
    case StateEventType::PlayerDefeated:{
        auto pe = dynamic_pointer_cast<PlayerEvent>(event);
        if(pe){
            showPlayerDefeated(pe);
        }
        break;
    }
    default:
        break;
    }


}

void GameWindow::dealWithInGamePlayerEvent(const shared_ptr<InGamePlayerEvent> &event)
{
    switch(event->getPType()){
    case InGamePlayerEventType::TECH_RESEARCHED:{
        processPlayerResearch(static_pointer_cast<TechResearchedEvent>(event));
        break;
    }
    }
}

void GameWindow::dealWithEntityEvent(const shared_ptr<EntityEvent> &event)
{
    switch(event->getEntityEventType()){
    case EntityEventType::EntityMoved:{
        processEntityMove(static_pointer_cast<EEntityMoved>(event));
        break;
    }
    case EntityEventType::EntityCreated:{
        processEntityCreated(event);
        break;
    }
    case EntityEventType::EntityPerformed:{
        processEntityPerform(event);
        break;
    }
    }
}

void GameWindow::processEntityPerform(const shared_ptr<EntityEvent> &event)
{
    auto& en = event->getEntity();
    SoundRepository::instance().playSound(en->getEntityInfo().getProperties().get("soundOnPerform","none"));
}

void GameWindow::processEntityMove(const shared_ptr<EEntityMoved> &event)
{

}

void GameWindow::processEntityCreated(const shared_ptr<EntityEvent> &event)
{

}

void GameWindow::processPlayerResearch(const shared_ptr<TechResearchedEvent> &event)
{

}


void GameWindow::arrangeUi()
{
    adjustPosMid(ui->frMenu,true,false);
    adjustPosMid(ui->frPlayerList,true,false);
    adjustPosToBorder( ui->btnEndTurn,10,false,10,false);
    adjustPosToBorder( ui->frTurnInfo,70,false,20,false);
    adjustPosToBorder( ui->btnMenu, 5,false,5,true);
    adjustPosToBorder( ui->btnDiplomacy, 10+ui->btnMenu->width(),false,5,true);
    adjustPosToBorder(ui->tabWidget,2,true, 20, false);

    adjustWidgetSize(ui->gameFrame,true,true);

}

void GameWindow::adjustPosMid(QWidget *t, bool xMid, bool yMid)
{
    int rx,ry;
    if(xMid){
        rx = width()/2 - t->width()/2;
    }else{
        rx = t->x();
    }

    if(yMid){
        ry = height()/2 - t->height()/2;
    }else{
        ry = t->y();
    }
    t->move(rx,ry);
}

void GameWindow::adjustWidgetSize(QWidget *t, bool xExpanding, bool yExpanding)
{
    int rw,rh;

    if(xExpanding){
        rw = width() - t->x();
    }else{
        rw = t->width();
    }
    if(yExpanding){
        rh = height() - t->y();
    }else{
        rh = t->width();
    }
    t->resize(rw,rh);
}

void GameWindow::adjustPosToBorder(QWidget *t, int x, bool toLeft, int y, bool toTop)
{
    int rx,ry;
    if(toLeft){
        rx = x;
    }else{
        rx = width() - x - t->width();
    }
    if(toTop){
        ry = y;
    }else{
        ry = height() - y - t->height();
    }
    t->move(rx,ry);
}

int GameWindow::getPlayerId()
{
    return gui->client->getPlayerId();
}

bool GameWindow::isCurrentTurn()
{
    return getWorld()->getCurrentPlayer() == getPlayerId();
}

Point &GameWindow::getSelectedPos()
{
    return ui->gameFrame->selPos;
}

void GameWindow::closeEvent(QCloseEvent *event)
{
    exitGame();
}




void GameWindow::refreshSelection(bool playerClicked)
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    auto pos = getSelectedPos();

    auto world = getWorld();
    int playerId = getClient()->getPlayerId();
    if(!world->isInside(pos)){
        ui->tabWidget->hide();
        ui->tabWidget2->hide();
        return;
    }
    auto vis = world->getTile(pos).getVisibility(playerId);
    if(vis == Visibility::DARK){
        ui->tabWidget->hide();
        ui->tabWidget2->hide();
        return;
    }
    ui->tabWidget->show();
    auto en = world->getEntity(pos);
    if(vis == Visibility::GREY|| !en){
        refreshTileInfo(false,*world,pos);
        ui->tabWidget2->hide();
        return;
    }
    refreshTileInfo(true,*world,pos);

    auto& enRepo = EntityRepository::instance();
    auto& enInfo = enRepo.getEntityInfo(en->getEntityName());

    ui->lblEntityName->setText(QString::fromStdString(enInfo.getDisplayName()));
    int ownerId = en->getOwnerId();
    ui->lblOwner->setText(QString::fromStdString(world->getPlayer(ownerId).getName()));
    ui->lblMoves->setText(QString::number(en->getRemainingMoves()));


    auto hpBar = ui->barHP;
    {
        auto wh = dynamic_pointer_cast<EntityWithHealth>(en);
        if(wh){
            hpBar->setEnabled(true);
            hpBar->setMaximum(wh->getMaxHealth());
            hpBar->setValue(wh->getHealth());
        }else{
            hpBar->setEnabled(false);
        }
    }
    auto melee = dynamic_pointer_cast<MeleeUnit>(en);
    if(melee){
        ui->lblAttack->setText(QString::number(melee->getAttackStrength()));
    }else{
        ui->lblAttack->setText("0");
    }

    if(playerClicked && ownerId == getPlayerId()){
        clickedOnOwnedEntity(en);
    }

    refreshContruction(en,*world,pos);
    refreshPerformAbility(en,*world,pos);
}

void GameWindow::clickedOnOwnedEntity(const shared_ptr<Entity> &en)
{
    SoundRepository::instance().playSound(en->getEntityInfo().getProperties().get("soundOnClick","none"));
}

void GameWindow::refreshPlayerInfo()
{

    auto client = getClient();
    auto w = getWorld();
    Player& player = w->getPlayer(client->getPlayerId());

    ui->lblPlayerName->setText(QString::fromStdString(player.getName()));
    ui->lbllGold->setNum(player.getGold());
    ui->lbllTechPoint->setNum(player.getTechPoints());
    //    ui->lblGroup->setNum(player.getGroupId());
    //    ui->lblP
}

void GameWindow::refreshTurnInfo()
{
    auto w = getWorld();
    int curPlayer = w->getCurrentPlayer();
    if(curPlayer == getPlayerId()){
        ui->frTurnInfo->hide();
        ui->btnEndTurn->setEnabled(true);
    }else{
        ui->frTurnInfo->show();
        ui->btnEndTurn->setEnabled(false);
        QString text = tr("等待 %1 的回合").arg(QString::fromStdString(w->getCurrentAsPlayer().getName()));
        ui->lblCurrentPlayer->setText(text);
    }

}

void GameWindow::refreshPerformAbility(shared_ptr<Entity> &en, World &w, Point &pos)
{
    auto btn =  ui->btnPerform;

    if(w.canPerform( pos,getClient()->getPlayerId())){
        btn->show();
        const string& pText = en->getEntityInfo().getDisplayPerformText();
        btn->setText(QString::fromStdString(pText));
    }else{
        btn->hide();
    }
}

void GameWindow::refreshContruction(shared_ptr<Entity> &en, World &w, Point &p)
{
    auto cons = dynamic_pointer_cast<ConstructionBase>(en);
    if(!cons || cons->getOwnerId() != getPlayerId()){
        ui->tabWidget2->hide();
        return;
    }
    ui->tabWidget2->show();
    ui->btnBuy->setEnabled(false);
    auto client = getClient();
    vector<const EntityInfo*> availables = w.getAvailableEntitiesFor(client->getPlayerId());
    int count = availables.size();
    auto table = ui->tableBuy;
    table->clearContents();

    table->setRowCount(availables.size());

    for(int i=0;i<count;i++){
        auto enInfo = availables[i];
        auto item = new QTableWidgetItem(QString::fromStdString(enInfo->getDisplayName()));
        table->setItem(i,0,item);
        item->setData(Qt::UserRole,qVariantFromValue((void*)enInfo));
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        item = new QTableWidgetItem(QString::number(enInfo->getProperties().getInt("price",-1)));
        table->setItem(i,1,item);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    }
    refreshTechnology(w);
}

void GameWindow::refreshTechnology(World &w)
{
    ui->btnResearch->setEnabled(false);
    auto client = getClient();
    auto availables = w.getResearchableTechFor(client->getPlayerId());
    int count = availables.size();
    auto table = ui->tableResearch;
    table->clearContents();

    table->setRowCount(availables.size());

    for(int i=0;i<count;i++){
        auto techInfo = availables[i];
        auto item = new QTableWidgetItem(QString::fromStdString(techInfo->getDisplayName()));
        table->setItem(i,0,item);
        item->setData(Qt::UserRole,qVariantFromValue((void*)techInfo));
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        item = new QTableWidgetItem(QString::number(techInfo->getProp().getInt("price",-1)));
        table->setItem(i,1,item);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    }
}


QString terrainToString(Terrain t){
    switch (t) {
    case Terrain::MOUNTAIN:
        return QObject::tr("山脉");
    case Terrain::HILL:
        return QObject::tr("丘陵");
    case Terrain::WATER:
        return QObject::tr("水域");
    case Terrain::DESERT:
        return QObject::tr("沙地");
    case Terrain::FOREST:
        return QObject::tr("森林");
    case Terrain::PLAIN:
    default:
        return QObject::tr("平原");
    }
}

QString resourceToString(Resource r){
    switch (r) {
    case Resource::GEM:
        return  QObject::tr("钻石");
    case Resource::MINE:
        return  QObject::tr("金矿");
    case Resource::NONE:
    default:
        return QObject::tr("无");
    }
}

void GameWindow::refreshTileInfo(bool entityInfo, World& w, Point& p)
{
    Tile& t = w.getTile(p);
    QString posStr("(%1,%2)");
    ui->lblTilePos->setText(posStr.arg(QString::number(p.x),QString::number(p.y)));
    ui->lblTileTerrain->setText(terrainToString(t.getTerrain()));
    ui->lblTileResource->setText(resourceToString(t.getResource()));
    if(entityInfo){
        ui->tabWidget->setCurrentIndex(1); // set to entity info
        ui->tabEntityInfo->show();
        ui->widgetEntityInfo->show();
    }else{
        ui->tabWidget->setCurrentIndex(0); // set to tile info
        ui->tabEntityInfo->hide();
        ui->widgetEntityInfo->hide();
    }
}

void GameWindow::showMenuPlayerList()
{
    auto w = getWorld();
    auto& ps = w->getPlayers();
    int pid = getPlayerId();
    auto& player = ps[pid];
    auto table = ui->tablePlayerList;
    table->clearContents();
    table->setRowCount(ps.size());
    for(int i=0;i<ps.size();i++){
        const Player& p = ps[i];
        auto item = new QTableWidgetItem(QString::number(i));
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        table->setItem(i,0,item); // order

        item = new QTableWidgetItem(QString::fromStdString(p.getName()));
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        table->setItem(i,1,item); // name


        if(p.isDead()){
            item = new QTableWidgetItem(tr("战败"));
        }else{
            item = new QTableWidgetItem(tr("存活"));
        }
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        table->setItem(i,2,item); // state


        if(pid == i){
            item = new QTableWidgetItem(tr("/"));
        }else if(p.isAlly(player.getGroupId())){
            item = new QTableWidgetItem(tr("盟友"));
        }else{
            item = new QTableWidgetItem(tr("敌人"));
        }
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        table->setItem(i,3,item); // diplomacy



    }

    ui->frPlayerList->show();
}

void GameWindow::saveGame()
{
    if(!isCurrentTurn()){
        QString title = tr("警告");
        QString text = tr("只有在自己的回合才能保存游戏！");
        QMessageBox::warning(this,title,text);
        return;
    }
    GameUtil::saveGame(this,getWorld());
}

void GameWindow::exitGame()
{
    if(gameState != RUNNING && gameState != ENDING){
        return;
    }
    gameState = ENDED;
    close();
    MessagePtr msg(new PlayerMessage(ControlMessageType::PlayerQuit,getPlayerId()));
    getClient()->sendMessageToServer(msg);

    cout << "Game Exited!" << endl;
    deleteLater();
    if(mainWindow){
        mainWindow->afterGameEnded();
    }
}

void GameWindow::showPlayerDefeated(const shared_ptr<PlayerEvent> &event)
{

    QString title = tr("玩家战败");
    QString text;
    if(event->getPlayerId() == getPlayerId()){
        text = tr("你战败了！");
    }else{
        auto world = getWorld();
        string pName = world->getPlayer(event->getPlayerId()).getName();
        text = tr("玩家 %1 战败了").arg(QString::fromStdString(pName));
    }
    QMessageBox::information(this,title,text);
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    int k = event->key();
    if(k == Qt::Key_C){
        //        cout << "Make CCB" << endl;
        ui->gameFrame->makeCenterConstructionBase();
        return;
    }
}

shared_ptr<QtGui> GameWindow::getGui()
{
    return this->gui;
}

void GameWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    arrangeUi();
}


QtGui::QtGui(GameWindow *gw) : window(gw)
{

}

void QtGui::update()
{
    emit window->notifyRefreshAll();
}

void QtGui::onPlayerTurnStarted(int playerId)
{
    update();
}

void QtGui::onPlayerTurnFinished(int playerId)
{
    update();
}

void QtGui::onGameStarted()
{
    //    cout << "QtGui: Game Started" << endl;
    emit window->notifyGameStarted();
}

void QtGui::onPlayerWin(int playerId)
{
    update();
}

void QtGui::onGameStopped()
{
    emit window->notifyGameEnded();
}

void QtGui::onEvent(const shared_ptr<GameEvent> &event)
{
    //    update();
    emit window->notifyGameEvent(event);

}

void GameWindow::on_btnPerform_clicked()
{
    Point pos = ui->gameFrame->selPos;
    shared_ptr<EntityPerform> msg(new EntityPerform(pos));
    getClient()->sendMessageToServer(msg);
}


void GameWindow::on_btnEndTurn_clicked()
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    getClient()->sendTurnFinishMessage();
}

void GameWindow::on_btnBuy_clicked()
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    auto table = ui->tableBuy;
    int row = table->currentRow();
    if(row < 0 || row >= table->rowCount()){
        return;
    }
    auto item = table->item(row,0);
    EntityInfo* enInfo = (EntityInfo*)(item->data(Qt::UserRole).value<void*>());
    auto& name = enInfo->getIdentifier();
    auto pos = getSelectedPos();
    auto client = getClient();
    shared_ptr<UnitBuy> msg(new UnitBuy(name,pos,client->getPlayerId()));
    client->sendMessageToServer(msg);
}

void GameWindow::on_btnResearch_clicked()
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    auto table = ui->tableResearch;
    int row = table->currentRow();
    if(row < 0 || row >= table->rowCount()){
        return;
    }
    auto item = table->item(row,0);
    Technology* techInfo = (Technology*)(item->data(Qt::UserRole).value<void*>());
    auto& name = techInfo->getId();
    auto client = getClient();
    shared_ptr<TechResearch> msg(new TechResearch(name,client->getPlayerId()));
    client->sendMessageToServer(msg);
}

void GameWindow::on_actionPlayerInfo_triggered()
{

}

void GameWindow::on_btnMenu_clicked()
{
    ui->frMenu->show();
}

void GameWindow::on_btnExitGame_clicked()
{
    exitGame();
}

void GameWindow::on_btnMenuCancel_clicked()
{
    ui->frMenu->hide();
}

void GameWindow::on_btnSaveGame_clicked()
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    saveGame();
}

void GameWindow::on_tableResearch_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    auto table = ui->tableResearch;
    int row = currentRow;
    if(row < 0 || row >= table->rowCount()){
        return;
    }
    auto item = table->item(row,0);
    Technology* techInfo = (Technology*)(item->data(Qt::UserRole).value<void*>());
    auto& name = techInfo->getId();
    auto world = getWorld();
    if(world->canResearchTechnology(getPlayerId(),name)){
        ui->btnResearch->setEnabled(true);
    }else{
        ui->btnResearch->setEnabled(false);
    }
}

void GameWindow::on_tableBuy_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    auto table = ui->tableBuy;
    int row = currentRow;
    if(row < 0 || row >= table->rowCount()){
        return;
    }
    auto item = table->item(row,0);
    EntityInfo* enInfo = (EntityInfo*)(item->data(Qt::UserRole).value<void*>());
    auto& name = enInfo->getIdentifier();
    auto pos = getSelectedPos();
    auto world =getWorld();

    if(world->canBuy(getPlayerId(),pos,name)){
        ui->btnBuy->setEnabled(true);
    }else{
        ui->btnBuy->setEnabled(false);
    }

}


void GameWindow::on_btnMenuCancel_2_clicked()
{
    ui->frPlayerList->hide();
}

void GameWindow::on_btnDiplomacy_clicked()
{
    if(gameState != GameWindow::RUNNING){
        return;
    }
    showMenuPlayerList();
}
