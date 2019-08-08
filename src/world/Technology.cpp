/*
 * Created by liyicheng on 2019/8/7.
 */

#include "Technology.h"

#include <utility>

int z2::Technology::getId() const {
    return id;
}

const set<int> &z2::Technology::getRequires() const {
    return requires;
}

const Properties &z2::Technology::getProp() const {
    return prop;
}

z2::Technology::Technology(int id, set<int> requires, int price, Properties prop) : id(id), requires(std::move(requires)),
                                                                                    prop(std::move(prop)),
                                                                                    price(price) {}

bool z2::Technology::isResearchable(const z2::Player &p) const{
    auto& ts = p.getTechnologies();
    if (ts.find(id) != ts.end()) { // contains
        return false;
    }
    for(int t : requires){
        if(ts.find(t) == ts.end()){
            return false;
        }
    }
    return true;
}

bool z2::Technology::isAvailable(const z2::Player &p) {
    auto& ts = p.getTechnologies();
    return ts.find(id) != ts.end(); // contains
}

z2::Technology::Technology(const Properties &prop) : prop(prop){
    id = prop.getInt("id", -1);
    requires = prop.getIntSet("requiredTech");
    price = prop.getInt("price", 0);
}

int z2::Technology::getPrice() const {
    return price;
}
