//
// Created by liyicheng on 2019/7/27.
//

#ifndef Z2_MAPREPOSITORY_H
#define Z2_MAPREPOSITORY_H


namespace z2 {

class MapRepository {


public:

    static MapRepository& instance();

    static void loadFromDirectory();
};

}


#endif //Z2_MAPREPOSITORY_H
