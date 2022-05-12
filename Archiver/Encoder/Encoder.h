#include "Archiver/Haffman.h"
#include "Input/BitReader.h"
#include "Output/BitWriter.h"

namespace Haffman {

class Encoder {
public:
    Encoder(const FileName &file_name);

    void AddFile(const FileName &file_name, bool is_file_the_last = false);

private:
    std::unordered_map<Letter, size_t> GetSymbolFrequency(BitReader &reader);

    std::vector<std::pair<Letter, size_t>> GetNewSymbolLength(std::unordered_map<Letter, size_t> &frequency);

    std::unordered_map<Letter, HaffmanCode> GetNewSymbolsCode(std::vector<std::pair<Letter, size_t>> &length);

    void AddOne(HaffmanCode &code) const;

    void ShiftToLeft(HaffmanCode &code) const;

    size_t LetterToInt(Letter letter);

private:
    BitWriter out_stream_;
};

}  // namespace Haffman
