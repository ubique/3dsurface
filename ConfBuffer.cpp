#include "ConfBuffer.h"

#include <cassert>
#include <cstring>
#include <iostream>

namespace xyz {

namespace {

const char *kNames[] = {
#define OP(type, name, default_value) #name
    CONF_BUFFER_FIELD(OP)
#undef OP
};

} // namespace

void ConfBuffer::init(GLuint program, ConfBufferValue conf_buffer_value) {
  index_ = glGetUniformBlockIndex(program, "Conf");

  GLint size{0};
  glGetActiveUniformBlockiv(program, index_, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
  if (size == GL_INVALID_VALUE || size == GL_INVALID_ENUM ||
      size == GL_INVALID_OPERATION) {
    assert(0);
  }
  buffer_.reset(new uint8_t[size]);

  glGetUniformIndices(program, __MAX_CONF_BUFFER_FIELD, kNames, indices_);
  glGetActiveUniformsiv(program, __MAX_CONF_BUFFER_FIELD, indices_,
                        GL_UNIFORM_OFFSET, offset_);
  glGetActiveUniformsiv(program, __MAX_CONF_BUFFER_FIELD, indices_,
                        GL_UNIFORM_SIZE, size_);
  glGetActiveUniformsiv(program, __MAX_CONF_BUFFER_FIELD, indices_,
                        GL_UNIFORM_TYPE, type_);

#define OP(type, name, default_value)                                          \
  {                                                                            \
    const ConfBufferField field = CONF_BUFFER_FIELD_##name;                    \
    const GLint value_offset = offset_[field];                                 \
    const GLint value_size = size_[field] * type_[field];                      \
    memcpy(buffer_.get() + value_offset, &conf_buffer_value.name, value_size); \
  }

  CONF_BUFFER_FIELD(OP)
#undef OP

  glGenBuffers(1, &ubo_);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo_);
  glBufferData(GL_UNIFORM_BUFFER, size, buffer_.get(), GL_STATIC_DRAW);
  glBindBufferBase(GL_UNIFORM, index_, ubo_);
}

} // namespace xyz
