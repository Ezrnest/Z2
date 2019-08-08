/*
 * Created by liyicheng on 2019/8/8.
 */

#include "InGamePlayerEvent.h"

z2::InGamePlayerEvent::InGamePlayerEvent(z2::InGamePlayerEventType pType, int playerId)
        : pType(pType), playerId(playerId) {}

z2::InGamePlayerEventType z2::InGamePlayerEvent::getPType() const {
    return pType;
}

int z2::InGamePlayerEvent::getPlayerId() const {
    return playerId;
}

z2::TechResearchedEvent::TechResearchedEvent(int playerId, const string& techId)
        : InGamePlayerEvent(InGamePlayerEventType ::TECH_RESEARCHED, playerId), techId(techId) {}


const string &z2::TechResearchedEvent::getTechId() const {
    return techId;
}
