#pragma once

#include <vector>

#include "GL/gl3w.h"
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "ConfBuffer.h"
#include "Grid.h"

namespace xyz {

class Scene {
public:
  explicit Scene(Grid grid);
  Scene(const Scene &) = delete;
  Scene(Scene &&) = default;
  ~Scene();

  Scene &operator=(const Scene &) = delete;
  Scene &operator=(Scene &&) = default;

  void init(const ConfBufferValue &conf_buffer_value);
  void display(GLFWwindow *window, float *values);

private:
  void load_shaders();
  void read_mouse(GLFWwindow *window);
  void read_keabord(GLFWwindow *window);

private:
  GLuint program_;
  GLint m_view_;
  double last_time_;
  double delta_time_;
  Vec4F from_{0.0f, 0.0f, 0.0f, 1.0f};
  Vec4F to_{0.0f, 0.0f, -1.0f, 1.0f};
  Vec4F delta_;
  double h_angle_{0.0};
  double v_angle_{0.0};
  Camera camera_;
  ConfBuffer conf_buffer_;
  std::vector<GLuint> vao_;
  Grid grid_;
};

} // namespace xyz
