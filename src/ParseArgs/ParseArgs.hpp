#pragma ocne 
#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>
#include <map>

namespace Args {

struct Arguments {
	char format;
	int format_A;
	int format_B;
	int rounding;
	uint32_t number_1;
	char operation;
	uint32_t number_2;
};

Arguments ParseArgs(int argc, char **argv);

}