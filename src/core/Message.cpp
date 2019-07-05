/*
 * Created by liyicheng on 2019/7/4.
 */

#include "Message.h"
using namespace z2;
z2::Message::Message(GeneralMessageType type) : mType(type) {}

GeneralMessageType z2::Message::getGeneralType() const {
    return mType;
}

z2::Message::~Message() = default;
