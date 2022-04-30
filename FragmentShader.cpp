#include "FragmentShader.h"

#include "Shader.h"

namespace xyz {

namespace {

const GLchar *kSrc = R"tag(
#version 450 core

uniform vec4 ambient;

in vec4 vs_color;
in vec4 vs_normal;
in vec3 vs_worldpos;
out vec4 f_color;

uniform vec4 color_diffuse = vec4(0.3, 0.3, 0.3, 1.0);
uniform vec4 color_specular = vec4(0.4, 0.4, 0.4, 1.0);
uniform float shininess = 77.0f;
uniform vec3 light_position = vec3(10.0f, 10.0f, 10.0f);

void main()
{
    f_color = ambient;
    vec3 light_direction = normalize(light_position - vs_worldpos);
    vec3 normal = normalize(vs_normal.xyz);
    vec3 half_vector = normalize(light_direction + normalize(vs_worldpos));
    float diffuse = max(0.0, dot(normal, light_direction));
    float specular = pow(max(0.0, dot(normal, half_vector)), shininess);
    f_color = vs_color * (ambient + diffuse + specular);
}
)tag";

} // namespace

GLuint FragmentShader::load() {
  return Shader::load(GL_FRAGMENT_SHADER, &kSrc);
}

} // namespace xyz
