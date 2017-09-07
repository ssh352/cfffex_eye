#ifndef BUFFER_H
#define BUFFER_H

#include <string>

class buffer
{
public:
    const static size_t INIT_BUFF_LEN = 4096;

    buffer(size_t length = INIT_BUFF_LEN);
    ~buffer();

    void reserve(size_t length);
    size_t capacity() const { return buff_length_ - length_; }

    char *begin() const  { return ptr_; }
    size_t length() const  { return length_; }

    char *end() const { return ptr_ + length_; }
    void add_length(size_t length) { length_ += length; }
    void reset(size_t length = 0) { length_ = length; }

private:
    buffer(const buffer &);
    const buffer &operator=(const buffer &);

private:
    char buff_[INIT_BUFF_LEN];
    size_t buff_length_;
    char *ptr_;
    size_t length_;
};

#endif // BUFFER_H
