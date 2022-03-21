#include "Decoder.h"

using namespace Haffman;

char Decoder::LetterToChar(Letter letter) const {
    return letter.to_ulong();
}

int16_t Decoder::BitStringToInt(const BitString& bit_string) const {
    int16_t ans = 0;
    for (size_t i = 0; i < LETTER_SIZE; ++i) {
        ans += (bit_string[i] << i);
    }
    return ans;
}

Letter Decoder::BitStringToLetter(const BitString& bit_string) const {
    Letter ans;
    for (size_t i = 0; i < LETTER_SIZE; ++i) {
        ans[i] = bit_string[i];
    }
    return ans;
}

void Decoder::AddOne(HaffmanCode& code) const {
    size_t i = code.size() - 1;
    while (true) {
        if (code[i] == 0) {
            code[i] = 1;
            break;
        } else {
            code[i] = 0;
        }
        if (i == 0) {
            break;
        }
        --i;
    }
}

void Decoder::ShiftToLeft(HaffmanCode& code) const {
    code.push_back(0);
}

HaffmanTree Decoder::GetLettersCodes(std::vector<Letter>& letters_in_code_order, std::vector<size_t> count_codes) {
    HaffmanTree root;
    std::vector<bool> code = {0};
    size_t i = 0;
    size_t count = 0;
    for (const auto& letter : letters_in_code_order) {
        while (count_codes[i] == count) {
            ++i;
            count = 0;
        }
        while (code.size() != i + 1) {
            ShiftToLeft(code);
        }
        root.AddBitString(code, letter);
        AddOne(code);
        ++count;
    }
    return root;
}

void Decoder::Decode(const FileName& file_name) {
    input_stream_.NewFile(file_name);
    bool is_the_last_file = true;
    std::vector<bool> str;
    do {
        if (!input_stream_.ReadBites(str, LETTER_SIZE)) {
            throw std::runtime_error(file_name + " is not an archive.");
        }
        size_t symbols_count = BitStringToInt(str);
        if (symbols_count > MAX_SYMBOL_COUNT) {
            throw std::runtime_error(file_name + " is not an archive.");
        }
        std::vector<Letter> letters;
        for (size_t i = 0; i < symbols_count; ++i) {
            if (!input_stream_.ReadBites(str, LETTER_SIZE)) {
                throw std::runtime_error(file_name + " is not an archive.");
            }
            letters.push_back(BitStringToLetter(str));
        }
        size_t numbers_count = 0;
        std::vector<size_t> count_codes;
        while (numbers_count < symbols_count) {
            if (!input_stream_.ReadBites(str, LETTER_SIZE)) {
                throw std::runtime_error(file_name + " is not an archive.");
            }
            size_t count = BitStringToInt(str);
            count_codes.push_back(count);
            numbers_count += count;
        }
        if (numbers_count != symbols_count) {
            throw std::runtime_error(file_name + " is not an archive.");
        }
        HaffmanTree root = GetLettersCodes(letters, count_codes);
        FileName output_file_name;
        Letter cur;
        root.ReadLetter(input_stream_, cur);
        while (cur != FILENAME_END) {
            output_file_name += LetterToChar(cur);
            root.ReadLetter(input_stream_, cur);
        }
        std::ofstream output_stream;
        output_stream.open(output_file_name);

        root.ReadLetter(input_stream_, cur);
        while (cur != ONE_MORE_FILE && cur != ARCHIVE_END) {
            output_stream << LetterToChar(cur);
            root.ReadLetter(input_stream_, cur);
        }

        if (cur == ONE_MORE_FILE) {
            is_the_last_file = false;
        } else {
            is_the_last_file = true;
        }

    } while (!is_the_last_file);
}
