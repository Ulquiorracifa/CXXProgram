#pragma once
#include <atomic>
//#include <memory>
//#include <limits>
//#include <cstdint>
//#include <cstddef>
//#include <concepts>
//#include <optional>
//#include <type_traits>
//#include <concepts>
namespace engine::utils 
{
    using namespace std;
    template<typename T>
    class Queue
    {
    public:
        Queue(const int& size = 16384);
        ~Queue() { delete[] m_data; }

        long Enqueue(const T& value);
        long Dequeue(T& value);

        void set_is_enqueue_done(const bool& is_enqueue_done)
        {
            m_is_enqueue_done = is_enqueue_done;
        }

        bool get_is_enqueue_done() const { return m_is_enqueue_done; }
        bool get_is_dequeue_done() const { return m_is_dequeue_done; }
        long get_dequeue_num() const { return m_dequeue_num; }
        long get_enqueue_num() const { return m_enqueue_num; }
        void Clear();

    private:
        int m_size;
        bool m_is_enqueue_done;
        bool m_is_dequeue_done;

        volatile atomic<long> m_enqueue_num;
        volatile atomic<long> m_dequeue_num;
        T* m_data;

        void set_size(const int& size);
    };

    template<typename T>
    Queue<T>::Queue(const int& size /* = 16384 */)
    {
        set_size(size);
        m_data = new T[m_size + 1];

        m_is_enqueue_done = m_is_dequeue_done = false;
        m_enqueue_num = m_dequeue_num = 0;
    }

    template<typename T>
    void Queue<T>::set_size(const int& size)
    {
        if (size <= 16384)
        {
            m_size = 16384;
            return;
        }

        m_size = 16384;
        while (m_size < size)
        {
            m_size <<= 1;
        }
        m_size >>= 1;
    }

    template<typename T>
    long Queue<T>::Enqueue(const T& value)
    {
        while (m_enqueue_num - m_dequeue_num >= m_size)
        {
            //this_thread::sleep_for(std::chrono::milliseconds(1));
            this_thread::yield();
        }
         

        long old_num;
        do
        {
        enqueue_loop:
            old_num = m_enqueue_num;
            if (-1 == m_enqueue_num)
            {
                this_thread::yield();
                goto enqueue_loop;
            }

        } while (!atomic_compare_exchange_weak(&m_enqueue_num, &old_num, (long)-1));
        m_data[old_num & (m_size - 1)] = value;
        m_enqueue_num = old_num + 1;
        return m_enqueue_num;
    }

    template<typename T>
    long Queue<T>::Dequeue(T& value)
    {
        long old_num_, new_num;
        do
        {
        dequeue_loop:
            old_num_ = m_dequeue_num;
            new_num = old_num_ + 1;
            if (m_dequeue_num >= m_enqueue_num)
            {
                if (m_is_enqueue_done)
                {
                    m_is_dequeue_done = true;
                    return 0;
                }
                else 
                {
                    this_thread::yield();
                }
                goto dequeue_loop;
            }
            value = m_data[m_dequeue_num & (m_size - 1)];
        } while (!atomic_compare_exchange_weak(&m_dequeue_num, &old_num_, new_num));
        return m_dequeue_num;
    }


    template<typename T>
    void Queue<T>::Clear()
    {
        m_enqueue_num = m_dequeue_num = 0;
        m_is_enqueue_done = m_is_dequeue_done = false;
    }

}
