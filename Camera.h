#pragma once

#include "Mat.h"
#include "Vec.h"

namespace xyz {

class Camera {
public:
  Camera()
      : Camera(Vec4F(0.0f, 0.0f, 0.0f, 1.0f), Vec4F(0.0f, 0.0f, -1.0f, 1.0f)) {}
  Camera(const Mat44F &cam_to_world) : cam_to_world_(cam_to_world) {}
  Camera(Vec4F from, Vec4F to);

  void look_at(Vec4F from, Vec4F to);

  Mat44F world_to_cam() const { return world_to_cam_; }

  const Mat44F &cam_to_world() const { return cam_to_world_; }

private:
  Vec4F from_;
  Vec4F to_;
  Mat44F cam_to_world_;
  Mat44F world_to_cam_;
};

} // namespace xyz
