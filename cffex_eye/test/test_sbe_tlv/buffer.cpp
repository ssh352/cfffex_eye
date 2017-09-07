#include "buffer.h"
#include <cstring>

#define BUFFER_ALIGN(length) (((length) + 4095) & ~4095)

buffer::buffer(size_t length)
    : length_(0)
{
    if (length <= sizeof(buff_))
    {
        ptr_ = buff_;
        buff_length_ = sizeof(buff_);
    }
    else
    {
        ptr_ = (char*)::malloc(length);
        buff_length_ = length;
    }
}

buffer::~buffer()
{
    if (ptr_ != buff_)
        ::free(ptr_);
}

void buffer::reserve(size_t length)
{
    if (capacity() < length)
    {
        buff_length_ += BUFFER_ALIGN(length);
        if (ptr_ != buff_)
        {
            ptr_ = (char*)::realloc(ptr_, buff_length_);
        }
        else
        {
            ptr_ = (char*)::malloc(buff_length_);
            ::memcpy(ptr_, buff_, length_);
        }
    }
}
