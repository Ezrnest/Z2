/*
 * Created by liyicheng on 2019/7/10.
 */

#ifndef Z2_GAMECONFIGURATION_H
#define Z2_GAMECONFIGURATION_H

#include <util/File.h>
#include <util/Properties.h>
#include "RepositoryTemplate.h"
namespace z2 {
class GameConfiguration : public RepositoryTemplate<GameConfiguration> {
private:
    ancono::Properties prop;
private:
    void initFromFolder(const ancono::File &dir) override;

public:
    const ancono::Properties &getProp() const;

    ancono::Properties& getProp();

    void saveProp();

    const string& getPlayerName();

    static void initAll();

    static void initRegistration();

    static void disposeAll();

    static ancono::File getResourceDir();

    static ancono::File getConfigDir();

    /**
     * Gets the directory for game saves.
     */
    static ancono::File getSaveDir();

    static void initGameConfig();
};

}
#endif //Z2_GAMECONFIGURATION_H
