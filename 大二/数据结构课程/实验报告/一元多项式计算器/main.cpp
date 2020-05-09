#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
using namespace std;
#define MAX_SIZE 1000

typedef struct {
    int coef; //系数
    int pow; //幂次
} ElemType;
// 线性表
typedef struct array{
    int size; // 开辟的空间大小
    int length; // 有效数据长度
    int *data;
}Array;
// 单链表
typedef struct node {
    ElemType data;
    struct node *next;
}Node, *Link;
int pows[MAX_SIZE * 2 + 10];
void inputMessage() {
    printf("因变量为x，不需要输入。输入多项式请按要求每一行输入两个数字，");
    printf("分别代表x的系数与幂次，幂次由高到低。\n两数请用空格隔开，以文件结束符终止输入\n");
}
// 创建一个空的线性表
Array getEmptyArray() {
    Array arr;
    arr.size = MAX_SIZE * 2 + 10;
    arr.data = (int*)malloc(sizeof(int) * arr.size);
    for (int i = 0; i < arr.size; ++ i) arr.data[i] = 0;
    arr.length = 0;
    return arr;
}
// 线性表：输入多项式
Array inputPolynomialArray() {
    int coef, pow;
    Array arr = getEmptyArray();
    while(~scanf("%d%d", &coef, &pow)) {
        arr.length = max(arr.length, pow);
        arr.data[pow] = coef;
    }
    ++ arr.length;
    return arr;
}
// 多项式相乘
Array mulPolynomial(Array A, Array B) {
    Array arr = getEmptyArray();
    int coef, pow;
    for (int i = 0; i < A.length; ++ i) {
        for (int j = 0; j < B.length; ++ j) {
            coef = A.data[i] * B.data[j];
            pow = i * j;
            arr.data[pow] += coef;
            arr.length = max(arr.length, pow);
        }
    }
    ++ arr.length;
    return arr;
}
// 线性表，输出多项式
void printPolynomialArray(Array arr) {
    bool f = true;
    printf("升幂：");
    for (int i = 0; i < arr.length; ++ i) {
        if(arr.data[i] == 0) continue;
        if(!f) printf(" %c ", arr.data[i] > 0 ? '+' : '-');
        else if(arr.data[i] < 0) printf("-");
        f = false;
        printf("%dx^%d", abs(arr.data[i]), i);
    }
    printf("\n降幂：");
    f = true;
    for (int i = arr.length - 1; i >= 0; -- i) {
        if(arr.data[i] == 0) continue;
        if(!f) printf(" %c ", arr.data[i] > 0 ? '+' : '-');
        f = false;
        printf("%dx^%d", abs(arr.data[i]), i);
    }
    puts("");
}
//单链表尾插法函数，减少代码量
Link rearInsert(Link p, int coef, int pow) {
    Link tmp = tmp = (Link)malloc(sizeof(Node));
    tmp->next = NULL;
    tmp->data.coef = coef;
    tmp->data.pow = pow;
    p->next = tmp;
    p = tmp;
    return p;
}
//单链表，输入一个多项式
Link inputPolynomialList() {
    Link head = (Link)malloc(sizeof(Node));
    head->next = NULL;
    int coef, pow;
    Link tmp, p;
    p = head;
    while(~scanf("%d%d", &coef, &pow))
        p = rearInsert(p, coef, pow);
    return head;
}
//单链表，输出多项式
void printPolynomialList(Link head) {
    Array arr = getEmptyArray();
    Link p = head;
    while(p->next != NULL) {
        int coef = p->next->data.coef;
        int pow = p->next->data.pow;
        arr.data[pow] += coef;
        arr.length = max(arr.length, pow);
        p = p->next;
    }
    ++ arr.length;
    printPolynomialArray(arr);
}
//多项式相加减
Link addOrSubPolynomial(Link A, Link B, bool add) {
    Link head = (Link)malloc(sizeof(Node));
    Link p, tmp;
    int coefA, coefB, coef;
    int powA, powB, pow;
    p = head;
    // 链表加减，先计算出幂次高的值
    while(A->next && B->next) {
        coefA = A->next->data.coef;
        coefB = B->next->data.coef;
        powA = A->next->data.pow;
        powB = B->next->data.pow;
        if(powA == powB) {
            if(add) coef = coefA + coefB;
            else coef = coefA - coefB;
            pow = powA;
            A = A->next;
            B = B->next;
            if(coef == 0) continue;
        } else if(powA > powB) {
            coef = coefA;
            pow = powA;
            A = A->next;
        } else {
            if(add) coef = coefB;
            else coef = -coefB;
            pow = powB;
            B = B->next;
        }
        p = rearInsert(p, coef, pow);
    }
    while(A->next) {
        p = rearInsert(p, A->next->data.coef, A->next->data.pow);
        A = A->next;
    }
    while(B->next) {
        if(add) p = rearInsert(p, B->next->data.coef, B->next->data.pow);
        else p = rearInsert(p, -B->next->data.coef, B->next->data.pow);
        B = B->next;
    }
    return head;
}
int main() {
    printf("1:多项式+，-\n");
    printf("2:多项式*\n");
    printf("0:退出\n");
    int choose;
    while(~scanf("%d", &choose)) {
        if(choose == 0) break;
        inputMessage();
        if(choose == 1) {
            printf("请输入多项式A：\n");
            Link A = inputPolynomialList();
            printf("请输入多项式B：\n");
            Link B = inputPolynomialList();
            int op;
            printf("0：-\n1：+\n");
            scanf("%d", &op);
            Link C = addOrSubPolynomial(A, B, op);
            printPolynomialList(C);
        } else {
            printf("请输入多项式A：\n");
            Array A = inputPolynomialArray();
            printf("请输入多项式B：\n");
            Array B = inputPolynomialArray();
            Array C = mulPolynomial(A, B);
            printPolynomialArray(C);
        }
        puts("---------");
        printf("1:多项式+，-\n");
        printf("2:多项式*\n");
        printf("0:退出\n");
    }
    return 0;
}
