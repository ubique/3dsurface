#include "Mat.h"

namespace xyz {

Vec4F operator*(const Mat44F &mat, Vec4F v) {
  __m128 r = _mm_dp_ps(mat.rows_[0], v.values_, 0xf1);
  r = _mm_add_ps(r, _mm_dp_ps(mat.rows_[1], v.values_, 0xf2));
  r = _mm_add_ps(r, _mm_dp_ps(mat.rows_[2], v.values_, 0xf4));
  r = _mm_add_ps(r, _mm_dp_ps(mat.rows_[3], v.values_, 0xf8));

  return Vec4F(r);
}

std::ostream &operator<<(std::ostream &ostream, const Mat44F &mat) {
  ostream << mat[0] << std::endl;
  ostream << mat[1] << std::endl;
  ostream << mat[2] << std::endl;
  ostream << mat[3] << std::endl;

  return ostream;
}

} // namespace xyz
