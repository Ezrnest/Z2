/*
 * Created by liyicheng on 2019/7/6.
 */

#ifndef Z2_GAMEEVENT_H
#define Z2_GAMEEVENT_H

#include <core/Serializable.h>

namespace z2 {
enum EventType {
    StateEvent,
    InGameEvent
};

/**
 * Describes an event of the game.
 *
 * This class is used only locally.
 */
class GameEvent{
protected:
    EventType type;
public:
    explicit GameEvent(EventType type);

    virtual ~GameEvent();

    EventType getType() const;

    void setType(EventType type);

    virtual const string& name() = 0;
};

class InGameEvent : public GameEvent{
public:
    InGameEvent();
};

class StateEvent : public GameEvent{
public:
    StateEvent();
};
}


#endif //Z2_GAMEEVENT_H
