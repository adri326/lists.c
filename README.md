# lists.c

Different kinds of lists and list-like structures, implemented in pure C (ie. using pretty much only macros).

## Currently implemented

- Linked Lists (`src/linkedlist.h`): comes in two flavors, a value-based one (`LL(type)`) and a pointer-based one (`LL_PTR(type)`); shipped without cyclicity assertions!
- Vector (TODO): scalable arrays
- Queue (TODO): scalable circular arrays

## Installation

The included build files make use of `cmake`.

You are very much going to have this repository as a depency of your program.
If you aren't using `git` for said program already, then simply create a directory called `dependencies` and run `git clone https://github.com/adri326/lists.c dependencies/lists`.

Otherwise, you may run the following set of commands:

```sh
mkdir dependencies # if this hasn't been done already
git submodule add https://github.com/adri326/lists.c dependencies/lists
# Don't forget to run `git commit`!
```

Once that is done, head to your `CMakeLists.txt` file and include this library:

```cmake
include(dependencies/lists/include.cmake)
target_link_libraries(YOUR_PROJECT PUBLIC lists-c)
target_include_directories(YOUR_PROJECT PUBLIC ${LISTS_C_INCLUDE_DIR})
```

You may now include this file in your source code:

```c
#include <linkedlist.h>

DECL_LL(int); // declare the structure (put this in your .h if you're using this structure as a return value or argument or if you are using the structure across files)
DECL_LL_SOURCES(int, "%d"); // fill in the body of the different linked list-related functions

int main() {
  LL(int)* int_list = int_ll_new(0); // create a new linked list containing [0]
  int_list = int_ll_push_head(1); // append 1 to the list
  int_ll_printf(int_list); // print the list ([1, 0])
  int_ll_free(int_list); // free the list
}
```
