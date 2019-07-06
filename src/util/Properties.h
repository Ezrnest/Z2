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
using namespace std;

namespace ancono {
    bool isNumber(const string &);

    class Properties {
    public:
        void loadFrom(istream &);

        void loadFromFile(const string &);

        string get(const string &, const string &);

        double getDouble(const string &, const double &);

        int getInt(const string &, const int &);

        void set(const string &, const string &);

        void setDouble(const string &, const double &);

        void setInt(const string &, const int &);

        void saveTo(ostream &output);

        void saveToFile(const string &);

    private:
        map<string, string> m;
    };

}
#endif //Z2_PROPERTIES_H
