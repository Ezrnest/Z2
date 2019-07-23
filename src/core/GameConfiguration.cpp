/*
 * Created by liyicheng on 2019/7/10.
 */

#include "GameConfiguration.h"
#include "EntityRepository.h"
#include "SerializableRegistry.h"
#include "../lib/easylogging++.h"

using namespace z2;

void GameConfiguration::initRegistration() {
    EntityRepository::initDefaultEntities();
    //TODO load from file

    SerializableRegistry::initSerializableClasses();
}

void initLogger() {
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    // Values are always std::string
    defaultConf.set(el::Level::Global,
                    el::ConfigurationType::Format, "[%level] %msg");
    el::Loggers::reconfigureLogger("default", defaultConf);
    el::Loggers::addFlag(el::LoggingFlag::HierarchicalLogging);
    el::Loggers::setLoggingLevel(el::Level::Info);
}


void GameConfiguration::initAll() {
    initLogger();
    initRegistration();
}

void GameConfiguration::disposeAll() {
}

