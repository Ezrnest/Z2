//
// Created by liyicheng on 2019/7/23.
//

#include "CountDownLatch.h"

z2::CountDownLatch::CountDownLatch(const unsigned int count) : m_count(count) {
}

void z2::CountDownLatch::await() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_count > 0) {
//        m_cv.wa
        m_cv.wait(lock, [this](){ return m_count == 0; });
    }
}



void z2::CountDownLatch::countDown() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_count > 0) {
        m_count--;
        m_cv.notify_all();
    }
}

unsigned int z2::CountDownLatch::getCount() {
    std::unique_lock<std::mutex> lock(m_mutex);
    return m_count;
}
