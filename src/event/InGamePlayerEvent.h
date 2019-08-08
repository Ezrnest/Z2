/*
 * Created by liyicheng on 2019/8/8.
 */

#ifndef Z2_INGAMEPLAYEREVENT_H
#define Z2_INGAMEPLAYEREVENT_H

#include "GameEvent.h"

namespace z2{

enum class InGamePlayerEventType{
    TECH_RESEARCHED,
    GOLD_ADDED,
};

class InGamePlayerEvent : public InGameEvent{
private:
    InGamePlayerEventType pType;
    int playerId;
public:
    explicit InGamePlayerEvent(z2::InGamePlayerEventType pType, int playerId);

    InGamePlayerEventType getPType() const;

    int getPlayerId() const;
};
class TechResearchedEvent : public InGamePlayerEvent{
private:
    int techId;
public:
    TechResearchedEvent(int playerId, int techId);

    int getTechId() const;
};


}

#endif //Z2_INGAMEPLAYEREVENT_H
