#include <stdexcept>
#include <algorithm>

#pragma region Internals

template <typename T>
Queue<T>::Queue_node::Queue_node(T value) : value(value), next(nullptr) {}

template <typename T>
Queue<T>::Queue_node::Queue_node(Queue_node* pointer) : next(pointer) {}

template <typename T>
void Queue<T>::traverse(Queue_node*& nodePointer)
{
    nodePointer = nodePointer->next;
}

template <typename T> template <typename... args_t>
void Queue<T>::traverse(Queue_node*& nodePointer, args_t&... args)
{
    traverse(nodePointer);
    traverse(args...);
}

template <typename T>
void Queue<T>::destroyFrom(Queue_node* startPointer)
{
    Queue_node dummyThis = Queue_node(startPointer);
    Queue_node* current = &dummyThis;

    for(Queue_node* next = startPointer; next != nullptr; delete current)
        traverse(current, next);

    if (startPointer == head)
    {
        head = nullptr;
        tail = nullptr;
    }
}

#pragma endregion

#pragma region Constructors, assigments, destructor

template <typename T>
Queue<T>::Queue() : head(nullptr), tail(nullptr) {}


template <typename T>
Queue<T>::~Queue()
{
    destroyFrom(head);
}

template <typename T>
Queue<T>::Queue(const Queue<T>& rhs) : Queue()
{
    *this = rhs;
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& rhs)
{
    Queue_node* current = head;
    Queue_node* rhs_current = rhs.head;

    while((current != nullptr) && (rhs_current != nullptr))
    {
        current->value = rhs_current->value;
        tail = current;
        traverse(current, rhs_current);
    }

    if(current == rhs_current) {}
    else if (rhs_current == nullptr)
    {
        destroyFrom(current);

        if(tail != nullptr)
            tail->next = nullptr;
    }
    else
    {
        while(rhs_current != nullptr)
        {
            enqueue(rhs_current->value);
            traverse(rhs_current);
        }
    }

    return *this;
}

template <typename T>
Queue<T>::Queue(Queue<T>&& rhs) : Queue()
{
    *this = std::move(rhs);
}

template <typename T>
Queue<T>& Queue<T>::operator=(Queue<T>&& rhs)
{
    std::swap(rhs.tail, tail);
    std::swap(rhs.head, head);
    return *this;
}

#pragma endregion

#pragma region Operations

template <typename T>
void Queue<T>::enqueue(T value)
{
    auto newNode = new Queue_node(value);

    if(empty())
        head = newNode;
    else
        tail->next = newNode;

    tail = newNode;
}

template <typename T>
T Queue<T>::dequeue()
{
    if(empty())
        throw std::runtime_error("Dequeue attempt on empty queue");

    T retval = head->value;

    Queue_node* dequeued_node = head;
    head = dequeued_node->next;
    delete dequeued_node;

    return retval;
}

template <typename T>
bool Queue<T>::empty()
{
    return (head == nullptr);
}

#pragma endregion

#pragma region Operators

template <typename T>
bool Queue<T>::operator==(const Queue<T>& rhs)
{
    Queue_node* current = head;
    Queue_node* rhs_current = rhs.head;

    while((current != nullptr) && (rhs_current != nullptr))
    {
        if (current->value != rhs_current->value)
            return false;

        traverse(current, rhs_current);
    }

    if(current != rhs_current)
        return false;

    return true;
}

template <typename T>
bool Queue<T>::operator!=(const Queue<T>& rhs)
{
    return !(*this == rhs);
}

#pragma endregion
