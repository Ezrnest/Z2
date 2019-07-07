/*
 * Created by liyicheng on 2019/7/7.
 */

#include "CommandLineGameGui.h"
#include <iostream>
using namespace z2;
void CommandLineGameGui::update() {

}

void CommandLineGameGui::onPlayerTurnStarted(int playerId) {

}

void CommandLineGameGui::onGameStarted() {

}

void CommandLineGameGui::onPlayerWin(int playerId) {

}

void CommandLineGameGui::printWorld() {
    shared_ptr<World> wPtr = client->getWorld();
    World& w = *wPtr;
    cout << "CurrentPlayer:" << w.getCurrentPlayer() << '\n';
    Tile** data = w.data;
    for(int j=0;j<w.height+1;j++){
        cout << "--";
    }
    cout << '\n';
    for(int i=w.height-1;i>-1;i--){
        cout << "|";
        for(int j=0;j<w.height;j++){
            Tile& t = data[i][j];
            if(!t.hasEntity()){
                cout << ' ';
            }else{
                cout << t.getEntity()->identifier()[0];
            }
            cout << ' ';
        }
        cout << "|\n";
    }
    for(int j=0;j<w.height+1;j++){
        cout << "--";
    }
    cout << '\n';
}
