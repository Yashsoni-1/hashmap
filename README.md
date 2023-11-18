# U_MAP
In this project I have tried to make fast, lightweight and cache friendly c++ hashmap. This project is based on **Malte Skarupke** cppnow talk [link](https://youtu.be/M2fKMP47slQ?si=zQ-g-f-WQBWil0c5) and [this](https://people.csail.mit.edu/shanir/publications/disc2008_submission_98.pdf) research paper from MIT. I have used [Hopscotch hashing](https://en.wikipedia.org/wiki/Hopscotch_hashing) with linear probing in this project. This hashmap is almost **5 times** faster than std::unordered_map, both while insertion as well as during lookup.

## Reasons for the std::unordered_map to be slower
1. Use of modulo operator.
2. Use of chaining.

()
   


## Optimisations.
1. Changed modulo operator to & comparision.

## Hasing Technique Used
The hashing technique that I have used in this map is Linear Probing. The main reason to choose the Linear Probing over other hasing technique is that Linear Probing guarantees the slot when the table size is of the power of 2. Other technique that can be used here is Quadratic Probing. But, the problem with Quadratic Probing is that it guarantees the free slot only when the 
