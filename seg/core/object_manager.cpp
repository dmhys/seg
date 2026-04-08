#include "seg/core/object_manager.h"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "seg/gl/shader.h"
#include "seg/object/gl_object.h"
#include "seg/object/object_base.h"
#include "seg/utilities/logger.h"

namespace seg {
namespace object {
void ObjectManager::setShader(gl::Shader* _shader) {
  std::lock_guard<std::mutex> lock(mtx_object);
  shader = _shader;
  for (auto& [name, obj] : objects) {
    if (obj->getObjectLayer() == ObjectLayer::GL)
      static_cast<GLObject*>(obj.get())->setShader(shader);
  }
}

void ObjectManager::onCoreShutdown() {
  std::lock_guard<std::mutex> lock(mtx_object);
  shut_down = true;

  for (auto&& object_iter : objects) {
    auto&& object = object_iter.second;
    if (object->getObjectLayer() != ObjectLayer::GL) continue;
    static_cast<GLObject*>(object.get())->glFree();
  }
}

std::string ObjectManager::addObject(ObjectBase* obj) {
  std::string name = makeObjectName(obj);
  addObject(name, std::shared_ptr<ObjectBase>(obj));
  return name;
}

std::string ObjectManager::addObject(const std::shared_ptr<ObjectBase>& obj) {
  std::string name = makeObjectName(obj.get());
  addObject(name, obj);
  return name;
}

void ObjectManager::addObject(const std::string& name, ObjectBase* obj) {
  addObject(name, std::shared_ptr<ObjectBase>(obj));
}

void ObjectManager::addObject(const std::string& name,
                              const std::shared_ptr<ObjectBase>& obj) {
  std::lock_guard<std::mutex> lock(mtx_object);
  if (shut_down) return;

  if (objects.find(name) != objects.end()) {
    LOG_WARN("objectManager - [{}] already exists !", name);
    LOG_WARN("Add Object fail.");
    return;
  }

  if (obj->getObjectLayer() == ObjectLayer::GL && shader)
    static_cast<GLObject*>(obj.get())->setShader(shader);

  objects[name] = obj;

  LOG_INFO("Object '{}({})' added. Total: {}", name, obj->getType(),
           objects.size());
}

std::weak_ptr<ObjectBase> ObjectManager::getObject(const std::string& name) {
  std::lock_guard<std::mutex> lock(mtx_object);

  auto obj_iter = objects.find(name);
  if (obj_iter == objects.end()) return {};

  return obj_iter->second;
}

bool ObjectManager::deleteObject(const std::string& name) {
  std::lock_guard<std::mutex> lock(mtx_object);
  if (shut_down) return false;

  auto obj_iter = objects.find(name);
  if (obj_iter == objects.end()) return false;

  objects_to_delete.push_back(obj_iter->second);
  objects.erase(name);

  return true;
}

void ObjectManager::clearObjects() {
  std::lock_guard<std::mutex> lock(mtx_object);

  for (auto&& object_iter : objects)
    objects_to_delete.push_back(object_iter.second);

  objects.clear();
}

void ObjectManager::draw() {
  std::vector<std::shared_ptr<ObjectBase>> to_delete;

  std::lock_guard<std::mutex> lock(mtx_object);
  to_delete.swap(objects_to_delete);

  for (auto&& object : to_delete) {
    if (object->getObjectLayer() != ObjectLayer::GL) continue;
    static_cast<GLObject*>(object.get())->glFree();
  }

  for (const auto& object : objects) object.second->draw();
}

void ObjectManager::forEachObject(
    const std::function<void(const std::string&, ObjectBase&)>& fn) {
  std::lock_guard<std::mutex> lock(mtx_object);
  for (auto& [name, obj] : objects) fn(name, *obj);
}

std::string ObjectManager::makeObjectName(object::ObjectBase* obj) {
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