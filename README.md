# U_MAP
In this project I have tried to make fast, lightweight and cache friendly c++ hashmap. This project is based on **Malte Skarupke** cppnow talk [link](https://youtu.be/M2fKMP47slQ?si=zQ-g-f-WQBWil0c5) and research paper from MIT [link](https://people.csail.mit.edu/shanir/publications/disc2008_submission_98.pdf). I have used [Hopscotch hashing](https://en.wikipedia.org/wiki/Hopscotch_hashing) with linear probing in this project. This hashmap is almost **5 times** faster than std::unordered_map, both while insertion as well as during lookup with ***O(1)*** worst case time complexity.

## Reasons for the std::unordered_map to be slower
1. Use of modulo operator.
2. Use of chaining.

![Performance of std::unordered_map](https://github.com/Yashsoni-1/u_map/blob/main/Images/graphs/std__unordered_map2.png)

> NOTE: Here Average lookup time means the average lookup time per key that is present in the table.
- It provided almost constant lookup time till 1 million capacity.
- Afterwards, due to cache misses, lookup time goes up rapidly.

## Optimisation 1
### Changed modulo operator to & comparision
![Performance improvement after 1st Optimisation graph](https://github.com/Yashsoni-1/u_map/blob/main/Images/graphs/std__unordered_map%20and%20h_map2.png)

> Note: Here h_map is the map after 1st optimisation.
- It was almost 2x faster till 40 million capacity.
- But, still it is not cache friendly.

## Optimisation 2
### Used simple Open Addressing with Linear Probing
![Performance improvement after 2nd Optimisation graph](https://github.com/Yashsoni-1/u_map/blob/main/Images/graphs/Average%20Lookup%20Time%20in%20std__unordered_map%20and%20t_map.png)

> Note: Here t_map is the map after 2nd optimisation.
- High performance boost can be seen after this optimisation.
- The problem with this map was that in case of key being not present we were looping entire table.
- ***O(n)*** worst case time complexity for lookup.

## Optimisation 3 (final)
### Using Hopscotch Hashing with Linear Probing
![Performance improvement after 3rd optimisation graph](https://github.com/Yashsoni-1/u_map/blob/main/Images/graphs/Average%20Lookup%20Time%20in%20std__unordered_map%2C%20h_map%20and%20mkw__u_map2.png)

> Note: Map mkw::u_map is the final product.
- **More performance boost can be seen after this optimisation.**
- **mkw::u_map is fast.**
- **mkw::u_map solves the problem of modulo operator.**
- **mkw::u_map solves the problem of cache misses.**
- **mkw::u_map solves the problem of worst case time complexity.**
- ***O(1)*** worst case time complexity.
- **Best insertion time of all the maps discussed above.**
- **At most 2 cache misses before object is found.**

### Choosing load factor

- This table relies on low load factor for performance.
- Range is 0.3 - 0.9.
- If performance matters then 0.4.
- If memory efficiency is what we looking for then 0.7.
- I have used 0.5.

> Note: Load factor does not play significant role in Hopscotch hashing.

### Choosing Neighbourhood size

![Choosing neighbourhood size for lookup graph](https://github.com/Yashsoni-1/u_map/blob/main/Images/graphs/Average%20Lookup%20Time.png)
- Hopscotch hashing is based on Neighbourhood size.
- Good size can really boost the performance.
- Poor size can damage the performance.
- Setting Neighbourhood size equal to size of cache line is what recommended.
- This will ensure that there is atmost 2 cache misses.
- I found 24 giving the best performance.
![Choosing neighbourhood size for insertion graph](https://github.com/Yashsoni-1/u_map/blob/main/Images/graphs/Average_Insertion_Time%20.png)

### To Do Next
- Make concurrent hashmap.
  
