/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_MESSAGE_H
#define Z2_MESSAGE_H

#include <memory>
#include "Serializable.h"
#include "SerializableRegistry.h"

namespace z2 {

enum class GeneralMessageType {
    ControlMessage,
    GameMessage,
    ChatMessage,
};

class Message : public Serializable {
private:
    GeneralMessageType mType;
public:
    explicit Message(GeneralMessageType type);

    virtual ~Message();

    GeneralMessageType getGeneralType() const;

//    template<typename Clazz>
//    static Clazz *deserializeT(istream &input);

    void serializeTo(ostream &output) override;

protected:
    virtual void serializeData(ostream &output);
public:
    virtual void deserializeData(istream &input);
};

using MessagePtr = std::shared_ptr<Message>;

}
#endif //Z2_MESSAGE_H
