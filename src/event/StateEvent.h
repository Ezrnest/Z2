//
// Created by liyicheng on 2019/7/28.
//

#ifndef Z2_STATEEVENT_H
#define Z2_STATEEVENT_H


#include "GameEvent.h"

namespace z2 {

enum class StateEventType{
    GameStarted,
    GameEnded,

    PlayerTurnStarted,
    PlayerTurnEnded,
    PlayerDefeated,
    GroupWon
};

class StateEvent : public GameEvent{

private:
    StateEventType sType;

public:
    explicit StateEvent(StateEventType sType);

    StateEventType getSType() const;


};

class PlayerEvent : public StateEvent{
private:
    int playerId;
public:
    PlayerEvent(StateEventType sType, int playerId);

    int getPlayerId() const;
};

class GroupEvent : public StateEvent{
private:
    int groupId;
public:
    GroupEvent(StateEventType sType, int groupId);

    int getGroupId() const;
};

}


#endif //Z2_STATEEVENT_H
