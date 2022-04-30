#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "GL/gl3w.h"

#include "Mat.h"

namespace xyz {

class Grid {
public:
  static constexpr size_t num_vao{1};

public:
  Grid(size_t i, size_t j);
  Grid(const Grid &) = delete;
  Grid(Grid &&other);
  ~Grid();

  Grid &operator=(const Grid &) = delete;
  Grid &operator=(Grid &&other);

  void init(GLuint *array);
  void display(float *values);
  size_t num_vertices() const;

private:
  enum { VertexBuffer, ColorBuffer, NormalBuffer, NumBuffers };
  static constexpr size_t VertexComponents{4};
  static constexpr size_t ColorComponents{4};
  static constexpr size_t NormalComponents{4};
  using VertexType = std::array<float, VertexComponents>;
  using ColorType = std::array<float, ColorComponents>;
  using NormalType = std::array<float, NormalComponents>;
  using TriangleVertices = std::array<VertexType, 3>;
  using TriangleColors = std::array<ColorType, 3>;
  using TriangleNormals = std::array<NormalType, 3>;
  static constexpr size_t TrianglesInPyramide{4};

private:
  const size_t i_;
  const size_t j_;
  std::vector<GLuint> buffers_;
  std::vector<TriangleVertices> vertices_;
  std::vector<TriangleColors> colors_;
  std::vector<TriangleNormals> normals_;
};

} // namespace xyz
