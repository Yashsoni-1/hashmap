#ifndef u_map_hpp
#define u_map_hpp

#include <iostream>
#include <functional>
#include <vector>

#define __open1 1
#define __debug_define 0

#if __open1


constexpr int umap_default_init_size = 4;
constexpr float umap_default_load_factor = 0.5;

template<typename K, typename V>
class u_map
{
private:
    
    struct u_map_node
    {
        uint8_t bit;
        std::pair<K, V> pair;
        u_map_node() : bit{0} { }
    };
    
    enum metadata
    {
        Empty,
        Deleted,
        Occupied
    };
    
    u_map_node *buckets;
public:
    size_t size;
    size_t capacity;
    float max_load_factor;
    
    using iterator = u_map_node *;
    
    u_map() : size{0}, capacity{umap_default_init_size}, max_load_factor{umap_default_load_factor}
    {
        buckets = new u_map_node[capacity];
    }
    
    iterator begin()
    {
        return buckets;
    }
    
    iterator end()
    {
        return buckets + size;
    }
    
    void insert(K key, V value)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        size_t index{};
        if(containskey(key, &index))
        {
            buckets[index].pair.second = value;
        }
        else
        {
            if(load_factor() >= max_load_factor)
            {
                rehash();
            }
        
            index = find_next_index(key);
            buckets[index].pair = std::make_pair(key, value);
            buckets[index].bit = 2;
            ++size;
        }
    }
    
    std::pair<K, V> *find(K key)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        size_t index{};
        
        return containskey(key, &index) ? &(buckets[index].pair) : nullptr;
    }
    
    float load_factor()
    {
        return (1.0 * size) / capacity;
    }
    
    void rehash()
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        capacity <<= 1;
        u_map_node *new_buckets;
        new_buckets = new u_map_node[capacity];
        
        size_t old_capacity = capacity >> 1;
        
        for(int i=0; i < old_capacity; ++i)
        {
            new_buckets[i] = buckets[i];
        }
        
        delete [] buckets;
        
        buckets = new_buckets;
        new_buckets = nullptr;
    }
    
    bool containskey(K key, size_t *idx)
    {
        #if __debug_define
                std::cout << __FUNCTION__ << '\n';
        #endif
        size_t index = bucket_index(key);
        size_t old_index = index;
        for(; (buckets[index]).bit != 0;)
        {
            if(buckets[index].pair.first == key)
            {
                *idx = index;
                return true;
            }
            
            index = (index == capacity ? 0 : index + 1);
            if(index == old_index) break;
        }
        
        return false;
    }
    
    size_t find_next_index(K key)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        
        size_t index = bucket_index(key);
        
        for(; (buckets[index]).bit == 2;)
        {
            index = (index == capacity ? 0 : index + 1);
        }
        
        return index;
    }
    
    
    std::size_t bucket_index(K key)
    {
#if __debug_define
        std::cout << __FUNCTION__ << '\n';
#endif
        std::size_t hash = std::hash<K>{}(key);
        return hash & (capacity-1);
    }
    
    void print_map()
    {
        for(size_t i=0; i<capacity; ++i)
        {
            std::cout << i << "-> ";
            std::cout << '(' << buckets[i].pair.first << ", " << buckets[i].pair.second << ')';
        }
        
        std::cout << '\n';
    }
    
    
};

#endif
#endif /* u_map_hpp */
