# Fast-Buffer
A buffer which only accepts ubyte/sbyte, else throws compile time errors.

This buffer API uses a temporary iterator as a hack for accessing array contents of Buffer with operators. The buffer only accepts types of `ubyte` and `byte` and otherwise will throw a compile time error using `static_assert`.

You can turn **`ON`** compile time errors by defining or compiling with `__TEMPLATE_DEBUG__` (see below example).

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
