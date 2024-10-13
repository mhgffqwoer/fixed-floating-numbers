#include "FixedPoint.hpp"

FixedPoint::FixedPoint(uint32_t number, int format_A, int format_B)
    : number(number), format_A(format_A), format_B(format_B) {}

FixedPoint& FixedPoint::operator+(const FixedPoint& other) {
  uint64_t result = number + other.number;
  number = static_cast<uint32_t>(result);
  return *this;
}

FixedPoint& FixedPoint::operator-(const FixedPoint& other) {
  uint64_t result = number - other.number;
  number = static_cast<uint32_t>(result);
  return *this;
}

FixedPoint& FixedPoint::operator*(const FixedPoint& other) {
  uint64_t result = number * other.number;
  result >>= format_B;
  number = static_cast<uint32_t>(result);
  return *this;
}

FixedPoint& FixedPoint::operator/(const FixedPoint& other) {
  if (other.number == 0) {
	std::cout << "division by zero" << std::endl;
	exit(0);
  }
  uint64_t temp = (number << format_B);
  number = static_cast<uint32_t>(temp / other.number);
  return *this;
}

uint32_t FixedPoint::GetWholePartDec() {
  int mask = 1;
  for (int i = 0; i < format_A - 2; ++i) {
    mask = (mask << 1) | 1;
  }
  std::string result = std::to_string((number >> format_B) & mask);
  return static_cast<uint32_t>(stoi(result));
}

std::string FixedPoint::GetFractionalPartDec() {
  uint64_t res = 1;
  uint32_t buff = 5;
  for (size_t i = 0; i < format_B; ++i) {
    res *= 10;
    int mask = (number & (1 << (format_B - i - 1))) >> (format_B - i - 1);
    res += mask * buff;
    buff *= 5;
  }
  std::string result = std::to_string(res);
  if (result.size() < 4) {
    result += "00000";
  }
  return result;
}

bool FixedPoint::EqualZero(std::string temp) {
  for (const auto& i : temp) {
    if (i != '0') {
      return false;
    }
  }
  return true;
}

std::string FixedPoint::TowardZero() {
  return GetFractionalPartDec().substr(1, 3);
}

std::string FixedPoint::NearestEven() {
  std::string temp = GetFractionalPartDec();
  if (stoi(temp.substr(4, 1)) < 5) {
    return temp.substr(1, 3);
  } else if (stoi(temp.substr(4, 1)) < 5) {
    temp = temp.substr(1, 3);
    temp[2] = (temp[2] - 47) % 10 + 48;
    return temp;
  } else if (EqualZero(temp.substr(5))) {
    if (stoi(temp.substr(3, 1)) % 2 == 0) {
      return temp.substr(1, 3);
    } else {
      temp = temp.substr(1, 3);
      temp[2] = (temp[2] - 47) % 10 + 48;
      return temp;
    }
  } else {
    temp = temp.substr(1, 3);
    temp[2] = (temp[2] - 47) % 10 + 48;
    return temp;
  }
}

std::string FixedPoint::TowardInfinity(bool is_negative) {
  std::string temp = GetFractionalPartDec();
  if (EqualZero(temp.substr(4)) || is_negative) {
    return temp.substr(1, 3);
  } else {
    std::string temp = GetFractionalPartDec().substr(1, 3);
    temp[2] = (temp[2] - 47) % 10 + 48;
    return temp;
  }
}

std::string FixedPoint::TowardNegInfinity(bool is_negative) {
  std::string temp = GetFractionalPartDec();

  if (EqualZero(temp.substr(4)) || !is_negative) {
    return temp.substr(1, 3);
  } else {
    std::string temp = GetFractionalPartDec().substr(1, 3);
    temp[2] = (temp[2] - 47) % 10 + 48;
    return temp;
  }
}

void FixedPoint::PrintDec(int rounding) {
  bool is_negative = number >> (format_A + format_B - 1) & 1;
  if (is_negative) {
    std::cout << '-';
    number = ~number + 1;
  }
  if (rounding == 0) {
    std::cout << GetWholePartDec() << '.' << TowardZero() << std::endl;
  } else if (rounding == 1) {
    std::cout << GetWholePartDec() << '.' << NearestEven() << std::endl;
  } else if (rounding == 2) {
    std::cout << GetWholePartDec() << '.' << TowardInfinity(is_negative)
              << std::endl;
  } else if (rounding == 3) {
    std::cout << GetWholePartDec() << '.' << TowardNegInfinity(is_negative)
              << std::endl;
  }
}
