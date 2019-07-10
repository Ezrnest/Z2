/*
 * Created by liyicheng on 2019/7/6.
 */

#ifndef Z2_GAMEEVENT_H
#define Z2_GAMEEVENT_H

namespace z2 {
enum EventType {
    EUnitMove,
    EUnitBuy,
    EUnitAttack,
};

class GameEvent {
protected:
    EventType type;
public:
    EventType getType() const;

    void setType(EventType type);
};
}

#endif //Z2_GAMEEVENT_H
