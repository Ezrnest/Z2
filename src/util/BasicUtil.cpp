//
// Created by liyicheng on 2019/7/2.
//
#include <string>
#include <stdexcept>
#include <c++/chrono>
#include "BasicUtil.h"

namespace ancono{
void require(bool expr, const std::string &message) {
    if (!expr) {
        throw std::invalid_argument(message);
    }
}

void require(bool expr) {
    if (!expr) {
        throw std::invalid_argument("Failed requirement.");
    }
}

void require(bool expr, const std::function<std::string()> &f) {
    if (!expr) {
        throw std::invalid_argument(f());
    }
}



long long currentTimeMillis() {
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}
}