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
  //                                      Constructors
  // Constructs a buffer using a pre-existing array of bytes.
  fast_buffer(uint64 address, int size);
  // Constructs a new buffer of the specified size.
  fast_buffer(int size);
  
  //                                      Destructor
  // Cleans up the buffer's dynamic memory. **DO NOT CALL DESTRUCTORS**
  ~fast_buffer();
  
  //                                      Clean Up
  // Clears all elements in the buffer to zero.
  void clear();
  // Clears the elements in the buffer from x1 to x2 to zero.
  void clear(int x1, int x2);
  
  //                                      Resizing
  // Exponentially resizes the buffer (size * size).
  void grow();
  // Resizes the buffer to the indicated size.
    // NOTE: This uses a `fast resize` where the actual size does not change when resizing downwards.
    // This is `caching` the rest of the buffer for when you might need to resize upwards.
  void resize(int size);
  
  //                                      Random Access
  // sets the seek position relative to the seek_mode.
  /*
    Seek Modes:
        start : seek to the new position from the start of the buffer.
        relative: seek to the new position from the current seek position.
        end : seek to the new position from the current seek position.
            NOTE: If you seek from the end you'll need to use negative positions to seek backwards.
  */
  void seek<T.(enum seek_mode mode, int index);
  // Write a byte from the indicated position in the buffer WITHOUT advancing the seek position.
  void poke<T>(T value, seek_mode mode, int index);
  void poke<T>(T value, int index);
  // Read a byte from the indicated position in the buffer WITHOUT advancing the seek position.
  T peek<T>(seek_mode mode, int index);
  T peek<T>(int index);
  
  //                                      Function Accessors
  // Writes a byte to the buffer AND advances the seek position.
  void write<T>(T value);
  // Reads a byte from the buffer AND advances the seek position.
  T read<T>();
  
  //                                      Overloaded Operators
  // Gets a fastbuff_iterator for the current buffer index and redirects reading/writing bytes to the fastbuff_iterator.
  fastbuff_iterator& operator[](int index);
  // Reads a byte from the buffer via cast ooperator AND advances the seek position.
  operator T();
  // Writes a byte to the buffer AND 
  T operator =(T value);
  // Deletes the fast_buffer and it's underlying buffer array when you dynamically allocate fast_buffer.
  delete my_fast_buffer;
}

class fastbuff_iter {
  //                                      Constructor
  // Constructs a fastbuff_iterator pointing to an index in the buffer.
  fastbuff_iter(ubyte* pointer, int index);
  
  //                                      Overloaded Operators
  // Reads a byte from the buffer AND advances the seek position.
  operator T();
  // Writes a byte to the buffer via array operator from `fast_buffer` AND advances the seek position.
  T operator =(T value);
}
```
If it's semi-confusing understanding the `delete` operator overload, that's because `delete` calls the destructor of the object it is deleting. In this case the destructor is called and both the object and buffer array are deleted.
