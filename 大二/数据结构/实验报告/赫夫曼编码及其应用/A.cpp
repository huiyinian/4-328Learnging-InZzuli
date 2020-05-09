#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cctype>
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>

using namespace std;
/**
1、读取文件
2、生成加密文件
3、比较两文件大小差别
4、对加密文件进行解密，生成解密文件
5、比较原始文件和解码文件的内容是否一致。
**/
typedef char* Path;
const int MAX_SIZE = 1010;
const int CODE_SIZE = 1010;
typedef struct HTNode{
    int weight;
    int parent;
    int lson;
    int rson;
    // 0表无
    HTNode(int _weight = 0, int _parent = 0, int _lson = 0, int _rson = 0) {
        weight = _weight;
        parent = _parent;
        lson = _lson;
        rson = _rson;
    }
}*HuffmanTree;
typedef struct LeafNodeArray{
    int len; // 这个从1开始，0不用
    int* weights;
    char* codes;
}List;
typedef char** HuffmanCode;
char* readAllTxt(Path txtPath) {
    FILE* fp = fopen(txtPath, "rb");
    fseek(fp, 0, SEEK_END);
    int flen = ftell(fp);
    char* str = (char*)malloc(sizeof(char) * (flen + 1));
    fseek(fp, 0, SEEK_SET);
    fread(str, flen, 1, fp);
    str[flen] = '\0';
    fclose(fp);
    return str;
}
void writeTxt(char* str, Path txtPath) {
    FILE* fp = fopen(txtPath, "wb");
    rewind(fp);
    fwrite(str, strlen(str), 1, fp);
    fclose(fp);
}
void getLeafsArray(List &leafs, map<char, int> &mp, char* s) {
    map<char, int> nums;
    for (int i = 0; i < strlen(s); ++ i)
        ++ nums[s[i]];
    leafs.len = nums.size() + 1;
    leafs.codes = (char*) malloc(sizeof(char) * leafs.len);
    leafs.weights = (int*) malloc(sizeof(int) * leafs.len);
    int len = 0;
    for (map<char, int>::iterator it = nums.begin(); it != nums.end(); it ++) {
        leafs.codes[++ len] = it->first;
        leafs.weights[len] = it->second;
        mp[it->first] = len;
    }
}
/**
找到两个权重最小的节点，当然此节点还没有父节点，有说明啥，用过一次了。出局了。
**/
void select(HuffmanTree &HT, int len, int &node1, int &node2) {
    int w1, w2;
    node1 = node2 = w1 = w2 = 0;
    for (int i = 1; i <= len; ++ i) {
        if(HT[i].parent != 0) continue; // 有父节点，一边玩去
        if(w1 == 0 || w1 > HT[i].weight) {
            w2 = w1;
            node2 = node1;
            w1 = HT[i].weight;
            node1 = i;
            continue;
        }
        if(w2 == 0 || w2 > HT[i].weight) {
            w2 = HT[i].weight;
            node2 = i;
            continue;
        }
    }
}
void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, List leafs, int &root) {
    int m = leafs.len - 1;
    int n = 2 * m - 1;
    // 存储二叉树
    HT = (HuffmanTree)malloc(sizeof(HTNode) * (n + 1));
    for (int i = 1; i <= m; ++ i) HT[i] = HTNode(leafs.weights[i], 0, 0, 0);
    // 建树
    for (int i = m + 1; i <= n; ++ i) {
        int node1, node2;
        select(HT, i - 1, node1, node2);
        HT[i] = HTNode(HT[node1].weight + HT[node2].weight, 0, node1, node2);
        HT[node1].parent = i; HT[node2].parent = i;
    }
    for (int i = 1; i <= n; ++ i)
        if(HT[i].parent == 0) {root = i; break; }

    HC = (char**) malloc(sizeof(char*) * (m + 1));
    char* str = (char*) malloc(sizeof(char) * m);
    int len = 0;
    int parent;
    for (int i = 1; i <= m; ++ i) {
        len = 0;
        for (int j = i; HT[j].parent != 0; j = HT[j].parent ) {
            parent = HT[j].parent;
            if(HT[parent].lson == j) str[len ++] = '0';
            else str[len ++] = '1';
        }
        HC[i] = (char*) malloc(sizeof(char) * len);
        for (int j = 0; j < len; ++ j) HC[i][j] = str[len - j - 1];
        HC[i][len] = '\0';
    }
}
void work(Path parentPath, Path txtName) {
    // 确定三个文件的路径
    Path oriTxtPath = (char*) malloc(sizeof(char) * MAX_SIZE);
    Path codeTxtPath = (char*) malloc(sizeof(char) * MAX_SIZE);
    Path decodeTxtPath = (char*) malloc(sizeof(char) * MAX_SIZE);
    Path codeName = "code.txt";
    Path decodeName = "decode.txt";
    strcat(parentPath, "\\\\");
    strcpy(oriTxtPath, parentPath);
    strcat(oriTxtPath, txtName);
    strcpy(codeTxtPath, parentPath);
    strcat(codeTxtPath, codeName);
    strcpy(decodeTxtPath, parentPath);
    strcat(decodeTxtPath, decodeName);
    // 编码
    char* oriMessage = readAllTxt(oriTxtPath);
    map<char, int> mp;
    List leafs;
    getLeafsArray(leafs, mp, oriMessage);
    HuffmanTree HT;
    HuffmanCode HC;
    int root;
    HuffmanCoding(HT, HC, leafs, root);
    // 加密
    char* codeMessage = (char*) malloc(sizeof(char) * (leafs.len * strlen(oriMessage)));
    codeMessage[0] = '\0';
    for (int i = 0; i < strlen(oriMessage); ++ i)
        strcat(codeMessage, HC[mp[oriMessage[i] ] ]);
    printf("加密文件路径：%s\n", codeTxtPath);
    printf("加密后的字符串：\n%s\n", codeMessage);
    puts("-------------------------");
    writeTxt(codeMessage, codeTxtPath);
    // 文件大小，因为C语言中一个char代表一个字节，结束符不写入文件，故文件字节数=strlen(str) - 1
    printf("加密文件字节数：%d\n", strlen(codeMessage) - 1);
    printf("原文件字节数：%d\n", strlen(oriMessage) - 1);
    puts("-------------------------");
    // 解码
    int p = root;
    char* decodeMessage = (char*) malloc(sizeof(char) * strlen(oriMessage));
    int len = 0;
    for (int i = 0; i < strlen(codeMessage); ++ i) {
        if(codeMessage[i] == '0') p = HT[p].lson;
        else p = HT[p].rson;
        if(HT[p].lson == 0 && HT[p].rson == 0) {
            decodeMessage[len ++] = leafs.codes[p];
            p = root;
        }
    }
    decodeMessage[len] = '\0';
    printf("解码文件路径：%s\n", decodeTxtPath);
    printf("解码内容：\n%s\n", decodeMessage);
    writeTxt(decodeMessage, decodeTxtPath);
    puts("-------------------------");
    // 对比
    if(strcmp(decodeMessage, oriMessage) == 0) puts("解码文件内容与原文件内容一致。");
    else puts("！！！解码文件内容与原文件内容不一致。");
}
int main() {

    char parentPath[MAX_SIZE];
    char txtName[MAX_SIZE];
    printf("输入文件的父路径："); scanf("%s", parentPath);
    printf("输入文件名："); scanf("%s", txtName);
    printf("working...\n\n"); work(parentPath, txtName);
    return 0;
}
