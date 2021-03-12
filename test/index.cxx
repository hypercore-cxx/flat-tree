#include "../index.hxx"

#include <iostream>
#include <vector>
#include <string>
#include <random>

#define ASSERT(message, ...) do { \
  if(!(__VA_ARGS__)) { \
    std::cerr << "FAIL: " << message << std::endl; \
    exit(1); \
  } \
  else { \
    std::cout << "OK: " << message << std::endl; \
    count = count + 1; \
  } \
} while(0);

int main() {
  namespace FlatTree = Hyper::Core::FlatTree;

  int plan = 111;
  int count = 0;

  ASSERT("base blocks 0", FlatTree::index(0, 0) == 0)
  ASSERT("base blocks 2", FlatTree::index(0, 1) == 2)
  ASSERT("base blocks 4", FlatTree::index(0, 2) == 4)

  ASSERT("parents 1", FlatTree::index(1, 0) == 1)
  ASSERT("parents 5", FlatTree::index(1, 1) == 5)
  ASSERT("parents 3", FlatTree::index(2, 0) == 3)

  ASSERT("parent 0", FlatTree::parent(0) == 1)
  ASSERT("parent 1", FlatTree::parent(2) == 1)
  ASSERT("parent 3", FlatTree::parent(1) == 3)

  //
  // children(0) does not return null, instead it returns an empty vector.
  // this deviates from the original implementation.
  //
  ASSERT("children 0", FlatTree::children(0).empty())
  ASSERT("children 1", FlatTree::children(1) == std::vector<size_t> { 0, 2 })
  ASSERT("children 3", FlatTree::children(3) == std::vector<size_t> { 1, 5 })
  ASSERT("children 9", FlatTree::children(9) == std::vector<size_t> { 8, 10 })

  ASSERT("leftChild 0", FlatTree::leftChild(0) == -1)
  ASSERT("leftChild 1", FlatTree::leftChild(1) == 0)
  ASSERT("leftChild 3", FlatTree::leftChild(3) == 1)

  ASSERT("rightChild 0", FlatTree::rightChild(0) == -1)
  ASSERT("rightChild 2", FlatTree::rightChild(1) == 2)
  ASSERT("rightChild 3", FlatTree::rightChild(3) == 5)

  ASSERT("sibling 0", FlatTree::sibling(0) == 2)
  ASSERT("sibling 2", FlatTree::sibling(2) == 0)
  ASSERT("sibling 1", FlatTree::sibling(1) == 5)
  ASSERT("sibling 5", FlatTree::sibling(5) == 1)

  ASSERT("fullRoots 0", FlatTree::fullRoots(0).empty())
  ASSERT("fullRoots 2", FlatTree::fullRoots(2) == std::vector<size_t> { 0 })
  ASSERT("fullRoots 8", FlatTree::fullRoots(8) == std::vector<size_t> { 3 })
  ASSERT("fullRoots 20", FlatTree::fullRoots(20) == std::vector<size_t> { 7, 17 })
  ASSERT("fullRoots 18", FlatTree::fullRoots(18) == std::vector<size_t> { 7, 16 })
  ASSERT("fullRoots 16", FlatTree::fullRoots(16) == std::vector<size_t> { 7 })

  ASSERT("depth 0", FlatTree::depth(0) == 0)
  ASSERT("depth 1", FlatTree::depth(1) == 1)
  ASSERT("depth 2", FlatTree::depth(2) == 0)
  ASSERT("depth 3", FlatTree::depth(3) == 2)
  ASSERT("depth 4", FlatTree::depth(4) == 0)

  ASSERT("offset 0", FlatTree::offset(0) == 0)
  ASSERT("offset 1", FlatTree::offset(1) == 0)
  ASSERT("offset 2", FlatTree::offset(2) == 1)
  ASSERT("offset 3", FlatTree::offset(3) == 0)
  ASSERT("offset 4", FlatTree::offset(4) == 2)

  ASSERT("spans 0", FlatTree::spans(0) == std::vector<size_t> { 0, 0 })
  ASSERT("spans 1", FlatTree::spans(1) == std::vector<size_t> { 0, 2 })
  ASSERT("spans 3", FlatTree::spans(3) == std::vector<size_t> { 0, 6 })
  ASSERT("spans 23", FlatTree::spans(23) == std::vector<size_t> { 16, 30 })
  ASSERT("spans 27", FlatTree::spans(27) == std::vector<size_t> { 24, 30 })

  ASSERT("leftSpan 0", FlatTree::leftSpan(0) == 0)
  ASSERT("leftSpan 1", FlatTree::leftSpan(1) == 0)
  ASSERT("leftSpan 3", FlatTree::leftSpan(3) == 0)
  ASSERT("leftSpan 23", FlatTree::leftSpan(23) == 16)
  ASSERT("leftSpan 27", FlatTree::leftSpan(27) == 24)

  ASSERT("rightSpan 0", FlatTree::rightSpan(0) == 0)
  ASSERT("rightSpan 1", FlatTree::rightSpan(1) == 2)
  ASSERT("rightSpan 3", FlatTree::rightSpan(3) == 6)
  ASSERT("rightSpan 23", FlatTree::rightSpan(23) == 30)
  ASSERT("rightSpan 27", FlatTree::rightSpan(27) == 30)

  ASSERT("count 0", FlatTree::count(0) == 1)
  ASSERT("count 1", FlatTree::count(1) == 3)
  ASSERT("count 3", FlatTree::count(3) == 7)
  ASSERT("count 5", FlatTree::count(5) == 3)
  ASSERT("count 23", FlatTree::count(23) == 15)
  ASSERT("count 27", FlatTree::count(27) == 7)
  
  ASSERT("parent > int32", FlatTree::parent(10000000000) == 10000000001)

  ASSERT("twopow 34", FlatTree::twoPow(34) == 17179869184)
  ASSERT("twopow 63", FlatTree::twoPow(63) == 9223372036854775808)

  {
    size_t child = 0;

    for (int i = 0; i < 50; i++) {
      child = FlatTree::parent(child);
    }

    ASSERT("child 1125899906842623", child == 1125899906842623)
    for (int j = 0; j < 50; j++) {
      child = FlatTree::leftChild(child);
    }

    ASSERT("child 0", child == 0)
  }

  {
    FlatTree::Iterator it;

    ASSERT("iterator.index == 0", it.index == 0)
    ASSERT("iterator.parent() == 1", it.parent() == 1)
    ASSERT("iterator.parent() == 3", it.parent() == 3)
    ASSERT("iterator.parent() == 7", it.parent() == 7)
    ASSERT("iterator.rightChild() == 11", it.rightChild() == 11)
    ASSERT("iterator.leftChild() == 9", it.leftChild() == 9)
    ASSERT("iterator.next() == 13", it.next() == 13)
    ASSERT("iterator.leftSpan() == 12", it.leftSpan() == 12)
  }

  {
    FlatTree::Iterator it(1);

    ASSERT("nonleaf index == 1", it.index == 1)
    ASSERT("nonleaf parent() == 3", it.parent() == 3)
    ASSERT("nonleaf parent() == 7", it.parent() == 7)
    ASSERT("nonleaf rightChild() == 11", it.rightChild() == 11)
    ASSERT("nonleaf leftChild == 9", it.leftChild() == 9)
    ASSERT("nonleaf next() == 13", it.next() == 13)
    ASSERT("nonleaf leftSpan() == 12", it.leftSpan() == 12)
  }

  {
    FlatTree::Iterator it(0);

    ASSERT("iterator fullRoot(0) == false", it.fullRoot(0) == false)

    ASSERT("iterator fullRoot(22) == true", it.fullRoot(22) == true)
    ASSERT("iterator index == 7", it.index == 7)

    ASSERT("iterator nextTree() == 16", it.nextTree() == 16)

    ASSERT("iterator fullRoot(22) == true", it.fullRoot(22) == true)
    ASSERT("iterator index == 17", it.index == 17)

    ASSERT("iterator nextTree() == 20", it.nextTree() == 20)

    ASSERT("iterator fullRoot(22) == true", it.fullRoot(22) == true)
    ASSERT("iterator index == 20", it.index == 20)

    ASSERT("iterator nextTree() == 22", it.nextTree() == 22)
    ASSERT("iterator.fullRoot(22) == false", it.fullRoot(22) == false)
  }

  {
    std::random_device randdev;
    std::mt19937 randgen(randdev());
    for (int i = 0; i < 10; i++) {
        FlatTree::Iterator it(0);
        size_t tree = static_cast<size_t>(std::generate_canonical<double, 32>(randgen) * 0xffffffff) * 2;
        std::vector<size_t> expected = FlatTree::fullRoots(tree);
        std::vector<size_t> actual;

        for (; it.fullRoot(tree); it.nextTree()) {
            actual.push_back(it.index);
        }

        ASSERT("big full root " << i << " actual == expected", actual == expected);
        ASSERT("big full root " << i << " fullRoot(tree) == false", it.fullRoot(tree) == false);
    }
  }

  ASSERT("PLAN == COUNT", plan == count)

  {
    std::vector<std::string> list(4);

    auto i = FlatTree::index(0, 0); // get array index for depth: 0, offset: 0
    auto j = FlatTree::index(1, 0); // get array index for depth: 2, offset: 0

    // use these indexes to store some data

    list[i] = "a";
    list[j] = "b";

    auto p = FlatTree::parent(j);
    list[p] = "parent of a and b";

    for (const auto& i: list) {
      std::cout << i << ' ' << std::endl;
    }
  }
}
