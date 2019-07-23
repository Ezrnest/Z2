/*
 * Created by liyicheng on 2019/7/4.
 */

#include "Message.h"
#include <iostream>

using namespace z2;

z2::Message::Message(GeneralMessageType type) : mType(type) {}

GeneralMessageType z2::Message::getGeneralType() const {
    return mType;
}

void Message::serializeData(ostream &output) {
    output << static_cast<int>(mType) << ' ';
}

void Message::deserializeData(istream &input) {
    int t;
    input >> t;
    mType = static_cast<GeneralMessageType >(t);
}

//void Message::serializeTo(ostream &output) {
//    output << getClassName() << ' ';
//    serializeData(output);
//}


z2::Message::~Message() = default;
