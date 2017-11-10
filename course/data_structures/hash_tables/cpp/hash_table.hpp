#pragma once
// Warning: this library may throw

#include <string>

template <typename T>
class HashTable
{
    // constants
    protected:
        static constexpr std::size_t default_size = 10;

    // subclasses and internal structures
    protected:
        // todo: move this inside Element
        // todo: redo all identifiers
        enum class State
        {
            empty,
            deleted,
            occupied
        };

        enum class Caller
        {
            insertion,
            other
        };

        struct Element
        {
            T value;
            std::string key;
            State state = State::empty;
        };

    // data members
    protected:
        std::size_t _size;
        std::size_t _capacity;
        // todo: move all this to hasher
        // todo: instantiate hasher in the constructor
        struct Hashing_constants
        {
            long long unsigned A, B, P;
            Hashing_constants();
            Hashing_constants(HashTable<T>* caller);
        } _constants;
        Element* _data;

    // private functions
    protected:
        // todo: move all this to a hasher, after complexity tests
        long long unsigned compute_nearest_prime() const;
        long long unsigned compute_initial_hash(std::string key) const;
        std::size_t next_probing_index(std::size_t previous_index) const;
        template <Caller _caller = Caller::other> std::size_t find_index_for(std::string key) const;

    // constructing, assigning, destructing
    public:
        HashTable();
        HashTable(std::size_t desired_capacity);
        ~HashTable();
        HashTable(const HashTable<T>& rhs);
        HashTable<T>& operator=(const HashTable<T>& rhs);
        HashTable(HashTable<T>&& rhs);
        HashTable<T>& operator=(HashTable<T>&& rhs);

    // public functions
    public:
        void add(std::string key, T value);
        bool exists(std::string key) const;
        T get(std::string key) const;
        void remove(std::string key);

    // operators
    public:
        bool operator== (const HashTable<T>& rhs);
        bool operator!= (const HashTable<T>& rhs);
};

#include "hash_table.cpp"
