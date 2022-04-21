#include "Camera.h"

#include "Mat.h"

namespace xyz {

namespace {

const Vec4F randomV(0.0f, 1.0f, 0.0f, 0.0f);
const Vec4F randomV2(0.0f, 0.0f, 1.0f, 0.0f);
const Vec4F A(0.0f, 0.0f, 0.0f, 1.0F);

} // namespace

Camera::Camera(Vec4F from, Vec4F to) : from_(from), to_(to) { look_at(from, to); }

void Camera::look_at(Vec4F from, Vec4F to) {
  from_ = from;
  to_ = to;

  auto forward = normalise(to_ - from_);

  Vec4F right;

  if (forward == randomV) {
    right = normalise(cross(forward, randomV2));
  } else {
    right = normalise(cross(forward, randomV));
  }

  auto up = normalise(cross(right, forward));

  cam_to_world_ = Mat44F(right, up, forward, from_);
  cam_to_world_.transpose();

  forward.negate();

  alignas(16) float right_values[4];
  right.storeu(right_values);
  right_values[3] = -dot(right, from_);

  alignas(16) float up_values[4];
  up.storeu(up_values);
  up_values[3] = -dot(up, from_);

  alignas(16) float forward_values[4];
  forward.storeu(forward_values);
  forward_values[3] = -dot(forward, from_);

  world_to_cam_ =
      Mat44F(Vec4F(right_values), Vec4F(up_values), Vec4F(forward_values), A);
}

} // namespace xyz
