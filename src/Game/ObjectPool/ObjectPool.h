#pragma once

#include <stdlib.h>
#include <iostream>

namespace MinerSpeed
{

    class DefaultMemoryAllocator
    {
    public:
        static inline void *Allocate(size_t size)
        {
            return ::operator new (size, ::std::nothrow);
        }
        static inline void Deallocate(void *pointer, size_t size)
        {
            ::operator delete (pointer);
        }
    };

    template <typename T, class TMemoryAllocator = DefaultMemoryAllocator>
    class ObjectPool
    {
    private:
        struct _Node {
            void *_memory;
            size_t _capacity;
            _Node *_nextNode;

            _Node(size_t capacity)
            {
                if (capacity < 1) {
                    throw std::invalid_argument("ObjectPool:: Capacity must be at least 1.");
                }

                _memory = TMemoryAllocator::Allocate(_itemSize * capacity);
                if (_memory == NULL) {
                    throw std::bad_alloc();
                }

                _capacity = capacity;
                _nextNode = NULL;
            }
            ~_Node()
            {
                TMemoryAllocator::Deallocate(_memory, _itemSize * _capacity);
            }
        };

        void *_nodeMemory;
        T *_firstDeleted;
        size_t _countInNode;
        size_t _nodeCapacity;
        _Node _firstNode;
        _Node *_lastNode;
        size_t _maxBlockLength;

        static const size_t _itemSize;

        ObjectPool(const ObjectPool<T, TMemoryAllocator> &source);
        void operator=(const ObjectPool<T, TMemoryAllocator> &source);

        void _AllocateNewNode()
        {
            size_t size = _countInNode;
            if (size >= _maxBlockLength) {
                size = _maxBlockLength;
            } else {
                size *= 2;

                if (size < _countInNode) {
                    throw std::overflow_error("ObjectPool:: Size became too big.");
                }

                if (size >= _maxBlockLength) {
                    size = _maxBlockLength;
                }
            }

            _Node *newNode = new _Node(size);
            _lastNode->_nextNode = newNode;
            _lastNode = newNode;
            _nodeMemory = newNode->_memory;
            _countInNode = 0;
            _nodeCapacity = size;
        }

    public:
        explicit ObjectPool(size_t initialCapacity = 64, size_t maxBlockLength = 1000000) : _firstDeleted(NULL),
            _countInNode(0),
            _nodeCapacity(initialCapacity),
            _firstNode(initialCapacity),
            _maxBlockLength(maxBlockLength)
        {
            if (maxBlockLength < 1) {
                throw std::invalid_argument("ObjectPool:: MaxBlockLength must be at least 1.");
            }

            _nodeMemory = _firstNode._memory;
            _lastNode = &_firstNode;
        }
        ~ObjectPool()
        {
            _Node *node = _firstNode._nextNode;
            while (node) {
                _Node *nextNode = node->_nextNode;
                delete node;
                node = nextNode;
            }
        }

        T *New()
        {
            if (_firstDeleted) {
                T *result = _firstDeleted;
                _firstDeleted = *((T **)_firstDeleted);
                new (result) T();
                return result;
            }

            if (_countInNode >= _nodeCapacity) {
                _AllocateNewNode();
            }

            char *address = (char *)_nodeMemory;
            address += _countInNode * _itemSize;
            T *result = new (address) T();
            _countInNode++;
            return result;
        }

        void Delete(T *content)
        {
            content->~T();

            *((T **)content) = _firstDeleted;
            _firstDeleted = content;
        }
    };

    template <typename T, class TMemoryAllocator>
    const size_t ObjectPool<T, TMemoryAllocator>::_itemSize = ((sizeof(T) + sizeof(void *) - 1) / sizeof(void *)) * sizeof(void *);

} // namespace MinerSpeed
