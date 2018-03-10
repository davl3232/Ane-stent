#ifndef TO_STRING_H
#define TO_STRING_H

#include <sstream>

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
};
#endif
