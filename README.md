# hashmap

## Reasons for the hashmap to be slower
1. Use of modulo operator.
2. Use of less cache due to chaining. 


## Optimisations.
1. Changed modulo operator to & comparision.

## Hasing Technique Used
The hashing technique that I have used in this map is Linear Probing. The main reason to choose the Linear Probing over other hasing technique is that Linear Probing guarantees the slot when the table size is of the power of 2. Other technique that can be used here is Quadratic Probing. But, the problem with Quadratic Probing is that it guarantees the free slot only when the 
