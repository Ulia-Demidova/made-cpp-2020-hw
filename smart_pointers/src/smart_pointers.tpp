#pragma once


namespace task {

    template<class T>
    UniquePtr<T>::UniquePtr(pointer ptr) : ptr(ptr) {}

    template<class T>
    UniquePtr<T>::UniquePtr(UniquePtr<T> &&other) noexcept {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    template<class T>
    UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr<T> &&other) noexcept {
        if (&other == this)
            return *this;
        ptr = other.ptr;
        other.ptr = nullptr;
        return *this;
    }

    template<class T>
    UniquePtr<T>::~UniquePtr() { delete ptr; }

    template<class T>
    typename std::add_lvalue_reference<T>::type UniquePtr<T>::operator*() const {
        return *get();
    }

    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::operator->() const noexcept {
        return get();
    }

    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::get() const noexcept {
        return ptr;
    }

    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::release() noexcept {
        pointer temp = ptr;
        ptr = nullptr;
        return temp;
    }

    template<class T>
    void UniquePtr<T>::reset(pointer p) noexcept {
        pointer old_ptr = ptr;
        ptr = p;
        if (old_ptr)
            delete old_ptr;
    }

    template<class T>
    void UniquePtr<T>::swap(UniquePtr<T> &lhs, UniquePtr<T> &rhs) noexcept {
        pointer temp = lhs.ptr;
        lhs.ptr = rhs.ptr;
        rhs.ptr = temp;
    }

    template<class T>
    SharedPtr<T>::SharedPtr(T *ptr) {
        cb = new ControlBlock<T>(ptr);
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const SharedPtr<T> &other) noexcept {
        cb = other.cb;
        ++cb->ref_count;
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(const SharedPtr<T> &other) noexcept {
        if (&other == this)
            return *this;

        this->~SharedPtr<T>();

        cb = other.cb;
        ++cb->ref_count;
        return *this;
    }

    template<class T>
    SharedPtr<T>::SharedPtr(SharedPtr<T> &&other) noexcept {
        cb = other.cb;
        other.cb = nullptr;
    }

    template<class T>
    SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr<T> &&other) noexcept {
        if (&other == this)
            return *this;
        this->~SharedPtr<T>();
        cb = other.cb;
        other.cb = nullptr;
        return *this;
    }

    template<class T>
    SharedPtr<T>::~SharedPtr() {
        if (cb) {
            if (cb->ref_count == 1) {
                delete cb->ptr;
                --cb->ref_count;
                if (cb->weak_count == 0)
                    delete cb;
            } else
                --cb->ref_count;
        }
    }

    template<class T>
    T &SharedPtr<T>::operator*() const noexcept {
        return *get();
    }

    template<class T>
    T *SharedPtr<T>::operator->() const noexcept {
        return get();
    }

    template<class T>
    T *SharedPtr<T>::get() const noexcept {
        return cb->ptr;
    }

    template<class T>
    long SharedPtr<T>::use_count() const noexcept {
        return cb->ref_count;
    }

    template<class T>
    void SharedPtr<T>::reset() noexcept {
        --cb->ref_count;
        if (cb->ref_count == 0) {
            delete cb->ptr;
            cb->ptr = nullptr;
            if (cb->weak_count == 0) {
                delete cb;
            }
        }
        cb = new ControlBlock<T>(0);
    }

    template<class T>
    void SharedPtr<T>::reset(T *other) {
        --cb->ref_count;
        if (cb->ref_count == 0) {
            delete cb->ptr;
            cb->ptr = nullptr;
            if (cb->weak_count == 0)
                delete cb;
        }
        cb = new ControlBlock<T>(other);
    }

    template<class T>
    void SharedPtr<T>::swap(SharedPtr<T> &other) noexcept {
        ControlBlock<T> *temp = cb;
        cb = other.cb;
        other.cb = temp;
    }

    template<class T>
    SharedPtr<T>::SharedPtr(const WeakPtr<T> &weak_ptr) {
        cb = weak_ptr.cb;
        ++cb->ref_count;
    }

    template<class T>
    WeakPtr<T>::WeakPtr() : WeakPtr<T>(SharedPtr<T>()) {}

    template<class T>
    WeakPtr<T>::WeakPtr(const SharedPtr<T> &shared_ptr) noexcept {
        cb = shared_ptr.cb;
        ++cb->weak_count;
    }

    template<class T>
    WeakPtr<T>::WeakPtr(const WeakPtr<T> &other) noexcept {
        cb = other.cb;
        ++cb->weak_count;
    }

    template<class T>
    WeakPtr<T>::WeakPtr(WeakPtr<T> &&other) noexcept {
        cb = other.cb;
        other.cb = nullptr;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(const WeakPtr<T> &other) noexcept {
        if (&other == this)
            return *this;
        this->~WeakPtr();
        cb = other.cb;
        ++cb->weak_count;
        return *this;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(WeakPtr<T> &&other) noexcept {
        if (&other == this)
            return *this;
        this->~WeakPtr();
        cb = other.cb;
        other.cb = nullptr;
        return *this;
    }

    template<class T>
    WeakPtr<T> &WeakPtr<T>::operator=(const SharedPtr<T> &shared_ptr) noexcept {
        this->~WeakPtr();
        cb = shared_ptr.cb;
        ++cb->weak_count;
        return *this;
    }

    template<class T>
    WeakPtr<T>::~WeakPtr() {
        if (cb) {
            --cb->weak_count;
            if (cb->ref_count == 0 && cb->weak_count == 0)
                delete cb;
        }
    }

    template<class T>
    bool WeakPtr<T>::expired() const noexcept {
        return !(cb->ref_count);
    }

    template<class T>
    long WeakPtr<T>::use_count() const noexcept {
        if (cb)
            return cb->ref_count;
        else
            return 0;
    }

    template<class T>
    SharedPtr<T> WeakPtr<T>::lock() const noexcept {
        if (!expired())
            return SharedPtr<T>(*this);
        else
            return SharedPtr<T>();
    }

    template<class T>
    void WeakPtr<T>::reset() noexcept {
        --cb->weak_count;
        if (cb->ref_count == 0 && cb->weak_count == 0)
            delete cb;
    }

    template<class T>
    void WeakPtr<T>::swap(WeakPtr &other) noexcept {
        ControlBlock<T> *temp = cb;
        cb = other.cb;
        other.cb = temp;
    }

}// namespace task