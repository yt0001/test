//restrict.c
#include <stdio.h>
 
int foo(int *a, int *b)
{
    *a = 5;
    *b = 6;
    return *a + *b;
}
 
int rfoo(int *restrict a, int *restrict b)
{
    *a = 5;
    *b = 6;
    return *a + *b;
}
 
int main(void)
{
    int i =0;
    int *a = &i;
    int *b = &i;
 
    printf("%d ",foo(a,b));    // 输出 12
    printf("%d ", rfoo(a,b));  // 可能输出 11
    return 0;
}
