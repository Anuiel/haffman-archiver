#include "Archiver/Haffman.h"
#include "Input/BitReader.h"
#include "Archiver/Haffman/HaffmanTree.h"

namespace Haffman {

class Decoder {
public:
    Decoder() {
    }
    void Decode(const FileName& file_name);

private:
    HaffmanTree GetLettersCodes(std::vector<Letter>& letters_in_code_order, std::vector<size_t> count_codes);

    void AddOne(HaffmanCode& code) const;
    void ShiftToLeft(HaffmanCode& code) const;

    char LetterToChar(Letter letter) const;
    int16_t BitStringToInt(const BitString& bit_string) const;
    Letter BitStringToLetter(const BitString& bit_string) const;

private:
    BitReader input_stream_;

private:
    const size_t MAX_SYMBOL_COUNT = 259;
};

}  // namespace Haffman
