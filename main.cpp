#include <iostream>
#include <list>
#include "hmap.hpp"
#include <unordered_map>
#include "Timer.h"
#include "u_map.hpp"

#define _test1 0
#define _test2 0
#define _test3 1
#define _test4 0
#define _test5 0

#if _test1
int main(int argc, const char * argv[]) {
    
    int N = 10;
    std::unordered_map<int, int> map1;

    {
       Timer timer;
        for(int i=0; i < N; ++i)
            map1.insert({i, i});
    }


    {
       Timer timer;
       
       for(int i=0; i<N; ++i)
           map1.find(i);
    }

    return 0;
}

#endif

#if _test2
int main(int argc, const char * argv[]) {
    
    int N = 10;
    std::unordered_map<int, int> map1;
    hmap<int, int> map2;
   
    {
       Timer timer;
        for(int i=0; i < N; ++i)
            map1.insert({i, i});
    }

    {
       Timer timer;
       
        for(int i=0; i < N; ++i)
            map2.insert(i, i);
    }

    {
       Timer timer;
       
       for(int i=0; i<N; ++i)
           map1.find(i);
    }


    {
        Timer timer;
        
        for(int i=0; i<N; ++i)
            map2.find(i);
    }

    return 0;
}
#endif

#if _test3

int main(int argc, const char * argv[]) {
    
    int N = 10;
    std::unordered_map<int, int> map1;
    mkw::u_map<int, int> t_map;

    {
       Timer timer;
        for(int i=0; i < N; ++i)
            map1.insert({i, i});
    }

    {
       Timer timer;
       
        for(int i=0; i < N; ++i)
            t_map.insert(i, i);
    }

    {
       Timer timer;
       
       for(int i=0; i<N; ++i)
           map1.find(i);
    }

    {
        Timer timer;
        
        for(int i=0; i<N; ++i)
            t_map.find(i);
    }

    return 0;
}
#endif


#if _test4
int main(int argc, const char * argv[]) {
    
    int N = 10;
    std::unordered_map<int, int> map1;
    hmap<int, int> map2;
    mkw::u_map<int, int> map3;

    {
       Timer timer;
        for(int i=0; i < N; ++i)
            map1.insert({i, i});
    }

    {
       Timer timer;
       
        for(int i=0; i < N; ++i)
            map2.insert(i, i);
    }

    {
       Timer timer;
       
        for(int i=0; i < N; ++i)
            map3.insert(i, i);
    }

    {
       Timer timer;
       
       for(int i=0; i<N; ++i)
           map1.find(i);
    }


    {
        Timer timer;
        
        for(int i=0; i<N; ++i)
            map2.find(i);
    }

    {
        Timer timer;
        
        for(int i=0; i<N; ++i)
            map3.find(i);
    }

    return 0;
}
#endif
