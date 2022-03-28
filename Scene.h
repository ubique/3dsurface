#pragma once

#include <vector>

#include "GL/gl3w.h"
#include <GLFW/glfw3.h>

#include "ConfBuffer.h"
#include "Grid.h"

namespace xyz {

class Scene {
public:
  explicit Scene(Grid grid);
  Scene(const Scene&) = delete;
  Scene(Scene&&) = default;
  ~Scene();

  Scene& operator=(const Scene&) = delete;
  Scene& operator=(Scene&&) = default;

  void init (const ConfBufferValue &conf_buffer_value);
  void display (float *values);

private:
  void load_shaders ();

private:
  GLuint program_;
  ConfBuffer conf_buffer_;
  std::vector<GLuint> vao_;
  Grid grid_;
};

} // namespace xyz
