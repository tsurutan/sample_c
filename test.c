#include<stdio.h>

int main(){
      int a = 1;
    int *p_a = &a;
    int **pp_a = &p_a;
    printf("変数aのアドレスは%d\n",a);
    printf("変数aのアドレスは%d\n",&a);
    printf("変数aのポインタは%d\n",p_a);
    printf("変数aのポインタは%d\n",*(p_a));
    printf("変数aのポインタは%d\n",**(pp_a));
    *p_a = 3;
    printf("変数aのアドレスは%d\n",a);
    return 0;
}
