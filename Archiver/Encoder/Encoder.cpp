#include "Encoder.h"
#include "Archiver/Haffman/HaffmanTree.h"
#include "Archiver/Haffman/Heap.h"

using namespace Haffman;

Encoder::Encoder(const FileName &file_name) {
    out_stream_.NewOutputFile(file_name);
}

std::unordered_map<Letter, size_t> Encoder::GetSymbolFrequency(BitReader &reader) {
    std::unordered_map<Letter, size_t> frequency;
    char c;
    while (reader.ReadByte(c)) {
        ++frequency[c];
    }
    for (const auto symbol : reader.GetInputFileName()) {
        ++frequency[symbol];
    }
    frequency[FILENAME_END] = 1;
    frequency[ONE_MORE_FILE] = 1;
    frequency[ARCHIVE_END] = 1;
    return frequency;
}

std::vector<std::pair<Letter, size_t>> Encoder::GetNewSymbolLength(std::unordered_map<Letter, size_t> &frequency) {
    Heap heap;
    for (const auto &[letter, count] : frequency) {
        auto ptr = new HaffmanTree(letter);
        heap.AddElement(std::make_pair(count, ptr));
    }
    while (heap.Size() > 1) {
        auto [count1, ptr1] = heap.ExtractMin();
        auto [count2, ptr2] = heap.ExtractMin();
        auto ptr = new HaffmanTree(ptr1, ptr2);
        heap.AddElement(std::make_pair(count1 + count2, ptr));
    }
    auto ptr = heap.ExtractMin().second;
    auto ans = ptr->GetLettersWithLength();
    delete ptr;
    return ans;
}

void Encoder::AddOne(HaffmanCode &code) const {
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

void Encoder::ShiftToLeft(HaffmanCode &code) const {
    code.push_back(0);
}

size_t Encoder::LetterToInt(Letter letter) {
    size_t ans = 0;
    for (size_t i = 0; i < LETTER_SIZE; ++i) {
        ans |= (letter[i] << i);
    }
    return ans;
}

std::unordered_map<Letter, HaffmanCode> Encoder::GetNewSymbolsCode(std::vector<std::pair<Letter, size_t>> &length) {
    std::sort(length.begin(), length.end(), [](const std::pair<Letter, size_t> &a, const std::pair<Letter, size_t> &b) {
        return a.second < b.second;
    });

    std::unordered_map<Letter, HaffmanCode> ans;
    HaffmanCode code = {0};
    for (const auto &[letter, size] : length) {
        while (code.size() != size) {
            ShiftToLeft(code);
        }
        ans[letter] = code;
        AddOne(code);
    }
    return ans;
}

void Encoder::AddFile(const FileName &file_name, bool is_file_the_last) {
    BitReader in_stream(file_name);
    std::unordered_map<Letter, size_t> frequency = GetSymbolFrequency(in_stream);
    std::vector<std::pair<Letter, size_t>> new_length = GetNewSymbolLength(frequency);
    std::unordered_map<Letter, HaffmanCode> codes = GetNewSymbolsCode(new_length);

    std::vector<size_t> codes_size = {0};
    out_stream_.Write(codes.size(), LETTER_SIZE);
    for (const auto &[letter, size] : new_length) {
        while (codes_size.size() != size) {
            codes_size.push_back(0);
        }
        ++codes_size.back();
        out_stream_.Write(LetterToInt(letter), LETTER_SIZE);
    }
    for (const auto size : codes_size) {
        out_stream_.Write(size, LETTER_SIZE);
    }
    for (const auto c : file_name) {
        out_stream_.Write(codes[c]);
    }
    out_stream_.Write(codes[FILENAME_END]);

    in_stream.Reset();

    char c;

    while (in_stream.ReadByte(c)) {
        out_stream_.Write(codes[c]);
    }
    if (!is_file_the_last) {
        out_stream_.Write(codes[ONE_MORE_FILE]);
    } else {
        out_stream_.Write(codes[ARCHIVE_END]);
    }
}
