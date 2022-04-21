#include "Shader.h"

#include <cassert>

namespace xyz {

GLuint Shader::load(GLenum type, const GLchar **src) {
  const GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, src, nullptr);
  glCompileShader(shader);

  GLint is_compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
  assert(is_compiled);

  return shader;
}

} // namespace xyz
