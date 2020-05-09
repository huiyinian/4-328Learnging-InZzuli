#include <stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct node *LIST;
typedef struct node *TREE;
struct node
{
    int high;//高位ascll
    int low;//判断是否是中文，低位
    int weight;//权值
    int code_index;//编码指针
    char code[100];//编码数组
    TREE Left;//左孩子
    TREE Right;//右孩子
    LIST NEXT;//下一个
};
void insert(LIST head, LIST tmp);//有序插入结点
LIST find_and_insert(LIST head, LIST tmp);//弹出内部的结点，然后调用insert函数
void output(LIST head);//输出这个读取文件的所有字符统计情况
LIST init_LIST(int high, int low, int weight);//初始化链表信息
TREE tree_node_init(int high, int low, int weight);//初始化哈夫曼树各个结点
TREE build_tree(LIST head);//建立哈夫曼树
void print_huffman_pre(TREE Tree, int flag);//前序输出,flag 控制是否打印信息
void update_tree(TREE Tree);//更新树的信息，也即更新编码信息
void save_file(TREE *a, int right, TREE Tree);//保存文件
void to_free_tree(TREE Tree);//释放树
void to_free_list(LIST head);//释放链表
void coding();//编码
void decoding();//译码
TREE queue[1000];//树的队列
int queue_index = 0;
int sum_bit_coding = 0;//编码bit
int sum_bit_decoding = 0;//解码bit
char file_in[100] = "D:\\file.txt";//路径
char file_out[100];
void init_main();
void update_file_out(char file_in[])//
{
    int i;
    for(i = strlen(file_in) - 1; i >= 0; i--)//先规范化输入的路径
        if(file_in[i] == '\\')
            break;
    int j;
    for(j = 0; j <= i; j++)
        file_out[j] = file_in[j];//找出输出的父目录
}
int main()
{
    init_main();
    return 0;
}
void init_main()
{
    LIST head = init_LIST(0, 0, 0);
    FILE *P;
    while(1)
    {
        printf("为了结果的准确性，请输入GBK格式的txt文件\n");
        printf("请输入需要执行的文件及路径\n例如\nD:\\file.txt\n");
        gets(file_in);//输入文件名
        P = fopen(file_in,"r");
        if (P==NULL)
        {
            printf("文件打开失败\n请重新输入\n");
            continue;
        }
        else
        {
            printf("打开成功\n");
            break;
        }
    }
    update_file_out(file_in);//找出文件路径
    int ch;
    while((ch = fgetc(P)) != EOF)//一个一个字符的读取
    {
        LIST tmp = init_LIST(ch, -1, 1);//高位为字符，低位，权值
        if(ch > 128)//如果大于128，是中文读取低位
            tmp->low = fgetc(P);//读取低位
        insert(head, find_and_insert(head, tmp));//插入节点
    }
    //output(head);
    TREE Tree = build_tree(head);
    coding(Tree);//建树
    //print_huffman_pre(Tree->NEXT, 0);//输出树的信息
    update_tree(Tree);//开始编码，并保存到coding里面
    queue_index = 0;
    print_huffman_pre(Tree->NEXT, 0);//更新树的队列，并设置为不可视化
    decoding();
    fclose(P);
    while(1)
    {
        int flag;
        printf("请选择操作命令-> \n \t1：统计信息；\n\t2：词频统计；\n\t3：编码详情；\n\t4：文件输出信息；\n\t5：return\n");
        scanf("%d", &flag);
        switch (flag)
        {
        case 1:
            printf("-----------------------------------\n");
            printf("文件已经保存，共写入%d比特\n", sum_bit_coding);
            printf("从文件读取%d比特\n", 8 * (Tree->high + Tree->low * 2));
            printf("压缩率是%.3f\n",1.0 * sum_bit_coding / 8 / (Tree->high + Tree->low * 2));
            printf("共写入解码文件%d比特\n", sum_bit_decoding);
            printf("-----------------------------------\n");
            break;
        case 2:
            output(head);
            break;
        case 3:
            print_huffman_pre(Tree->NEXT, 1);
            break;
        case 4:
        {
            //路径coding
            char coding_file_name[100];
            strcpy(coding_file_name, file_out);
            strcat(coding_file_name, "coding.txt");
            //路径encoding
            char encoding_file_name[100];
            strcpy(encoding_file_name, file_out);
            strcat(encoding_file_name, "encoding.txt");
            printf("输入文件的路径为%s\n", file_in);
            printf("加密文件的路径为%s\n", coding_file_name);
            printf("解码文件的路径为%s\n", encoding_file_name);
            break;
        }
        case 5:
            return;
        }
    }

    to_free_tree(Tree);
    to_free_list(head);
}
void decoding()
{
    sum_bit_decoding = 0;
    //路径coding
    char coding_file_name[100];
    strcpy(coding_file_name, file_out);
    strcat(coding_file_name, "coding.txt");
    //路径encoding
    char encoding_file_name[100];
    strcpy(encoding_file_name, file_out);
    strcat(encoding_file_name, "encoding.txt");

    FILE *in = fopen(coding_file_name, "r");
    FILE *out = fopen(encoding_file_name, "wb");
    int ch;
    int str_index = 0, left;
    char str[100];
    while((ch = fgetc(in)) != EOF)
    {
        str[str_index++] = ch;
        str[str_index] = '\0';
        for(left = 0; left < queue_index; left++)
        {
            if(strcmp(queue[left]->code, str) == 0)
            {
                //统计bits
                if(queue[left]->high > 128) sum_bit_decoding += 16;//统计中文
                else sum_bit_decoding += 8;
                if((char)queue[left]->high == '\n')//对换行符进行特判
                {
                    fprintf(out, "\r\n");
                }
                else
                {
                    fprintf(out, "%c", queue[left]->high);//高位字节
                    if(queue[left]->high > 128) fprintf(out, "%c", queue[left]->low);//输出是中文
                }
                str_index = 0;
                break;
            }
        }
    }
    fclose(in);
    fclose(out);
}
void to_free_list(LIST head)
{
    LIST P = head;
    while(head->NEXT)
    {
        P = head->NEXT;
        head->NEXT = head->NEXT->NEXT;
        free(P);
    }
    free(head);
}
void to_free_tree(TREE Tree)
{
    if(!Tree) return;
    to_free_tree(Tree->Left);
    to_free_tree(Tree->Right);
    free(Tree);
}
void save_file(TREE *a, int right, TREE Tree)//解码
{
    int left;
    sum_bit_coding = 0;
    FILE *P = fopen(file_in,"r");
    //路径
    char coding_file_name[100];
    strcpy(coding_file_name, file_out);
    strcat(coding_file_name, "coding.txt");

    FILE *out = fopen(coding_file_name, "wb");
    if (P==NULL)
        printf("文件打开失败\n");
    int ch;
    while((ch = fgetc(P)) != EOF)
    {
        LIST tmp = init_LIST(ch, -1, 1);
        if(ch > 128)//中文
            tmp->low = fgetc(P);
        // 查找
        for(left = 0; left < right; left++)
        {
            if(a[left]->high == tmp->high)
            {
                if(tmp->high > 128 && tmp->low == a[left]->low)//中文
                {
                    fprintf(out, "%s", a[left]->code);//写入编码
                    sum_bit_coding += strlen(a[left]->code);//编码bits
                }
                if(tmp->high <= 128)
                {
                    fprintf(out, "%s", a[left]->code);
                    sum_bit_coding += strlen(a[left]->code);
                }
            }
        }
        free(tmp);
    }
    fclose(P);
    fclose(out);
}
void update_tree(TREE Tree)//开始编码
{
    TREE a[1000];
    int left = 0, right = 0;
    if(!Tree) return;
    a[right++] = Tree->NEXT;//a[0] = head;
    while(left < right)//层次遍历
    {
        //左
        if(a[left]->Left)
        {
            a[right++] = a[left]->Left;//把左节点编号，right记录节点的个数
            strcpy(a[left]->Left->code, a[left]->code);//将此节点的编号复制给下一个左节点
            a[left]->Left->code_index = strlen(a[left]->code);//找出长度，也就是找出编码指针
            a[left]->Left->code[a[left]->Left->code_index++] = '0';
            //a[left]->Left->code[a[left]->Left->code_index] = '\0';
        }
        //右
        if(a[left]->Right)
        {
            a[right++] = a[left]->Right;
            strcpy(a[left]->Right->code, a[left]->code);
            a[left]->Right->code_index = strlen(a[left]->code);
            a[left]->Right->code[a[left]->Right->code_index++] = '1';
            //a[left]->Right->code[a[left]->Right->code_index] = '\0';
        }
        left++;
    }
    save_file(a, right, Tree);
}
TREE tree_node_init(int high, int low, int weight)
{
    TREE tmp = (TREE)malloc(sizeof(struct node));
    tmp->high = high;
    tmp->low = low;
    tmp->weight = weight;
    strcpy(tmp->code, "\0");
    tmp->code_index = 0;
    tmp->Right = NULL;
    tmp->Left = NULL;
    tmp->NEXT = NULL;
    return tmp;
}
TREE build_tree(LIST head)
{
    //copy head
    //Tree->weight用来记录不重复的字符个数；即链表的总长度（头结点不算）；
    //又即记录当前构造哈夫曼树的时候的队列剩余的结点个数。
    TREE Tree = tree_node_init(head->high, head->low, 0);
    TREE T = Tree;
    LIST P = head->NEXT;
    while(P)
    {
        T->NEXT = tree_node_init(P->high, P->low, P->weight);
        T = T->NEXT;
        //结点数
        Tree->weight++;
        P = P->NEXT;
    }
    //output(Tree);
    return Tree;
}
void coding(TREE Tree)
{
    while(Tree->weight > 1)
    {
        TREE t1 = Tree->NEXT;
        Tree->NEXT = Tree->NEXT->NEXT;
        TREE t2 = Tree->NEXT;
        Tree->NEXT = Tree->NEXT->NEXT;
        //add t1 and t2 to t; 合并树
        TREE t = tree_node_init(-1, -1, t1->weight + t2->weight);
        //左树
        t->Left = t1;
        //右树
        t->Right = t2;
        insert(Tree, t);
        Tree->weight--;
    }
}
void print_huffman_pre(TREE Tree, int flag)
{
    //遍历
    if(!Tree) return;
    if((char)Tree->high == '\n')//对换行进行特判
    {
        queue[queue_index++] = Tree;
        if(flag)
            printf("\\n   weight == %d coding = %s\n", Tree->weight, Tree->code);
    }
    else if(Tree->high > 128)//中文
    {
        queue[queue_index++] = Tree;
        if(flag)
        {
            putchar(Tree->high);
            putchar(Tree->low);
            printf("   weight == %d coding = %s\n", Tree->weight, Tree->code);
        }
    }
    else if(Tree->high != -1)
    {
        queue[queue_index++] = Tree;
        if(flag)
        {
            putchar(Tree->high);
            printf("   weight == %d coding = %s\n", Tree->weight, Tree->code);
        }
    }
    print_huffman_pre(Tree->Left, flag);
    print_huffman_pre(Tree->Right, flag);
}
LIST find_and_insert(LIST head, LIST tmp)
{
    //统计汉字和其它字符的不重复个数
    if(tmp->low != -1) head->low++;//中文字符个数，计算输入bits用
    else head->high++;//ascall字符个数，计算ascall，输入bits用
    LIST P = head;
    while(P->NEXT)
    {
        //这个字符相同
        if(P->NEXT->high == tmp->high)
        {
            //汉字相同情况
            if(P->NEXT->low != -1 && tmp->low != -1 && P->NEXT->low == tmp->low)
            {
                //取出当前结点
                LIST found = init_LIST(P->NEXT->high, P->NEXT->low, P->NEXT->weight + 1);//加权值
                //删除
                LIST del = P->NEXT;
                P->NEXT = P->NEXT->NEXT;
                del->NEXT = NULL;
                free(del);
                return found;
            }
            //非汉字统计情况
            if(P->NEXT->low == -1 && tmp->low == -1)
            {
                //取出当前结点
                LIST found = init_LIST(P->NEXT->high, P->NEXT->low, P->NEXT->weight + 1);
                //删除
                LIST del = P->NEXT;
                P->NEXT = P->NEXT->NEXT;
                del->NEXT = NULL;
                free(del);
                return found;
            }
        }
        P = P->NEXT;
    }
    return tmp;
}

void insert(LIST head, LIST tmp)
{
    LIST P = head;
    while(P->NEXT)//权值由高到低排列
    {
        if(tmp->weight < P->NEXT->weight)
            break;
        P = P->NEXT;
    }
    //找不到位置，直接插在后面
    if(!P->NEXT)
    {
        P->NEXT = tmp;
        return;
    }
    //insert插入，此刻的位置
    tmp->NEXT = P->NEXT;
    P->NEXT = tmp;
}

void output(LIST head)
{
    LIST P = head->NEXT;
    while(P)
    {
        if((char)P->high == '\n')
        {
            printf("字符 \\n 个数是%d\t占用字节为%d\t总字节为%d\n", P->weight, P->low == -1 ? 1 : 2, P->weight * (P->low == -1 ? 1 : 2));
            P = P->NEXT;
            continue;
        }
        printf("字符 ");
        putchar(P->high);
        if(P->high > 128)
            putchar(P->low);
        printf(" 个数是%d\t占用字节为%d\t总字节为%d\n", P->weight, P->low == -1 ? 1 : 2, P->weight * (P->low == -1 ? 1 : 2));
        P = P->NEXT;
    }
    printf("总字节数为%d\n", head->high + head->low * 2);
}
LIST init_LIST(int high, int low, int weight)//定义高位，低位，还有权值
{
    LIST tmp = (LIST)malloc(sizeof(struct node));
    tmp->high = high;
    tmp->low = low;
    tmp->weight = weight;
    tmp->NEXT = NULL;
    return tmp;
}
