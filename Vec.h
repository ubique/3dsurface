#pragma once

#include <smmintrin.h>
#include <xmmintrin.h>

#include <cassert>
#include <iostream>

#include "align.h"

namespace xyz {

class Mat44F;

static_assert(alignof(__m128) == 16, "Unexpected alignment for __m128");

class Vec4F {
public:
  Vec4F() : Vec4F(0.0f, 0.0f, 0.0f, 0.0f) {}
  Vec4F(__m128 values) : values_(values) {}
  Vec4F(float x, float y, float z) : Vec4F(x, y, z, 1.0f) {}
  Vec4F(float x, float y, float z, float w) {
    values_ = _mm_setr_ps(x, y, z, w);
  }
  Vec4F(float (&values)[4]) { values_ = _mm_load_ps(values); }

  Vec4F &operator-=(const Vec4F &other) {
    assert(IS_ALIGNED(&values_, 16));
    assert(IS_ALIGNED(&other.values_, 16));

    values_ = _mm_sub_ps(values_, other.values_);

    return *this;
  }

  Vec4F &operator+=(Vec4F other) {
    assert(IS_ALIGNED(&values_, 16));
    assert(IS_ALIGNED(&other.values_, 16));

    values_ = _mm_add_ps(values_, other.values_);

    return *this;
  }

  bool operator==(const Vec4F &other) const {
    assert(IS_ALIGNED(&values_, 16));
    assert(IS_ALIGNED(&other.values_, 16));

    return 15 == _mm_movemask_ps(_mm_cmpeq_ps(values_, other.values_));
  }

  void storeu(float (&values)[4]) const { _mm_storeu_ps(values, values_); }

  float operator[](size_t i) const {
    assert(0 <= i && i <= 3);

    float values[4];
    storeu(values);

    return values[i];
  }

  auto as_native() const { return values_; }

  Vec4F &cross(Vec4F other) {
    assert(IS_ALIGNED(&values_, 16));
    assert(IS_ALIGNED(&other.values_, 16));

    values_ = _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(values_, values_, _MM_SHUFFLE(3, 0, 2, 1)),
                   _mm_shuffle_ps(other.values_, other.values_,
                                  _MM_SHUFFLE(3, 1, 0, 2))),
        _mm_mul_ps(_mm_shuffle_ps(values_, values_, _MM_SHUFFLE(3, 1, 0, 2)),
                   _mm_shuffle_ps(other.values_, other.values_,
                                  _MM_SHUFFLE(3, 0, 2, 1))));

    return *this;
  }

  float dot(Vec4F v) const {
    return _mm_cvtss_f32(_mm_dp_ps(values_, v.as_native(), 0x71));
  }

  float length() const {
    return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(values_, values_, 0x71)));
  }

  void normalise() {
    values_ =
        _mm_div_ps(values_, _mm_sqrt_ps(_mm_dp_ps(values_, values_, 0x7f)));
  }

  void negate() { values_ = _mm_sub_ps(_mm_setzero_ps(), values_); }

private:
  friend Vec4F operator*(const Mat44F &mat, Vec4F v);

private:
  __m128 values_;
};

static inline Vec4F operator+(Vec4F x, Vec4F y) {
  x += y;

  return x;
}

static inline Vec4F operator-(Vec4F x, Vec4F y) {
  x -= y;

  return x;
}

static inline Vec4F normalise(Vec4F x) {
  x.normalise();

  return x;
}

static inline Vec4F cross(Vec4F x, Vec4F y) {
  x.cross(y);

  return x;
}

static inline float dot(Vec4F x, Vec4F y) { return x.dot(y); }

static inline Vec4F negate(Vec4F v) {
  v.negate();

  return v;
}

std::ostream &operator<<(std::ostream &ostream, const Vec4F vec);

} // namespace xyz
