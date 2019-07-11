/*
 * Created by liyicheng on 2019/7/5.
 */

#include "ControlMessage.h"
#include <iostream>

z2::ControlMessage::ControlMessage(z2::ControlMessageType type1) : Message(GeneralMessageType::ControlMessage),
                                                                   controlType(type1) {}

z2::ControlMessageType z2::ControlMessage::getControlType() const {
    return controlType;
}

void z2::ControlMessage::serializeData(ostream &output) {
//    Message::serializeData(output);
    output << static_cast<int>(controlType) << ' ';
}

void z2::ControlMessage::deserializeData(istream &input) {
//    Message::deserializeData(input);
    int t;
    input >> t;
    controlType = static_cast<ControlMessageType>(t);
}

const string &z2::ControlMessage::getClassName()const {
    static string name = "ControlMessage";
    return name;
}

z2::ControlMessage::ControlMessage() : ControlMessage(ControlMessageType::StartGame){

}
