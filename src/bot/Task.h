/*
 * Created by liyicheng on 2019/8/23.
 */

#ifndef Z2_TASK_H
#define Z2_TASK_H

#include "Bot.h"

namespace z2 {
namespace bot {
enum class TaskState {
    Active,
    Cancelled,
    Done
};

class Task {
private:
    TaskState state = TaskState::Active;
protected:
    Bot* bot;
    virtual void doPerform() = 0;

    virtual void doCheckState();

    void done();
public:

    explicit Task(Bot *bot);

    TaskState checkState();

    void cancel();

    bool isActive();

    bool isDone();

    bool isCancelled();

    TaskState getState() const;

    void perform();

    template <typename T> static
    vector<shared_ptr<T>> performAndFilter(const vector<shared_ptr<T>>& tasks);
};

template<typename T>
vector<shared_ptr<T>> Task::performAndFilter(const vector<shared_ptr<T>> &tasks) {
    static_assert(is_base_of<Task, T>::value, "T should be subclass of Task");
    vector<shared_ptr<T>> re;
    re.reserve(tasks.size());
    for (auto &t : tasks) {
        if(!t){
            continue;
        }
        t->perform();
        if(t->isActive()){
            re.push_back(t);
        }
    }
    return re;
}

class EntityTask : public Task{
protected:
    unsigned int entityId;
public:
    EntityTask(Bot *bot, unsigned int entityId);

public:
    unsigned int getEntityId() const;

    bool entityDied() const;


};



}
}

#endif //Z2_TASK_H
