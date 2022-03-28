#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "GL/gl3w.h"

namespace xyz {

class Grid {
 public:
  static constexpr size_t num_vao{1};

 public:
  Grid(size_t x, size_t y);
  Grid(const Grid& ) = delete;
  Grid(Grid && other);
  ~Grid();

  Grid& operator=(const Grid&) = delete;
  Grid& operator=(Grid&& other);

  void init(GLuint* array);
  void display(float *values);
  size_t num_vertices() const;

 private:
  const size_t x_;
  const size_t y_;
  GLuint vertices_buffer_{0};
  GLuint colors_buffer_{0};
  std::vector<std::array<std::array<float, 2>, 3>> vertices_;
  std::vector<std::array<std::array<float, 4>, 3>> colors_;
};

} // namespace xyz
