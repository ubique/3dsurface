#include "FragmentShader.h"

#include "Shader.h"

namespace xyz {

namespace {

const GLchar *kSrc = R"tag(
#version 450 core

uniform vec4 ambient;

in vec4 vs_color;
out vec4 f_color;

void main()
{
    f_color = min(vs_color * ambient, vec4(1.0));
}
)tag";

} // namespace

GLuint FragmentShader::load() {
  return Shader::load(GL_FRAGMENT_SHADER, &kSrc);
}

} // namespace xyz
