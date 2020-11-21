#pragma once


namespace task {

    template<class T>
    class UniquePtr {
    private:
        // Your code goes here...
        T *ptr;

    public:
        using pointer = T *;
        UniquePtr(pointer ptr);
        UniquePtr(const UniquePtr<T> &) = delete;
        UniquePtr<T> &operator=(const UniquePtr<T> &) = delete;
        UniquePtr(UniquePtr<T> &&) noexcept;
        UniquePtr<T> &operator=(UniquePtr<T> &&) noexcept;
        ~UniquePtr();
        typename std::add_lvalue_reference<T>::type operator*() const;
        pointer operator->() const noexcept;
        pointer get() const noexcept;
        pointer release() noexcept;
        void reset(pointer p = pointer()) noexcept;
        void swap(UniquePtr<T> &lhs, UniquePtr<T> &rhs) noexcept;
    };

    template<class T>
    class WeakPtr;

    template<class T>
    struct ControlBlock {
        T *ptr;
        size_t ref_count;
        size_t weak_count;
        ControlBlock(T *ptr) : ptr(ptr), ref_count(1), weak_count(0) {}
    };

    template<class T>
    class SharedPtr {
    public:
        ControlBlock<T> *cb;
        explicit SharedPtr(T *ptr = 0);
        SharedPtr(const SharedPtr<T> &) noexcept;
        SharedPtr(SharedPtr<T> &&) noexcept;
        SharedPtr<T> &operator=(const SharedPtr<T> &) noexcept;
        SharedPtr<T> &operator=(SharedPtr<T> &&r) noexcept;
        ~SharedPtr();
        explicit SharedPtr(const WeakPtr<T> &);

        T &operator*() const noexcept;
        T *operator->() const noexcept;
        T *get() const noexcept;
        long use_count() const noexcept;
        void reset() noexcept;
        void reset(T *);
        void swap(SharedPtr<T> &) noexcept;
    };

    template<class T>
    class WeakPtr {

    public:
        ControlBlock<T> *cb;
        WeakPtr();
        WeakPtr(const SharedPtr<T> &) noexcept;
        WeakPtr(const WeakPtr<T> &) noexcept;
        WeakPtr(WeakPtr<T> &&) noexcept;
        WeakPtr<T> &operator=(const WeakPtr<T> &) noexcept;
        WeakPtr<T> &operator=(WeakPtr<T> &&) noexcept;
        WeakPtr<T> &operator=(const SharedPtr<T> &) noexcept;
        ~WeakPtr();

        bool expired() const noexcept;
        long use_count() const noexcept;
        SharedPtr<T> lock() const noexcept;
        void reset() noexcept;
        void swap(WeakPtr &) noexcept;
    };


    // Your template function definitions may go here...

}// namespace task


#include "smart_pointers.tpp"
