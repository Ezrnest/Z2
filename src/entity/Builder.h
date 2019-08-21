/*
 * Created by liyicheng on 2019/8/18.
 */

#ifndef Z2_BUILDER_H
#define Z2_BUILDER_H

#include "GameUnit.h"

namespace z2 {
class Builder : public GameUnit {
private:
    string buildingName;
public:

    explicit Builder(unsigned int objectId);

    void initialize(const Properties &prop) override;

public:
    const string &getClassName() const override;

    static const string& className();

    void serializeTo(ostream &output) override;

    static Builder* loadFrom(istream& input);

    static Entity* create(int objectId, const Properties& initializer);

    void performAbility(const Point &pos, World &world) override;

    bool canPerformAbility(const Point &pos, World &world) override;

protected:
    void serializeDataPart(ostream &output) override;

    static void deserializeDataPart(istream &input, Builder *en);

};
}

#endif //Z2_BUILDER_H
