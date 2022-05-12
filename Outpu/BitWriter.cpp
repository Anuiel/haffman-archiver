#include "BitWriter.h"

BitWriter::BitWriter(const std::string &file_name) {
    out_.open(file_name, std::ios::binary);
    buf_ = 0;
    bit_written_ = 0;
}

BitWriter::BitWriter() : buf_(0), bit_written_(0) {}

void BitWriter::NewOutputFile(const std::string &file_name) {
    out_.open(file_name, std::ios::binary);
}

BitWriter::~BitWriter() {
    Flush();
    out_.close();
}

void BitWriter::Write(bool c) {
    buf_ |= (c << bit_written_);
    ++bit_written_;
    if (bit_written_ == 8) {
        out_.write(reinterpret_cast<char *>(&buf_), sizeof buf_);
        buf_ = 0;
        bit_written_ = 0;
    }
}

void BitWriter::Write(char ch) {
    char to_out = buf_ | (ch << bit_written_);
    out_.write(reinterpret_cast<char *>(&to_out), sizeof to_out);
    buf_ = ch << (8 - bit_written_);
}

void BitWriter::Write(size_t val, size_t bits_to_write) {
    for (size_t i = 0; i < 9; ++i) {
        Write(static_cast<bool>((val >> i) & 1));
    }
}

void BitWriter::Write(std::vector<bool> &bit_string) {
    for (const auto c: bit_string) {
        Write(c);
    }
}

void BitWriter::Flush() {
    if (bit_written_ > 0) {
        out_.write(reinterpret_cast<char *>(&buf_), sizeof buf_);
        buf_ = 0;
        bit_written_ = 0;
    }
}
