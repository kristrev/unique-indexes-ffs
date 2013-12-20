unique-indexes-ffs
==================

A project I recently belonged to required me to implement an algorithm for allocating unique indexes to objects. A straight forward way to do this would be to create a FIFO containing all the indexes and then pushing/popping. This technique would provide O(1) insert and removal, but the storage requirement would be sizeof(type used for index)*number of indexes.

Our project was storage-bound and I therefore decided to create a bitmap and use the magnificent and often under-appreciated ffs() to distribute the indexes. ffs() finds the first (least significant) bit that is set in an integer (ffsl() does the same for long). The bitmap is an array of unsigned integers and we iterate through this array until we find an index with an available index (bitmap[i] != 0). ffs() is then used to extract the bit and the bit is set to 0 to mark that the index is not available.

This gives a worst-case lookup of O(N) and an unset of O(1). However, the space requirement is siginificantly reduced as four bytes can store 32 values. Our number of indexes were always multiples of 32, so no space was wasted.
