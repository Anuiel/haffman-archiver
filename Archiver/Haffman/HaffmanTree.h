#include "Archiver/Haffman.h"
#include "Input/BitReader.h"

namespace Haffman {

class Heap;

class HaffmanTree {
public:
    HaffmanTree(Letter letter);
    HaffmanTree(HaffmanTree *left, HaffmanTree *right);
    HaffmanTree();
    ~HaffmanTree();

    void AddBitString(BitString &bit_string, Letter letter);
    Letter ReadLetter(BitReader &input_stream, Letter &letter) const;
    std::vector<std::pair<Letter, size_t>> GetLettersWithLength() const;

private:
    struct HaffmanNode {
        Letter letter_;
        HaffmanNode *left;
        HaffmanNode *right;

        ~HaffmanNode() {
            if (left != nullptr) {
                delete left;
            }
            if (right != nullptr) {
                delete right;
            }
        };
        HaffmanNode();
        HaffmanNode(Letter letter);
        HaffmanNode(HaffmanNode *a, HaffmanNode *b);
    };

    HaffmanNode *root_;
};

}  // namespace Haffman
