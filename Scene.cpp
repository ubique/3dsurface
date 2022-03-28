#include "Scene.h"

#include <cassert>
#include <iostream>

#include "FragmentShader.h"
#include "VertexShader.h"

namespace xyz {

Scene::Scene(Grid grid) : grid_(std::move (grid))
{
	vao_.resize(Grid::num_vao);
}

Scene::~Scene() {
	if (program_) {
		glDeleteProgram (program_);
	}
}

void Scene::init (const ConfBufferValue &conf_buffer_value) {
	glGenVertexArrays(vao_.size (), vao_.data());

	load_shaders ();
	conf_buffer_.init(program_, conf_buffer_value);

	glUseProgram(program_);

	glBindVertexArray(vao_[0]);
	grid_.init (vao_.data ());
}

void Scene::display(float *values) {
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);
	grid_.display (values);
	glDrawArrays( GL_TRIANGLES, 0, grid_.num_vertices ());
}

void Scene::load_shaders () {
	program_ = glCreateProgram ();

	const auto vertex_shader = VertexShader::load ();
	glAttachShader (program_, vertex_shader);

	const auto fragment_shader = FragmentShader::load ();
	glAttachShader (program_, fragment_shader);

	glLinkProgram (program_);

	GLint is_linked;
	glGetProgramiv(program_, GL_LINK_STATUS, &is_linked);
	assert (is_linked);
}

} // namespace xyz
