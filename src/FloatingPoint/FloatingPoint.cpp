#include "FloatingPoint.hpp"

FloatingPoint_f::FloatingPoint_f(uint32_t number, int rounding)
    : number(number), rounding(rounding) {}

FloatingPoint_f& FloatingPoint_f::operator+(const FloatingPoint_f& other) {
  if (number == kNZero || number == kZero) {
    number = other.number;
    return *this;
  } else if (other.number == kNZero || other.number == kZero) {
    return *this;
  } else if ((number == kNInf && other.number == kInf) ||
             (number == kInf && other.number == kNInf)) {
    number = kNaN;
    return *this;
  } else if (number == kNInf || number == kInf) {
    return *this;
  } else if (other.number == kNInf || other.number == kInf) {
    number = other.number;
    return *this;
  }
  uint64_t mant1 = (1 << 23) | (number & 0x7fffff);
  uint64_t mant2 = (1 << 23) | (other.number & 0x7fffff);
  int exp1 = (number >> 23 & 0xff) - 127;
  int exp2 = (other.number >> 23 & 0xff) - 127;
  int res_exp;
  int sign = 0;
  if (exp1 > exp2) {
    mant1 <<= exp1 - exp2;
    res_exp = exp2;
  } else {
    mant2 <<= exp2 - exp1;
    res_exp = exp1;
  }
  if (number >> 31 == 1) {
    mant1 = ~mant1 + 1;
  }
  if (other.number >> 31 == 1) {
    mant2 = ~mant2 + 1;
  }
  mant1 += mant2;
  if (mant1 >> 63 == 1) {
    mant1 = ~mant1 + 1;
    sign = 1;
  }
  int i;
  for (i = 0; mant1 >> 63 != 1; ++i) {
    mant1 <<= 1;
    if (i == 64) {
      break;
    }
  }
  mant1 <<= 1;
  res_exp += 167 - i;
  number = (sign << 31) | (res_exp << 23) | Rounding(mant1, sign);
  return *this;
}

FloatingPoint_f& FloatingPoint_f::operator-(const FloatingPoint_f& other) {
  if (number == kNZero || number == kZero) {
    number = other.number;
    return *this;
  } else if (other.number == kNZero || other.number == kZero) {
    return *this;
  } else if ((number == kNInf && other.number == kNInf) ||
             (number == kInf && other.number == kInf)) {
    number = kNaN;
    return *this;
  } else if (number == kNInf || number == kInf) {
    return *this;
  } else if (other.number == kNInf || other.number == kInf) {
    number = other.number;
    return *this;
  }
  uint64_t mant1 = (1 << 23) | (number & 0x7fffff);
  uint64_t mant2 = (1 << 23) | (other.number & 0x7fffff);
  int exp1 = (number >> 23 & 0xff) - 127;
  int exp2 = (other.number >> 23 & 0xff) - 127;
  int res_exp;
  int sign = 0;
  if (exp1 > exp2) {
    mant1 <<= exp1 - exp2;
    res_exp = exp2;
  } else {
    mant2 <<= exp2 - exp1;
    res_exp = exp1;
  }
  if (number >> 31 == 1) {
    mant1 = ~mant1 + 1;
  }
  if (other.number >> 31 == 1) {
    mant2 = ~mant2 + 1;
  }
  mant1 -= mant2;
  if (mant1 >> 63 == 1) {
    mant1 = ~mant1 + 1;
    sign = 1;
  }
  int i;
  for (i = 0; mant1 >> 63 != 1; ++i) {
    mant1 <<= 1;
    if (i == 64) {
      break;
    }
  }
  mant1 <<= 1;
  res_exp += 167 - i;
  number = (sign << 31) | (res_exp << 23) | Rounding(mant1, sign);
  return *this;
}

FloatingPoint_f& FloatingPoint_f::operator*(const FloatingPoint_f& other) {
  int sign = (number >> 31) ^ (other.number >> 31);
  if (number == kNZero || number == kZero) {
    if (other.number == kNInf || other.number == kInf) {
      number = kNaN;
    } else {
      number = (other.number << 1) >> 1;
      number |= sign << 31;
    }
    return *this;
  }
  if (other.number == kNZero || other.number == kZero) {
    if (number == kNInf || number == kInf) {
      number = kNaN;
    } else {
      number = (number << 1) >> 1;
      number |= sign << 31;
    }
    return *this;
  }
  if (number == kNInf || number == kInf || other.number == kNInf ||
      other.number == kInf) {
    number = kInf | sign << 31;
    return *this;
  }
  uint64_t mant1 = (1 << 23) | (number & 0x7fffff);
  uint64_t mant2 = (1 << 23) | (other.number & 0x7fffff);
  mant1 *= mant2;
  uint32_t exp = (number >> 23 & 0xff) + (other.number >> 23 & 0xff) - 127;
  exp += mant1 >> 47;
  mant1 <<= 16;
  if (mant1 >> 63 == 1) {
    mant1 <<= 1;
  } else {
    mant1 <<= 2;
  }
  number = (sign << 31) | (exp << 23) | Rounding(mant1, sign);
  return *this;
}

FloatingPoint_f& FloatingPoint_f::operator/(const FloatingPoint_f& other) {
  int sign = (number >> 31) ^ (other.number >> 31);
  if (other.number == kNZero || other.number == kZero) {
    if (number == kNZero || number == kZero) {
      number = kNaN;
      return *this;
    }
    number = kInf | (sign << 31);
    return *this;
  }
  if (number == kNInf || number == kInf) {
    if (other.number == kNInf || other.number == kInf) {
      number = kNaN;
      return *this;
    }
    number = kInf | (sign << 31);
    return *this;
  }
  uint64_t mant1 = ((1 << 23) | (number & 0x7fffff));
  uint64_t mant2 = (1 << 23) | (other.number & 0x7fffff);
  mant1 = (mant1 << 40) / mant2;
  uint32_t exp = (number >> 23 & 0xff) - (other.number >> 23 & 0xff) + 127;
  if (mant1 >> 40 == 0) {
    exp -= 1;
    mant1 <<= 1;
  }
  mant1 <<= 24;
  number = (sign << 31) | (exp << 23) | Rounding(mant1, sign);
  return *this;
}

uint32_t FloatingPoint_f::Rounding(uint64_t mant, int sign) {
  if (rounding == 0) {
    return TowardZero(mant);
  } else if (rounding == 1) {
    return NearestEven(mant);
  } else if (rounding == 2) {
    return TowardInfinity(mant, sign);
  } else if (rounding == 3) {
    return TowardNegInfinity(mant, sign);
  }
  return 0;
}

uint32_t FloatingPoint_f::TowardZero(uint64_t mant) {
  return static_cast<uint32_t>((mant >> 41) & 0x7fffff);
}

uint32_t FloatingPoint_f::NearestEven(uint64_t mant) {
  if ((mant & 0x1ffffffffff) <= 0xffffffffff) {
    return static_cast<uint32_t>((mant >> 41) & 0x7fffff);
  } else {
    return static_cast<uint32_t>((mant >> 41) & 0x7fffff) + 1;
  }
}

uint32_t FloatingPoint_f::TowardInfinity(uint64_t mant, int sign) {
  if (sign || (mant & 0x1ffffffffff) == 0) {
    return static_cast<uint32_t>((mant >> 41) & 0x7fffff);
  } else {
    return static_cast<uint32_t>((mant >> 41) & 0x7fffff) + 1;
  }
}

uint32_t FloatingPoint_f::TowardNegInfinity(uint64_t mant, int sign) {
  if (!sign || (mant & 0x1ffffffffff) == 0) {
    return static_cast<uint32_t>((mant >> 41) & 0x7fffff);
  } else {
    return static_cast<uint32_t>((mant >> 41) & 0x7fffff) + 1;
  }
}

std::string FloatingPoint_f::toHex(uint32_t mant) {
  std::string result;
  std::map<int, char> hex_map = {{0, '0'},  {1, '1'},  {2, '2'},  {3, '3'},
                                 {4, '4'},  {5, '5'},  {6, '6'},  {7, '7'},
                                 {8, '8'},  {9, '9'},  {10, 'a'}, {11, 'b'},
                                 {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'}};
  result += hex_map[(mant >> 20) & 0xf];
  result += hex_map[(mant >> 16) & 0xf];
  result += hex_map[(mant >> 12) & 0xf];
  result += hex_map[(mant >> 8) & 0xf];
  result += hex_map[(mant >> 4) & 0xf];
  result += hex_map[mant & 0xf];
  return result;
}

void FloatingPoint_f::PrintDec() {
  int exp = (number >> 23 & 0xff) - 127;
  uint32_t mant = (number & 0x7fffff) << 1;
  int sign = number >> 31;
  std::string exp_str;

  if (number == 0) {
    std::cout << "0x0.000000p+0" << std::endl;
    return;
  } else if (exp == -127 && mant == 0 && sign == 1) {
    std::cout << "-0x0.000000p+0" << std::endl;
    return;
  } else if (number == kNaN) {
    std::cout << "NaN" << std::endl;
    return;
  } else if (number == kInf) {
    std::cout << "inf" << std::endl;
    return;
  } else if (number == kNInf) {
    std::cout << "-inf" << std::endl;
    return;
  }

  if (exp < 0) {
    exp_str = "-" + std::to_string(-exp);
  } else {
    exp_str = "+" + std::to_string(exp);
  }

  if (sign) {
    std::cout << "-0x1." << toHex(mant) << "p" << exp_str << std::endl;
  } else {
    std::cout << "0x1." << toHex(mant) << "p" << exp_str << std::endl;
  }
}

FloatingPoint_h::FloatingPoint_h(uint32_t number, int rounding)
    : number(static_cast<uint16_t>(number)), rounding(rounding) {}

FloatingPoint_h& FloatingPoint_h::operator+(const FloatingPoint_h& other) {
  if (number == kNZero || number == kZero) {
    number = other.number;
    return *this;
  } else if (other.number == kNZero || other.number == kZero) {
    return *this;
  } else if ((number == kNInf && other.number == kInf) ||
             (number == kInf && other.number == kNInf)) {
    number = kNaN;
    return *this;
  } else if (number == kNInf || number == kInf) {
    return *this;
  } else if (other.number == kNInf || other.number == kInf) {
    number = other.number;
    return *this;
  }
  uint32_t mant1 = (1 << 10) | (number & 0x3ff);
  uint32_t mant2 = (1 << 10) | (other.number & 0x3ff);
  int exp1 = (number >> 10 & 0x1f) - 15;
  int exp2 = (other.number >> 10 & 0x1f) - 15;
  int res_exp;
  int sign = 0;
  if (exp1 > exp2) {
    mant1 <<= exp1 - exp2;
    res_exp = exp2;
  } else {
    mant2 <<= exp2 - exp1;
    res_exp = exp1;
  }
  if (number >> 15 == 1) {
    mant1 = ~mant1 + 1;
  }
  if (other.number >> 15 == 1) {
    mant2 = ~mant2 + 1;
  }
  mant1 += mant2;
  if (mant1 >> 31 == 1) {
    mant1 = ~mant1 + 1;
    sign = 1;
  }
  int i;
  for (i = 0; mant1 >> 31 != 1; ++i) {
    mant1 <<= 1;
    if (i == 32) {
      break;
    }
  }
  mant1 <<= 1;
  res_exp += 47 - i;
  number = (sign << 15) | (res_exp << 10) | Rounding(mant1, sign);
  return *this;
}

FloatingPoint_h& FloatingPoint_h::operator-(const FloatingPoint_h& other) {
  if (number == kNZero || number == kZero) {
    number = other.number;
    return *this;
  } else if (other.number == kNZero || other.number == kZero) {
    return *this;
  } else if ((number == kNInf && other.number == kNInf) ||
             (number == kInf && other.number == kInf)) {
    number = kNaN;
    return *this;
  } else if (number == kNInf || number == kInf) {
    return *this;
  } else if (other.number == kNInf || other.number == kInf) {
    number = other.number;
    return *this;
  }
  uint32_t mant1 = (1 << 10) | (number & 0x3ff);
  uint32_t mant2 = (1 << 10) | (other.number & 0x3ff);
  int exp1 = (number >> 10 & 0x1f) - 15;
  int exp2 = (other.number >> 10 & 0x1f) - 15;
  int res_exp;
  int sign = 0;
  if (exp1 > exp2) {
    mant1 <<= exp1 - exp2;
    res_exp = exp2;
  } else {
    mant2 <<= exp2 - exp1;
    res_exp = exp1;
  }
  if (number >> 15 == 1) {
    mant1 = ~mant1 + 1;
  }
  if (other.number >> 15 == 1) {
    mant2 = ~mant2 + 1;
  }
  mant1 -= mant2;
  if (mant1 >> 31 == 1) {
    mant1 = ~mant1 + 1;
    sign = 1;
  }
  int i;
  for (i = 0; mant1 >> 31 != 1; ++i) {
    mant1 <<= 1;
    if (i == 32) {
      break;
    }
  }
  mant1 <<= 1;
  res_exp += 47 - i;
  number = (sign << 15) | (res_exp << 10) | Rounding(mant1, sign);
  return *this;
}

FloatingPoint_h& FloatingPoint_h::operator*(const FloatingPoint_h& other) {
  int sign = (number >> 15) ^ (other.number >> 15);
  if (number == kNZero || number == kZero) {
    if (other.number == kNInf || other.number == kInf) {
      number = kNaN;
    } else {
      number = (other.number << 1) >> 1;
      number |= sign << 15;
    }
    return *this;
  }
  if (other.number == kNZero || other.number == kZero) {
    if (number == kNInf || number == kInf) {
      number = kNaN;
    } else {
      number = (number << 1) >> 1;
      number |= sign << 15;
    }
    return *this;
  }
  if (number == kNInf || number == kInf || other.number == kNInf ||
      other.number == kInf) {
    number = kInf | sign << 15;
    return *this;
  }
  uint32_t mant1 = (1 << 10) | (number & 0x3ff);
  uint32_t mant2 = (1 << 10) | (other.number & 0x3ff);
  mant1 *= mant2;
  uint16_t exp = (number >> 10 & 0x1f) + (other.number >> 10 & 0x1f) - 15;
  exp += mant1 >> 21;
  mant1 <<= 10;
  if (mant1 >> 31 == 1) {
    mant1 <<= 1;
  } else {
    mant1 <<= 2;
  }
  number = (sign << 15) | (exp << 10) | Rounding(mant1, sign);
  return *this;
}

FloatingPoint_h& FloatingPoint_h::operator/(const FloatingPoint_h& other) {
  int sign = (number >> 15) ^ (other.number >> 15);
  if (other.number == kNZero || other.number == kZero) {
    if (number == kNZero || number == kZero) {
      number = kNaN;
      return *this;
    }
    number = kInf | (sign << 15);
    return *this;
  }
  if (number == kNInf || number == kInf) {
    if (other.number == kNInf || other.number == kInf) {
      number = kNaN;
      return *this;
    }
    number = kInf | (sign << 15);
    return *this;
  }
  uint32_t mant1 = ((1 << 10) | (number & 0x3ff));
  uint32_t mant2 = (1 << 10) | (other.number & 0x3ff);
  mant1 = (mant1 << 22) / mant2;
  uint16_t exp = (number >> 10 & 0x1f) - (other.number >> 10 & 0x1f) + 15;
  if (mant1 >> 22 == 0) {
    exp -= 1;
    mant1 <<= 1;
  }
  mant1 <<= 10;
  number = (sign << 15) | (exp << 10) | Rounding(mant1, sign);
  return *this;
}

uint16_t FloatingPoint_h::Rounding(uint32_t mant, int sign) {
  if (rounding == 0) {
    return TowardZero(mant);
  } else if (rounding == 1) {
    return NearestEven(mant);
  } else if (rounding == 2) {
    return TowardInfinity(mant, sign);
  } else if (rounding == 3) {
    return TowardNegInfinity(mant, sign);
  }
  return 0;
}

uint16_t FloatingPoint_h::TowardZero(uint32_t mant) {
  return static_cast<uint16_t>((mant >> 22) & 0x3ff);
}

uint16_t FloatingPoint_h::NearestEven(uint32_t mant) {
  if ((mant & 0x3fffff) <= 0xfffff) {
    return static_cast<uint16_t>((mant >> 22) & 0x3ff);
  } else {
    return static_cast<uint16_t>((mant >> 22) & 0x3ff) + 1;
  }
}

uint16_t FloatingPoint_h::TowardInfinity(uint32_t mant, int sign) {
  if (sign || (mant & 0x3fffff) == 0) {
    return static_cast<uint16_t>((mant >> 22) & 0x3ff);
  } else {
    return static_cast<uint16_t>((mant >> 22) & 0x3ff) + 1;
  }
}

uint16_t FloatingPoint_h::TowardNegInfinity(uint32_t mant, int sign) {
  if (!sign || (mant & 0x3fffff) == 0) {
    return static_cast<uint16_t>((mant >> 22) & 0x3ff);
  } else {
    return static_cast<uint16_t>((mant >> 22) & 0x3ff) + 1;
  }
}

std::string FloatingPoint_h::toHex(uint16_t mant) {
  std::string result;
  std::map<int, char> hex_map = {{0, '0'},  {1, '1'},  {2, '2'},  {3, '3'},
                                 {4, '4'},  {5, '5'},  {6, '6'},  {7, '7'},
                                 {8, '8'},  {9, '9'},  {10, 'a'}, {11, 'b'},
                                 {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'}};
  result += hex_map[(mant >> 8) & 0xf];
  result += hex_map[(mant >> 4) & 0xf];
  result += hex_map[mant & 0xf];
  return result;
}

void FloatingPoint_h::PrintDec() {
  int exp = (number >> 10 & 0x1f) - 15;
  uint32_t mant = (number & 0x3ff) << 2;
  int sign = number >> 15;
  std::string exp_str;

  if (number == 0) {
    std::cout << "0x0.000p+0" << std::endl;
    return;
  } else if (exp == -15 && mant == 0 && sign == 1) {
    std::cout << "-0x0.000p+0" << std::endl;
    return;
  } else if (number == kNaN) {
    std::cout << "NaN" << std::endl;
    return;
  } else if (number == kInf) {
    std::cout << "inf" << std::endl;
    return;
  } else if (number == kNInf) {
    std::cout << "-inf" << std::endl;
    return;
  }

  if (exp < 0) {
    exp_str = "-" + std::to_string(-exp);
  } else {
    exp_str = "+" + std::to_string(exp);
  }

  if (sign) {
    std::cout << "-0x1." << toHex(mant) << "p" << exp_str << std::endl;
  } else {
    std::cout << "0x1." << toHex(mant) << "p" << exp_str << std::endl;
  }
}
