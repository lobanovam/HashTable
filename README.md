# HashTable
## Introduction

This projects is part of I.R.Dedinskiy programming course (1st year MIPT DREC). \
**Goals**: \
**1)** implement hash table \
**2)** research several hash functions \
**3)** find some ways to optimize searching

## About Hash Table
A hash table is a data structure that is used to store keys/value pairs. 
It uses a hash function to compute an index into an array in which an element will be inserted or searched. By using a good hash function,
hashing can work well. Under reasonable assumptions, the average time required to search for an element in a hash table is O(1).

To solve collisions (when hashFunc(key) = certainValue for more than one key) i use separate chaining technique. \
In separate chaining, the process involves building a linked list with key–value pair for each search array index. 
The collided items are chained together through a single linked list, which can be traversed to access the item with a unique search key.

<img src="png's/hashTableExample.png">

For more detailed information please visit the link below:
https://en.wikipedia.org/wiki/Hash_table


