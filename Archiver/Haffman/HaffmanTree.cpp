#include "HaffmanTree.h"
#include <deque>

using namespace Haffman;

HaffmanTree::HaffmanNode::HaffmanNode(Letter letter) : letter_(letter), left(nullptr), right(nullptr) {
}

HaffmanTree::HaffmanNode::HaffmanNode(HaffmanNode *a, HaffmanNode *b) : left(a), right(b) {
}

HaffmanTree::HaffmanNode::HaffmanNode() : left(nullptr), right(nullptr) {
}

HaffmanTree::HaffmanTree() {
    root_ = new HaffmanNode();
}

HaffmanTree::HaffmanTree(Letter letter) {
    root_ = new HaffmanNode(letter);
}

HaffmanTree::HaffmanTree(HaffmanTree *left, HaffmanTree *right) {
    root_ = new HaffmanNode(left->root_, right->root_);
}

HaffmanTree::~HaffmanTree() {
    delete root_;
}

std::vector<std::pair<Letter, size_t>> HaffmanTree::GetLettersWithLength() const {
    std::vector<std::pair<Letter, size_t>> ans;
    std::deque<std::pair<size_t, HaffmanNode *>> q;
    q.push_back(std::make_pair(0, root_));
    while (!q.empty()) {
        auto [height, ptr] = q.front();
        q.pop_front();
        if (ptr->left == nullptr) {
            ans.push_back(std::make_pair(ptr->letter_, height));
        } else {
            q.push_back(std::make_pair(height + 1, ptr->left));
            q.push_back(std::make_pair(height + 1, ptr->right));
        }
    }
    return ans;
}

void HaffmanTree::AddBitString(BitString &bit_string, Letter letter) {
    HaffmanNode *cur_node = root_;
    for (const auto bit : bit_string) {
        if (bit) {
            if (cur_node->right == nullptr) {
                cur_node->right = new HaffmanNode();
            }
            cur_node = cur_node->right;
        } else {
            if (cur_node->left == nullptr) {
                cur_node->left = new HaffmanNode();
            }
            cur_node = cur_node->left;
        }
    }
    cur_node->letter_ = letter;
}

Letter HaffmanTree::ReadLetter(BitReader& input_stream, Letter &letter) const {
    auto code_end = root_;
    while (true) {
        bool new_bit;
        if (!input_stream.ReadBit(new_bit)) {
            throw std::runtime_error(input_stream.GetInputFileName() + " is not an archive.");
        }
        code_end = (new_bit ? code_end->right : code_end->left);
        if (code_end->left == nullptr) {
            letter = code_end->letter_;
            return letter;
        }
    }
}
