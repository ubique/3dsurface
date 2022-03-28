#include "Grid.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>

namespace xyz {

namespace {

const std::array<std::array<float, 4>, 10> CLUT = {{
	{0.00f, 0.03f, 0.90f, 1.0f},
	{0.10f, 0.06f, 0.80f, 1.0f},
	{0.20f, 0.09f, 0.70f, 1.0f},
	{0.30f, 0.12f, 0.60f, 1.0f},
	{0.40f, 0.15f, 0.50f, 1.0f},
	{0.50f, 0.18f, 0.40f, 1.0f},
	{0.60f, 0.21f, 0.30f, 1.0f},
	{0.70f, 0.24f, 0.20f, 1.0f},
	{0.80f, 0.27f, 0.10f, 1.0f},
	{0.90f, 0.30f, 0.00f, 1.0f},
}};

} // namespace

Grid::Grid(size_t x, size_t y) : x_(x), y_(y) {
	vertices_.resize (x_ * y_);
	colors_.resize (x_ * y_);
}

Grid::Grid(Grid&& other) : x_(other.x_), y_(other.y_) {
	*this = std::move(other);
}

Grid::~Grid() {
	if (vertices_buffer_) {
		glDeleteBuffers(1, &vertices_buffer_);
	}
	if (colors_buffer_) {
		glDeleteBuffers(1, &colors_buffer_);
	}
}

Grid& Grid::operator=(Grid&& other) {
	vertices_buffer_ = std::exchange(other.vertices_buffer_, 0);
	colors_buffer_ = std::exchange(other.colors_buffer_, 0);
	vertices_ = std::move (other.vertices_);
	colors_ = std::move (other.colors_);

	return *this;
}

void Grid::init(GLuint* array) {
  const float width = 2.0f / x_;
  const float hwidth = width / 2;
  const float height = 2.0f / y_;

  float y_pos = 1.0f;
  for(size_t x = 0; x < x_; ++x) {
	float x_pos = -1.0f;
	y_pos -= height;

	for (size_t y = 0; y < y_; ++y) {
		const size_t index = x * x_ + y;

		vertices_[index][0] = { x_pos, y_pos };
		vertices_[index][1] = { x_pos + width, y_pos};
		vertices_[index][2] = { x_pos + hwidth, y_pos + height};

		colors_[index][0] = CLUT[0];
		colors_[index][1] = CLUT[0];
		colors_[index][2] = CLUT[9];

		x_pos += width;
	}
  }

  glCreateBuffers(1, &vertices_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_);
  glBufferData(GL_ARRAY_BUFFER, x_ * y_ * sizeof (decltype(vertices_)::value_type), vertices_.data(), GL_DYNAMIC_DRAW);

  glCreateBuffers(1, &colors_buffer_);
  glBindBuffer(GL_ARRAY_BUFFER, colors_buffer_);
  glBufferData(GL_ARRAY_BUFFER, x_ * y_ * sizeof (decltype(colors_)::value_type), colors_.data(), GL_DYNAMIC_DRAW);

  glBindVertexArray (array[0]);
}

void Grid::display(float *values) {
	const float height = 2.0f / y_;

	for (size_t x = 0; x < x_; ++x) {
		for (size_t y = 0; y < y_; ++y) {
			const size_t index = x * x_ + y;
			const float value = values[index];
			const float vindex = std::clamp(
			    static_cast<int>(floor(value / 0.1)), 0, 9);

			vertices_[index][2][1] =
			    height * value + vertices_[index][1][1];
			colors_[index][2] = CLUT[vindex];
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_);
	glBufferData(GL_ARRAY_BUFFER, x_ * y_ * sizeof (decltype(vertices_)::value_type), vertices_.data(), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colors_buffer_);
	glBufferData(GL_ARRAY_BUFFER, x_ * y_ * sizeof (decltype(colors_)::value_type), colors_.data(), GL_DYNAMIC_DRAW);
}

size_t Grid::num_vertices() const {
	return vertices_.size () * 3;
}

} // namespace xyz
