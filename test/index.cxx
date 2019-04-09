#include "../index.hxx"

#include <iostream>
#include <vector>
#include <string>

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
  int plan = 81;
  int count = 0;

  ASSERT("base blocks 0", flatTree::index(0, 0) == 0)
  ASSERT("base blocks 2", flatTree::index(0, 1) == 2)
  ASSERT("base blocks 4", flatTree::index(0, 2) == 4)
  ASSERT("base blocks 4", flatTree::index(0, 2) == 4)

  ASSERT("parents 1", flatTree::index(1, 0) == 1)
  ASSERT("parents 5", flatTree::index(1, 1) == 5)
  ASSERT("parents 3", flatTree::index(2, 0) == 3)

  ASSERT("parent 0", flatTree::parent(0) == 1)
  ASSERT("parent 1", flatTree::parent(2) == 1)
  ASSERT("parent 3", flatTree::parent(1) == 3)

  //
  // children(0) does not return null, instead it returns an empty vector.
  // this deviates from the original implementation.
  //
  ASSERT("children 0", flatTree::children(0).empty())
  ASSERT("children 1", flatTree::children(1) == std::vector<size_t> { 0, 2 })
  ASSERT("children 3", flatTree::children(3) == std::vector<size_t> { 1, 5 })
  ASSERT("children 9", flatTree::children(9) == std::vector<size_t> { 8, 10 })

  ASSERT("leftChild 0", flatTree::leftChild(0) == -1)
  ASSERT("leftChild 1", flatTree::leftChild(1) == 0)
  ASSERT("leftChild 3", flatTree::leftChild(3) == 1)

  ASSERT("rightChild 0", flatTree::rightChild(0) == -1)
  ASSERT("rightChild 2", flatTree::rightChild(1) == 2)
  ASSERT("rightChild 3", flatTree::rightChild(3) == 5)

  ASSERT("sibling 0", flatTree::sibling(0) == 2)
  ASSERT("sibling 2", flatTree::sibling(2) == 0)
  ASSERT("sibling 1", flatTree::sibling(1) == 5)
  ASSERT("sibling 5", flatTree::sibling(5) == 1)

  ASSERT("fullRoots 0", flatTree::fullRoots(0).empty())
  ASSERT("fullRoots 2", flatTree::fullRoots(2) == std::vector<size_t> { 0 })
  ASSERT("fullRoots 8", flatTree::fullRoots(8) == std::vector<size_t> { 3 })
  ASSERT("fullRoots 20", flatTree::fullRoots(20) == std::vector<size_t> { 7, 17 })
  ASSERT("fullRoots 18", flatTree::fullRoots(18) == std::vector<size_t> { 7, 16 })
  ASSERT("fullRoots 16", flatTree::fullRoots(16) == std::vector<size_t> { 7 })

  ASSERT("depth 0", flatTree::depth(0) == 0)
  ASSERT("depth 1", flatTree::depth(1) == 1)
  ASSERT("depth 2", flatTree::depth(2) == 0)
  ASSERT("depth 3", flatTree::depth(3) == 2)
  ASSERT("depth 4", flatTree::depth(4) == 0)

  ASSERT("offset 0", flatTree::offset(0) == 0)
  ASSERT("offset 1", flatTree::offset(1) == 0)
  ASSERT("offset 2", flatTree::offset(2) == 1)
  ASSERT("offset 3", flatTree::offset(3) == 0)
  ASSERT("offset 4", flatTree::offset(4) == 2)

  ASSERT("spans 0", flatTree::spans(0) == std::vector<size_t> { 0, 0 })
  ASSERT("spans 1", flatTree::spans(1) == std::vector<size_t> { 0, 2 })
  ASSERT("spans 3", flatTree::spans(3) == std::vector<size_t> { 0, 6 })
  ASSERT("spans 23", flatTree::spans(23) == std::vector<size_t> { 16, 30 })
  ASSERT("spans 27", flatTree::spans(27) == std::vector<size_t> { 24, 30 })

  ASSERT("leftSpan 0", flatTree::leftSpan(0) == 0)
  ASSERT("leftSpan 1", flatTree::leftSpan(1) == 0)
  ASSERT("leftSpan 3", flatTree::leftSpan(3) == 0)
  ASSERT("leftSpan 23", flatTree::leftSpan(23) == 16)
  ASSERT("leftSpan 27", flatTree::leftSpan(27) == 24)

  ASSERT("rightSpan 0", flatTree::rightSpan(0) == 0)
  ASSERT("rightSpan 1", flatTree::rightSpan(1) == 2)
  ASSERT("rightSpan 3", flatTree::rightSpan(3) == 6)
  ASSERT("rightSpan 23", flatTree::rightSpan(23) == 30)
  ASSERT("rightSpan 27", flatTree::rightSpan(27) == 30)

  ASSERT("count 0", flatTree::count(0) == 1)
  ASSERT("count 1", flatTree::count(1) == 3)
  ASSERT("count 3", flatTree::count(3) == 7)
  ASSERT("count 5", flatTree::count(5) == 3)
  ASSERT("count 23", flatTree::count(23) == 15)
  ASSERT("count 27", flatTree::count(27) == 7)
  
  ASSERT("parent > int32", flatTree::parent(10000000000) == 10000000001)

  ASSERT("twopow 34", flatTree::twoPow(34) == 17179869184)
  ASSERT("twopow 63", flatTree::twoPow(63) == 9223372036854775808)

  {
    size_t child = 0;

    for (int i = 0; i < 50; i++) {
      child = flatTree::parent(child);
    }

    ASSERT("child 1125899906842623", child == 1125899906842623)
    for (int j = 0; j < 50; j++) {
      child = flatTree::leftChild(child);
    }

    ASSERT("child 0", child == 0)
  }

  {
    flatTree::Iterator it;

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
    flatTree::Iterator it(1);

    ASSERT("iterator index == 1", it.index == 1)
    ASSERT("iterator parent() == 3", it.parent() == 3)
    ASSERT("iterator parent() == 7", it.parent() == 7)
    ASSERT("iterator rightChild() == 11", it.rightChild() == 11)
    ASSERT("iterator leftChild == 9", it.leftChild() == 9)
    ASSERT("iterator next() == 13", it.next() == 13)
    ASSERT("iterator leftSpan() == 12", it.leftSpan() == 12)
  }

  ASSERT("PLAN == COUNT", plan == count)

  {
    std::vector<std::string> list(4);

    auto i = flatTree::index(0, 0); // get array index for depth: 0, offset: 0
    auto j = flatTree::index(1, 0); // get array index for depth: 2, offset: 0

    // use these indexes to store some data

    list[i] = "a";
    list[j] = "b";

    auto p = flatTree::parent(j);
    list[p] = "parent of a and b";

    for (const auto& i: list) {
      std::cout << i << ' ' << std::endl;
    }
  }
}
