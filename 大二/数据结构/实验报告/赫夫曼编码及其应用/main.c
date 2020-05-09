#include <stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct node *LIST;
typedef struct node *TREE;
struct node
{
    int high;//��λascll
    int low;//�ж��Ƿ������ģ���λ
    int weight;//Ȩֵ
    int code_index;//����ָ��
    char code[100];//��������
    TREE Left;//����
    TREE Right;//�Һ���
    LIST NEXT;//��һ��
};
void insert(LIST head, LIST tmp);//���������
LIST find_and_insert(LIST head, LIST tmp);//�����ڲ��Ľ�㣬Ȼ�����insert����
void output(LIST head);//��������ȡ�ļ��������ַ�ͳ�����
LIST init_LIST(int high, int low, int weight);//��ʼ��������Ϣ
TREE tree_node_init(int high, int low, int weight);//��ʼ�����������������
TREE build_tree(LIST head);//������������
void print_huffman_pre(TREE Tree, int flag);//ǰ�����,flag �����Ƿ��ӡ��Ϣ
void update_tree(TREE Tree);//����������Ϣ��Ҳ�����±�����Ϣ
void save_file(TREE *a, int right, TREE Tree);//�����ļ�
void to_free_tree(TREE Tree);//�ͷ���
void to_free_list(LIST head);//�ͷ�����
void coding();//����
void decoding();//����
TREE queue[1000];//���Ķ���
int queue_index = 0;
int sum_bit_coding = 0;//����bit
int sum_bit_decoding = 0;//����bit
char file_in[100] = "D:\\file.txt";//·��
char file_out[100];
void init_main();
void update_file_out(char file_in[])//
{
    int i;
    for(i = strlen(file_in) - 1; i >= 0; i--)//�ȹ淶�������·��
        if(file_in[i] == '\\')
            break;
    int j;
    for(j = 0; j <= i; j++)
        file_out[j] = file_in[j];//�ҳ�����ĸ�Ŀ¼
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
        printf("Ϊ�˽����׼ȷ�ԣ�������GBK��ʽ��txt�ļ�\n");
        printf("��������Ҫִ�е��ļ���·��\n����\nD:\\file.txt\n");
        gets(file_in);//�����ļ���
        P = fopen(file_in,"r");
        if (P==NULL)
        {
            printf("�ļ���ʧ��\n����������\n");
            continue;
        }
        else
        {
            printf("�򿪳ɹ�\n");
            break;
        }
    }
    update_file_out(file_in);//�ҳ��ļ�·��
    int ch;
    while((ch = fgetc(P)) != EOF)//һ��һ���ַ��Ķ�ȡ
    {
        LIST tmp = init_LIST(ch, -1, 1);//��λΪ�ַ�����λ��Ȩֵ
        if(ch > 128)//�������128�������Ķ�ȡ��λ
            tmp->low = fgetc(P);//��ȡ��λ
        insert(head, find_and_insert(head, tmp));//����ڵ�
    }
    //output(head);
    TREE Tree = build_tree(head);
    coding(Tree);//����
    //print_huffman_pre(Tree->NEXT, 0);//���������Ϣ
    update_tree(Tree);//��ʼ���룬�����浽coding����
    queue_index = 0;
    print_huffman_pre(Tree->NEXT, 0);//�������Ķ��У�������Ϊ�����ӻ�
    decoding();
    fclose(P);
    while(1)
    {
        int flag;
        printf("��ѡ���������-> \n \t1��ͳ����Ϣ��\n\t2����Ƶͳ�ƣ�\n\t3���������飻\n\t4���ļ������Ϣ��\n\t5��return\n");
        scanf("%d", &flag);
        switch (flag)
        {
        case 1:
            printf("-----------------------------------\n");
            printf("�ļ��Ѿ����棬��д��%d����\n", sum_bit_coding);
            printf("���ļ���ȡ%d����\n", 8 * (Tree->high + Tree->low * 2));
            printf("ѹ������%.3f\n",1.0 * sum_bit_coding / 8 / (Tree->high + Tree->low * 2));
            printf("��д������ļ�%d����\n", sum_bit_decoding);
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
            //·��coding
            char coding_file_name[100];
            strcpy(coding_file_name, file_out);
            strcat(coding_file_name, "coding.txt");
            //·��encoding
            char encoding_file_name[100];
            strcpy(encoding_file_name, file_out);
            strcat(encoding_file_name, "encoding.txt");
            printf("�����ļ���·��Ϊ%s\n", file_in);
            printf("�����ļ���·��Ϊ%s\n", coding_file_name);
            printf("�����ļ���·��Ϊ%s\n", encoding_file_name);
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
    //·��coding
    char coding_file_name[100];
    strcpy(coding_file_name, file_out);
    strcat(coding_file_name, "coding.txt");
    //·��encoding
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
                //ͳ��bits
                if(queue[left]->high > 128) sum_bit_decoding += 16;//ͳ������
                else sum_bit_decoding += 8;
                if((char)queue[left]->high == '\n')//�Ի��з���������
                {
                    fprintf(out, "\r\n");
                }
                else
                {
                    fprintf(out, "%c", queue[left]->high);//��λ�ֽ�
                    if(queue[left]->high > 128) fprintf(out, "%c", queue[left]->low);//���������
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
void save_file(TREE *a, int right, TREE Tree)//����
{
    int left;
    sum_bit_coding = 0;
    FILE *P = fopen(file_in,"r");
    //·��
    char coding_file_name[100];
    strcpy(coding_file_name, file_out);
    strcat(coding_file_name, "coding.txt");

    FILE *out = fopen(coding_file_name, "wb");
    if (P==NULL)
        printf("�ļ���ʧ��\n");
    int ch;
    while((ch = fgetc(P)) != EOF)
    {
        LIST tmp = init_LIST(ch, -1, 1);
        if(ch > 128)//����
            tmp->low = fgetc(P);
        // ����
        for(left = 0; left < right; left++)
        {
            if(a[left]->high == tmp->high)
            {
                if(tmp->high > 128 && tmp->low == a[left]->low)//����
                {
                    fprintf(out, "%s", a[left]->code);//д�����
                    sum_bit_coding += strlen(a[left]->code);//����bits
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
void update_tree(TREE Tree)//��ʼ����
{
    TREE a[1000];
    int left = 0, right = 0;
    if(!Tree) return;
    a[right++] = Tree->NEXT;//a[0] = head;
    while(left < right)//��α���
    {
        //��
        if(a[left]->Left)
        {
            a[right++] = a[left]->Left;//����ڵ��ţ�right��¼�ڵ�ĸ���
            strcpy(a[left]->Left->code, a[left]->code);//���˽ڵ�ı�Ÿ��Ƹ���һ����ڵ�
            a[left]->Left->code_index = strlen(a[left]->code);//�ҳ����ȣ�Ҳ�����ҳ�����ָ��
            a[left]->Left->code[a[left]->Left->code_index++] = '0';
            //a[left]->Left->code[a[left]->Left->code_index] = '\0';
        }
        //��
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
    //Tree->weight������¼���ظ����ַ���������������ܳ��ȣ�ͷ��㲻�㣩��
    //�ּ���¼��ǰ�������������ʱ��Ķ���ʣ��Ľ�������
    TREE Tree = tree_node_init(head->high, head->low, 0);
    TREE T = Tree;
    LIST P = head->NEXT;
    while(P)
    {
        T->NEXT = tree_node_init(P->high, P->low, P->weight);
        T = T->NEXT;
        //�����
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
        //add t1 and t2 to t; �ϲ���
        TREE t = tree_node_init(-1, -1, t1->weight + t2->weight);
        //����
        t->Left = t1;
        //����
        t->Right = t2;
        insert(Tree, t);
        Tree->weight--;
    }
}
void print_huffman_pre(TREE Tree, int flag)
{
    //����
    if(!Tree) return;
    if((char)Tree->high == '\n')//�Ի��н�������
    {
        queue[queue_index++] = Tree;
        if(flag)
            printf("\\n   weight == %d coding = %s\n", Tree->weight, Tree->code);
    }
    else if(Tree->high > 128)//����
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
    //ͳ�ƺ��ֺ������ַ��Ĳ��ظ�����
    if(tmp->low != -1) head->low++;//�����ַ���������������bits��
    else head->high++;//ascall�ַ�����������ascall������bits��
    LIST P = head;
    while(P->NEXT)
    {
        //����ַ���ͬ
        if(P->NEXT->high == tmp->high)
        {
            //������ͬ���
            if(P->NEXT->low != -1 && tmp->low != -1 && P->NEXT->low == tmp->low)
            {
                //ȡ����ǰ���
                LIST found = init_LIST(P->NEXT->high, P->NEXT->low, P->NEXT->weight + 1);//��Ȩֵ
                //ɾ��
                LIST del = P->NEXT;
                P->NEXT = P->NEXT->NEXT;
                del->NEXT = NULL;
                free(del);
                return found;
            }
            //�Ǻ���ͳ�����
            if(P->NEXT->low == -1 && tmp->low == -1)
            {
                //ȡ����ǰ���
                LIST found = init_LIST(P->NEXT->high, P->NEXT->low, P->NEXT->weight + 1);
                //ɾ��
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
    while(P->NEXT)//Ȩֵ�ɸߵ�������
    {
        if(tmp->weight < P->NEXT->weight)
            break;
        P = P->NEXT;
    }
    //�Ҳ���λ�ã�ֱ�Ӳ��ں���
    if(!P->NEXT)
    {
        P->NEXT = tmp;
        return;
    }
    //insert���룬�˿̵�λ��
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
            printf("�ַ� \\n ������%d\tռ���ֽ�Ϊ%d\t���ֽ�Ϊ%d\n", P->weight, P->low == -1 ? 1 : 2, P->weight * (P->low == -1 ? 1 : 2));
            P = P->NEXT;
            continue;
        }
        printf("�ַ� ");
        putchar(P->high);
        if(P->high > 128)
            putchar(P->low);
        printf(" ������%d\tռ���ֽ�Ϊ%d\t���ֽ�Ϊ%d\n", P->weight, P->low == -1 ? 1 : 2, P->weight * (P->low == -1 ? 1 : 2));
        P = P->NEXT;
    }
    printf("���ֽ���Ϊ%d\n", head->high + head->low * 2);
}
LIST init_LIST(int high, int low, int weight)//�����λ����λ������Ȩֵ
{
    LIST tmp = (LIST)malloc(sizeof(struct node));
    tmp->high = high;
    tmp->low = low;
    tmp->weight = weight;
    tmp->NEXT = NULL;
    return tmp;
}
