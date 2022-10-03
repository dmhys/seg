#ifndef SEG_OBJECT_OBJECT_MANAGER_H
#define SEG_OBJECT_OBJECT_MANAGER_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace seg {
namespace gl {
class Scene;
};
namespace ui {
class ObjectListWindow;
};

namespace object {
class ObjectBase;

class ObjectManager
{
   public:
    void onCoreShutdown();

    void setScene(gl::Scene* _scene) { scene = _scene; }

    std::string addObject(ObjectBase * obj);
    std::string addObject(const std::shared_ptr<ObjectBase> & obj);
    void addObject(const std::string& name, ObjectBase* obj);
    void addObject(const std::string& name, const std::shared_ptr<ObjectBase>& obj);
    ObjectBase* getObject(const std::string& name);

    bool deleteObject(const std::string& name);
    void clearObjects();

    void draw();

   private:
    std::string makeObjectName(object::ObjectBase* object);

    friend class ui::ObjectListWindow;
    gl::Scene* scene;
    std::mutex mtx_object;
    std::map<std::string, std::shared_ptr<ObjectBase>> objects;

    std::vector<std::shared_ptr<ObjectBase>> objects_to_delete;  // To Delete in gl thread;

};  // class ObjectManager
}  // namespace object
}  // namespace seg

#endif