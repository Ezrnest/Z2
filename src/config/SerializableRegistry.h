/*
 * Created by liyicheng on 2019/7/9.
 */

#ifndef Z2_SERIALIZABLEREGISTRY_H
#define Z2_SERIALIZABLEREGISTRY_H

#include <unordered_map>
#include <functional>
#include "core/Serializable.h"

using namespace std;
namespace z2 {
using DeserializingFunction = function<Serializable *(istream &)>;

class SerializableRegistry {
private:
    unordered_map<string, DeserializingFunction> nameMap;

    SerializableRegistry() = default;

public:
    SerializableRegistry(const SerializableRegistry &serializableRegistry) = delete;

    SerializableRegistry &operator=(const SerializableRegistry &serializableRegistry) = delete;

    /**
     * Register a serializable class to the registry.
     */
    void registerClass(const string &className, const DeserializingFunction &df);

    /**
     * Register a class. It is required that two static methods `const string& className()` and
     * `Clazz* loadFrom(istream& input) ` are defined.
     */
    template<typename Clazz>
    void registerClass();

    /**
     * Gets a deserializing function.
     */
    const DeserializingFunction &getDFunction(const string &className);

    Serializable *deserialize(istream &input);

    static SerializableRegistry &instance();

    static void initSerializableClasses();

    template<typename Clazz>
    static
    Clazz *deserializeT(istream &input);


};

}


#endif //Z2_SERIALIZABLEREGISTRY_H
