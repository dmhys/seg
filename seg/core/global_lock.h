#ifndef SEG_CORE_GLOBAL_LOCK_H
#define SEG_CORE_GLOBAL_LOCK_H

#include <mutex>

namespace seg {
/**
 * This class locks destructor of GL Based objects,
 * until SEG thread frees all gl buffers.
 */
class GlobalLock
{
   public:
    static void lock() { global_lock.lock(); }

    static void unlock() { global_lock.unlock(); }

    static void wait()
    {
        global_lock.lock();
        global_lock.unlock();
    }

   private:
    static std::mutex global_lock;

};  // class GlobalLock
}  // namespace seg

#endif