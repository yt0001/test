
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
/*优化前s
 
int ar[10];
int* par = ar;    //既可以通过par也可以通过ar访问本数组
 
restrict int* p1 = (int*)malloc(10 * sizeof(int));
 
for(int i = 0; i < 10; ++i)
{
    par[i] += 5;    //语句1
    p1[i] += 5;     //语句2
    arr[i] *= 2;    //语句3
    par[i] += 3;    //语句4
    p1[i] += 3;    //语句5
}  
 */

/*优化后

 int ar[10];
int* par = ar;    //既可以通过par也可以通过ar访问本数组
 
restrict int* p1 = (int*)malloc(10 * sizeof(int));
 
for(int i = 0; i < 10; ++i)
{
    par[i] += 5;    //语句1
    p1[i] += 8;     //语句2
    arr[i] *= 2;    //语句3
    par[i] += 3;    //语句4
}  
 */