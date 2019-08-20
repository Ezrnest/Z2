/*
 * Created by liyicheng on 2019/8/20.
 */

#include <plog/Log.h>
#include "TextRepository.h"
#include "GameConfiguration.h"

void z2::TextRepository::loadFromFile(const ancono::File &f) {
    ancono::Properties prop;
    prop.loadFromFile(f.getPath());
    for (auto &en : prop.getBackingMap()) {
        textMap.insert(en);
    }
}


void z2::TextRepository::initFromFolder(const ancono::File &dir) {
    auto& config = GameConfiguration::instance();
    auto& lang = config.getLanguage();
    stringstream ss;
    ss << "Text_" << lang << ".ini";
    ancono::File target = dir.subFile(ss.str());
    if (!target.exists()) {
        PLOG_WARNING << "Failed to load text from " << target.getFileName();
    }
    loadFromFile(target);
}

const string &z2::TextRepository::getText(const string &key) {
    return getText(key,key);
}

bool z2::TextRepository::containsText(const string &key) {
    return textMap.find(key) != textMap.end();
}

const string &z2::TextRepository::getText(const string &key, const string &defaultValue) {
    auto it = textMap.find(key);
    if (it == textMap.end()) {
        return defaultValue;
    }
    return it->second;
}


