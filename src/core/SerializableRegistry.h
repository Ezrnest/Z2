/*
 * Created by liyicheng on 2019/7/9.
 */

#ifndef Z2_SERIALIZABLEREGISTRY_H
#define Z2_SERIALIZABLEREGISTRY_H

#include <unordered_map>
#include <functional>
#include "Serializable.h"
using namespace std;
namespace z2{
using DeserializingFunction = function<Serializable *(istream &)>;
class SerializableRegistry {
private:
    unordered_map<string,DeserializingFunction> nameMap;
    SerializableRegistry() = default;
public:
    SerializableRegistry(const SerializableRegistry& serializableRegistry) = delete;

    SerializableRegistry& operator=(const SerializableRegistry& serializableRegistry) = delete;
    /**
     * Register a serializable class to the registry.
     */
    void registerClass(const string &className, const DeserializingFunction &df);

    /**
     * Gets a deserializing function.
     */
    const DeserializingFunction & getDFunction(const string& className);

    Serializable *deserialize(istream &input);

    static SerializableRegistry& instance();

    static void initSerializableClasses();
};

}


#endif //Z2_SERIALIZABLEREGISTRY_H
