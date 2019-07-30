/*
 * Created by liyicheng on 2019/7/3.
 */

#ifndef Z2_EVENTDISPATCHER_H
#define Z2_EVENTDISPATCHER_H




#include <memory>
#include <vector>
#include <functional>
#include "GameEvent.h"
namespace z2 {
class GameEvent;
using namespace std;
using EventListener = function<void(const GameEventPtr&)>;
class EventDispatcher {
private:
    vector<EventListener> listeners;
public:

    void addListener(const EventListener &listener);

    void publish(shared_ptr<GameEvent> &event);


};


}


#endif //Z2_EVENTDISPATCHER_H
