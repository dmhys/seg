#ifndef SEG_OBJECT_OBJECT_H
#define SEG_OBJECT_OBJECT_H

#include <memory>

#include "seg/object/gl/basic_renderers.h"
#include "seg/object/gl/path.h"
#include "seg/object/gl/pose.h"
#include "seg/object/primitives.h"
#include "seg/object/ui/image.h"

namespace seg {
namespace object {
class ObjectBase;
class GLObject;

typedef StaticMeshRenderer Mesh; // mesh is usually static.

typedef StaticLineRenderer StaticLine;
typedef StaticPointcloudRenderer StaticPointcloud;

typedef LineRenderer Line;
typedef PointcloudRenderer Pointcloud;

}  // namespace object
}  // namespace seg

#endif