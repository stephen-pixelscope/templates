#pragma once
#include <atomic>
#include <shared_mutex>

namespace kangsw {
/**
 * ���μ����� ������ �ٱ����� ������ ���� ����.
 * ��Ƽ������ ȯ�濡��, Ŭ���� ��� ���� �Ʒ��ʿ� ��ġ�Ͽ� �Ҹ� ������ ������ �� �ֽ��ϴ�.
 * ��ø�ؼ� ��� �� �ִ� ������ �淮 ���ؽ��̸�, �ݵ�� ��� �� �����Ǿ�� �մϴ�.
 */
class destruction_guard {
public:
    void lock() { lock_.fetch_add(1); }
    bool try_lock() { return lock_.fetch_add(1), true; }
    void unlock() { lock_.fetch_sub(1); }
    bool is_locked() const { return lock_.load(); }

    ~destruction_guard()
    {
        for (; lock_.load(); std::this_thread::sleep_for(poll_interval)) {}
    }

public:
    std::chrono::system_clock::duration poll_interval = std::chrono::milliseconds(1);

private:
    mutable std::atomic_int64_t lock_;
};

} // namespace kangsw