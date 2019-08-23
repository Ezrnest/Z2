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

class ConstructionBase : public Building {
private:
    /**
     * Determines the relative position of the unit created through this construction base.
     */
    int productionDirection = 0;
protected:


public:
    explicit ConstructionBase(unsigned int objectId);

    void performAbility(const Point &pos, World &world) override;

    bool canPerformAbility(const Point &pos, World &world) override;

    int getProductionDirection() const;

    void buyEntity(const Point &pos,const string& name,World &world);

    const std::string &getClassName() const override;

    static std::string &className();

    void initialize(const Properties &prop) override;

    static ConstructionBase *create(int objectId, const Properties &initializer);



protected:
    static void deserializeDataPart(istream &input, ConstructionBase *en);

    void serializeDataPart(ostream &output) override;

public:

    void serializeTo(ostream &output) override;

    static ConstructionBase* loadFrom(istream& input);
};

}
#endif //Z2_CONSTRUCTIONBASE_H
