/*
 * Created by liyicheng on 2019/8/23.
 */

#include "Task.h"
namespace z2{
namespace bot{
void Task::cancel() {
    if (isActive()) {
        state = TaskState::Cancelled;
    }
}

bool Task::isActive() {
    return state == TaskState::Active;
}

bool Task::isDone() {
    return state == TaskState::Done;
}

bool Task::isCancelled() {
    return state == TaskState::Cancelled;
}

TaskState Task::getState() const {
    return state;
}

Task::Task(Bot *bot) : bot(bot) {}

void Task::perform() {
    if (isActive()) {
        doPerform();
    }
}

void Task::done() {
    if (isActive()) {
        state = TaskState::Done;
    }
}

TaskState Task::checkState() {
    doCheckState();
    return getState();
}

void Task::doCheckState() {
}


unsigned int EntityTask::getEntityId() const {
    return entityId;
}

EntityTask::EntityTask(Bot *bot, unsigned int entityId) : Task(bot), entityId(entityId) {}

bool EntityTask::entityDied() const {
    return !bool(bot->world->getEntity(entityId));
}

}
}

