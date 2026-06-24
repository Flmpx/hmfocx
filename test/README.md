# Test Code Format

## Main Test
**This is a example** with `hm_list` test
```c
int main()
{
    function_test();

    boundary_test();

    stress_test();

    return all_failure_num;
}
```
1. **Function Test**
- The format of every test funciton's name is `test_{container}_{main content of test}`

```c
void function_test() {
    test_list_init();                               printf("\n");
    // You should add function of `printf("\n");` after every test done
    
    test_list_insert_head();                        printf("\n");
    test_list_insert_tail();                        printf("\n");
    test_list_insert_index();                       printf("\n");
    
    test_iter_list();                               printf("\n");
    
    // and more ...

}
```

2. **Boundary Test**

- The format of every test funciton's name is `test_{main content of test}`

```c
void boundary_test() {
    test_empty_list_oper();                         printf("\n");

    test_freed_list_oper();                         printf("\n");

    test_single_listnode_oper();                    printf("\n");

}
```

3. **Stress Test**
- The format of every test funciton's name is `test_{container}_{main content of test}_stress`
```c
void stress_test() {

    test_list_insert_tail_stress();                 printf("\n");
    
    test_list_insert_head_stress();                 printf("\n");

    test_list_insert_index_stress();                printf("\n");
    
    // and more ...

}
```
