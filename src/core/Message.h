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

/**
 * <h2>Overview</h2>
 * Message class describes the operations and notifications that should be sent between the client side and
 * server side. An operation of the client will be wrapped to a Message object and it will be sent to the
 * server side, the server will then broadcast the message to all the clients (including the sender) and
 * process the message locally. After a client receives the message, it will process the message locally
 * as well. The consistency of the process of the both side ensures the consistency of the world.
 * <h2>Serializable</h2>
 * To be sent through socket, the `Message` class must be serializable. A concrete subclass of `Message`
 * should override the methods `getClassName`, `serializeData` and `deserializeData`. See class `DataSerializable`
 * for more info. 
 * <h2>Message Types:</h2>
 * Messages are generally divided to three types. 
 * <br> The type ControlMessage represents the messages that are
 * related to the game flow. This kind of messages is usually created and sent by the server. 
 * <br> The type GameMessage represents the operations of a player. They are usually created by the player and 
 * broadcast by the server.
 * <br> The type ChatMessage represents the chat of a player. They are usually created by the player and 
 * broadcast by the server.
 * 
 */
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
