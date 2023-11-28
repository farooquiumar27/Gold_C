#ifndef ARENA_HPP_INCLUDED
#define ARENA_HPP_INCLUDED
#include<iostream>
class ArenaAllocator
{
    public:
    inline explicit ArenaAllocator(size_t bytes)
    : m_bytes(bytes)
    {
        m_buffer=static_cast<std::byte *>(malloc(m_bytes));
        m_offset=m_buffer;
    }
    inline ArenaAllocator(const ArenaAllocator& other) = delete;
    inline ArenaAllocator operator=(const ArenaAllocator& other) = delete;
    ~ArenaAllocator()
    {
        free(m_buffer);
    }
    template<typename T>
    inline T* alloc()
    {
        void *offset;
        offset=m_offset;
        m_offset+=sizeof(T);
        return static_cast<T*>(offset);
    }
    private:
    size_t m_bytes;
    std::byte* m_buffer;
    std::byte* m_offset;
};

#endif