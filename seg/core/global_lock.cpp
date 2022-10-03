#include "seg/core/global_lock.h"

#include <mutex>

namespace seg {
std::mutex GlobalLock::global_lock;  // explicit of static global_lock
}  // namespace seg