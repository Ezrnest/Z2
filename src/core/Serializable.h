/*
 * Created by liyicheng on 2019/7/9.
 */

#ifndef Z2_SERIALIZABLE_H
#define Z2_SERIALIZABLE_H

#include <ostream>

using namespace std;
namespace z2 {
/**
 * <h2>Serializing</h2>
 * When serializing, the serializable object should firstly save its class name.
 * <h2>Deserializing</h2>
 * The general deserializing
 */
class Serializable {
public:
    /**
     * Serialize `this` to the output stream.
     * @param output
     */
    virtual void serializeTo(ostream &output) = 0;

    /**
     * The method that will create a new object of this type. There is no leading class name.
     * <br> <b>This method is only an example!</b>
     * <br><b>Subclasses of this interface should define a static method the same as this method.</b>
     */
    static Serializable* loadFrom(istream& input);
};

}


#endif //Z2_SERIALIZABLE_H