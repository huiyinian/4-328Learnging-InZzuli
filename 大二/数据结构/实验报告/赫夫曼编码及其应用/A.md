> 作者：zzulihyn
>
> 代码文件：该目录下的A.cpp
>
> 时间：2020年5月



# 实验二：哈夫曼编码

## 输入和输出

1. 输入：硬盘上原始文件的文件路径
2. 输出：
   - 生成一个加密文件，并在控制台输出其路径。
   - 生成一个解码文件，并在控制台输出其路径。
   - 在控制台输出原始文件与解码文件的对比结果。

## 编写环境

C/C++

## 实验分析

分析个锤子，就两部分。会就是会，不会就百度。

1. 文件操作
2. 哈夫曼编码

**文件操作咱们就用C语言吧，起码还会一点点，C++就要从零开始了**

### 文件操作

编写两个函数，一个用来读取文本内容，一个写入文本内容。

1. 文本路径Path，及字符串。typedef char* Path。
2. 读取文本内容，char* readAllTxt(Path txtPath)
3. 写入文本内容，void writeTxt(char* str, Path txtPath)。

**code**

```cpp
char* readAllTxt(Path txtPath) {
    FILE* fp = fopen(txtPath, "rb"); // rb打开用于读取的二进制文件，不要写r。不然会错
    fseek(fp, 0, SEEK_END); // 将fp的输入流移动到文本末端
    int flen = ftell(fp); // 计算文本长度
    char* str = (char*)malloc(sizeof(char) * (flen + 1)); // 创建一个长度为flen+1的字符数组。结束符'\0'要占一位。所以最小的长度就是flen+1
    fseek(fp, 0, SEEK_SET); // 再讲fp的输入流移动到文本起始处
    fread(str, flen, 1, fp); // 从fp的输入流位置开始，读取1个长度为flen的字符串给str
    str[flen] = '\0'; // 最后一位结束符
    fclose(fp); // 关闭输入流
    return str; 
}
void writeTxt(char* str, Path txtPath) {
    FILE* fp = fopen(txtPath, "wb"); //wb打开用于写入的二进制文件
    rewind(fp); // 把文件指针移到由stream(流)指定的开始处, 同时清除和流相关的错误和EOF标记.
    fwrite(str, strlen(str), 1, fp); // 往fp中写入一个字符串。从str中读取，长度为strlen(str)
    fclose(fp);
}
```

### 哈夫曼树编码

原理啥的这里就不细写了，网上有很多博客。这里就简单说下这是个啥玩意。

有个文本**abbbcccddd**。它比较害羞，所以它要咱们帮它加个密，一个字符对应一个字符串。除此之外，它还喜欢二叉树。非得要咱们用**二叉树**帮它加密。孩子们扮演0和1，根节点不喜欢和它们一起玩，就出来露个脸。只有在**所需编码的字符个数**为**1**时，根节点才。。。一个人你玩个毛线。

游戏也很简单。所需编码字符**{a, b, c, d}**。每个都为一个叶子节点，作为一个节点集。在文本中出现的次数为权值。每次操作便从节点集中挑选出**两个权值最小**的节点（**不用再放回去**），组成一个新的节点，放进节点集。直到只剩下一个节点——根节点。

获取编码字符**{a, b, c, d}**所对应的编码str，除根节点外，所有节点都标上**0、1，0左、1右**。然后从所对应的叶子节点开始往上走，走到根节点。其路程所对应的字符串的**逆序**就是该字符所对应的编码。

还原哈夫曼编码，从根节点开始，**0走左、1走右**，不能走了便得到所对应的字符，然后再从根节点开始走。

### 补充

实验要求中，还需要对中文进行编码。中文字符的所占字节一般情况下都会超过2个字节。C/C++环境下的char所占字节为1。故我们无法对字符进行编码，我们对字节编码。一个char代表一个字节。

然后再说一下，记事本的编码与控制台的编码。

**直接新建记事本用的应该是utf8的编码，然而咱们的控制台大部分都是gbk编码的。**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200508114654613.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0Mzc5NDU4,size_16,color_FFFFFF,t_70)

所以如果你的控制台上出现乱码。像这样。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200508123613103.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0Mzc5NDU4,size_16,color_FFFFFF,t_70)

**不要慌。记事本里的还是完好的。**</br>
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200508115918425.png)

若是想消除控制台上的乱码，可以用记事本新建，保存的时候将编码保存成ANSI。
ANSI编码会根据电脑系统的不同为你选择合适的编码。简体中文环境下就是gbk。详见[百度百科](https://baike.baidu.com/item/ANSI/10401940?fr=aladdin)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200508120114976.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0Mzc5NDU4,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200508123531641.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQ0Mzc5NDU4,size_16,color_FFFFFF,t_70)

**code**

```cpp
typedef char* Path;
const int MAX_SIZE = 1010;
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
//获取叶子节点集
void getLeafsArray(List &leafs, map<char, int> &mp, char* s) {
    map<char, int> nums; // 存储每个字节出现的次数
    for (int i = 0; i < strlen(s); ++ i)
        ++ nums[s[i]];
    leafs.len = nums.size() + 1;
    leafs.codes = (char*) malloc(sizeof(char) * leafs.len);
    leafs.weights = (int*) malloc(sizeof(int) * leafs.len);
    int len = 0;
    // 获取叶子节点集
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
    node1 = node2 = w1 = w2 = 0; // 初值为0
    /**
先更新w1，后更新w2。另w1始终<=w2。故每次更新w1的时候，就将原本的{w1,node1}给{w2,node2}
    **/
    for (int i = 1; i <= len; ++ i) {
        if(HT[i].parent != 0) continue; // 有父节点，一边玩去
		// 更新w1，node1
        if(w1 == 0 || w1 > HT[i].weight) {
            w2 = w1;
            node2 = node1;
            w1 = HT[i].weight;
            node1 = i;
            continue;
        }
        // 更新w2，node2
        if(w2 == 0 || w2 > HT[i].weight) {
            w2 = HT[i].weight;
            node2 = i;
            continue;
        }
    }
}
// 哈夫曼编码
void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, List leafs, int &root) {
    int m = leafs.len - 1; // 叶子节点个数,0没用到，故-1
    int n = 2 * m - 1; // 二叉树节点个数，用到了二叉树的性质，自行百度

    // 存储二叉树，共n个节点，0不用，开辟n+1个
    HT = (HuffmanTree)malloc(sizeof(HTNode) * (n + 1));
    for (int i = 1; i <= m; ++ i) HT[i] = HTNode(leafs.weights[i], 0, 0, 0); // m个叶子节点
    // 建树
    for (int i = m + 1; i <= n; ++ i) {
        int node1, node2;
        select(HT, i - 1, node1, node2); // 从节点集中找到两个权重最小的节点
        HT[i] = HTNode(HT[node1].weight + HT[node2].weight, 0, node1, node2); // 合成新节点
        HT[node1].parent = i; HT[node2].parent = i; // 更新子节点的父节点
    }
    // 找到跟节点root，根节点最大，没父节点的就是跟节点。
    for (int i = 1; i <= n; ++ i) {
        if(HT[i].parent == 0) {root = i; break; }
    }
	// 0不用，故开辟m + 1个长度未知的字符串
    HC = (char**) malloc(sizeof(char*) * (m + 1));
	// 开辟一个长度为m的字符串，编码长度肯定会<m,m是叶子节点数，编码长度最大就是高度，m绝对够用
    char* str = (char*) malloc(sizeof(char) * m);
    int len = 0;
    int parent;
    for (int i = 1; i <= m; ++ i) {
        len = 0;
        // 父节点为0，代表该节点为root，跳出循环
        for (int j = i; HT[j].parent != 0; j = HT[j].parent ) {
            parent = HT[j].parent;
            if(HT[parent].lson == j) str[len ++] = '0'; // 是父节点的左孩子 0
            else str[len ++] = '1'; // 右孩子 1
        }
        HC[i] = (char*) malloc(sizeof(char) * len); // 存储二叉树，共n个节点，0不用，开辟n+1个
        for (int j = 0; j < len; ++ j) HC[i][j] = str[len - j - 1]; // 逆序
        HC[i][len] = '\0'; // 结束符
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
    map<char, int> mp; // 字节映射，方便后续操作
    List leafs; // 叶子节点集
    getLeafsArray(leafs, mp, oriMessage);
    HuffmanTree HT; // 哈夫曼树
    HuffmanCode HC; // 哈夫曼码
    int root; // 根节点
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
```

现在，就只剩下输入了。

**code**

```cpp
int main() {
    /**注意：输入路径时，里面的\要最好用\\来代替**/
    char parentPath[MAX_SIZE];
    char txtName[MAX_SIZE];
    printf("输入文件的父路径："); scanf("%s", parentPath);
    printf("输入文件名："); scanf("%s", txtName);
    printf("working\n\n"); work(parentPath, txtName);
    return 0;
}
```

