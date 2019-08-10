#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QMessageBox>
#include <QPainter>
#include "entity/Farmer.h"
#include <entity/ConstructionBase.h>
#include "config/EntityRepository.h"
#include "config/TechRepository.h"
#include "core/messages/UnitBuy.h"
#include "core/messages/TechResearch.h"
#include "core/messages/EntityPerform.h"
#include "event/StateEvent.h"
#include "world/Technology.h"
#include "core/messages/ControlMessage.h"
#include "mainwindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    gui = shared_ptr<QtGui>(new QtGui(this));
    //    ui->frEntityInfo->

    QPalette pal = palette();
    // set black background
    pal.setColor(QPalette::Background, Qt::white);

    ui->gameFrame->setWindow(this);

    setupTable(ui->tableBuy);
    setupTable(ui->tableResearch);

    connect(this,SIGNAL(notifyRefreshAll()),this,SLOT(refreshAll()),Qt::AutoConnection);
    connect(this,SIGNAL(notifyGameEvent(const shared_ptr<GameEvent>&)),this,SLOT(dealWithGameEvent(const shared_ptr<GameEvent>&)),Qt::AutoConnection);
    connect(this,SIGNAL(notifyGameEnded()),this,SLOT(showGameEnded()),Qt::AutoConnection);

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
    refreshSelection();
    refreshPlayerInfo();
    refreshTurnInfo();
    update();
}

void GameWindow::showGameEnded()
{
    if(gameState == 1){
        return;
    }
    //    update();
    QString title= "游戏结束";
    QString detail = "有玩家退出或者失去连接，游戏结束";
    QMessageBox::warning(this,title,detail);
    this->close();
}

void GameWindow::dealWithGameEvent(const shared_ptr<GameEvent> &event)
{
    auto we = dynamic_pointer_cast<GroupEvent>(event);
    if(we && we->getSType() == StateEventType::GroupWon){
        showGameWin(we);
        return;
    }
}

void GameWindow::showGameWin(const shared_ptr<GroupEvent> &event)
{
    QString title= "游戏结束";
    QString context = "游戏结束，玩家组%1获胜";
    QMessageBox::information(this,title,context.arg(event->getGroupId()));
    this->close();
}




shared_ptr<World> GameWindow::getWorld()
{
    return gui->client->getWorld();
}

shared_ptr<Client> GameWindow::getClient()
{
    return gui->client;
}

Point &GameWindow::getSelectedPos()
{
    return ui->gameFrame->selPos;
}

void GameWindow::closeEvent(QCloseEvent *event)
{
    exitGame();
}




void GameWindow::refreshSelection()
{
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
    refreshContruction(en,*world,pos);
    refreshPerformAbility(en,*world,pos);
}

void GameWindow::refreshPlayerInfo()
{

    auto client = getClient();
    auto w = getWorld();
    Player& player = w->getPlayer(client->getPlayerId());
    ui->lblPlayerName->setText(QString::fromStdString(player.getName()));
    ui->lbllGold->setNum(player.getGold());
    ui->lbllTechPoint->setNum(player.getTechPoints());
    ui->lblGroup->setNum(player.getGroupId());
//    ui->lblP
}

void GameWindow::refreshTurnInfo()
{
    auto w = getWorld();
    ui->lblCurrentPlayer->setText(QString::fromStdString(w->getCurrentAsPlayer().getName()));
}

void GameWindow::refreshPerformAbility(shared_ptr<Entity> &en, World &w, Point &pos)
{
    auto btn =  ui->btnPerform;

    if(w.canPerform( pos,getClient()->getPlayerId())){
        btn->show();
        if(dynamic_pointer_cast<Farmer>(en)){
            btn->setText("采矿");
        }
    }else{
        btn->hide();
    }
}

void GameWindow::refreshContruction(shared_ptr<Entity> &en, World &w, Point &p)
{
    auto cons = dynamic_pointer_cast<ConstructionBase>(en);
    if(!cons){
        ui->tabWidget2->hide();
        return;
    }
    ui->tabWidget2->show();
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
        return "山脉";
    case Terrain::HILL:
        return "丘陵";
    case Terrain::PLAIN:
    default:
        return "平原";
    }
}

QString resourceToString(Resource r){
    switch (r) {
    case Resource::GEM:
        return  "钻石";
    case Resource::MINE:
        return  "金矿";
    case Resource::NONE:
    default:
        return "无";
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
        ui->tabWidget->setCurrentIndex(1); // set to tile info
        ui->tabEntityInfo->show();
    }else{
        ui->tabWidget->setCurrentIndex(0); // set to tile info
        ui->tabEntityInfo->hide();
    }
}

void GameWindow::exitGame()
{
    if(gameState == 1){
        return;
    }
    cout << "Game Exited!" << endl;
    gameState = 1;
    MessagePtr msg(new ControlMessage(ControlMessageType::EndGame));
    getClient()->sendMessageToServer(msg);
    deleteLater();
}

shared_ptr<QtGui> GameWindow::getGui()
{
    return this->gui;
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

void QtGui::onGameStarted()
{
    update();
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
    update();
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
    getClient()->sendTurnFinishMessage();
}

void GameWindow::on_btnBuy_clicked()
{
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
    auto table = ui->tableResearch;
    int row = table->currentRow();
    if(row < 0 || row >= table->rowCount()){
        return;
    }
    auto item = table->item(row,0);
    Technology* techInfo = (Technology*)(item->data(Qt::UserRole).value<void*>());
    auto& name = techInfo->getId();
    auto pos = getSelectedPos();
    auto client = getClient();
    shared_ptr<TechResearch> msg(new TechResearch(name,client->getPlayerId()));
    client->sendMessageToServer(msg);
}
