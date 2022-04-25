#include "Camera.h"

#include <cmath>

#include "UI.h"

namespace xyz {

namespace {

const Vec4F randomV(0.0f, 1.0f, 0.0f, 0.0f);
const Vec4F randomV2(0.0f, 0.0f, 1.0f, 0.0f);
const Vec4F A(0.0f, 0.0f, 0.0f, 1.0F);

} // namespace

Camera::Camera()
    : Camera(Vec4F(0.0f, 0.0f, 1.0f, 1.0f), Vec4F(0.0f, 0.0f, 0.0f, 1.0f)) {
  compute_projection();
}

Camera::Camera(Vec4F from, Vec4F to) : from_(from), to_(to) {
  look_at(from, to);
  compute_projection();
}

void Camera::look_at(Vec4F from, Vec4F to) {
  from_ = from;
  to_ = to;

  forward_ = normalise(to_ - from_);

  if (forward_ == randomV) {
    right_ = normalise(cross(forward_, randomV2));
  } else {
    right_ = normalise(cross(forward_, randomV));
  }

  up_ = normalise(cross(right_, forward_));

  cam_to_world_ = Mat44F(right_, up_, forward_, from_);
  cam_to_world_.transpose();

  alignas(16) float right_values[4];
  right_.storeu(right_values);
  right_values[3] = -dot(right_, from_);

  alignas(16) float up_values[4];
  up_.storeu(up_values);
  up_values[3] = -dot(up_, from_);

  auto forward = forward_;
  forward.negate();
  alignas(16) float forward_values[4];
  forward.storeu(forward_values);
  forward_values[3] = -dot(forward, from_);

  world_to_cam_ =
      Mat44F(Vec4F(right_values), Vec4F(up_values), Vec4F(forward_values), A);
}

void Camera::compute_projection() {
  const auto ratio = static_cast<double>(UI::width) / UI::height;
  const auto scale = tan(fovy_ * 0.5 * M_PI / 180);
  const auto r = ratio * scale;
  const auto l = -r;
  const auto t = scale;
  const auto b = -t;

  Vec4F row_0(2.0f * near_ * (r - l), 0.0f, (r + l) / (r - l), 0.0f);
  Vec4F row_1(0.0f, 2.0f * near_ / (t - b), (t + b) / (t - b), 0.0f);
  Vec4F row_2(0.0f, 0.0f, -(far_ + near_) / (far_ - near_),
              -2.0f * far_ * near_ / (far_ - near_));
  Vec4F row_3(0.0f, 0.0f, -1.0f, 0.0f);

  m_projection_ = Mat44F(row_0, row_1, row_2, row_3);
}

} // namespace xyz
