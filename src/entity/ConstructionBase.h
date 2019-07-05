/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_CONSTRUCTIONBASE_H
#define Z2_CONSTRUCTIONBASE_H

#include "Building.h"
#include "../util/Properties.h"
#include <map>
using namespace std;
namespace z2 {

class ConstructionBase : public Building{


public:
    explicit ConstructionBase(int objectId);


    const std::string &identifier() const override;

    static std::string& getIdentifier();

    void initialize(const Properties &prop) override;

public:

    static ConstructionBase* create(int objectId, const Properties& initializer);
};

}
#endif //Z2_CONSTRUCTIONBASE_H
