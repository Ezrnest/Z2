/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_BUILDING_H
#define Z2_BUILDING_H

#include "EntityWithHealth.h"

namespace z2 {
class Building : public EntityWithHealth{
protected:
    static void deserializeDataPart(istream &input, Building *en);
public:
    explicit Building(int objectId);
};

}
#endif //Z2_BUILDING_H
