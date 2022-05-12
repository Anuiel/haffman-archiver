#pragma once

#include <string>
#include <vector>
#include <fstream>

class BitReader {
public:
    BitReader() {}
    BitReader(const std::string& input_file);
    bool ReadByte(char& byte);
    bool ReadBit(bool& bit);
    bool ReadBites(std::vector<bool>& bit_string, size_t count);
    void Reset();
    void NewFile(const std::string& file_name);
    std::string GetInputFileName() const;

private:
    bool Eof() const;

private:
    std::string input_file_;
    std::ifstream in_;
    char buf_;
    size_t bites_read_;
};
