#pragma ocne
#include <stdint.h>

#include <iostream>
#include <string>
#include <vector>

class FixedPoint {
 private:
  uint32_t number;
  int format_A;
  int format_B;

 public:
  FixedPoint(uint32_t number, int format_A, int format_B);

  ~FixedPoint() = default;
  FixedPoint(const FixedPoint&) = default;
  FixedPoint& operator=(const FixedPoint&) = default;

  FixedPoint& operator+(const FixedPoint& other);

  FixedPoint& operator-(const FixedPoint& other);

  FixedPoint& operator*(const FixedPoint& other);

  FixedPoint& operator/(const FixedPoint& other);

  uint32_t GetWholePartDec();

  std::string GetFractionalPartDec();

  bool EqualZero(std::string temp);

  std::string TowardZero();

  std::string NearestEven();

  std::string TowardInfinity(bool is_negative);

  std::string TowardNegInfinity(bool is_negative);

  void PrintDec(int rounding);
};