/*
 * Created by liyicheng on 2019/7/4.
 */

#ifndef Z2_MESSAGE_H
#define Z2_MESSAGE_H

#include <memory>

namespace z2 {

enum class GeneralMessageType {
    ControlMessage,
    GameMessage,
    ChatMessage,
};

class Message {
private:
    const GeneralMessageType mType;
public:
    explicit Message(GeneralMessageType type);

    virtual ~Message();

    GeneralMessageType getGeneralType() const;
};

using MessagePtr = std::shared_ptr<Message>;

}
#endif //Z2_MESSAGE_H
