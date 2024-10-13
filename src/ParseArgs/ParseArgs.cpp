#include "ParseArgs.hpp"

namespace Args {

// Hex -> Dec
uint32_t HexToDec(const std::string& hex) {
  uint32_t result = 0;
  std::map<char, int> hex_map = {
      {'0', 0b0000}, {'1', 0b0001}, {'2', 0b0010}, {'3', 0b0011}, {'4', 0b0100},
      {'5', 0b0101}, {'6', 0b0110}, {'7', 0b0111}, {'8', 0b1000}, {'9', 0b1001},
      {'A', 0b1010}, {'B', 0b1011}, {'C', 0b1100}, {'D', 0b1101}, {'E', 0b1110},
      {'F', 0b1111}, {'a', 0b1010}, {'b', 0b1011}, {'c', 0b1100}, {'d', 0b1101},
      {'e', 0b1110}, {'f', 0b1111}};
  for (char i : hex.substr(2)) {
    result <<= 4;
	result |= hex_map[i];
  }
  return result;
}

// парсер A.B формата
void ParseFormat(const std::string& format, Arguments& result) {
  int idx_dot = format.find('.');
  result.format_A = std::stoi(format.substr(0, idx_dot));
  result.format_B = std::stoi(format.substr(idx_dot + 1));
}

// основной парсер
Arguments ParseArgs(std::vector<std::string> args) {
  Arguments result;

  if (args.size() != 3 && args.size() != 5) {
    std::cerr << "Error: Invalid number of arguments" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (args[0] == "f" || args[0] == "h") {
    result.format = args[0][0];
	result.format_A = 0;
	result.format_B = 0;
  } else {
    result.format = 'a';
    ParseFormat(args[0], result);
  }

  result.rounding = std::stoi(args[1]);
  result.number_1 = HexToDec(args[2]);

  if (args.size() == 5) {
	result.operation = args[3][0];
	result.number_2 = HexToDec(args[4]);
  } else {
	result.operation = '!';
	result.number_2 = 0;
  }

  return result;
}

// видимый парсер
Arguments ParseArgs(int argc, char **argv) {
  std::vector<std::string> args_;
  for (int i = 1; i < argc; i++) {
    args_.push_back(argv[i]);
  }
  return ParseArgs(args_);
}

}  // namespace Args