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
    string id;
    set<string> requires;
    Properties prop;
    int price;
public:

    Technology(string id, set<string> requires, int price, Properties prop);

    explicit Technology(const Properties &prop);

    const string &getId() const;

    const set<string> &getRequires() const;

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

    const string& getDisplayName() const;
};

}


#endif //Z2_TECHNOLOGY_H
