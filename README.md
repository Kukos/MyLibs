# C Library Set (CLS)

#### Overview
CLS is a set of basic libraries which can be found in C++, java etc. Unfortunately stdlib in C lang is quite poor, so new library set was created.
Because ANSI and c99 standarts are not enough to write a user friendly interface, I chose std gnu99 and I created interface (compiler.h / common.h) to provide compiler extensions in code as macros.

To keep code in good resist of static bugs, in this project I use a lot of gcc flags that check a lot of things analysed in compiled time.

The main idea in this project is intoduce interfaces of common structrues that can be used to create polymorphism
So a lot of C++ features were implemented here, like iterators, constructors / destructors, RAII with gcc extenstions and abstract classes (like Tree interface).
By using pointer to void and pointers to known functions we can obtain inheritance and polymorphism and a lot of absract levels in simple C code.

To get a good performance (like c++ stl or better) the bodies of funtions are optimized on algorithms level.

CLS supports also checking memory leaks and usage (during tests) with valgrind (special target in Makefile)

#### OS
Please note that project is compatible only with linux. (Tested on debian, ubuntu and arch 64Bit)

#### Licence
GPL 3.0

#### CLS contains:

##### Interfaces:

    assert.h - standart assert but with simple backtrace

    common.h - set of common defines, macros, functions like abs, min

    compiler.h - compiler extenstions as macros

    generic.h - interface to read architecture (32 / 64) and set "types like asm" BYTE, WORD etc

    iterators.h - interface for iteratos for your structure

    slist.h - abstract class of sorted lists

    test.h - basic test framework, used to test this project

    tree.h - abstract class of trees

    ulist.h - abstract class of unsorted lists

##### Libraries:
    arraylist - unsorted list implemented like normal linked list

    avl - self balanced tree avl

    bitset - array of bits

    bst - normal binary sorted tree

    crc - crc 8/16/32/64

    cstring - string container (like std :: string)

    darray - dynamic array (like std :: vector)

    fifo - dynamic fifo queue (like std :: queue)

    filebuffer - buffer for large files (wrapper for mmap)

    getch - get character without enter (like conio.h on windows)

    hash - set of non-crypto hash funtions

    heap - min and max heap on dynamic array

    histogram - draw self-resizing histogram on stdout

    klist - kernel list in user space

    list - normal linked list

    list2d - 2directional cyclic linked list

    log - simple logger (console / file)

    rbt - self balanced red black tree (like std :: map)

    ringbuffer - cyclic buffer on fixed array (overwriting oldest data by newest)

    search - implementation of linear and binary search

    sort - implementation of few sort algorithms (very good quicksort implementation better that classic sort in C)

    stack - stack of dynamic array

    trie - special tree for strings

    ufset - union find set with master -> slaves feature

### How to configure env

To build / developing CLS you need:

    gcc

    gnu makefile

    grep

    awk

    valgrind

    wc

    bc

If you have linux based on debian (like ubuntu) you can run ./scripts/install_tools.sh to install what you need.


### How to build

You can type make help to get list of all targets:

    Targets:

        all[D=1]          - build libraries, D =1 --> debug mode

        test              - make static tests

        install[P = Path] - install libs to path P or default Path

        memcheck          - make mem check using valgrind for tests

        memusage          - prepare mem measurements


    Makefile supports Verbose mode when V=1

#### Contact
email: michalkukowski10@gmail.com