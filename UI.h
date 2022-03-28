#pragma once

#include <atomic>

#include "GL/gl3w.h"

#include "ConfBuffer.h"
#include "Scene.h"
#include "TripleBuffer.h"

namespace xyz {

class UI {
public:
	UI(TripleBuffer *triple_buffer, Scene scene);
	~UI();

	bool has_stopped () { return stopped_.load(std::memory_order_acquire); }

	void init (const ConfBufferValue &conf_buffer_value);
	void display();

private:
	ConfBuffer conf_buffer_;
	TripleBuffer *triple_buffer_;
	Scene scene_;
	GLFWwindow *window_{nullptr};
	std::atomic<bool> stopped_;
};

} // namespace xyz
