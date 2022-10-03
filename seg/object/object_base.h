#ifndef SEG_OBJECT_OBJECT_BASE_H
#define SEG_OBJECT_OBJECT_BASE_H

#include <memory>
#include <string>

// default dtor of ObjectBase class need to know size of general inspector,
// therefore forbiding forward declation of GeneralInspector class.
#include "seg/ui/general_inspector.h"

namespace seg {
namespace object {
enum ObjectLayer
{
    GL,
    UI,
};

class ObjectBase
{
   public:
    ObjectBase() {}
    virtual ~ObjectBase() {}
    void draw()
    {
        if (is_visible) drawImpl();
    }
    virtual ObjectLayer getObjectLayer() = 0;
    virtual const std::string getType() const = 0;

    bool is_visible = true;
    std::unique_ptr<ui::GeneralInspector> inspector;

   private:
    // Uncopyable
    ObjectBase(const ObjectBase&) = delete;
    ObjectBase& operator=(const ObjectBase&) = delete;

    virtual void drawImpl() = 0;

};  // class ObjectBase
}  // namespace object
}  // namespace seg

#endif