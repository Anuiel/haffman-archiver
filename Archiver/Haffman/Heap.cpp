#include "Heap.h"

using namespace Haffman;

Heap::Heap() {
}

size_t Heap::GetLeftChild(size_t index) const {
    return 2 * index + 1;
}

size_t Heap::GetRightChild(size_t index) const {
    return 2 * index + 2;
}

void Heap::AddElement(Element a) {
    size_t index = heap_.size();
    heap_.push_back(a);
    while (index != 0) {
        if (heap_[index].first < heap_[(index - 1) >> 1].first) {
            swap(heap_[index], heap_[(index - 1) >> 1]);
        }
        index = (index - 1) >> 1;
    }
}

Element Heap::ExtractMin() {
    auto ans = heap_[0];
    heap_[0] = heap_.back();
    heap_.pop_back();
    size_t index = 0;
    while (GetLeftChild(index) < heap_.size()) {
        size_t left = GetLeftChild(index);
        size_t right = GetRightChild(index);
        size_t min_index = left;
        if (right < heap_.size() && heap_[right].first < heap_[left].first) {
            min_index = right;
        }
        if (heap_[index].first <= heap_[min_index].first) {
            break;
        }
        swap(heap_[min_index], heap_[index]);
        index = min_index;
    }
    return ans;
}

size_t Heap::Size() const {
    return heap_.size();
}
