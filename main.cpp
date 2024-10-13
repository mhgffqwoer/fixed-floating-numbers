#include <iostream>

#include "src/FixedPoint/FixedPoint.hpp"
#include "src/FloatingPoint/FloatingPoint.hpp"
#include "src/ParseArgs/ParseArgs.hpp"

int main(int argc, char **argv) {
  Args::Arguments args = Args::ParseArgs(argc, argv);

  if (args.format == 'a') {
    FixedPoint my_float1(args.number_1, args.format_A, args.format_B);
    FixedPoint my_float2(args.number_2, args.format_A, args.format_B);

    if (args.operation == '+') {
      my_float1 = my_float1 + my_float2;
    } else if (args.operation == '-') {
      my_float1 = my_float1 - my_float2;
    } else if (args.operation == '*' || args.operation == 'M') {
      my_float1 = my_float1 * my_float2;
    } else if (args.operation == '/') {
      my_float1 = my_float1 / my_float2;
    }

    my_float1.PrintDec(args.rounding);
  } else if (args.format == 'f') {
    FloatingPoint_f my_float1(args.number_1, args.rounding);
    FloatingPoint_f my_float2(args.number_2, args.rounding);

    if (args.operation == '+') {
      my_float1 = my_float1 + my_float2;
    } else if (args.operation == '-') {
      my_float1 = my_float1 - my_float2;
    } else if (args.operation == '*' || args.operation == 'M') {
      my_float1 = my_float1 * my_float2;
    } else if (args.operation == '/') {
      my_float1 = my_float1 / my_float2;
    }

    my_float1.PrintDec();
  } else if (args.format == 'h') {
    FloatingPoint_h my_float1(args.number_1, args.rounding);
    FloatingPoint_h my_float2(args.number_2, args.rounding);

    if (args.operation == '+') {
      my_float1 = my_float1 + my_float2;
    } else if (args.operation == '-') {
      my_float1 = my_float1 - my_float2;
    } else if (args.operation == '*' || args.operation == 'M') {
      my_float1 = my_float1 * my_float2;
    } else if (args.operation == '/') {
      my_float1 = my_float1 / my_float2;
    }

    my_float1.PrintDec();
  }

  return 0;
}