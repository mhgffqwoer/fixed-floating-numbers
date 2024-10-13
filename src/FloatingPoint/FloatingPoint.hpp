#pragma once
#include <stdint.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

class FloatingPoint_f {
  static const uint32_t kNaN = 0x7fc00000;
  static const uint32_t kInf = 0x7f800000;
  static const uint32_t kNInf = 0xff800000;
  static const uint32_t kZero = 0x00000000;
  static const uint32_t kNZero = 0x80000000;

 private:
  uint32_t number;
  int rounding;

 public:
  FloatingPoint_f(uint32_t number, int rounding);

  ~FloatingPoint_f() = default;
  FloatingPoint_f(const FloatingPoint_f&) = default;
  FloatingPoint_f& operator=(const FloatingPoint_f&) = default;

  FloatingPoint_f& operator+(const FloatingPoint_f& other);

  FloatingPoint_f& operator-(const FloatingPoint_f& other);

  FloatingPoint_f& operator*(const FloatingPoint_f& other);

  FloatingPoint_f& operator/(const FloatingPoint_f& other);

  uint32_t Rounding(uint64_t mant, int sign);

  uint32_t TowardZero(uint64_t mant);

  uint32_t NearestEven(uint64_t mant);

  uint32_t TowardInfinity(uint64_t mant, int sign);

  uint32_t TowardNegInfinity(uint64_t mant, int sign);

  std::string toHex(uint32_t mant);

  void PrintDec();
};

class FloatingPoint_h {
  static const uint32_t kNaN = 0x7e00;
  static const uint32_t kInf = 0x7c00;
  static const uint32_t kNInf = 0xfc00;
  static const uint32_t kZero = 0x0000;
  static const uint32_t kNZero = 0x8000;

 private:
  uint16_t number;
  int rounding;

 public:
  FloatingPoint_h(uint32_t number, int rounding);

  ~FloatingPoint_h() = default;
  FloatingPoint_h(const FloatingPoint_h&) = default;
  FloatingPoint_h& operator=(const FloatingPoint_h&) = default;

  FloatingPoint_h& operator+(const FloatingPoint_h& other);

  FloatingPoint_h& operator-(const FloatingPoint_h& other);

  FloatingPoint_h& operator*(const FloatingPoint_h& other);

  FloatingPoint_h& operator/(const FloatingPoint_h& other);

  uint16_t Rounding(uint32_t mant, int sign);

  uint16_t TowardZero(uint32_t mant);

  uint16_t NearestEven(uint32_t mant);

  uint16_t TowardInfinity(uint32_t mant, int sign);

  uint16_t TowardNegInfinity(uint32_t mant, int sign);

  std::string toHex(uint16_t mant);

  void PrintDec();
};