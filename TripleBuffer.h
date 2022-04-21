#pragma once

#include <array>
#include <atomic>
#include <cstdint>

namespace xyz {

class TripleBuffer {
public:
  explicit TripleBuffer(std::array<void *, 3> buffers);

  void *producer_buffer();
  void switch_producer_buffer();

  void *consumer_buffer();
  void switch_consumer_buffer();

private:
  std::array<void *, 3> buffers_;
  std::atomic<unsigned> flags_;
};

} // namespace xyz
