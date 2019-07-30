/*
 * Created by liyicheng on 2019/7/6.
 */

#ifndef Z2_GAMEEVENT_H
#define Z2_GAMEEVENT_H

#include <core/Serializable.h>
#include <memory>
namespace z2 {

enum EventType {
    STATE_EVENT,
    IN_GAME_EVENT
};


/**
 * Describes an event of the game.
 *
 * This class is used only locally.
 */
class GameEvent {
protected:
    EventType type;
public:
    explicit GameEvent(EventType type);

    virtual ~GameEvent();

    EventType getType() const;

    void setType(EventType type);

};

using GameEventPtr = std::shared_ptr<GameEvent>;

class InGameEvent : public GameEvent {
public:
    InGameEvent();
};
}



#endif //Z2_GAMEEVENT_H
