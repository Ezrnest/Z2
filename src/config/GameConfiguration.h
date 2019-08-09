/*
 * Created by liyicheng on 2019/7/10.
 */

#ifndef Z2_GAMECONFIGURATION_H
#define Z2_GAMECONFIGURATION_H

#include <util/File.h>

namespace z2 {
class GameConfiguration {
public:

    static void initAll();

    static void initRegistration();

    static void disposeAll();

    static ancono::File getResourceDir();

};

}
#endif //Z2_GAMECONFIGURATION_H
