#include "FragmentShader.h"

#include "Shader.h"

namespace xyz {

namespace {

const GLchar *kSrc = R"tag(
#version 450 core

in vec4 vs_color;
out vec4 fColor;

void main()
{
    fColor = vs_color;
}
)tag";

} // namespace

GLuint FragmentShader::load() {
  return Shader::load(GL_FRAGMENT_SHADER, &kSrc);
}

} // namespace xyz
