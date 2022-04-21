#include "Grid.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <iterator>

namespace xyz {

namespace {

constexpr float ndc_width = 2.0f;
constexpr float ndc_height = 2.0f;

const std::array<std::array<float, 4>, 10> CLUT = {{
    {0.10f, 0.12f, 0.80f, 1.0f},
    {0.20f, 0.14f, 0.70f, 1.0f},
    {0.30f, 0.16f, 0.60f, 1.0f},
    {0.40f, 0.18f, 0.50f, 1.0f},
    {0.50f, 0.20f, 0.40f, 1.0f},
    {0.60f, 0.22f, 0.30f, 1.0f},
    {0.70f, 0.24f, 0.20f, 1.0f},
    {0.80f, 0.26f, 0.10f, 1.0f},
    {0.90f, 0.28f, 0.00f, 1.0f},
    {1.00f, 0.30f, 0.00f, 1.0f},
}};

} // namespace

Grid::Grid(size_t i, size_t j) : i_(i), j_(j) {
  vertices_.resize(i_ * j_);
  colors_.resize(i_ * j_);
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
    float z = 0.0f;
    for (size_t j = 0; j < j_; ++j) {
      const size_t index = i * i_ + j;

      vertices_[index][0] = {x, 0.0f, z};
      vertices_[index][1] = {x + width, 0.0f, z};
      vertices_[index][2] = {x + hwidth, height, z};

      colors_[index][0] = CLUT[0];
      colors_[index][1] = CLUT[0];
      colors_[index][2] = CLUT[9];

      z += width;
    }
    x += width;
  }

  glCreateBuffers(buffers_.size(), buffers_.data());

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER, i_ * j_ * sizeof(TriangleVertices),
               vertices_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, VertexComponents, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[ColorBuffer]);
  glBufferData(GL_ARRAY_BUFFER, i_ * j_ * sizeof(TriangleColors),
               colors_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, ColorComponents, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void Grid::display(float *values) {
  for (size_t i = 0; i < i_; ++i) {
    for (size_t j = 0; j < j_; ++j) {
      const size_t index = i * i_ + j;
      const float value = values[index];
      const float nvalue =
          std::clamp(static_cast<int>(floor(value / 0.1f)), 0, 9);

      vertices_[index][2][1] = value;
      colors_[index][2] = CLUT[nvalue];
    }
  }

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[VertexBuffer]);
  glBufferData(GL_ARRAY_BUFFER, i_ * j_ * sizeof(TriangleVertices),
               vertices_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, VertexComponents, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, buffers_[ColorBuffer]);
  glBufferData(GL_ARRAY_BUFFER, i_ * j_ * sizeof(TriangleColors),
               colors_.data(), GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, ColorComponents, GL_FLOAT, GL_FALSE, 0, nullptr);
}

size_t Grid::num_vertices() const { return vertices_.size() * 3; }

} // namespace xyz
