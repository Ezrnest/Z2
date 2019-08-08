/*
 * Created by liyicheng on 2019/7/9.
 */

#include "Serializable.h"
#include <stdexcept>
z2::Serializable *z2::Serializable::loadFrom(istream &input) {
    throw std::logic_error("Not implemented!");
}


void z2::DataSerializable::serializeTo(ostream &output) {
    output << getClassName() << ' ';
    serializeData(output);
}
