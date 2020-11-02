#include <iostream>

template <typename T> struct Chunk {
  Chunk *next = nullptr;
  T *ptr;
  size_t size = 0;
  explicit Chunk(size_t capacity)
      : ptr((T *)(new char[capacity * sizeof(T)])) {}
  ~Chunk() { delete[](char *) ptr; }
};

template <typename T> class Allocator {
  static const size_t capacity = 1e3;
  size_t *instances_count;
  Chunk<T> *chunk_ptr = nullptr;

public:
  using value_type = T;
  using pointer = T *;
  using const_pointer = const T *;
  using reference = T &;
  using const_reference = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  template <typename U> struct rebind { using other = Allocator<U>; };

  Allocator() : instances_count(new size_t(1)) {}
  Allocator(const Allocator &alloc) {
    chunk_ptr = alloc.chunk_ptr;
    instances_count = alloc.instances_count;
    instances_count++;
  }
  ~Allocator() {
    if (*instances_count == 1) {
      while (chunk_ptr != nullptr) {
        auto p = chunk_ptr;
        chunk_ptr = chunk_ptr->next;
        p->~Chunk();
      }
      delete instances_count;
    } else {
      --(*instances_count);
    }
  }
  Allocator &operator=(const Allocator &alloc) {
    if (&alloc == this)
      return *this;

    this->~Allocator();
    chunk_ptr = alloc.chunk_ptr;
    instances_count = alloc.instances_count;
    instances_count++;
    return *this;
  }
  pointer allocate(std::size_t n) {
    if (n > capacity)
      return nullptr;

    if (chunk_ptr == nullptr) {
      chunk_ptr = new Chunk<value_type>(capacity);
    }

    Chunk<T> *p = chunk_ptr;
    while (p != nullptr) {
      if (capacity - p->size >= n) {
        size_t prev_size = p->size;
        p->size += n;
        return p->ptr + prev_size;
      } else {
        if (p->next == nullptr) {
          p->next = new Chunk<value_type>(capacity);
        }
        p = p->next;
      }
    }
  }

  void deallocate(pointer p, std::size_t n) {}

  template <typename... Args> void construct(pointer p, Args &&... args) {
    new (p) T(std::forward<Args>(args)...);
  }
  void destroy(T *p) { p->~T(); }
};

struct A {
  A(int x, int y) { std::cout << x << y << std::endl; }
  ~A() { std::cout << '~' << std::endl; }
};

int main() {
  Allocator<A> alloc;
  auto p = alloc.allocate(1);
  alloc.construct(p, 1, 2);
  alloc.destroy(p);
  alloc.deallocate(p, 100);

  Allocator<A> alloc2;
  auto p2 = alloc2.allocate(10);
  alloc2 = alloc;
  return 0;
}
