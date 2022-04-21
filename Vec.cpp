#include "Vec.h"

namespace xyz {

std::ostream &operator<<(std::ostream &ostream, const Vec4F vec) {
  return ostream << "{" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", "
                 << vec[3] << "}";
}

} // namespace xyz
