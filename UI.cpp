#include "UI.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

namespace xyz {

UI::UI(TripleBuffer *triple_buffer, Scene scene)
    : triple_buffer_(triple_buffer), scene_(std::move(scene)) {}

UI::~UI() {
  if (window_) {
    glfwDestroyWindow(window_);
    glfwTerminate();
  }
}

void UI::init(const ConfBufferValue &conf_buffer_value) {
  assert(!window_);

  glfwInit();
  window_ = glfwCreateWindow(height, width, "3dsurface", nullptr, nullptr);
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwMakeContextCurrent(window_);
  gl3wInit();

  scene_.init(window_, conf_buffer_value);
}

void UI::display() {
  while (!glfwWindowShouldClose(window_)) {
    auto buffer = reinterpret_cast<float *>(triple_buffer_->consumer_buffer());
    scene_.display(window_, buffer);
    glfwSwapBuffers(window_);
    glfwPollEvents();
    triple_buffer_->switch_consumer_buffer();

    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  stopped_.store(true, std::memory_order_release);
}

} // namespace xyz
