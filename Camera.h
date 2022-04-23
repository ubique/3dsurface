#pragma once

#include "Mat.h"
#include "Vec.h"

namespace xyz {

class Camera {
public:
  Camera();
  Camera(Vec4F from, Vec4F to);

  void look_at(Vec4F from, Vec4F to);

  const Mat44F &world_to_cam() const { return world_to_cam_; }
  const Mat44F &cam_to_world() const { return cam_to_world_; }
  const Mat44F &m_projection() const { return m_projection_; }

private:
  void compute_projection();

private:
  float fovy_{90.0f};
  float near_{0.1f};
  float far_{10.0f};
  Vec4F from_;
  Vec4F to_;
  Mat44F cam_to_world_;
  Mat44F world_to_cam_;
  Mat44F m_projection_;
};

} // namespace xyz
