#include <thread>
#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>

using namespace std;

class atomic_count
{
public:
    explicit atomic_count(int v = 0) : value_(v) {}
    int operator++() { return fetch_add(value_, 1) + 1; }
    int operator++(int) { return fetch_add(value_, 1); }
    int operator--() { return fetch_add(value_, -1) - 1; }
    int operator--(int) { return fetch_add(value_, -1); }
    operator int() const { return (int volatile &)value_; }
private:
    int fetch_add(int volatile& storage, int v)
    {
        __asm__ __volatile__
            (
            "lock; xaddl %0, %1"
            : "+q" (v), "+m" (storage)
            :
            : "cc", "memory"
             );
        return v;
    }
private:
    int value_;
};

class queue_base
{
public:
    queue_base() : size_(0), read_(0), write_(0)
    {
        array_ = new int[MAX_SIZE];
        memset(array_, 0, sizeof(int) * MAX_SIZE);
    }
    virtual bool push(int data) = 0;
    virtual int pop() = 0;
    virtual void debug() {}
    void print()
    {
        cout << "queue: ";
        for (int i = 0; i < size_; ++i)
        {
            cout << (long)array_[get_next(read_ + 1)] << ", ";
        }
        cout << endl;
    }
protected:
    int get_next(int pos)
    {
        return (pos + 1) % MAX_SIZE;
    }
protected:
    static const int MAX_SIZE = 102400;
    int *array_;
    int size_;
    int read_;
    int write_;
};

class lock_queue : public queue_base
{
public:
    lock_queue()
    {
        pthread_mutex_init(&mutex_, 0);
        pthread_cond_init(&condr_, 0);
        pthread_cond_init(&condw_, 0);
    }
    virtual bool push(int data)
    {
        pthread_mutex_lock(&mutex_);
        while (size_ == MAX_SIZE)
            pthread_cond_wait(&condw_, &mutex_);

        array_[write_] = data;
        write_ = get_next(write_);
        if (size_++ == 0)
            pthread_cond_signal(&condr_);
        pthread_mutex_unlock(&mutex_);
        return true;
    }
    virtual int pop()
    {
        pthread_mutex_lock(&mutex_);
        while (size_ == 0)
            pthread_cond_wait(&condr_, &mutex_);

        int ret = array_[read_];
        read_ = get_next(read_);
        if (size_-- == MAX_SIZE)
            pthread_cond_signal(&condw_);
        pthread_mutex_unlock(&mutex_);
        return ret;
    }
private:
    pthread_mutex_t mutex_;
    pthread_cond_t condw_;
    pthread_cond_t condr_;
};

class cas_queue : public queue_base
{
public:
    cas_queue()
    {
        sem_init(&semaw_, 0, MAX_SIZE - 1);
        sem_init(&semar_, 0, 0);
    }
    virtual bool push(int data)
    {
        //if (sem_wait(&semaw_) != 0)
        //    return false;
        int read_c, write_c, write_n;
        do
        {
            read_c = read_;
            write_c = write_;
            write_n = get_next(write_c);
            if (write_n == read_c || array_[read_c] != 0)
            {
                //cout << "push falied" << endl;
                //sem_post(&semaw_);
                return false;
            }
        } while (!__sync_bool_compare_and_swap(&write_, write_c, write_n));

        array_[write_c] = data;
        //sem_post(&semar_);
        return true;
    }
    virtual int pop()
    {
        //if (sem_wait(&semar_) != 0)
        //    return 0;
        int read_c, read_n;
        do 
        {
            read_c = read_;
            read_n = get_next(read_c);
            if (array_[read_c] == 0)
            {
                //cout << "pop falied" << endl;
                //sem_post(&semar_);
                return 0;
            }
        } while (!__sync_bool_compare_and_swap(&read_, read_c, read_n));

        int ret = array_[read_c];
        array_[read_c] = 0;
        //sem_post(&semaw_);
        return ret;
    }
    virtual void debug()
    {
        int v = 0, v2 = 0;
        sem_getvalue(&semaw_, &v);
        sem_getvalue(&semar_, &v2);
        cout << "debug: " << v << "|" << v2 << ", " << write_ << "|" << read_ << endl;
    }

private:
    sem_t semaw_;
    sem_t semar_;
};

lock_queue queue1;
cas_queue queue2;
queue_base *queue;
atomic_count count_push;
atomic_count count_pop;
int exit_count = 60;

void queue_push()
{
    while (exit_count)
    {
        if (queue->push(1))
            count_push++;
    }
}

void queue_pop()
{
    while (true)
    {
        if (queue->pop())
            count_pop++;
    }
}

// g++ -g testq.cpp -std=c++11 -lpthread
int main(int argc, char **argv)
{
    cout << "use age:" << argv[0] << " lock|cas thread_num" << endl;
    int thread_num = 5;
    queue = &queue1;
    if (argc >= 2 && strcmp(argv[1], "cas") == 0)
    {
        cout << "test cas_queue" << endl;
        queue = &queue2;
    }
    else
    {
        cout << "test lock_queue" << endl;
    }
    if (argc >= 3) thread_num = atoi(argv[2]);
    cout << "thread num is " << thread_num << endl;

    for (int i = 0; i < thread_num; ++i)
    {
        thread * t1 = new thread(queue_push);
        thread * t2 = new thread(queue_pop);
    }
    int old_count_push = 0;
    int old_count_pop = 0;
    while (true)
    {
        sleep(1);
        int cur_count_push = count_push;
        int cur_count_pop = count_pop;
        cout << "push count:" << cur_count_push << "|" << cur_count_push - old_count_push
             << ", pop count:" << cur_count_pop << "|" << cur_count_pop - old_count_pop << endl;
        old_count_push = cur_count_push;
        old_count_pop = cur_count_pop;
        exit_count--;
        //queue->debug();
    }
}
