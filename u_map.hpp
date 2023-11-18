#ifndef u_map_hpp
#define u_map_hpp

#include <iostream>
#include <functional>
#include <vector>

#define __v1 0
#define __hopscotch 1
#define __debug_define 0

namespace mkw
{
    #if __v1

    constexpr int UMAP_DEFAULT_INIT_SIZE = 4;
    constexpr float UMAP_DEFAULT_LOAD_FACTOR = 0.5;

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
        size_t __size;
        size_t capacity;
        float max_load_factor;
        
        using iterator = u_map_node *;
        
        u_map() : __size{0}, capacity{UMAP_DEFAULT_INIT_SIZE}, max_load_factor{UMAP_DEFAULT_LOAD_FACTOR}
        {
            buckets = new u_map_node[capacity];
        }
        
        iterator begin()
        {
            return buckets;
        }
        
        iterator end()
        {
            return buckets + __size;
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
                ++__size;
            }
        }
        
        void remove(K key)
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
            return (1.0 * __size) / capacity;
        }
        
        void rehash()
        {
    #if __debug_define
            std::cout << __FUNCTION__ << '\n';
    #endif
            capacity <<= 1;
            u_map_node *old_buckets = buckets;
            buckets = new u_map_node[capacity];
            
            size_t old_capacity = capacity >> 1;
            
            for(int i=0; i < old_capacity; ++i)
            {
                insert(old_buckets[i].pair.first, old_buckets[i].pair.second);
            }
            
            delete [] old_buckets;
            old_buckets = nullptr;
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
                if((buckets[index]).bit != 1 && buckets[index].pair.first == key)
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
            for(size_t i=0; i<capacity; ++i)
            {
                std::cout << i << "->";
                std::cout << '(' << (int)(buckets[i].bit) << ", " << buckets[i].pair.first << ", " << buckets[i].pair.second << ") ";
            }
            
            std::cout << '\n';
        }
    };
    #endif





    #if __hopscotch

    constexpr int UMAP_DEFAULT_INIT_SIZE = 8;
    constexpr float UMAP_DEFAULT_LOAD_FACTOR = 0.5;

    template<typename K, typename V>
    class u_map
    {
    private:
        
        struct u_map_node
        {
            uint8_t bit;
            std::pair<K, V> pair;
            size_t home_bucket;
            u_map_node() : bit{0}, home_bucket{0} { }
        };
        
        enum metadata
        {
            Empty,
            Deleted,
            Occupied
        };
        
        u_map_node *buckets;
    public:
        size_t __size;
        size_t capacity;
        static constexpr int _H{24};
        const float max_load_factor;
        
        using iterator = u_map_node *;
        
        u_map() : __size{0}, capacity{UMAP_DEFAULT_INIT_SIZE}, max_load_factor{UMAP_DEFAULT_LOAD_FACTOR}
        {
            buckets = new u_map_node[capacity];
        }
        
        ~u_map()
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
        
        void insert(const K key, const V value)
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
                
                
                while(true)
                {
                    bool is_rehash{};
                    index = find_next_index(key, &is_rehash);
                    if(is_rehash)
                        rehash();
                    else
                        break;
                }
                
                
                buckets[index].pair = std::make_pair(key, value);
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
            return (1.0 * __size) / capacity;
        }
        
        void rehash()
        {
    #if __debug_define
            std::cout << __FUNCTION__ << '\n';
    #endif
            capacity <<= 1;
            u_map_node *old_buckets = buckets;
            buckets = new u_map_node[capacity];
            
            size_t old_capacity = capacity >> 1;
            
            for(size_t i{}; i < old_capacity; ++i)
            {
                insert(old_buckets[i].pair.first, old_buckets[i].pair.second);
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
                if((buckets[index]).bit != 1 && buckets[index].pair.first == key)
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
            std::swap(buckets[empty_index].pair, buckets[to_be_index].pair);
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
                std::cout << '(' << (int)(buckets[i].bit) << ", " << buckets[i].pair.first << ", " << buckets[i].pair.second << ") ";
            }
            
            std::cout << '\n';
        }
    };
    #endif

}




#endif /* u_map_hpp */
