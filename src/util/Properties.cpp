/*
 * Created by liyicheng on 2019/7/5.
 */

#include "Properties.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <map>

namespace ancono {
    bool isNumber(const string &key) {
        stringstream ss(key);
        double t = 0;
        char p;
        if (!(ss >> t)) {
            return false;
        }
        if (ss >> p) {
            return false;
        } else {
            return true;
        }
    }

    void Properties::loadFrom(istream &input) {
        int pos = 0;
        string tmp;
        while (!input.eof()) {
            getline(input, tmp);
            if (tmp[0] != '#') {
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
        return;
    }

    void Properties::loadFromFile(const string &path) {
        const char * file = path.c_str();
        ifstream fin(file);
        this->loadFrom(fin);
        fin.close();
        return;
    }

    const string & Properties::get(const string &key, const string &defaultValue) const{
        auto iter = m.find(key);
        if (iter == m.end()) {
            return defaultValue;
        } else {
            return iter->second;
        }
    }

    double Properties::getDouble(const string &key, const double &defaultValue) const{
        auto iter = m.find(key);
        if(iter == m.end()){
            return defaultValue;
        }
        const string & k = iter->second;
        if (!isNumber(k)) {
            return defaultValue;
        } else {
            stringstream ss(k);
            double a = 0.0;
            ss >> a;
            return a;
        }
    }

    int Properties::getInt(const string &key, const int & defaultValue) const {
        auto iter = m.find(key);
        if(iter == m.end()){
            return defaultValue;
        }
        const string & k = iter->second;
        if (!isNumber(k)) {
            return defaultValue;
        } else {
            stringstream ss(k);
            int a = 0;
            ss >> a;
            return a;
        }
    }

    void Properties::set(const string &key, const string &value) {
        m[key] = value;
    }

    void Properties::setDouble(const string &key, const double & value) {
        stringstream ss("");
        ss << value;
        string v;
        ss >> v;
        m[key] = v;
        return;
    }

    void Properties::setInt(const string & key, const int & value) {
        setDouble(key, value); // does not involve conversion from double to int
        return;
    }

    void Properties::saveTo(ostream & output) const{
        for (const auto & iter : m) {
            output << iter.first << "=" << iter.second << endl;
        }
    }

    void Properties::saveToFile(const string &path) const{
        const char *file = path.c_str();
        ofstream fout(file);
        this->saveTo(fout);
        fout.close();
    }
}