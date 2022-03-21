#pragma once

#include <bitset>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <exception>

namespace Haffman {
const size_t LETTER_SIZE = 9;

using BitString = std::vector<bool>;
using HaffmanCode = std::vector<bool>;

using Letter = std::bitset<LETTER_SIZE>;

using FileName = std::string;

const Letter FILENAME_END = 256;
const Letter ONE_MORE_FILE = 257;
const Letter ARCHIVE_END = 258;
}  // namespace Haffman
