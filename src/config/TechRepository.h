/*
 * Created by liyicheng on 2019/8/7.
 */

#ifndef Z2_TECHREPOSITORY_H
#define Z2_TECHREPOSITORY_H

#include "RepositoryTemplate.h"
#include <map>
#include <world/Technology.h>

namespace z2{
class TechRepository : public RepositoryTemplate<TechRepository> {
private:
    map<int,Technology> technologies;
public:

    void addTech(const Technology& tech);

    void initFromFolder(const ancono::File &dir) override;

    const map<int, Technology>& getTechnologies() const;

    const Technology& getTechnology(int id) const;

    vector<int> getResearchableTech(const Player& player);

    bool containsTech(int techId) const;

};

}


#endif //Z2_TECHREPOSITORY_H
