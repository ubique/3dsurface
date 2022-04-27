#include "Scene.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

#include "FragmentShader.h"
#include "UI.h"
#include "VertexShader.h"

namespace xyz {

namespace {

double G_x_pos{UI::width / 2.0f};
double G_y_pos{UI::height / 2.0f};
bool G_has_seen_mouse;
double G_fov{90.0f};

} // namespace

Scene::Scene(Grid grid) : grid_(std::move(grid)) { vao_.resize(Grid::num_vao); }

Scene::~Scene() {
  if (program_) {
    glDeleteProgram(program_);
  }
}

void Scene::init(GLFWwindow *window) {
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  glGenVertexArrays(vao_.size(), vao_.data());

  glfwSetCursorPosCallback(window,
                           [](GLFWwindow *window, double xpos, double ypos) {
                             G_x_pos = xpos;
                             G_y_pos = ypos;
                           });
  glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset) {
	G_fov -= yoffset;
	G_fov = std::clamp(G_fov, 1.0, 90.0);
  });

  load_shaders();

  glUseProgram(program_);

  glBindVertexArray(vao_[0]);
  grid_.init(vao_.data());

  m_view_ = glGetUniformLocation(program_, "m_view");
  m_proj_ = glGetUniformLocation(program_, "m_proj");

  last_time_ = glfwGetTime();
}

void Scene::display(GLFWwindow *window, float *values) {
  static int fps;
  static int fps_time;
  const auto current_time_ = glfwGetTime();
  delta_time_ = current_time_ - last_time_;

  read_mouse(window);
  read_keabord(window);
  camera_.set_fov(G_fov);
  camera_.look_at(from_, from_ + to_);

  alignas(16) GLfloat m_view[4][4];
  camera_.world_to_cam().storeu(m_view);
  glUniformMatrix4fv(m_view_, 1, GL_TRUE, &m_view[0][0]);

  alignas(16) GLfloat m_proj[4][4];
  camera_.m_projection().storeu(m_proj);
  glUniformMatrix4fv(m_proj_, 1, GL_TRUE, &m_proj[0][0]);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  grid_.display(values);
  glDrawArrays(GL_TRIANGLES, 0, grid_.num_vertices());

  last_time_ = glfwGetTime();

  ++fps;

  if (last_time_ - fps_time > 1.0) {
    fps = 0;
    fps_time = last_time_;
  }
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
  if (!G_has_seen_mouse) {
    last_x_pos_ = G_x_pos;
    last_y_pos_ = G_y_pos;
    G_has_seen_mouse = true;
  }
  float x_offset = G_x_pos - last_x_pos_;
  float y_offset = last_y_pos_ - G_y_pos;
  last_x_pos_ = G_x_pos;
  last_y_pos_ = G_y_pos;

  constexpr float sensivity = 0.1;
  x_offset *= sensivity;
  y_offset *= sensivity;

  yaw_ += x_offset;
  pitch_ += y_offset;

  pitch_ = std::clamp(pitch_, -89.0f, 89.0f);

  Vec4F to(cos(yaw_ * M_PI / 180.0f) * cos(pitch_ * M_PI / 180.0f),
           sin(pitch_ * M_PI / 180.0f),
           sin(yaw_ * M_PI / 180.0f) * cos(pitch_ * M_PI / 180.0f), 0.0f);


  to_ = normalise(to);
}

void Scene::read_keabord(GLFWwindow *window) {
  const float speed = 50.00f;

  const auto &forward = camera_.forward();
  const auto &up = camera_.up();

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    from_ += delta_time_ * speed * forward;
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    from_ -= delta_time_ * speed * forward;
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    from_ += delta_time_ * speed * normalise(cross(forward, up));
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    from_ -= delta_time_ * speed * normalise(cross(forward, up));
  }
}

} // namespace xyz
