#include "TripleBuffer.h"

#include <atomic>
#include <cassert>

namespace xyz {

namespace {

constexpr unsigned consumer_buffer_index_mask{0x00000003};
constexpr unsigned spare_buffer_index_mask{0x0000000c};
constexpr unsigned spare_buffer_index_shift{2};
constexpr unsigned producer_buffer_index_mask{0x00000030};
constexpr unsigned producer_buffer_index_shift{4};
constexpr unsigned spare_buffer_dirty_flag{0x00000040};

}  // namespace

TripleBuffer::TripleBuffer(std::array<void*, 3> buffers) : buffers_(buffers) {
	flags_ = 0 | (1 << spare_buffer_index_shift) | (2 << producer_buffer_index_shift);
}

void* TripleBuffer::producer_buffer() {
	unsigned flags = flags_.load(std::memory_order_acquire);

	flags &= producer_buffer_index_mask;
	flags >>= producer_buffer_index_shift;

	return buffers_[flags];
}

void TripleBuffer::switch_producer_buffer() {
	unsigned flags = flags_.load(std::memory_order_acquire);

	// Spare buffer hasn't been consumed by producer yet.
	if (flags & spare_buffer_dirty_flag) {
		return;
	}

	const unsigned spare_index = (flags & spare_buffer_index_mask) >> spare_buffer_index_shift;
	const unsigned producer_index = (flags & producer_buffer_index_mask) >> producer_buffer_index_shift;

	flags |= spare_buffer_dirty_flag;
	flags &= ~spare_buffer_index_mask;
	flags &= ~producer_buffer_index_mask;
	flags |= spare_index << producer_buffer_index_shift;
	flags |= producer_index << spare_buffer_index_shift;

	flags_.store(flags, std::memory_order_release);
}

void* TripleBuffer::consumer_buffer() {
	unsigned flags = flags_.load(std::memory_order_acquire);

	flags &= consumer_buffer_index_mask;

	return buffers_[flags];
}

void TripleBuffer::switch_consumer_buffer() {
	unsigned flags = flags_.load (std::memory_order_acquire);

	if (!(flags & spare_buffer_dirty_flag)) {
		return;
	}

	// spare buffer with dirty flag set becomes new consumer buffer
	// current consumer buffer becomes new spare buffer
	// unset dirty flag
	const unsigned spare_index = (flags & spare_buffer_index_mask) >> spare_buffer_index_shift;
	const unsigned consumer_index = flags & consumer_buffer_index_mask;

	flags &= ~spare_buffer_dirty_flag;
	flags &= ~spare_buffer_index_mask;
	flags &= ~consumer_buffer_index_mask;
	flags |= spare_index;
	flags |= (consumer_index << spare_buffer_index_shift);

	flags_.store(flags, std::memory_order_release);
}

}  // namespace xyz
