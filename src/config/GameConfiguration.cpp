/*
 * Created by liyicheng on 2019/7/10.
 */

#include <plog/Formatters/ServityFormatter.h>
#include "GameConfiguration.h"
#include "config/EntityRepository.h"
#include "config/SerializableRegistry.h"
#include "plog/Log.h"
#include "plog/Appenders/ConsoleAppender.h"
#include "MapRepository.h"
#include "TechRepository.h"
#include "TextRepository.h"
#include "BotRepository.h"
using namespace z2;

ancono::File getDirNamed(const string &name) {
    File curDir = File::currentDirectory();
    File resDir = curDir.subFile(name);
    if (resDir.exists()) {
        return resDir;
    }
    resDir = curDir.parent().subFile(name);
    if (resDir.exists()) {
        return resDir;
    }
    return curDir.parent().parent().parent().subFile(name);
}

ancono::File GameConfiguration::getResourceDir() {
    return getDirNamed("resources");
}

ancono::File GameConfiguration::getConfigDir() {
    return getDirNamed("conf");
}

ancono::File GameConfiguration::getSaveDir() {
    return getDirNamed("saves");
}


void GameConfiguration::initRegistration() {
    auto resDir = getResourceDir();
    LOG_INFO << "Loading resources from: " << resDir.getPath();

    SerializableRegistry::initSerializableClasses();

    EntityRepository::initEntityClasses();
    EntityRepository::initFromFolder(resDir.subFile("entity"));

    TechRepository::instance().initFromFolder(resDir.subFile("tech"));

    MapRepository::initFromFolder(resDir.subFile("map"));

    TextRepository::instance().initFromFolder(resDir.subFile("text"));

    BotRepository::initBots();
    BotRepository::instance().initFromFolder(resDir.subFile("bot"));
    //TODO load from file
}

void GameConfiguration::initGameConfig() {
    auto confDir = getConfigDir();
    LOG_INFO << "Loading config from: " << confDir.getPath();

    GameConfiguration::instance().initFromFolder(confDir);

}


void initLogger() {
//    el::Configurations defaultConf;
//    defaultConf.setToDefault();
//    // Values are always std::string
//    defaultConf.set(el::Level::Global,
//                    el::ConfigurationType::Format, "[%level] %msg");
//    el::Loggers::reconfigureLogger("default", defaultConf);
//    el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
//    el::Loggers::setLoggingLevel(el::Level::Info);
    static plog::ConsoleAppender<plog::ServityFormatter> consoleAppender;
    plog::init(plog::info, &consoleAppender);
}


void GameConfiguration::initAll() {
    initLogger();
    initGameConfig();
    initRegistration();
}

void GameConfiguration::disposeAll() {
}

void GameConfiguration::initFromFolder(const ancono::File &dir) {
    File conf = dir.subFile("config.ini");
    if (conf.exists()) {
        prop.loadFromFile(conf.getPath());
    } else {
        LOG_WARNING << "Failed to init config";
    }
}

const string &GameConfiguration::getPlayerName() {
    const static string pn = "player";
    const string &name = prop.get("playerName", pn);
    if (name.find_first_of(' ') == std::__cxx11::string::npos) {
        return name;
    }
    string n = name.substr(0, name.find_first_of(' '));
    prop.set("playerName", n);
    return prop.get("playerName", n);
}

const Properties &GameConfiguration::getProp() const {
    return prop;
}

ancono::Properties &GameConfiguration::getProp() {
    return prop;
}

void GameConfiguration::saveProp() {
    File dir = getConfigDir().subFile("config.ini");
    if (dir.exists()) {
        prop.saveToFile(dir.getPath());
    } else {
        LOG_WARNING << "Failed to save config!";
    }

}

const string &GameConfiguration::getLanguage() {
    static string lang = "cn";
    return getProp().get("language",lang);
}

int GameConfiguration::getBotOperationInterval() {
    return getProp().getInt("botOperationInterval", 100);
}






