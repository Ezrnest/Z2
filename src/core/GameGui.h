/*
 * Created by liyicheng on 2019/7/7.
 */

#ifndef Z2_GAMEGUI_H
#define Z2_GAMEGUI_H

#include <event/GameEvent.h>
#include "Client.h"

namespace z2 {
class Client;

/**
 * Describes the gui interface for the game. This interface can be implemented in several ways,
 * such as CommandLineGameGui or QtGameGui.
 */
class GameGui {
protected:
    shared_ptr<Client> client;

    void attachListener(const shared_ptr<World>& w);

public:
    virtual ~GameGui();

    /**
     * Sets the client to the game gui. Further user input will be dispatched to the
     * client(as controller).
     */
    void setControllerAndView(const shared_ptr<Client> &c);

    virtual void onWorldLoaded(const shared_ptr<World>& w);

    /**
     * Calls the game gui to update.
     */
    virtual void update() = 0;

    virtual void onPlayerTurnStarted(int playerId) = 0;

    virtual void onPlayerTurnFinished(int playerId) = 0;

    virtual void onGameStarted() = 0;

    /**
     * Deprecated!
     */
    virtual void onPlayerWin(int playerId) = 0;

    virtual void onPlayerQuit(int playerId);

    /**
     * Called when the game is stopped.
     */
    virtual void onGameStopped() = 0;

    virtual void onEvent(const shared_ptr<GameEvent> &event) = 0;
};

}

#endif //Z2_GAMEGUI_H
