//
// Created by liyicheng on 2019/7/2.
//
#include <string>
#include <stdexcept>
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

}

