## s21_containers


C++ STL containers reimplementation developed as a practice project at School 21 (formerly 42.fr).

## Features

- Sequence containers: `vector`, `list`, `array`
- Container adapters: `stack`, `queue` 
- Associative containers: `map`, `set`, `multiset`
- STL-compatible iterators supporting <algorithm> header functions

## Implementation Details

- `vector`: 🤖📦 dynamic array with automatic reallocation
- `list`: 🔗↔️ doubly-linked list with sentinel node
- `array`: 🔒📐 compile-time C-style array wrapper
- `deque`: 🧩 chunked array as map of fixed-size blocks
- `stack`, `queue`: 🔌 adapters over `deque`
- `map`/`set`/`multiset`: 🔴⚫🌳 self-balancing red-black tree

## Installation


### To use in your project:

```bash
git clone https://github.com/makesnosense/s21_containers.git
```

Copy headers to your project:
```bash
cp s21_containers/src/*.h /path/to/your/project/
```

### For development/testing:

```bash
cd s21_containers
make test          # run tests
make coverage      # generate coverage report  
make valgrind      # memory leak check
```

## Usage examples

### Headers

```cpp
// include all:
#include "s21_containers.h"      // vector, list, map, set, stack, queue
#include "s21_containersplus.h"  // array, multiset

// or include individual containers:
#include "s21_vector.h"
#include "s21_list.h"
#include "s21_map.h"
#include "s21_set.h"
#include "s21_multiset.h"
#include "s21_stack.h"
#include "s21_queue.h"
#include "s21_array.h"
#include "s21_deque.h"
```

### Sequence Containers

#### vector
```cpp
s21::vector<int> v = {1, 2, 3};
v.push_back(4);
v.insert(v.begin(), 0);
```

#### list
```cpp
s21::list<int> l = {1, 2, 3};
l.push_front(0);
l.sort();
l.unique();
```

#### array
```cpp
s21::array<int, 5> arr = {1, 2, 3, 4, 5};
arr.fill(42);
```

### Container Adapters

#### stack
```cpp
s21::stack<int> s;
s.push(42);
int top = s.top();
s.pop();
```

#### queue
```cpp
s21::queue<int> q;
q.push(1);
int front = q.front();
q.pop();
```

### Associative Containers

#### map
```cpp
s21::map<std::string, int> m;
m["key"] = 42;
m.insert({"another", 24});
```

#### set
```cpp
s21::set<int> s = {3, 1, 4, 1, 5};  // {1, 3, 4, 5}
s.insert(2);
```

#### multiset
```cpp
s21::multiset<int> ms = {1, 2, 2, 3};
size_t count = ms.count(2);  // 2
```

### Insert Many Operations

```cpp
s21::vector<int> v;
v.insert_many_back(1, 2, 3, 4, 5);

s21::list<int> l;
l.insert_many_front(5, 4, 3, 2, 1);

s21::map<int, std::string> m;
m.insert_many({1, "one"}, {2, "two"});
```


## Requirements

- C++17 compiler (tested with GCC on Linux, Clang on macOS)
- Google Test framework (for testing)
- [GCOVR](https://github.com/gcovr/gcovr) (for coverage reports)
- Valgrind (for memory leak checks)