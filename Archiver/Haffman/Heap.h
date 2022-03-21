#include "Archiver/Haffman.h"

namespace Haffman {

class HaffmanTree;

using Element = std::pair<size_t, HaffmanTree*>;

class Heap {
public:
    Heap();
    void AddElement(Element a);
    Element ExtractMin();
    size_t Size() const;

private:
    size_t GetLeftChild(size_t index) const;
    size_t GetRightChild(size_t index) const;

    std::vector<Element> heap_;
};

}  // namespace Haffman
