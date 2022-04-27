#pragma once

#include <atomic>

#include "GL/gl3w.h"

#include "Scene.h"
#include "TripleBuffer.h"

namespace xyz {

class UI {
public:
  static constexpr int height{1024};
  static constexpr int width{1024};

public:
  UI(TripleBuffer *triple_buffer, Scene scene);
  ~UI();

  bool has_stopped() { return stopped_.load(std::memory_order_acquire); }

  void init();
  void display();

private:
  TripleBuffer *triple_buffer_;
  Scene scene_;
  GLFWwindow *window_{nullptr};
  std::atomic<bool> stopped_{false};
};

} // namespace xyz
