#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include "GL/gl3w.h"

#include "Camera.h"
#include "UI.h"
#include "Vec.h"

int main() {
  using namespace xyz;

  Vec4F v(0.9, 0.442435, -1.9, 0.0);
  std::cout << "v: " << v << std::endl;

  Vec4F from(0.0f, 0.0f, 1.0f, 0.0f);
  std::cout << "from: " << from << std::endl;
  Vec4F to(0.0f, 0.0f, 0.0f, 1.0f);
  std::cout << "to: " << to << std::endl;

  Camera camera(from, to);
  std::cout << "CamToWorld:" << std::endl;
  std::cout << camera.cam_to_world() << std::endl;
  std::cout << "WorldToCam:" << std::endl;
  std::cout << camera.world_to_cam() << std::endl;
  std::cout << "Mprojection:" << std::endl;
  std::cout << camera.m_projection() << std::endl;

  std::cout << "v':" << (camera.world_to_cam() * v) << std::endl;
  std::cout << "v'':" << camera.m_projection() * (camera.world_to_cam() * v)
            << std::endl;

  // return 0;

  size_t x = 50;
  size_t y = 50;

  std::vector<float> values[3];
  std::array<void *, 3> buffers;

  for (size_t i = 0; i < buffers.size(); ++i) {
    values[i].resize(x * y);
    buffers[i] = values[i].data();
  }

  TripleBuffer triple_buffer(buffers);

  UI ui(&triple_buffer, Scene(Grid(x, y)));

  std::thread ui_thread([&]() {
    ui.init();

    ui.display();
  });

  std::random_device rd;
  std::mt19937 mtrand(rd());
  std::uniform_real_distribution<float> dist(0.0, 1.0);

  while (!ui.has_stopped()) {
    auto buffer = triple_buffer.producer_buffer();
    auto values = reinterpret_cast<float *>(buffer);

    for (size_t i = 0; i < x * y; ++i) {
      values[i] = dist(mtrand);
    }

    triple_buffer.switch_producer_buffer();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  ui_thread.join();
}
