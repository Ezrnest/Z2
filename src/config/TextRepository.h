/*
 * Created by liyicheng on 2019/8/20.
 */

#ifndef Z2_TEXTREPOSITORY_H
#define Z2_TEXTREPOSITORY_H

#include "RepositoryTemplate.h"
#include <map>
namespace z2 {
class TextRepository : public RepositoryTemplate<TextRepository> {
private:
    map<string,string> textMap;

    void loadFromFile(const ancono::File &f);
public:

    const string& getText(const string& key, const string& defaultValue);

    const string& getText(const string& key);

    bool containsText(const string &key);

    void initFromFolder(const ancono::File &dir) override;
};
}

#endif //Z2_TEXTREPOSITORY_H
