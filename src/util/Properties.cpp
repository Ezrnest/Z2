/*
 * Created by liyicheng on 2019/7/5.
 * Completed by chenzc18 on 2019/7/10.
 */

#include "Properties.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>

namespace ancono {

void Properties::loadFrom(istream &input) {
    int pos = 0;
    string tmp;
    while (!input.eof()) {
        getline(input, tmp);
        if (tmp.empty()) {
            continue;
        }
        if (tmp[0] == '#') {
            continue;
        }
        if (tmp[0] == '$') {
            break;
        }
        int l = tmp.length();
        bool valid = false;
        for (int i = 0; i < l; i++) {
            if (tmp[i] == '=') {
                pos = i;
                valid = true;
                break;
            }
        }
        if (valid) {
            string tmpKey = tmp.substr(0, pos);
            string tmpValue = tmp.substr(pos + 1);
            m[tmpKey] = tmpValue;
        }
    }
}

void Properties::loadFromFile(const string &path) {
    const char *file = path.c_str();
    ifstream fin(file);
    this->loadFrom(fin);
    fin.close();
}

const string &Properties::get(const string &key, const string &defaultValue) const {
    auto iter = m.find(key);
    if (iter == m.end()) {
        return defaultValue;
    } else {
        return iter->second;
    }
}

double Properties::getDouble(const string &key, const double &defaultValue) const {
    auto iter = m.find(key);
    if (iter == m.end()) {
        return defaultValue;
    }
    const string &k = iter->second;
    return parseDouble(k, defaultValue);
}

int Properties::getInt(const string &key, const int &defaultValue) const {
    auto iter = m.find(key);
    if (iter == m.end()) {
        return defaultValue;
    }
    const string &k = iter->second;
    return parseInt(k, defaultValue);
}

void Properties::set(const string &key, const string &value) {
    m[key] = value;
}

void Properties::setDouble(const string &key, const double &value) {
    stringstream ss;
    ss << value;
    m[key] = ss.str();
}

void Properties::setInt(const string &key, const int &value) {
    stringstream ss;
    ss << value;
    m[key] = ss.str();
}

void Properties::saveTo(ostream &output) const {
    for (const auto &iter : m) {
        output << iter.first << "=" << iter.second << endl;
    }
}

void Properties::saveToFile(const string &path) const {
    const char *file = path.c_str();
    ofstream fout(file);
    this->saveTo(fout);
    fout.close();
}

set<string> Properties::getStrSet(const string &key) const{
    std::set<string> s;
    auto iter = m.find(key);
    if (iter == m.end()) {
        return s;
    }
    const string &val = iter->second;
    stringstream ss(val);
    while (ss.good() && !ss.eof()) {
        string t;
        ss >> t;
        if(!t.empty()){
            s.insert(t);
        }
    }
    return s;
}


set<int> Properties::getIntSet(const string &key) const {
    std::set<int> s;
    auto iter = m.find(key);
    if (iter == m.end()) {
        return s;
    }
    const string &val = iter->second;
    stringstream ss(val);
    while (ss.good()) {
        int t;
        ss >> t;
        s.insert(t);
    }
    return s;
}

bool Properties::getBool(const string &key, bool defaultValue) const {
    auto iter = m.find(key);
    if (iter == m.end()) {
        return defaultValue;
    }
    const string &k = iter->second;
    return parseBool(k, defaultValue);
}

const map<string, string> &Properties::getBackingMap() const {
    return m;
}



int parseInt(const string &str, int defaultValue) {
    if (str.empty()) {
        return defaultValue;
    }
    stringstream ss(str);
    int t;
    ss >> t;
    if (ss.eof()) {
        return t;
    } else {
        return defaultValue;
    }
}

int parseInt(const string &str, bool *ok) {
    if (str.empty()) {
        if (ok != nullptr) {
            *ok = false;
        }
        return 0;
    }
    stringstream ss(str);
    int t;
    ss >> t;
    if (ss.eof()) {
        if (ok != nullptr) {
            *ok = true;
        }
        return t;
    } else {
        if (ok != nullptr) {
            *ok = false;
        }
        return 0;
    }
}


double parseDouble(const string &str, double defaultValue) {
    if (str.empty()) {
        return false;
    }
    stringstream ss(str);
    double t;
    ss >> t;
    if (ss.eof()) {
        return t;
    } else {
        return defaultValue;
    }
}


double parseDouble(const string &str, bool *ok) {
    if (str.empty()) {
        if (ok != nullptr) {
            *ok = false;
        }
        return 0;
    }
    stringstream ss(str);
    double t;
    ss >> t;
    if (ss.eof()) {
        if (ok != nullptr) {
            *ok = true;
        }
        return t;
    } else {
        if (ok != nullptr) {
            *ok = false;
        }
        return 0;
    }
}

bool equalsIgnoreCase(const char* s1, const char* s2){
    return strcasecmp(s1,s2) == 0;
}

bool parseBool(const string &str, bool defaultValue) {
    if (equalsIgnoreCase(str.c_str(),"true")) {
        return true;
    } else if (equalsIgnoreCase(str.c_str(),"false")) {
        return false;
    }
    return defaultValue;
}

bool parseBool(const string &str, bool *ok) {
    if (str == "true") {
        if (ok != nullptr) {
            *ok = true;
        }
        return true;
    } else if (str == "false") {
        if (ok != nullptr) {
            *ok = true;
        }
        return false;
    }
    if (ok != nullptr) {
        *ok = false;
    }
    return false;
}


}