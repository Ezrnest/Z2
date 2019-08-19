/*
 * Created by liyicheng on 2019/8/7.
 */

#include "Technology.h"

#include <utility>

#include "config/GameConfiguration.h"

const Properties &z2::Technology::getProp() const {
    return prop;
}


bool z2::Technology::isResearchable(const z2::Player &p) const{
    auto& ts = p.getTechnologies();
    if (ts.find(id) != ts.end()) { // contains
        return false;
    }
    for(auto& t : requires){
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
    id = prop.get("id", "Null");
    requires = prop.getStrSet("requiredTech");
    price = prop.getInt("price", 0);
}

int z2::Technology::getPrice() const {
    return price;
}

const string &z2::Technology::getId() const {
    return id;
}

z2::Technology::Technology(string id, set<string> requires, int price, Properties prop) : id(std::move(id)),
                                                                                                               requires(std::move(
                                                                                                                       requires)),
                                                                                                               prop(std::move(prop)),
                                                                                                               price(price) {}


const set<string> &z2::Technology::getRequires() const {
    return requires;
}

const string &z2::Technology::getDisplayName() const {
    auto& config = GameConfiguration::instance();
    const string& s =  prop.get("displayName", id);
    if(config.getLanguage() == "en"){
        return prop.get("displayNameEn",s);
    }
    return s;
}

