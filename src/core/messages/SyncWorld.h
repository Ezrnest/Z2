//
// Created by liyicheng on 2019/7/22.
//

#ifndef Z2_SYNCWORLD_H
#define Z2_SYNCWORLD_H

#include "ControlMessage.h"

namespace z2 {
class World;
class SyncWorld : public ControlMessage {
private:
    shared_ptr<World> world;
public:
    SyncWorld();

    explicit SyncWorld(shared_ptr<World> world);

    const shared_ptr<World> &getWorld() const;

    const string &getClassName() const override;

    void deserializeData(istream &input) override;

protected:
    void serializeData(ostream &output) override;
};
}

#endif //Z2_SYNCWORLD_H
