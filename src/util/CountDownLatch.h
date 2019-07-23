//
// Created by liyicheng on 2019/7/23.
//

#ifndef Z2_COUNTDOWNLATCH_H
#define Z2_COUNTDOWNLATCH_H
#include <chrono>
#include <condition_variable>
#include <mutex>

namespace z2{
class CountDownLatch {
public:
    explicit CountDownLatch(unsigned int count);
    virtual ~CountDownLatch() = default;

    void await();

    template <class Rep, class Period>
    bool await(const std::chrono::duration<Rep, Period>& timeout);

    void countDown();

    unsigned int getCount();

protected:
    std::mutex m_mutex;
    std::condition_variable m_cv;
    unsigned int m_count = 0;
};
}

template<class Rep, class Period>
bool z2::CountDownLatch::await(const std::chrono::duration<Rep, Period> &timeout) {
    std::unique_lock<std::mutex> lock(m_mutex);
    bool result = true;
    if (m_count > 0) {
        result = m_cv.wait_for(lock, timeout, [this](){ return m_count == 0; });
    }

    return result;
}


#endif //Z2_COUNTDOWNLATCH_H
