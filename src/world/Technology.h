/*
 * Created by liyicheng on 2019/8/7.
 */

#ifndef Z2_TECHNOLOGY_H
#define Z2_TECHNOLOGY_H

#include <set>
#include <util/Properties.h>
#include "Player.h"

using namespace std;
using namespace ancono;
namespace z2 {
class Technology {
private:
    int id;
    set<int> requires;
    Properties prop;
    int price;
public:
    Technology(int id, set<int> requires, int price, Properties prop);

    explicit Technology(const Properties &prop);

    int getId() const;

    const set<int> &getRequires() const;

    int getPrice() const;

    const Properties &getProp() const;
    /**
     * Determines whether the tech can be researched by the player, regardless of the money.
     */
    bool isResearchable(const Player &p) const;

    /**
     * Determines whether the player has researched this technology.
     */
    bool isAvailable(const Player &p);
};

}


#endif //Z2_TECHNOLOGY_H
