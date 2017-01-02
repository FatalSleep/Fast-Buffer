#ifndef __FAST_BUFFER__
#define __FAST_BUFFER__
    #include <type_traits>
    typedef unsigned char ubyte;
    typedef signed char sbyte;
    typedef unsigned __int64 uint64;
    enum seek_mode { start, relative, end };

    class fastbuff_iterator {
    private:
        ubyte* pointer = nullptr;
        int index = -1;

    public:
        fastbuff_iterator(ubyte* pointer, int index) {
            this->pointer = pointer;
            this->index = index;
        }

        template<typename T>
        operator T() {
            #ifdef _DEBUG
                static_assert(std::is_same<T, ubyte>::value || std::is_same<T, sbyte>::value, "ERROR: T must be of type *sbyte* or *ubyte*");
            #endif

            ubyte* pointer = this->pointer;
            int index = this->index;
            return *(pointer + index);
        }

        template<typename T>
        T operator =(T value) {
            #ifdef _DEBUG
                static_assert(std::is_same<T, ubyte>::value || std::is_same<T, sbyte>::value, "ERROR: T must be of type *sbyte* or *ubyte*");
            #endif

            ubyte* pointer = this->pointer;
            int index = this->index;
            return *(pointer + index) = value;
        }
    };

    class fast_buffer {
    private:
        ubyte* buffer = nullptr;
        int size = 0, seekpos = 0;
        int cached_size = -1;

    public:
        fast_buffer(int size) {
            buffer = new ubyte[size]();
            this->size = size;
        }
        
        fast_buffer(uint64 address, int size) {
            buffer = reinterpret_cast<ubyte*>(address);
             this->size = size;
        }
        
        fast_buffer(void* pointer, int size) {
            buffer = reinterpret_cast<ubyte*>(pointer);
             this->size = size;
        }

        ~fast_buffer() {
            if (buffer != nullptr)
                delete[] buffer;
        }

        static void operator delete(void* buff) noexcept {
            fast_buffer* buffer = (fast_buffer*) buff;
            ::operator delete(buff);
        }

        int length() {
            return size;
        }

        int position() {
            return seekpos;
        }

        void clear() {
            for (int i = 0; i < size; i++)
                buffer[i] = 0;
        }

        void clear(int x1, int x2) {
            int endPoint = (x1 + x2 > size) ? size : x2;

            for (int i = x1; i < endPoint; i++)
                buffer[i] = 0;
        }

        void grow() {
            ubyte* buff = new ubyte[size * size];

            for(int i = 0; i < size; i++)
                buff[i] = buffer[i];

            delete[] buffer;
            buffer = buff;
        }

        void resize(int size) {
            if (cached_size <= 0) {
                if (this->size >= size) {
                    cached_size = this->size;
                    this->size = size;
                    return;
                }
            } else {
                if (size <= cached_size) {
                    this->size = size;
                    return;
                }
            }

            ubyte* buff = new ubyte[size];

            for (int i = 0; i < size; i++)
                buff[i] = buffer[i];

            delete[] buffer;
            buffer = buff;
            cached_size = -1;
            this->size;
        }

        void seek(seek_mode mode, int seek) {
            switch (mode) {
                case seek_mode::start:
                    this->seekpos = seek;
                break;
                case seek_mode::relative:
                    this->seekpos += seek;
                break;
                case seek_mode::end:
                    this->seekpos = (size - 1) + seek;
                break;
            }
        }

        template<typename T>
        void write(T value) {
            #ifdef _DEBUG
                static_assert(std::is_same<T, ubyte>::value || std::is_same<T, sbyte>::value, "ERROR: T must be of type *sbyte* or *ubyte*");
            #endif

            buffer[seekpos++] = (T)value;
        }

        template<typename T>
        T read() {
            #ifdef _DEBUG
                static_assert(std::is_same<T, ubyte>::value || std::is_same<T, sbyte>::value, "ERROR: T must be of type *sbyte* or *ubyte*");
            #endif

            return (T)buffer[seekpos++];
        }

        template<typename T>
        void poke(T value, int pos) {
            #ifdef _DEBUG
                static_assert(std::is_same<T, ubyte>::value || std::is_same<T, sbyte>::value, "ERROR: T must be of type *sbyte* or *ubyte*");
            #endif

            buffer[pos] = (T)value;
        }

        template<typename T>
        T peek(int pos) {
            #ifdef _DEBUG
                static_assert(std::is_same<T, ubyte>::value || std::is_same<T, sbyte>::value, "ERROR: T must be of type *sbyte* or *ubyte*");
            #endif

            return (T)buffer[pos];
        }

        fastbuff_iterator operator[](int index) {
            return fastbuff_iterator(buffer, index);
        };

        template<typename T>
        operator T() {
            #ifdef _DEBUG
                static_assert(std::is_same<T, ubyte>::value || std::is_same<T, sbyte>::value, "ERROR: T must be of type *sbyte* or *ubyte*");
            #endif
            return *(buffer + seekpos++);
        }

        template<typename T>
        T operator =(T value) {
            #ifdef _DEBUG
                static_assert(std::is_same<T, ubyte>::value || std::is_same<T, sbyte>::value, "ERROR: T must be of type *sbyte* or *ubyte*");
            #endif

            return *(buffer + seekpos++) = value;
        }
    };
#endif
