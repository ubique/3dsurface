#include "Scene.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include "FragmentShader.h"
#include "UI.h"
#include "VertexShader.h"

namespace xyz {

Scene::Scene(Grid grid) : grid_(std::move(grid)) { vao_.resize(Grid::num_vao); }

Scene::~Scene() {
  if (program_) {
    glDeleteProgram(program_);
  }
}

void Scene::init(const ConfBufferValue &conf_buffer_value) {
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glGenVertexArrays(vao_.size(), vao_.data());

  load_shaders();
  conf_buffer_.init(program_, conf_buffer_value);

  glUseProgram(program_);

  glBindVertexArray(vao_[0]);
  grid_.init(vao_.data());

  m_view_ = glGetUniformLocation(program_, "m_view");

  last_time_ = glfwGetTime();
}

void Scene::display(GLFWwindow *window, float *values) {
  const auto current_time_ = glfwGetTime();
  delta_time_ = current_time_ - last_time_;

  read_mouse(window);
  read_keabord(window);
  camera_.look_at(from_, to_);

  std::cout << "Delta: " << delta_ << std::endl;
  std::cout << "From: " << from_ << std::endl;
  to_ = camera_.cam_to_world () * to_;
  from_ = camera_.cam_to_world () * delta_;
  std::cout << "From: " << from_ << std::endl;

  alignas(16) GLfloat m_view[4][4];
  camera_.world_to_cam().storeu(m_view);
  glUniformMatrix4fv(m_view_, 1, GL_FALSE, &m_view[0][0]);

  //glClearBufferfv(GL_COLOR, 0, black);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  grid_.display(values);
  glDrawArrays(GL_TRIANGLES, 0, grid_.num_vertices());

  last_time_ = glfwGetTime();
}

void Scene::load_shaders() {
  program_ = glCreateProgram();

  const auto vertex_shader = VertexShader::load();
  glAttachShader(program_, vertex_shader);

  const auto fragment_shader = FragmentShader::load();
  glAttachShader(program_, fragment_shader);

  glLinkProgram(program_);

  GLint is_linked;
  glGetProgramiv(program_, GL_LINK_STATUS, &is_linked);
  assert(is_linked);
}

void Scene::read_mouse(GLFWwindow *window) {
  static double xpos_center{UI::height / 2.0};
  static double ypos_center{UI::width / 2.0};
  static double speed{10.0};

  double xpos, ypos;

  glfwGetCursorPos(window, &xpos, &ypos);
  glfwSetCursorPos(window, xpos_center, ypos_center);

  h_angle_ += speed * delta_time_ * (xpos_center - xpos);
  v_angle_ += speed * delta_time_ * (ypos_center - ypos);

  Vec4F to(cos(v_angle_) * sin(h_angle_), sin(v_angle_),
           cos(v_angle_) * cos(h_angle_), 1.0f);
  to_ = to;
}

void Scene::read_keabord(GLFWwindow *window) {
  alignas(16) float delta[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  const float speed = 10.0f;

  // Move forward
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    delta[2] -= delta_time_ * speed;
  }

  // Move backward
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    delta[2] += delta_time_ * speed;
  }

  // Strafe right
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    delta[0] += delta_time_ * speed;
  }
  // Strafe left
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    delta[0] -= delta_time_ * speed;
  }

  delta_ = Vec4F(delta);
}

} // namespace xyz
