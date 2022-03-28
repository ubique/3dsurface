#include <atomic>
#include <chrono>
#include <random>
#include <thread>

#include "GL/gl3w.h"

#include "UI.h"

int main() {
	using namespace xyz;

	ConfBufferValue conf_buffer_value;
	conf_buffer_value.x = 3;
	conf_buffer_value.y = 3;

	std::vector<float> values[3];
	std::array<void *, 3> buffers;

	for (size_t i = 0; i < buffers.size (); ++i) {
		values[i].resize (conf_buffer_value.x * conf_buffer_value.y);
		buffers[i] = values[i].data ();
	}

	TripleBuffer triple_buffer(buffers);

	UI ui(&triple_buffer,
	      Scene(Grid(conf_buffer_value.x, conf_buffer_value.y)));

	std::thread ui_thread([&]() {
		ui.init(conf_buffer_value);

		ui.display();
	});

	std::random_device rd;
	std::mt19937 mtrand (rd());
	std::uniform_real_distribution<float> dist(0, 1.0);

	while (!ui.has_stopped()) {
		auto buffer = triple_buffer.producer_buffer();
		auto values = reinterpret_cast<float *>(buffer);

		for (size_t i = 0;
		     i < conf_buffer_value.x * conf_buffer_value.y; ++i) {
			values[i] = dist(mtrand);
		}

		triple_buffer.switch_producer_buffer ();

		std::this_thread::sleep_for(std::chrono::milliseconds (40));
	}

	ui_thread.join();
}
