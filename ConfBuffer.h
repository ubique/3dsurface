#pragma once

#include <cstdint>
#include <memory>

#include "GL/gl3w.h"

namespace xyz {

#define CONF_BUFFER_FIELD(OP)                                                  \
  OP(GLuint, x, 100)                                                           \
  OP(GLuint, y, 100)

enum ConfBufferField {
#define OP(type, name, default_value) CONF_BUFFER_FIELD_##name,
  CONF_BUFFER_FIELD(OP)
#undef OP
      __MAX_CONF_BUFFER_FIELD
};

struct ConfBufferValue {
#define OP(type, name, default_value) type name{default_value};
  CONF_BUFFER_FIELD(OP)
#undef OP
};

class ConfBuffer {
public:
  void init(GLuint program, ConfBufferValue conf_buffer_value);

#undef OP
private:
  std::unique_ptr<uint8_t[]> buffer_;
  GLuint index_{0};
  GLuint ubo_{0};
  GLuint indices_[__MAX_CONF_BUFFER_FIELD];
  GLint size_[__MAX_CONF_BUFFER_FIELD];
  GLint offset_[__MAX_CONF_BUFFER_FIELD];
  GLint type_[__MAX_CONF_BUFFER_FIELD];
};

} // namespace xyz
