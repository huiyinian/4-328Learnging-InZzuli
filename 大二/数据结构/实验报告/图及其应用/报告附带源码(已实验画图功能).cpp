
#include <stdio.h>
#include <stdlib.h>
#define inf 99999999
#define max_element 50
int e[max_element][max_element];  //保存地图的数组
int path_e[max_element][max_element], path_t[max_element][max_element];
double t[max_element][max_element];  //保存地图的速度

void Menu();
void Old_Map();
void New_Map();
void Floyd(int (*e)[max_element],double (*t)[max_element], int n);       //计算最短路径
void Floyd_dist(int (*e)[max_element], int n, int start, int end);
void Floyd_time(double (*e)[max_element], int n, int start, int end);
int main()
{
    int Mu=5;
    Menu();
    while(scanf("%d", &Mu), Mu!=0)
    {
        switch(Mu)
        {
            case 1: Old_Map();break;
            case 2: New_Map();break;
            case 3: system("cls");break;
            default:printf("\n请输入正确指令！！！");break;
        }

        Menu();
    }
    printf("\n成功退出！");
    return 0;
}

void Menu()
{
    printf("\n    ---选择使用已保存的地图：1---");
    printf("\n\n    ---选择重新录入地图信息：2---\n");
    printf("\n    -----------清屏：3-----------\n");
    printf("\n  -------------退出：0-------------\n");
}
//使用原有地图
void Old_Map()
{
    int i, j;
    FILE *fp;
    int count = 0;
    //读入文档count.txt
    if((fp=fopen("count.txt","r")) == NULL)
    {
        printf("File open failed!\n");
        exit(0);
    }
    fscanf(fp,"%d", &count);
    fclose(fp);
    printf("顶点个数 == %d\n", count);
    if(count == 0 )
    {
        printf("\n信息读入错误！！\n错误原因：没有已保存的地图，请选择重新输入地图信息！！\n");
        return ;
    }

    ///读入文档map.txt
    if((fp=fopen("map.txt","r")) == NULL)
    {
        printf("File open failed!\n");
        exit(0);
    }
    for(i = 1; i <= count; i++)
        for(j = 1; j <= count; j++)
        {
            fscanf(fp,"%d", &e[i][j]);
        }
    fclose(fp);
    ///读入文档map_speed.txt
    if((fp=fopen("map_speed.txt","r")) == NULL)
    {
        printf("File open failed!\n");
        exit(0);
    }
    for(i = 1; i <= count; i++)
        for(j = 1; j <= count; j++)
        {
            fscanf(fp,"%lf", &t[i][j]);
        }
    fclose(fp);
    ///将地图信息打印*************能否采用图形界面将位置信息打印?
    system("mspaint map.png");
    Floyd(e,t, count);
}
//新录入地图
void New_Map()
{
    //map数组初始化
    int i, j;
    for(i = 0; i < max_element; i++ )
        for(j = 0; j < max_element; j++)
            if(i == j)
            {
                e[i][j] = 0;
                t[i][j] = 0;
            }

            else {
                    e[i][j] = inf;
                    t[i][j] = inf;
            }

    printf("\n请输入每条路的起点、终点、路的长度、速度\n（中间以空格隔开，按下Ctrl+Z结束输入）：\n");
    int s, ee, l, speed;
    int count = 0;
    //地图写入
    while(scanf("%d %d %d %d", &s, &ee, &l,&speed) != EOF)
    {
        e[s][ee] = l;
        e[ee][s] = l;
        t[s][ee] = (l*1.0)/speed;
        t[ee][s] = (l*1.0)/speed;
        printf("%.2f ", t[s][ee]);
        count++;
    }
    ///将地图存入文件map.txt
    FILE *fp;
    if((fp=fopen("map.txt","w")) == NULL)
    {
        printf("the file can not open...");
        exit(0);
    }
    for(i = 1; i <= count; i++)
        for(j = 1; j <= count; j++)
        {
            fprintf(fp,"%d", e[i][j]);
            fprintf(fp,"\n");
        }
    fclose(fp);

    ///将速度-时间存入文件中"map_speed.txt
    if((fp=fopen("map_speed.txt","w")) == NULL)
    {
        printf("the file can not open...");
        exit(0);
    }
    for(i = 1; i <= count; i++)
        for(j = 1; j <= count; j++)
        {
            fprintf(fp,"%lf", t[i][j]);
            fprintf(fp,"\n");
        }
    fclose(fp);
    ///将顶点的个数存入文件
    if((fp=fopen("count.txt","w")) == NULL)
    {
        printf("the file can not open...");
        exit(0);
    }
    fprintf(fp,"%d", count);
    fclose(fp);
    Floyd(e,t, count);
}
//佛洛依德算法
void Floyd(int (*e)[max_element],double (*t)[max_element],int n)
{
    int start, end; //起始位置，终点
    //初始化记录路径详细信息数组path
    int i, j;
   //初始化终点***
    for( i = 0; i <= n; i++)
        for(j = 0; j <= n; j++)
        {
            path_e[i][j] = j;
            path_t[i][j] = j;
        }

    while(1)
    {
        printf("\n请输入要查询路径起点、终点位置：");
        scanf("%d %d", &start, &end);
        if(start > n || end > n)
        {
            printf("\n出现错误！！！\n错误原因：输入了不存在的顶点！！\n请重新输入！！\n");
            printf("\n顶点个数为：%d", n);
            continue;
        }
        int Floyd_xuanze = 0;
        printf("\n请输入查询方式：\n");
        printf("1---最短路径\n");
        printf("2---最短时间\n");
        scanf("%d", &Floyd_xuanze);

        /**
         *异常处理
         */

        switch(Floyd_xuanze)
        {
            case 1: Floyd_dist(e, n, start, end);break;
            case 2: Floyd_time(t,n, start, end);break;
            default : printf("请输入正确指令！！！\n");break;
        }
        int temp = 0;
        printf("是否继续查询：yes：1 / no：0\n");
        scanf("%d", &temp);
        if(!temp)
            break;
    }

}
void Floyd_dist(int (*e)[max_element], int n, int start, int end)
{
    int k, i, j;
     ///佛洛依德算法---距离
    for(k = 1; k<=n; k++)
        for(i = 1; i <= n; i++)
            for(j = 1; j <= n; j++)
                if(e[i][j]>e[i][k]+e[k][j])
                {
                    e[i][j]=e[i][k]+e[k][j];
                    path_e[i][j] = path_e[i][k];
                }

    ///打印出最短路径及相应路径信息--距离
    printf("\n查询成功！！信息如下：\n\n");
    printf("%d=>%d, length:%d, ",start, end, e[start][end]);
    int v = path_e[start][end];
    printf("path:%d", start);
    while(v!=end)
    {
        printf("->%d",v);
        v = path_e[v][end];
    }
    printf("->%d", end);
    printf("\n___________________________________________\n");
}
void Floyd_time(double (*t)[max_element], int n, int start, int end)
{
    int k, i, j;

    ///佛洛依德算法---时间
    for(k = 1; k<=n; k++)
        for(i = 1; i <= n; i++)
            for(j = 1; j <= n; j++)
                if(t[i][j]>t[i][k]+t[k][j])
                {
                    t[i][j]=t[i][k]+t[k][j];
                    path_t[i][j] = path_t[i][k];
                }
    ///打印出最短路径及相应路径信息--距离
    printf("\n查询成功！！信息如下：\n\n");
    printf("%d=>%d, time:%.2fh, ",start, end, t[start][end]);
    int v = path_t[start][end];
    printf("path:%d", start);
    while(v!=end)
    {
        printf("->%d",v);
        v = path_t[v][end];
    }
    printf("->%d", end);
    printf("\n___________________________________________\n");
}
