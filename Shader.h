#pragma once

#include "GL/gl3w.h"

namespace xyz {

class Shader {
public:
	static GLuint load(GLenum type, const GLchar **src);
};

} // namespace xyz
