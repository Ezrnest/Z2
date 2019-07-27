/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_MESSAGE_H
#define Z2_MESSAGE_H

#include <memory>
#include "Serializable.h"
#include "config/SerializableRegistry.h"

namespace z2 {

enum class GeneralMessageType {
    ControlMessage,
    GameMessage,
    ChatMessage,
};

class Message : public DataSerializable {
private:
    GeneralMessageType mType;
public:
    explicit Message(GeneralMessageType type);

    virtual ~Message();

    GeneralMessageType getGeneralType() const;

//    void serializeTo(ostream &output) override;

protected:
    void serializeData(ostream &output) override;
public:
    void deserializeData(istream &input) override;


};

using MessagePtr = std::shared_ptr<Message>;

}
#endif //Z2_MESSAGE_H
