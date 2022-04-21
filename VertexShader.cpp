#include "VertexShader.h"

#include "Shader.h"

namespace xyz {

namespace {

const GLchar *kSrc = R"tag(
#version 400 core

layout( location = 0 ) in vec4 vPosition;
layout( location = 1 ) in vec4 vColor;

uniform mat4 m_view;

out vec4 vs_color;

void
main()
{
    vs_color = vColor;
    gl_Position = m_view * vPosition;
    // gl_Position = vPosition;
}
)tag";

} // namespace

GLuint VertexShader::load() { return Shader::load(GL_VERTEX_SHADER, &kSrc); }

} // namespace xyz
