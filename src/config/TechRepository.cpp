/*
 * Created by liyicheng on 2019/8/7.
 */

#include <plog/Log.h>
#include "TechRepository.h"
using namespace z2;
void z2::TechRepository::initFromFolder(const ancono::File &dir) {
    if(!dir.exists()){
        LOG_WARNING << "Unable to load tech from: " << dir.getPath();
        return;
    }
    auto& repo = instance();
    auto entityFiles = dir.listSubfiles();
    for(File& f : entityFiles){
        ifstream in;
        f.inStream(in);
        while(!in.eof()){
            Properties p;
            p.loadFrom(in);
            repo.addTech(Technology(p));
        }
        in.close();
    }
}

void z2::TechRepository::addTech(const z2::Technology &tech) {
    technologies.insert(make_pair(tech.getId(), tech));
    PLOG_INFO << "[TechRepository] Loaded tech: " << tech.getId();
}

const map<int,Technology> &z2::TechRepository::getTechnologies() const {
    return technologies;
}

const Technology &TechRepository::getTechnology(int id) const {
    return technologies.at(id);
}

vector<int> TechRepository::getResearchableTech(const Player &player) {
    vector<int> v;
    for(auto& en : technologies){
        if(en.second.isResearchable(player)){
            v.push_back(en.first);
        }
    }
    return v;
}

bool TechRepository::containsTech(int techId) const{
    return technologies.find(techId) != technologies.end();
}

