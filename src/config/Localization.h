//
// Created by liyicheng on 2019/7/27.
//

#ifndef Z2_LOCALIZATION_H
#define Z2_LOCALIZATION_H

#include "util/File.h"
namespace z2 {

class Localization {

public:
    static void loadFromDirectory(const ancono::File& dir);
};

}


#endif //Z2_LOCALIZATION_H
