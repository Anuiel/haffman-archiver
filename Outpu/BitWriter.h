#pragma once

#include <string>
#include <vector>
#include <fstream>

class BitWriter {
public:
    BitWriter(const std::string &file_name);
    BitWriter();

    void NewOutputFile(const std::string &file_name);

    void Write(char ch);
    void Write(bool c);
    void Write(std::vector<bool> &bit_string);
    void Write(size_t val, size_t bits_to_write);

    ~BitWriter();

private:
    void Flush();

private:
    char buf_;
    size_t bit_written_;
    std::ofstream out_;
};
