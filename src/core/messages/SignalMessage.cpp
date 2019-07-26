//
// Created by liyicheng on 2019/7/25.
//

#include "SignalMessage.h"
#include <iostream>
int z2::SignalMessage::getCode() const {
    return code;
}

z2::SignalMessage::SignalMessage(int code) : ControlMessage(ControlMessageType::Signal), code(code){

}

const string &z2::SignalMessage::getClassName() const {
    static string name = "SignalMessage";
    return name;
}

void z2::SignalMessage::deserializeData(istream &input) {
//    ControlMessage::deserializeData(input);
    input >> code;
}

void z2::SignalMessage::serializeData(ostream &output) {
//    ControlMessage::serializeData(output);
    output << code << ' ';
}

z2::SignalMessage::SignalMessage() : SignalMessage(0) {}
