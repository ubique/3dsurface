#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include "GL/gl3w.h"

#include "Camera.h"
#include "Grid.h"
#include "Scene.h"
#include "TripleBuffer.h"
#include "UI.h"
#include "Values.h"
#include "Vec.h"

int main() {

  size_t x = 50;
  size_t y = 40;

  std::vector<float> values[3];
  std::array<void *, 3> buffers;

  for (size_t i = 0; i < buffers.size(); ++i) {
    values[i].resize(x * y);
    buffers[i] = values[i].data();
  }

  xyz::TripleBuffer triple_buffer(buffers);

  xyz::UI ui(&triple_buffer, xyz::Scene(xyz::Grid(x, y)));

  std::thread ui_thread([&]() {
    ui.init();

    ui.display();
  });

  std::random_device rd;
  std::mt19937 mtrand(rd());
  std::uniform_real_distribution<float> dist(0.0, 1.0);

  assert (!xyz::fvalues.empty ());
  auto fv = xyz::fvalues.begin ();
  while (!ui.has_stopped()) {
    auto buffer = triple_buffer.producer_buffer();
    auto values = reinterpret_cast<float *>(buffer);
    assert (fv->size () == x * y);

    for (size_t i = 0; i < x * y; ++i) {
      values[i] = (*fv)[i] / 53.3f;
    }

    triple_buffer.switch_producer_buffer();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (++fv == xyz::fvalues.end ()) {
      fv = xyz::fvalues.begin ();
    }
  }

  ui_thread.join();
}
