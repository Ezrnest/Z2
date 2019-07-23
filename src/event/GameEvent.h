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

class GameEvent : public Serializable {
protected:
    EventType type;
public:
    explicit GameEvent(EventType type);

    EventType getType() const;

    void setType(EventType type);
};
}

#endif //Z2_GAMEEVENT_H
