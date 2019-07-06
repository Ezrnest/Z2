/*
 * Created by liyicheng on 2019/7/3.
 */

#ifndef Z2_EVENTDISPATCHER_H
#define Z2_EVENTDISPATCHER_H

class GameEvent;


#include <memory>
#include <vector>
#include <functional>
#include "GameEvent.h"
namespace z2 {

using namespace std;

template <typename T>
class EventDispatcher {
private:
    using EventListener = function<void(T)>;
    vector<EventListener> listeners;

public:

    void addListener(const EventListener &listener);


    void dispatchEvent(shared_ptr<GameEvent> &event);


};

template<typename T>
void EventDispatcher<T>::addListener(const EventDispatcher::EventListener &listener) {
    listeners.push_back(listener);
}

template<typename T>
void EventDispatcher<T>::dispatchEvent(shared_ptr<GameEvent> &event) {
    for(auto& listener : listeners){
        listener(event);
    }
}

}


#endif //Z2_EVENTDISPATCHER_H
