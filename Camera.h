#pragma once

#include "Mat.h"
#include "Vec.h"

namespace xyz {

class Camera {
public:
  Camera();
  Camera(Vec4F from, Vec4F to);

  void set_fov(double fov) {
    fov_ = fov;
    compute_projection();
  }
  void look_at(Vec4F from, Vec4F to);

  const Mat44F &world_to_cam() const { return world_to_cam_; }
  const Mat44F &cam_to_world() const { return cam_to_world_; }
  const Mat44F &m_projection() const { return m_projection_; }

  const Vec4F &up() const { return up_; }

  const Vec4F &right() const { return right_; }

  const Vec4F &forward() const { return forward_; }

private:
  void compute_projection();

private:
  float fov_{90.0f};
  float near_{1.0f};
  float far_{10.0f};
  Vec4F from_;
  Vec4F to_;
  Vec4F forward_;
  Vec4F right_;
  Vec4F up_;
  Mat44F cam_to_world_;
  Mat44F world_to_cam_;
  Mat44F m_projection_;
};

} // namespace xyz
