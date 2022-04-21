#pragma once

#include <smmintrin.h>
#include <xmmintrin.h>

#include <cassert>
#include <cstddef>
#include <iostream>

#include "Vec.h"
#include "align.h"

namespace xyz {

class Mat44F {
public:
  Mat44F() { rows_[0] = rows_[1] = rows_[2] = rows_[3] = _mm_setzero_ps(); }

  Mat44F(__m128 raw_0, __m128 raw_1, __m128 raw_2, __m128 raw_3) {
    rows_[0] = raw_0;
    rows_[1] = raw_1;
    rows_[2] = raw_2;
    rows_[3] = raw_3;
  }

  Mat44F(Vec4F raw_0, Vec4F raw_1, Vec4F raw_2, Vec4F raw_3)
      : Mat44F(raw_0.as_native(), raw_1.as_native(), raw_2.as_native(),
               raw_3.as_native()) {}

  Vec4F operator[](size_t i) const {
    assert(0 <= i && i < 4);

    return Vec4F(rows_[i]);
  }

  void storeu(float (&values)[4][4]) {
    _mm_storeu_ps(values[0], rows_[0]);
    _mm_storeu_ps(values[1], rows_[1]);
    _mm_storeu_ps(values[2], rows_[2]);
    _mm_storeu_ps(values[3], rows_[3]);
  }

  void transpose() {
	_MM_TRANSPOSE4_PS (rows_[0], rows_[1], rows_[2], rows_[3]);
  }

private:
  friend Vec4F operator*(const Mat44F &mat, Vec4F v);

private:
  __m128 rows_[4];
};

Vec4F operator*(const Mat44F &mat, Vec4F v);

std::ostream &operator<<(std::ostream &ostream, const Mat44F &mat);

} // namespace xyz
