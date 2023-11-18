#ifndef u_set_hpp
#define u_set_hpp

#include <iostream>

namespace mkw
{
constexpr int USET_DEFAULT_INIT_SIZE = 8;
constexpr float USET_DEFAULT_LOAD_FACTOR = 0.5;

template<typename K>
class u_set
{
private:
    
    struct u_set_node
    {
        uint8_t bit;
        K key;
        size_t home_bucket;
        u_set_node() : bit{0}, home_bucket{0} { }
    };
    
    enum metadata
    {
        Empty,
        Deleted,
        Occupied
    };
    
    u_set_node *buckets;
public:
    size_t __size;
    size_t capacity;
    static constexpr int _H{24};
    const float max_load_factor;
    
    using iterator = u_set_node *;
    
    u_set() : __size{0}, capacity{USET_DEFAULT_INIT_SIZE}, max_load_factor{USET_DEFAULT_LOAD_FACTOR}
    {
        buckets = new u_set_node[capacity];
    }
    
    ~u_set()
    {
        if(buckets)
            delete [] buckets;
        
        buckets = nullptr;
    }
    
    iterator begin()
    {
        return buckets;
    }
    
    iterator end()
    {
        return buckets + __size;
    }
    
    void insert(const K key)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        size_t index{};
        if(containskey(key, &index))
        {
            buckets[index].key = key;
        }
        else
        {
            if(load_factor() >= max_load_factor)
            {
                rehash();
            }
            
            
            while(true)
            {
                bool is_rehash{};
                index = find_next_index(key, &is_rehash);
                if(is_rehash)
                    rehash();
                else
                    break;
            }
            
            
            buckets[index].key = key;
            buckets[index].bit = 2;
            buckets[index].home_bucket = bucket_index(key);
            ++__size;
        }
    }
    
    void remove(const K key)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        size_t index{};
        if(containskey(key, &index))
        {
            buckets[index].bit = 1;
            --__size;
        }
    }
    
    K *find(K key)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        size_t index{};
        
        return containskey(key, &index) ? &(buckets[index].key) : nullptr;
    }
    
    float load_factor()
    {
        return (1.0 * __size) / capacity;
    }
    
    void rehash()
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        capacity <<= 1;
        u_set_node *old_buckets = buckets;
        buckets = new u_set_node[capacity];
        
        size_t old_capacity = capacity >> 1;
        
        for(size_t i{}; i < old_capacity; ++i)
        {
            insert(old_buckets[i].key);
        }
        
        delete [] old_buckets;
        old_buckets = nullptr;
    }
    
    bool containskey(const K key, size_t *idx)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
    
        size_t index{bucket_index(key)};
        size_t old_index{index};
        size_t i{0};
        for(; (buckets[index]).bit != 0 && i < _H; ++i)
        {
            if((buckets[index]).bit != 1 && buckets[index].key == key)
            {
                *idx = index;
                return true;
            }
            
            index = (index == capacity ? 0 : index + 1);
            if(index == old_index) break;
        }
        
        return false;
    }
    
    size_t find_next_index(const K key, bool *is_rehash)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        
        size_t empty_index {bucket_index(key)};
        size_t unbounded_index {empty_index};
        size_t home_bucket_index {empty_index};
        bool is_swapped {false};
        
        for(; (buckets[empty_index]).bit == 2;)
        {
            empty_index = (empty_index == capacity ? 0 : empty_index + 1);
            ++unbounded_index;
        }
        
        
        if(unbounded_index < (home_bucket_index + _H))
        {
            *is_rehash = false;
            return empty_index;
        }
        
        
        while(unbounded_index >= (home_bucket_index + _H))
        {
            int64_t i{static_cast<int64_t>(empty_index - (_H - 1))};
            
            if(i < 0) i += capacity;
            
            for(int j{}; j < (_H - 1); ++j)
            {
                if((buckets[i].home_bucket < home_bucket_index &&
                    empty_index < buckets[i].home_bucket + _H) ||
                   (buckets[i].home_bucket > home_bucket_index &&
                    unbounded_index < buckets[i].home_bucket + _H))
                {
                    _swap(i, empty_index);
                    empty_index = i;
                    unbounded_index -= _H - 1 - j;
                    is_swapped = true;
                    break;
                }
                
                ++i;
                i = (i == capacity ? 0 : i);
            }
            
            
            
            if(!is_swapped) break;
        }
        
        
        if (unbounded_index >= (home_bucket_index + _H))
            *is_rehash = true;
        else
            *is_rehash = false;

        
        return  empty_index;
    }
    
    void _swap(const size_t to_be_index,const size_t empty_index)
    {
        std::swap(buckets[empty_index].bit, buckets[to_be_index].bit);
        std::swap(buckets[empty_index].home_bucket, buckets[to_be_index].home_bucket);
        std::swap(buckets[empty_index].key, buckets[to_be_index].key);
    }
    
    static bool in_range(size_t index, size_t home_bucket_index)
    {
        
    }
    
    
    std::size_t bucket_index(const K key)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        std::size_t hash = std::hash<K>{}(key);
        return hash & (capacity-1);
    }
    
    bool empty()
    {
        return __size == 0;
    }
    
    size_t size()
    {
        return __size;
    }
    
    void print_map()
    {
        for(size_t i{}; i<capacity; ++i)
        {
            std::cout << i << "->";
            std::cout << '(' << (int)(buckets[i].bit) << ", " << buckets[i].pair.key << ") ";
        }
        
        std::cout << '\n';
    }
};
};

#endif /* u_set_hpp */
