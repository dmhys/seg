#ifndef SEG_SEG_H
#define SEG_SEG_H

#define SEG_VERSION_MAJOR 0
#define SEG_VERSION_MINOR 1

#include <memory>
#include <string>

#include "seg/options.h"
#include "seg/types.h"

namespace seg {
namespace object {
class ObjectBase;
}  // namespace object

/**
 * @brief Initializes SEG Core.
 *        Note - SEG uses its own thred to render.
 *        Calling this Function will run SEG thread imediately.
 */
void initialize(const std::string& window_name = "",
                const WindowSize& window_size = WindowSize(960, 480), Options options = Options());

/**
 * @brief Adds object to SEG.
 *        This Objects will be managed inside SEG.
 * @param name name to be displayed. If not given, SEG will generate one for you.
 * @param object Raw or shared ptr of class derived from ObjectBase.
 *               Shared_ptr is recommended when you want to access it thereafter.
 */
void addObject(const std::string& name, const std::shared_ptr<object::ObjectBase>& object);

/** @overload
 */
void addObject(const std::string& name, object::ObjectBase* object);

/** @overload
 *  @return name made by seg.
 */
std::string addObject(const std::shared_ptr<object::ObjectBase>& object);

/** @overload
 *  @return name made by seg.
 */
std::string addObject(object::ObjectBase* object);

/**
 * @brief Deletes object with name.
 * @return True if deleted, false otherwise. (No object with given name)
 */
bool deleteObject(const std::string& name);

}  // namespace seg

#endif