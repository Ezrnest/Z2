/*
 * Created by liyicheng on 2019/7/3.
 */

#ifndef Z2_ENTITYWITHHEALTH_H
#define Z2_ENTITYWITHHEALTH_H

#include "Entity.h"

namespace z2 {
class EntityWithHealth : public Entity{
protected:
    int health;
    int maxHealth;


public:

    explicit EntityWithHealth(unsigned int objectId);



    ~EntityWithHealth() override;

    int getHealth() const;

    void setHealth(int health);

    int getMaxHealth() const;

    void setMaxHealth(int maxHealth);

    bool isDead();

    /**
     * Deals the given amount of damage to this unit.
     * @return `true` if this unit is dead after the damage is dealt
     */
    bool dealDamage(int damage);

    void initialize(const Properties &prop) override;

protected:
    void serializeDataPart(ostream &output) override;

    static void deserializeDataPart(istream &input, EntityWithHealth *en);
};

}
#endif //Z2_ENTITYWITHHEALTH_H
