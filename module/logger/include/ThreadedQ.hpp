#include <atomic>
#include <condition_variable>
#include <deque>
#include <mutex>

#ifndef _THREADEDQ_HPP_
#define _THREADEDQ_HPP_

class QError : public std::exception
{
public:
    QError(const std::string &s): msg(s) {}
    ~QError() {}
    const std::string &getMsg() const { return msg; }

private:
    std::string msg;
};

template <typename T>
class ThreadedQ
{
public:
    ThreadedQ() = default;
    ThreadedQ(const size_t size): q(size) {}
    ThreadedQ(const ThreadedQ<T> &) = delete;
    virtual ~ThreadedQ() { this->clear(); }

    const T &front()
    {
        std::scoped_lock lock(q_mut);
        if (q.size() == 0) { throw QError("queue is empty"); }
        return q.front();
    }
    const T &back()
    {
        std::scoped_lock lock(q_mut);
        if (q.size() == 0) { throw QError("queue is empty"); }
        return q.back();
    }
    bool empty()
    {
        std::scoped_lock lock(q_mut);
        return q.empty();
    }
    size_t size()
    {
        std::scoped_lock lock(q_mut);
        return q.size();
    }
    void clear()
    {
        std::scoped_lock lock(q_mut);
        q.clear();
    }
    T pop_front()
    {
        std::scoped_lock lock(q_mut);
        if (q.size() == 0) { throw QError("queue is empty"); }
        T t = std::move(q.front());
        q.pop_front();
        return t;
    }
    bool pop_front(T &t)
    {
        std::scoped_lock lock(q_mut);
        if (q.empty()) { return false; }
        t = std::move(q.front());
        q.pop_front();
        return true;
    }

    T pop_back()
    {
        std::scoped_lock lock(q_mut);
        if (q.size() == 0) { throw QError("queue is empty"); }
        T t = std::move(q.front());
        q.pop_back();
        return t;
    }
    void push_back(const T &i)
    {
        std::scoped_lock lock(q_mut);
        q.emplace_back(std::move(i));

        std::unique_lock<std::mutex> ul(mutBlocking);
        vBlocking.notify_one();
    }
    void push_front(const T &i)
    {
        std::scoped_lock lock(q_mut);
        q.emplace_front(std::move(i));

        std::unique_lock<std::mutex> ul(mutBlocking);
        vBlocking.notify_one();
    }

    void wait()
    {
        while (this->bWait && this->empty()) {
            std::unique_lock<std::mutex> ul(mutBlocking);
            vBlocking.wait(ul);
        }
    }

    void notify()
    {
        std::unique_lock<std::mutex> ul(mutBlocking);
        vBlocking.notify_all();
    }

    void setWaitMode(bool mode)
    {
        bWait = mode;
        this->notify();
    }

private:
    std::mutex q_mut;
    std::deque<T> q;

    std::atomic_bool bWait = true;
    std::mutex mutBlocking;
    std::condition_variable vBlocking;
};

#endif    //_THREADEDQ_HPP_
