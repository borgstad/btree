# B-Tree
A B-tree is a ubiquitous data structure used in
[filesystems](https://en.wikipedia.org/wiki/Btrfs) and
[databases](https://www.postgresql.org/docs/current/indexes-types.html). It can
be seen as a generalization of a self-balancing binary tree, where each node has
t >= 2 keys, and t + 1 pointers to their children, a property that minimizes I/O
operations when the amount of data cannot be held in memory. 

The B-tree was first described in 
[1970](https://infolab.usc.edu/csci585/Spring2010/den_ar/indexing.pdf) 
and has continued to be relevant and an area of active research. Modern
implementations (like the [Lehman and
Yao](https://www.csd.uoc.gr/~hy460/pdf/p650-lehman.pdf) algorithm used by
Postgres, although not so modern) supports high level of concurrency. However,
this repository contains an implementation of a B-tree as described in
[CLRS](https://en.wikipedia.org/wiki/Introduction_to_Algorithms) which is not
concurrent, but the supported operations are out-of-core. 

This project has been created to get further experience in C, and have fun with
algorithms!


## Implementation
The CLRS implementation is followed strictly, and integers are supported as the
only data type. The `btree.c` module exposes the functions `btreeInsert` and
`btreeSearch` and every optimization and design choice is done to make these operations faster.

In the process of doing this project, I added a hash table,
linked list, and a cache. Only the latter will be explained.


### B-tree struct
The nodes in the tree are implemented as a struct, `Node`, which holds satellite
data, `BlockId`, which is a reference to children, and metadata. 

The `BlockId` is a strictly monotonically increasing number, uniquely
identifying each node. This number is also a disk offset, more in this in the
next subsection.

The size of each node struct is determined by the
underlying OS page size defined by the POSIX function `getpagesize`. The 
OS page size is also the unit of I/O and should yield a (minor) optimization.

Creating and inserting data into the B-tree is a simple two step process, using
the functions:
```C
Btree btreeInit();
Btree btreeInsert(Btree T, int value);
```
The function `btreeInit` initializes an empty B-tree -- The `Btree` struct
simply contains a pointer to the root node, and the `BlockId` of the root node.
`btreeInsert` Takes a `Btree` as input, along with the value to be inserted.
That is all.


### I/O and caching
The B-tree uses the read and write functions implemented by the `io` module.
The `io` module implements an LRU scheme which decides when to read and write. This means that
when the B-tree calls the write function for a particular node and the cache is
not full:

1. A pointer reference to the node is inserted into a hash table whose key is 
the `BlockId`. If the node already exists in the hast table, nothing happens.
2. The node is inserted in the LRU-cache, which is a linked list holding the
   `BlockId` of the node. If the node exists in the cache, nothing is done.

If the cache is full and a node is to be inserted, a `BlockId` is popped from
the cache and the corresponding node is saved to disk by offsetting it with the
`BlockId`. Lastly, the entry in the hash table is removed.

To flush the content of the B-tree, for example after a couple of millions of
inserts and searches, use the function:
```C
void btreeFlush(int maxDegree);
```
This saves all data to a file `index.b`


### Directory structure
The project is implemented in C. The `src` directory contains the `.c` source
files, the `include` directory contains the headers. There is one header per
source file. The `tests` directory contains test for each of the source files.
The 


## Testing
The tests are implemented simply as programs compiled and executed in
the build/Make process.