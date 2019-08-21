//
// Created by liyicheng on 2019/7/25.
//

#ifndef Z2_SIGNALMESSAGE_H
#define Z2_SIGNALMESSAGE_H

#include "ControlMessage.h"

namespace z2 {
class SignalMessage : public ControlMessage{
private:
    int code;
public:
    explicit SignalMessage(int code);

    SignalMessage();

    int getCode() const;

    const string &getClassName() const override;

    void deserializeData(istream &input) override;

    enum {GOOD = 0, BAD};

protected:
    void serializeData(ostream &output) override;
};
}

#endif //Z2_SIGNALMESSAGE_H
