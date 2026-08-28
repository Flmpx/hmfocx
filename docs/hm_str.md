<a id = "head"></a>

# The detailed information about `hm_str`


## Navigation
- [Introduction](#intro)
- [Functions](#func)
    - [Small Functions](#smallfunc)
    - [Initialize](#init)
    - [Append](#append)
    - [Get](#get)
    - [Pop](#pop)
    - [Shrink](#shrink)
    - [Clear](#clear)
    - [Free](#free)
- [Other Things](#otherthings)
- [Other Containers](#othercontainer)



<a id = "intro"></a>

## Introduction
- You can use the function(like append) to operate a string
- It provides base str operations


<a id = "func"></a>

## Functions

>  [!Tip]
>  - `\0` is a valid characters in this str container
>  - Cosidering the string saftey, `\0` will existed in the string at the index of str's length


>  [!Note]  
>  - Most of the functions will **abort**(in debug build) when `hm_str*` is `NULL`, so, you have to make sure the pointer is valid
>  - **Exception:** Pass `NULL` in `hm_str_free()` is allowed
>  - Whether other pointer parameters can be `NULL` depends on the function comments below


<br><br><br>

<a id = "smallfunc"></a>

> **Small Functions**

```c
size_t hm_str_len(hm_str* str);
size_t hm_str_capacity(hm_str* str);
```
<br><br><br>



<a id = "init"></a>

> **Initialize**

```c
/**
 * Initialize str
 */
hm_str_ret hm_str_init(hm_str* str);

/**
 * Reserve capacity and initialize str
 * 
 * @note Use the parameter **capacity** to set the start capacity of this str
 * 
 * @return Return **hm_str_ret_suc** when initialize success
 * @return Return **hm_str_ret_error** when initialize failure
 */
hm_str_ret hm_str_init_reserve(hm_str* str, size_t capacity);
```

<details>
<summary>try: init</summary>

```c
#include <hm_str.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_str_status(hm_str* str) {
    printf("len: %zu, capacity: %zu\n", hm_str_len(str), hm_str_capacity(str));
}

int main() 
{
    hm_str str;
    // init
    hm_str_init(&str);
    print_str_status(&str);
    hm_str_free(&str);

    // init with reserve
    size_t capacity = 1314;
    hm_str_init_reserve(&str, capacity);
    print_str_status(&str);
    hm_str_free(&str);
    
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
len: 0, capacity: 0
len: 0, capacity: 1314
```

</details>

</details>
<br><br><br>


<a id = "append"></a>

> **Append**

```c
/**
 * Append string in str
 * 
 * @note Use the parameter to set the length of sub_str
 * 
 * @return Return **hm_str_ret_error** when append fail
 * @return Return **hm_str_ret_suc** when append success
 * 
 * @warning Append itself is prohibited
 * @warning The parameter **sub_str** shouldn't be **NULL**
 */
hm_str_ret hm_str_append(hm_str* str, const char* sub_str, size_t sub_str_len);

/**
 * Append character in str
 * 
 * @return Return **hm_str_ret_error** when append fail
 * @return Return **hm_str_ret_suc** when append success
 */
hm_str_ret hm_str_append_ch(hm_str* str, char ch);
```

<a id = "get"></a>

> **Get**

```c
/**
 * Get the str at the specified index
 * 
 * @note **Index**  must be >= **0**, and <= **the len of str**
 * 
 * @return Return **NULL** when **index** is out of bounds
 * 
 * @warning Change the string is prohibited
 */
const char* hm_str_get(hm_str* str, size_t index);
```

<details>
<summary>try: append & get</summary>

```c
#include <hm_str.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* strings[] = {"ABC", "DEF", "GHI"};
int num = sizeof(strings) / sizeof(char*);

void print_str(hm_str* str) {
    size_t len = hm_str_len(str);
    // get
    for (int i = 0; i < len; i++) {
        const char* s = hm_str_get(str, i);
        printf("%s\n", s);
    }
}

int main() {
    hm_str str;
    hm_str_init(&str);

    // append
    for (int i = 0; i < num; i++) {
        hm_str_append(&str, strings[i], strlen(strings[i]));
    }

    // print
    print_str(&str);

    hm_str_free(&str);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
ABCDEFGHI
BCDEFGHI
CDEFGHI
DEFGHI
EFGHI
FGHI
GHI
HI
I
```

</details>

</details>

<details>
<summary>try: append character & get</summary>

```c
#include <hm_str.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char chs[] = {'I', 'L', 'X', 'L'};
int num = sizeof(chs) / sizeof(char);

void print_str(hm_str* str) {
    size_t len = hm_str_len(str);
    // get
    for (int i = 0; i < len; i++) {
        const char* s = hm_str_get(str, i);
        printf("%s\n", s);
    }
}

int main() {
    hm_str str;
    hm_str_init(&str);

    // append character
    for (int i = 0; i < num; i++) {
        hm_str_append_ch(&str, chs[i]);
    }

    // print
    print_str(&str);

    hm_str_free(&str);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
ILXL
LXL
XL
L
```

</details>

</details>
<br><br><br>

<a id = "pop"></a>

> **Pop**

```c
/**
 * Pop the string from str
 * 
 * @note The string will be removed but not free it(Memory Ownership Transfer)
 * @note Please free this return string after use
 * 
 * @warning The str can't be used after call this function because the lifetime of str is over
 */
char* hm_str_pop(hm_str* str);
```

<details>
<summary>try: pop</summary>

```c
#include <hm_str.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* strings[] = {"Hi, ", "I'm ", "Flmpx"};
int num = sizeof(strings) / sizeof(char*);

int main() {
    hm_str str;
    hm_str_init(&str);

    // append
    for (int i = 0; i < num; i++) {
        hm_str_append(&str, strings[i], strlen(strings[i]));
    }

    // pop
    char* s = hm_str_pop(&str);     // str can't be use after pop
    printf("%s\n", s);

    free(s);

    return 0;
}
```

<details>
<summary>run result</summary>

```txt
Hi, I'm Flmpx
```

</details>

</details>
<br><br><br>


<a id = "shrink"></a>

> **Shrink**

```c
/**
 * Shrink the capacity of str if possible
 * 
 * @note Shrink to the half of original capacity every call this function
 * 
 * @return Return **hm_str_ret_suc** when shrink success
 * @return Return **hm_str_ret_none** when the str can't be shrunk
 * @return Return **hm_str_ret_error** when shrink failure
 */
hm_str_ret hm_str_shrink(hm_str* str);
```

<details>
<summary>try: shrink</summary>

```c
#include <hm_str.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void print_str_status(hm_str* str) {
    printf("| len: %-5zu capacity: %5zu\n", hm_str_len(str), hm_str_capacity(str));
}

int main() {
    hm_str str;

    int capcity = 1314;
    hm_str_init_reserve(&str, capcity);

    // shrink and print status
    print_str_status(&str);
    while (hm_str_shrink(&str) == hm_str_ret_suc) {
        print_str_status(&str);
    }

    hm_str_free(&str);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
| len: 0     capacity:  1314
| len: 0     capacity:   657
| len: 0     capacity:   328
| len: 0     capacity:   164
| len: 0     capacity:    82
| len: 0     capacity:    41
| len: 0     capacity:    20
| len: 0     capacity:    10
| len: 0     capacity:     5
| len: 0     capacity:     2
| len: 0     capacity:     1
```

</details>

</details>
<br><br><br>

<a id = "clear"></a>

> **Clear**

```c
/**
 * Clear the str
 */
void hm_str_clear(hm_str* str);
```

<details>
<summary>try: clear</summary>

```c
#include <hm_str.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* strings[] = {"Hi, ", "I'm ", "Flmpx"};
int num = sizeof(strings) / sizeof(char*);

void print_str_status(hm_str* str) {
    printf("| len: %-5zu capacity: %5zu\n", hm_str_len(str), hm_str_capacity(str));
}

int main() {
    hm_str str;
    hm_str_init(&str);

    // append
    for (int i = 0; i < num; i++) {
        hm_str_append(&str, strings[i], strlen(strings[i]));
    }
    printf("%s\n", hm_str_get(&str, 0));
    print_str_status(&str);
    
    // clear
    hm_str_clear(&str);
    printf("%s\n", hm_str_get(&str, 0));
    print_str_status(&str);

    hm_str_free(&str);
    return 0;
}
```

<details>
<summary>run result</summary>

```txt
Hi, I'm Flmpx
| len: 13    capacity:    16

| len: 0     capacity:    16
```

</details>

</details>
<br><br><br>


<a id = "free"></a>

> **Free**

```c
/**
 * Free the str
 * 
 * @warning The str can't be used after call this function because the lifetime of str is over
 */
void hm_str_free(hm_str* str);
```

<details>
<summary>try: free</summary>

```c
#include <hm_str.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* strings[] = {"Hi, ", "I'm ", "Flmpx"};
int num = sizeof(strings) / sizeof(char*);

int main() {
    hm_str str;
    hm_str_init(&str);

    // append
    for (int i = 0; i < num; i++) {
        hm_str_append(&str, strings[i], strlen(strings[i]));
    }

    // str must be freed after use
    hm_str_free(&str);      // // str can't be use after free
    return 0;
}
```

</details>
<br><br><br>





<a id = "otherthings"></a>

## Other Things

>  [!Tip]
>  - You can't use the str after `pop` or `free` because the lifetime of str is over , you should init it when you want to use it again
>  




<a id = "othercontainer"></a>

## Other Containers

1. [hm_list](hm_list.md)

2. [hm_map](hm_map.md)

3. [hm_pool](hm_pool.md)

4. [hm_stack](hm_stack.md)

5. [hm_queue](hm_queue.md)

6. [hm_heap](hm_heap.md)

7. [hm_set](hm_set.md)

8. [hm_arr](hm_arr.md)

9. hm_str



<br><br><br>
<div align = "right">
    <a href="#head">↑ Top</a>
</div>