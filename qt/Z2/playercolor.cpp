#include "playercolor.h"
namespace z2 {
PlayerColor::PlayerColor()
{
    colorNames.append("红色");
    colors.push_back(QColor(Qt::red));

    colorNames.append("蓝色");
    colors.push_back(QColor(Qt::blue));

    colorNames.append("黄色");
    colors.push_back(QColor(Qt::yellow));

    colorNames.append("绿色");
    colors.push_back(QColor(Qt::green));

    colorNames.append("紫色");
    colors.push_back(QColor(Qt::magenta));

    colorNames.append("淡蓝色");
    colors.push_back(QColor(Qt::cyan));

    colorNames.append("橙色");
    colors.push_back(QColor(255,165,0));

    colorNames.append("淡绿色");
    colors.push_back(QColor(154,255,154));
}

PlayerColor &PlayerColor::instance(){
    static PlayerColor pc;
    return pc;
}

const QStringList &PlayerColor::getColorNames(){
    return instance().colorNames;
}

const vector<QColor> &PlayerColor::getColors(){
    return instance().colors;
}

const QColor &PlayerColor::getColor(int cc){
    auto& cs = getColors();
    if(cc > -1 && cc < cs.size()){
        return cs[cc];
    }
    return cs[0];
}
}
