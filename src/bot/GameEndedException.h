/*
 * Created by liyicheng on 2019/8/26.
 */

#ifndef Z2_GAMEENDEDEXCEPTION_H
#define Z2_GAMEENDEDEXCEPTION_H

#include <c++/stdexcept>
using namespace std;
namespace z2{
class GameEndedException : public runtime_error {
public:
    explicit GameEndedException(const string &arg);

    explicit GameEndedException(const char *string);
};

}


#endif //Z2_GAMEENDEDEXCEPTION_H
