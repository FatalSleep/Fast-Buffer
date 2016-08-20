# Fast-Buffer
A buffer which only accepts ubyte/sbyte, else throws compile time errors.

This buffer API uses a temporary iterator as a hack for accessing array contents of Buffer with operators. The buffer only accepts types of `ubyte` and `byte` and otherwise will throw a compile time error using `static_assert`.

You can turn **`ON`** compile time errors by defining or compiling with `__TEMPLATE_DEBUG__` (see below example).

***NOTE:*** Using the array operators will use the same functionality as `T peek(int index)` and `poke(T value, int index)` by not moving the `seek` position when reading or writing.

EXAMPLE
```C++
//                                      ARRAY OPERATOR DEMONSTRATION
#include <iostream>
#include "fast_buffer.h"
using namespace std;

int main() {
    fast_buffer buffer(24);
    
    for(int i = 0; i < buffer.length(); i ++)
      cout << (int) (buffer[i] = (byte) i) << ", ";
    
    buffer.free();
    cin.get();
}
```
```C++
//                                      CAST & ASSIGNMENT DEMONSTRATION
#include <iostream>
#include "fast_buffer.h"
using namespace std;

int main() {
    fast_buffer buffer(24);

    for (int i = 0; i < buffer.length(); i++) {
        buffer = (ubyte) i;
    }

    buffer.seek(seek_mode::start, 0);

    for (int i = 0; i < buffer.length(); i++) {
        cout << (int) (ubyte) buffer << endl;
    }

    buffer.free();
    cin.get();
}
```
```C++
//                                      COMPILE TIME ERRORS DEMONSTRATION
// Note this define for error debugging...
#define __TEMPLATE_DEBUG__
#include <iostream>
#include "fast_buffer.h"
using namespace std;

int main() {
    fast_buffer buffer(24);
    buffer.write<ubyte>(45);
    buffer.seek(seek_mode::start, 0);
    cout << buffer.read<ubyte>() << endl;
    buffer.free();
    cin.get();
}

/*
  OUTPUT WINDOW:
    1>------ Build started: Project: FastBuffer, Configuration: Debug x64 ------
    1>  Source.cpp
    1>c:\...\fastbuffer\fastbuffer\fast_buffer.h(30): error C2338: ERROR: T must be of type *sbyte* or *ubyte*
    1>  c:\...\fastbuffer\fastbuffer\source.cpp(13): note: see reference to function template instantiation 'T fastbuff_iterator::operator =<int>(T)' being compiled
    1>          with
    1>          [
    1>              T=int
    1>          ]
    ========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
*/
```

API
```C++
class fast_buffer {
  // Constructors
  fast_buffer(uint64 address, int size);
  fast_buffer(int size);
  
  // Clean Up
  void free();
  void clear();
  void clear(int x1, int x2);
  
  // Resizing
  void grow();
  void resize(int size);
  
  // Random Access
  void seek(enum seek_mode mode, int index);
  void poke(T value, int index);
  T peek(int index);
  
  // Function Accessors
  void write(T value);
  T read();
  
  // Overloaded Operators
  fastbuff_iterator& operator[](int index);
  operator T();
  T operator =(T value);
}

class fastbuff_iter {
  // Constructor
  fastbuff_iter(ubyte* pointer, int index);
  
  // Overloaded Operators
  operator T();
  T operator =(T value);
}
```
