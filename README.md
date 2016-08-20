# Fast-Buffer
A buffer which only accepts ubyte/sbyte, else throws compile time errors.

This buffer API uses a temporary iterator as a hack for accessing array contents of Buffer with operators. The buffer only accepts types of `ubyte` and `byte` and otherwise will throw a compile time error using `static_assert`.

You can turn **`ON`** compile time errors by defining or compiling with `__TEMPLATE_DEBUG__` (see below example).

EXAMPLE
```C++
//                                            BASIC DEMONSTRATION
#include <iostream>
#include "fast_buffer.h"
using namespace std;

int main() {
    fast_buffer buffer(24);
    
    // Demonstrates [retrieve] and [assignment] via operators.
    for(int i = 0; i < 64; i ++)
      cout << (int) (buffer[i] = (ubyte) i) << ", ";
    
    cout << endl << endl;
    
    // Denibstrates accessor functions.
    buffer.seek(seek_mode::start, 0);
    for(int i = 0; i < 64; i ++)
      cout << buffer.write<ubyte>((ubyte) i) << ", ";
    
    cout << endl << endl;
    
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
    
    // Demonstrates [retrieve] and [assignment] via operators.
    for(int i = 0; i < 64; i ++)
      cout << (int) (buffer[i] = (int) i) << ", ";
    
    cout << endl;
    
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
  fastbuff_iterator& operator[](int index)
}

class fastbuff_iter {
  // Constructor
  fastbuff_iter(ubyte* pointer, int index);
  
  // Overloaded Operators
  operator T();
  T operator =(T value);
}
```
