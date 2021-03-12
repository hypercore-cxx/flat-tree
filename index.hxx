#ifndef HYPER_CORE_FLAT_TREE_H
#define HYPER_CORE_FLAT_TREE_H

#include <vector>
#include <memory>

namespace Hyper {
  namespace Core {
    namespace FlatTree {
      size_t constexpr T0 = 0;
      size_t constexpr T1 = 1;
      size_t constexpr T2 = 2;
      size_t constexpr T30 = 30;
      size_t constexpr T31 = 31;

      auto rightShift (size_t n) {
        return (n - (n & T1)) / T2;
      }

      auto depth (size_t index) {
        size_t depth = T0;

        index += T1;
        while (!static_cast<bool>(index & T1)) {
          depth++;
          index = rightShift(index);
        }

        return depth;
      }

      auto twoPow (size_t n) {
        if (n < T31) {
          return T1 << n;
        } else {
          return ((T1 << T30) * (T1 << (n - T30)));
        }
      }

      auto index (size_t depth, size_t offset) {
        return (T1 + T2 * offset) * twoPow(depth) - T1;
      }

      auto count (size_t index, size_t depth) {
        if (!static_cast<bool>(index & T1)) {
          return T1;
        }

        return twoPow(depth + T1) - T1;
      }
      
      auto count (size_t index) {
        auto depth = FlatTree::depth(index);
        return FlatTree::count(index, depth);
      }

      auto offset (size_t index, size_t depth) {
        if (!static_cast<bool>(index & T1)) {
          return index / T2;
        }

        if (depth == 0) {
          depth = FlatTree::depth(index);
        }

        return ((index + T1) / FlatTree::twoPow(depth) - T1) / T2;
      }

      auto offset (size_t index) {
        size_t depth = T0;

        if (!static_cast<bool>(index & T1)) {
          return index / T2;
        }

        if (depth == 0) {
          depth = FlatTree::depth(index);
        }

        return ((index + T1) / FlatTree::twoPow(depth) - T1) / T2;
      }

      auto leftChild (size_t index, size_t depth) {
        if (!static_cast<bool>(index & 1)) {
          return -T1;
        }

        auto offset = FlatTree::offset(index, depth) * T2;
        return FlatTree::index(depth - T1, offset);
      }

      auto leftChild (size_t index) {
        auto depth = FlatTree::depth(index);
        return leftChild(index, depth);
      }

      auto rightChild (size_t index, size_t depth) {
        if (!static_cast<bool>(index & T1)) {
          return -T1;
        }

        auto offset = FlatTree::offset(index, depth) * T2;
        return FlatTree::index(depth - T1, T1 + offset);
      }

      auto rightChild (size_t index) {
        auto depth = FlatTree::depth(index);
        return rightChild(index, depth);
      }

      auto children (size_t index, size_t depth) {
        std::vector<size_t> result;

        if (!static_cast<bool>(index & T1)) {
          return result;
        }

        auto offset = FlatTree::offset(index, depth) * T2;

        result.push_back(FlatTree::index(depth - T1, offset));
        result.push_back(FlatTree::index(depth - T1, offset + T1));

        return result;
      }

      auto children (size_t index) {
        auto depth = FlatTree::depth(index);
        return children(index, depth);
      }

      auto spans (size_t index, size_t depth) {
        std::vector<size_t> result;

        if (!static_cast<bool>(index & T1)) {
          result.push_back(index);
          result.push_back(index);
          return result;
        }

        if (!static_cast<bool>(depth)) {
          depth = FlatTree::depth(index);
        }

        auto offset = FlatTree::offset(index, depth);
        auto width = FlatTree::twoPow(depth + T1);

        result.push_back(offset * width);
        result.push_back((offset + T1) * width - T2);
        return result;
      }

      auto spans (size_t index) {
        auto depth = FlatTree::depth(index);
        return spans(index, depth);
      }

      auto sibling (size_t index, size_t depth) {
        auto offset = FlatTree::offset(index, depth);
        auto isOffset = static_cast<bool>(offset & T1);

        return FlatTree::index(depth, isOffset ? offset - T1 : offset + T1);
      }

      auto sibling (size_t index) {
        auto depth = FlatTree::depth(index);
        return sibling(index, depth);
      }

      auto parent (size_t index, size_t depth) {
        auto offset = FlatTree::offset(index, depth);
        return FlatTree::index(depth + T1, rightShift(offset));
      }

      auto parent (size_t index) {
        auto depth = FlatTree::depth(index);
        auto offset = FlatTree::offset(index, depth);
        return FlatTree::index(depth + T1, rightShift(offset));
      }

      auto leftSpan (size_t index, size_t depth) {
        if (!static_cast<bool>(index & T1)) {
          return index;
        }

        return FlatTree::offset(index, depth) * twoPow(depth + T1);
      }

      auto leftSpan (size_t index) {
        auto depth = FlatTree::depth(index);
        return leftSpan(index, depth);
      }

      auto rightSpan (size_t index, size_t depth) {
        if (!static_cast<bool>(index & T1)) {
          return index;
        }

        return (FlatTree::offset(index, depth) + T1) * twoPow(depth + T1) - T2;
      }

      auto rightSpan (size_t index) {
        auto depth = FlatTree::depth(index);
        return rightSpan(index, depth);
      }

      auto fullRoots (size_t index, std::vector<size_t> result) {
        if (static_cast<bool>(index & T1)) {
          throw std::runtime_error {
            "You can only look up roots for depth(0) blocks"
          };
        }

        index /= T2;

        auto offset = T0;
        auto factor = T1;

        while (true) {
          if (!static_cast<bool>(index)) {
            return result;
          }

          while (factor * T2 <= index) {
            factor *= T2;
          }

          result.push_back(offset + factor - T1);
          offset = offset + T2 * factor;
          index -= factor;
          factor = T1;
        }
      }

      auto fullRoots (size_t index) {
        std::vector<size_t> result;
        return FlatTree::fullRoots(index, result);
      }

      class Iterator {
        public:

        size_t index = T0;
        size_t offset = T0;
        size_t factor = T0;

        Iterator (size_t index) : index(index) {
          this->seek(index);
        };

        Iterator () {
          this->seek(T0);
        };

        void seek (size_t index) {
          this->index = index;

          if (this->index % 2 != 0) {
            this->offset = FlatTree::offset(index);
            this->factor = twoPow(FlatTree::depth(index) + T1);
          } else {
            this->offset = index / T2;
            this->factor = T2;
          }
        }

        bool isLeft () {
          return this->offset % 2 == 0;
        }

        bool isRight () {
          return !this->isLeft();
        }

        size_t prev () {
          if (!static_cast<bool>(this->offset)) {
            return this->index;
          }

          this->offset--;
          this->index -= this->factor;
          return this->index;
        }

        size_t next () {
          this->offset++;
          this->index += this->factor;
          return this->index;
        }

        size_t sibling () {
          return this->isLeft() ? this->next() : this->prev();
        }

        size_t parent () {
          if (this->offset % T2 != 0) {
            this->index -= this->factor / T2;
            this->offset = (this->offset - T1) / T2;
          } else {
            this->index += this->factor / T2;
            this->offset /= T2;
          }

          this->factor *= T2;
          return this->index;
        }

        size_t leftSpan () {
          this->index = this->index - this->factor / T2 + T1;
          this->offset = this->index / T2;
          this->factor = T2;
          return this->index;
        }

        size_t rightSpan () {
          this->index = this->index + this->factor / T2 - T1;
          this->offset = this->index / T2;
          this->factor = T2;
          return this->index;
        }

        size_t leftChild () {
          if (this->factor == T2) {
            return this->index;
          }

          this->factor /= T2;
          this->index -= this->factor / T2;
          this->offset *= T2;
          return this->index;
        }

        size_t rightChild () {
          if (this->factor == T2) {
            return this->index;
          }

          this->factor /= T2;
          this->index += this->factor / T2;
          this->offset = T2 * this->offset + T1;
          return this->index;
        }

        size_t nextTree () {
          this->index = this->index + this->factor / T2 + T1;
          this->offset = this->index / T2;
          this->factor = T2;
          return this->index;
        }

        size_t prevTree () {
          if (!static_cast<bool>(this->offset)) {
            this->index = 0;
            this->factor = 2;
          } else {
            this->index = this->index - this->factor / T2 - T1;
            this->offset = this->index / T2;
            this->factor = T2;
          }
          return this->index;
        }

        size_t fullRoot (size_t index) {
          if (index <= this->index || (this->index & 1) > T0) {
            return false;
          }
          while (index > this->index + this->factor + this->factor / 2) {
            this->index += this->factor / 2;
            this->factor *= 2;
            this->offset /= 2;
          }
          return true;
        }
      };
    } // namespace FlatTree
  } // namespace Core
} // namespace Hyper

#endif // DAT_FLAT_TREE_H
