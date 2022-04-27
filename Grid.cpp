#include "Grid.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iterator>

namespace xyz {

namespace {

constexpr float ndc_width = 2.0f;
constexpr float ndc_height = 2.0f;
constexpr float y_base = -1.0f;
constexpr float z_base = 1.0f;

const std::array<std::array<float, 4>, 16> CLUT = {{
	{0.050382f,0.029802f,0.527975f,1.0f},
{0.200444f,0.017900f,0.593364f,1.0f},
{0.312543f,0.008236f,0.635700f,1.0f},
{0.417642f,0.000560f,0.658390f,1.0f},
{0.517932f,0.021557f,0.654109f,1.0f},
{0.610667f,0.090200f,0.619951f,1.0f},
{0.692840f,0.165138f,0.564522f,1.0f},
{0.764193f,0.240393f,0.502126f,1.0f},
{0.826588f,0.315712f,0.441316f,1.0f},
{0.881443f,0.392527f,0.383229f,1.0f},
{0.928329f,0.472972f,0.326067f,1.0f},
{0.965024f,0.559116f,0.268513f,1.0f},
{0.988260f,0.652323f,0.211364f,1.0f},
{0.994141f,0.753134f,0.161404f,1.0f},
{0.977995f,0.861430f,0.142808f,1.0f},
{0.940015f,0.975156f,0.131326f,1.0f},
}};

} // namespace

Grid::Grid(size_t i, size_t j) : i_(i), j_(j) {
  vertices_.resize(i_ * j_ * TrianglesInPyramide);
  colors_.resize(i_ * j_ * TrianglesInPyramide);
  buffers_.resize(NumBuffers, 0);
}

Grid::Grid(Grid &&other) : i_(other.i_), j_(other.j_) {
  *this = std::move(other);
}

Grid::~Grid() {
  if (!buffers_.empty()) {
    glDeleteBuffers(buffers_.size(), buffers_.data());
  }
}

Grid &Grid::operator=(Grid &&other) {
  buffers_ = std::move(other.buffers_);
  vertices_ = std::move(other.vertices_);
  colors_ = std::move(other.colors_);

  return *this;
}

void Grid::init(GLuint *array) {
  const float width = ndc_width / i_;
  const float hwidth = width / 2;
  const float height = sqrt(width * width - hwidth * hwidth);

  float x = -1.0f;
  for (size_t i = 0; i < i_; ++i) {
    float z = z_base;

    for (size_t j = 0; j < j_; ++j) {
      const size_t index = (i * j_ + j) * TrianglesInPyramide;

      vertices_[index + 0][0] = {x, y_base, z};
      vertices_[index + 0][1] = {x + width, y_base, z};
      vertices_[index + 0][2] = {x + hwidth, y_base + height, z + hwidth};

      vertices_[index + 1][0] = {x + width, y_base, z};
      vertices_[index + 1][1] = {x + width, y_base, z + width};
      vertices_[index + 1][2] = {x + hwidth, y_base + height, z + hwidth};

      vertices_[index + 2][0] = {x, y_base, z + width};
      vertices_[index + 2][1] = {x + width, y_base, z + width};
      vertices_[index + 2][2] = {x + hwidth, y_base + height, z + hwidth};

      vertices_[index + 3][0] = {x, y_base, z + width};
      vertices_[index + 3][1] = {x, y_base, z};
      vertices_[index + 3][2] = {x + hwidth, y_base + height, z + hwidth};

      colors_[index + 0][0] = CLUT[0];
      colors_[index + 0][1] = CLUT[0];
      colors_[index + 0][2] = CLUT[15];

      colors_[index + 1][0] = CLUT[0];
      colors_[index + 1][1] = CLUT[0];
      colors_[index + 1][2] = CLUT[15];

      colors_[index + 2][0] = CLUT[0];
      colors_[index + 2][1] = CLUT[0];
      colors_[index + 2][2] = CLUT[15];

      colors_[index + 3][0] = CLUT[0];
      colors_[index + 3][1] = CLUT[0];
      colors_[index + 3][2] = CLUT[15];

      z += width;
    }
    x += width;
  }

  glCreateBuffers(buffers_.size(), buffers_.data());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER,
               i_ * j_ * TrianglesInPyramide * sizeof(TriangleVertices),
               vertices_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, VertexComponents, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[ColorBuffer]);
  glBufferData(GL_ARRAY_BUFFER,
               i_ * j_ * TrianglesInPyramide * sizeof(TriangleColors),
               colors_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, ColorComponents, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void Grid::display(float *values) {
  for (size_t i = 0; i < i_; ++i) {
    for (size_t j = 0; j < j_; ++j) {
      const size_t index = (i * j_ + j) * TrianglesInPyramide;

      const float value = values[i * j_ + j];
      const float nvalue =
          std::clamp(static_cast<int>(floor(value / (1.0f / 16))), 0, 15);

      vertices_[index + 0][2][1] = y_base + value;
      vertices_[index + 1][2][1] = y_base + value;
      vertices_[index + 2][2][1] = y_base + value;
      vertices_[index + 3][2][1] = y_base + value;

      colors_[index + 0][2] = CLUT[nvalue];
      colors_[index + 1][2] = CLUT[nvalue];
      colors_[index + 2][2] = CLUT[nvalue];
      colors_[index + 3][2] = CLUT[nvalue];
    }
  }

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER,
               i_ * j_ * TrianglesInPyramide * sizeof(TriangleVertices),
               vertices_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, VertexComponents, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[ColorBuffer]);
  glBufferData(GL_ARRAY_BUFFER,
               i_ * j_ * TrianglesInPyramide * sizeof(TriangleColors),
               colors_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, ColorComponents, GL_FLOAT, GL_FALSE, 0, nullptr);
}

size_t Grid::num_vertices() const {
  return vertices_.size() * TrianglesInPyramide * 3;
}

} // namespace xyz
