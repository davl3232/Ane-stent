#ifndef TO_STRING_H
#define TO_STRING_H

#include <btBulletDynamicsCommon.h>
#include <sstream>
#include <string>

class ToString {
 public:
  template <typename T>
  static inline std::string MatArr(T *mat) {
    std::stringstream sstr;
    sstr << "Mat: {";
    for (size_t i = 0; i < 16; i++) {
      if (i % 4 == 0) {
        sstr << std::endl;
      } else {
        sstr << ',';
      }
      sstr << mat[i];
    }
    sstr << std::endl;
    sstr << "}";

    return sstr.str();
  }
  template <typename T>
  static inline std::string Vec3Arr(T *vec) {
    std::stringstream sstr;
    sstr << "(" << vec[0] << "," << vec[1] << "," << vec[2] << ")";
    return sstr.str();
  }
  static inline std::string btVector3(btVector3 vec) {
    std::stringstream sstr;
    sstr << "(" << vec.getX() << "," << vec.getY() << "," << vec.getZ() << ")";
    return sstr.str();
  }
};
#endif