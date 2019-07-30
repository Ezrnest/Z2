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
using namespace z2;

void GameConfiguration::initRegistration() {
    File curDir = File::currentDirectory();
    File resDir = curDir.parent().subFile("resources");
    LOG_INFO << "Loading resources from: " << resDir.getPath();

    SerializableRegistry::initSerializableClasses();

    EntityRepository::initEntityClasses();
    EntityRepository::initFromFolder(resDir.subFile("entity"));

    MapRepository::initFromFolder(resDir.subFile("map"));



    //TODO load from file
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
    initRegistration();
}

void GameConfiguration::disposeAll() {
}

