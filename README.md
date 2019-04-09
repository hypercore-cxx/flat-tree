# SYNOPSIS

A series of functions to map a binary tree to a list. This is a port of
[this](https://github.com/mafintosh/flat-tree) library and matches the tests.

# USAGE

You can represent a binary tree in a simple flat list using the following
structure.

```
        3
    1       5
  0   2   4   6  ...
```

Each number represents an **index** in a flat list. So the following tree...

```text
      A
  B       C
D   E   F   G  ...
```

...is represented as a list: `[D B E A F C G]`.

Indexes `0`, `2`, `4`, `6` are on **depth** `0`. `1`, `5`, `9` on depth `1`.

```text
depth = 2  ^        3
depth = 1  |    1       5
depth = 0  |  0   2   4   6  ...
```

In some cases it is also useful to calculate an **offset**.
Indexes `0`, `1`, `3`, `7` have an offset `0`...

```text
                (7)
       (3)
  (1)       5
(0)   2   4   6      ...
```

`2`, `5`, `11`, `23` offset `1`...

```text
                 7
       3                  (11)
  1        (5)        9          13
0   (2)   4   6    10   12    14    15
```

This module exposes a series of functions to help you build and maintain
this data structure.

```cpp
#include "flat-tree.hxx"
#include <vector>
#include <string>

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
```

# TESTING

```bash
c++ test.cxx -o test -std-c++2a
./test
```

## See also

- [mafintosh/flat-tree][rs]: A series of functions to map a binary tree to a list.
- [mafintosh/print-flat-tree][flat-tree]: A cli that can pretty print flat-trees.

- [datrs/flat-tree][rs]: A port of the node module to rust.
- [bcomnes/flattree][ftg]: A port of the node module to Go.

## License

MIT

[print]: https://github.com/mafintosh/print-flat-tree
[flat-tree]: https://github.com/mafintosh/flat-tree
[rs]: https://github.com/datrs/flat-tree
[ftg]: https://github.com/bcomnes/flattree
