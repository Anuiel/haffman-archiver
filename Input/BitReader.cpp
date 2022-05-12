#include "BitReader.h"

BitReader::BitReader(const std::string& input_file) : input_file_(input_file), buf_(0), bites_read_(8) {
    in_.open(input_file, std::ios::binary);
    if (!in_.is_open()) {
        throw std::runtime_error("Could not open the file " + input_file);
    }
}

void BitReader::NewFile(const std::string& file_name) {
    in_.close();
    in_.open(file_name, std::ios::binary);
    if (!in_.is_open()) {
        throw std::runtime_error("Could not open the file " + file_name);
    }
    input_file_ = file_name;
    buf_ = 0;
    bites_read_ = 8;
}

void BitReader::Reset() {
    in_.close();
    in_.open(input_file_, std::ios::binary);
    buf_ = 0;
    bites_read_ = 8;
}

std::string BitReader::GetInputFileName() const {
    return input_file_;
}

bool BitReader::Eof() const {
    return !in_.good();
}

bool BitReader::ReadByte(char& byte) {
    if (Eof()) {
        return false;
    }
    char c;
    in_.read(reinterpret_cast<char*>(&c), sizeof c);
    if (Eof()) {
        return false;
    }
    size_t other_bites = 8 - bites_read_;
    byte = ((buf_ >> bites_read_) & ((1 << other_bites) - 1)) | (c << other_bites);
    buf_ = c;
    return true;
}

bool BitReader::ReadBit(bool& bit) {
    if (bites_read_ == 8 && Eof()) {
        return false;
    }
    if (bites_read_ == 8) {
        in_.read(reinterpret_cast<char*>(&buf_), sizeof buf_);
        if (Eof()) {
            return false;
        }
        bites_read_ = 0;
    }
    bit = (buf_ >> bites_read_) & 1;
    ++bites_read_;
    return true;
}

// from bit_string[i] = i-th bit
bool BitReader::ReadBites(std::vector<bool>& bit_string, size_t count) {
    bit_string.clear();
    while (count > 8) {
        char c;
        if (!ReadByte(c)) {
            return false;
        }
        for (size_t i = 0; i < 8; ++i) {
            bit_string.push_back((c >> i) & 1);
        }
        count -= 8;
    }
    while (count > 0) {
        bool c;
        if (!ReadBit(c)) {
            return false;
        }
        bit_string.push_back(c);
        --count;
    }
    return true;
}
