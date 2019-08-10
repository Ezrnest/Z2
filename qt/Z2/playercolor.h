#ifndef PLAYERCOLOR_H
#define PLAYERCOLOR_H
#include <QStringList>
#include <vector>
#include <QColor>
using namespace std;
namespace z2 {

class PlayerColor{

private:
    QStringList colorNames;
    vector<QColor> colors;

    PlayerColor();

    static PlayerColor& instance();

public:
    static const QStringList &getColorNames();

    static const vector<QColor>& getColors();

    static const QColor& getColor(int cc);


};

}

#endif // PLAYERCOLOR_H
