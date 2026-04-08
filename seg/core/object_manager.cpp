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

  for (auto&& [_, object] : objects) {
    if (object->getObjectLayer() != ObjectLayer::GL) continue;
    static_cast<GLObject*>(object.get())->glFree();
  }

  for (auto&& object : objects_to_delete) {
    if (object->getObjectLayer() != ObjectLayer::GL) continue;
    static_cast<GLObject*>(object.get())->glFree();
  }
  objects_to_delete.clear();
}

std::string ObjectManager::addObject(ObjectBase* obj) {
  return insertObject(std::shared_ptr<ObjectBase>(obj));
}

std::string ObjectManager::addObject(const std::shared_ptr<ObjectBase>& obj) {
  return insertObject(obj);
}

void ObjectManager::addObject(const std::string& name, ObjectBase* obj) {
  insertObject(std::shared_ptr<ObjectBase>(obj), name);
}

void ObjectManager::addObject(const std::string& name,
                              const std::shared_ptr<ObjectBase>& obj) {
  insertObject(obj, name);
}

std::string ObjectManager::insertObject(const std::shared_ptr<ObjectBase>& obj,
                                        const std::string& _name) {
  std::lock_guard<std::mutex> lock(mtx_object);
  if (shut_down) return "";

  std::string name = _name.empty() ? generateName(obj.get()) : _name;

  if (objects.find(name) != objects.end()) {
    LOG_WARN("objectManager - [{}] already exists !", name);
    LOG_WARN("Add Object fail.");
    return "";
  }

  if (obj->getObjectLayer() == ObjectLayer::GL && shader)
    static_cast<GLObject*>(obj.get())->setShader(shader);

  objects[name] = obj;

  LOG_INFO("Object '{}({})' added. Total: {}", name, obj->getType(),
           objects.size());
  return name;
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

  for (auto&& [_, object] : objects) objects_to_delete.push_back(object);

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

  for (const auto& [_, object] : objects) object->draw();
}

void ObjectManager::forEachObject(
    const std::function<void(const std::string&, ObjectBase&)>& fn) {
  std::lock_guard<std::mutex> lock(mtx_object);
  for (auto& [name, obj] : objects) fn(name, *obj);
}

std::string ObjectManager::generateName(ObjectBase* obj) {
  std::string base_name = obj->getType();

  if (objects.find(base_name) == objects.end()) return base_name;

  int i = 2;
  while (objects.find(base_name + std::to_string(i)) != objects.end()) i++;

  return base_name + std::to_string(i);
}

}  // namespace object
}  // namespace seg