/*
 * Created by liyicheng on 2019/7/5.
 */

#ifndef Z2_PROPERTIES_H
#define Z2_PROPERTIES_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <c++/set>

using namespace std;

namespace ancono {
//bool isNumber(const string &);

int parseInt(const string& str, int defaultValue = 0);

int parseInt(const string &str, bool *ok = nullptr);

double parseDouble(const string& str, double defaultValue = 0.0);

double parseDouble(const string &str, bool *ok = nullptr);

bool parseBool(const string& str, bool defaultValue = false);

bool parseBool(const string &str, bool *ok = nullptr);

class Properties {
public:
    void loadFrom(istream &);

    void loadFromFile(const string &);

    const string &get(const string &, const string &) const;

    double getDouble(const string &, const double &) const;

    int getInt(const string &, const int &) const;

    bool getBool(const string&, bool defaultValue = false) const;

    set<int> getIntSet(const string &) const;

    void set(const string &, const string &);

    void setDouble(const string &, const double &);

    void setInt(const string &, const int &);

    void saveTo(ostream &output) const;

    void saveToFile(const string &) const;

private:
    map<string, string> m;
};

}
#endif //Z2_PROPERTIES_H
