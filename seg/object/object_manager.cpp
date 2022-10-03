#include "seg/object/object_manager.h"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "seg/gl/scene.h"
#include "seg/object/gl_object.h"
#include "seg/object/object_base.h"
#include "seg/utilities/logger.h"

namespace seg {
namespace object {
void ObjectManager::onCoreShutdown()
{
    // no need to lock
    for (auto&& object_iter : objects) {
        auto&& object = object_iter.second;
        if (object->getObjectLayer() != ObjectLayer::GL) continue;

        static_cast<GLObject*>(object.get())->glFree();
    }
}

std::string ObjectManager::addObject(ObjectBase * obj)
{
    std::string name = makeObjectName(obj);
    addObject(name, std::shared_ptr<ObjectBase>(obj));
    return name;
}

std::string ObjectManager::addObject(const std::shared_ptr<ObjectBase> & obj)
{
    std::string name = makeObjectName(obj.get());
    addObject(name, obj);
    return name;
}

void ObjectManager::addObject(const std::string& name, ObjectBase* obj)
{
    addObject(name, std::shared_ptr<ObjectBase>(obj));
}

void ObjectManager::addObject(const std::string& name,
                              const std::shared_ptr<ObjectBase>& obj)
{
    std::lock_guard<std::mutex> lock(mtx_object);

    if (objects.find(name) != objects.end()) {
        LOG_WARN("objectManager - [{}] already exists !", name);
        LOG_WARN("Add Object fail.");
        return;
    }

    if (obj->getObjectLayer() == ObjectLayer::GL)
        static_cast<GLObject*>(obj.get())->setShader(&(scene->shader));

    objects[name] = obj;

    LOG_INFO("Object '{}({})' added.", name, obj->getType(), objects.size());
}

ObjectBase* ObjectManager::getObject(const std::string& name)
{
    std::lock_guard<std::mutex> lock(mtx_object);

    auto obj_iter = objects.find(name);
    if (obj_iter == objects.end()) return nullptr;

    return obj_iter->second.get();
}

bool ObjectManager::deleteObject(const std::string& name)
{
    std::lock_guard<std::mutex> lock(mtx_object);

    auto obj_iter = objects.find(name);
    if (obj_iter == objects.end()) return false;

    objects_to_delete.push_back(obj_iter->second);
    objects.erase(name);

    return true;
}

void ObjectManager::clearObjects()
{
    std::lock_guard<std::mutex> lock(mtx_object);

    for (auto&& object_iter : objects) objects_to_delete.push_back(object_iter.second);

    objects.clear();
}

void ObjectManager::draw()
{
    for (auto&& object : objects_to_delete) {
        if (object->getObjectLayer() != ObjectLayer::GL) continue;

        if (object.use_count() != 1) static_cast<GLObject*>(object.get())->glFree();
    }
    objects_to_delete.clear();

    scene->shader.bind();

    std::lock_guard<std::mutex> lock(mtx_object);

    for (const auto& object : objects) object.second->draw();

    scene->shader.unbind();
}

std::string ObjectManager::makeObjectName(object::ObjectBase* obj)
{
    std::string base_name = obj->getType();

    std::lock_guard<std::mutex> lock(mtx_object);

    if (objects.find(base_name) == objects.end()) return base_name;

    int i = 2;
    while (objects.find(base_name + std::to_string(i)) != objects.end()) {
        i++;
    }

    return base_name + std::to_string(i);
}

}  // namespace object
}  // namespace seg