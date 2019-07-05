/*
 * Created by liyicheng on 2019/7/5.
 */

#include "ControlMessage.h"


z2::ControlMessage::ControlMessage( z2::ControlMessageType type1) : Message(GeneralMessageType::ControlMessage),
                                                                                                controlType(type1) {}

z2::ControlMessageType z2::ControlMessage::getControlType() const {
    return controlType;
}
