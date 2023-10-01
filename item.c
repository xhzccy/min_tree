#include <stdio.h>
#include <stdlib.h>

#define MAX 13
#define MAX_LENGTH 10000
//邻接矩阵存储
typedef struct {
    int adjacent_matrix[MAX+1][MAX+1];//邻接矩阵，下标对应图中序号，0不用
    int edge;//边数
    int point;//点数
}Graph1;

//边表存储
typedef struct GRAPH2{
    int start;//起点
    int end;//终点
    int value;//权值
    struct GRAPH2*next;
}Graph2;

//最小生成树边结构体
typedef struct {
    int start;//起点
    int length;//边
    int end;//边连接的点
}Min_tree;

//邻接链表的打印
void print1(Graph1*graph1){
    printf("\n邻接链表形式输出：\n");
    for(int i=1;i<=graph1->point;i++){
        printf("%2d --> ",i);
        for(int l=1;l<=graph1->point;l++){
            if(graph1->adjacent_matrix[i][l]<MAX_LENGTH&&graph1->adjacent_matrix[i][l]>0){
                printf("%2d|%-3d --> ",l,graph1->adjacent_matrix[i][l]);
            }
        }
        printf("NULL\n");
    }
    printf("\n");
}
//邻接矩阵创建
void init_Graph1(Graph1*graph1){
    for(int i=1;i<MAX+1;i++){
        for(int l=1;l<MAX+1;l++){
            if(i==l) graph1->adjacent_matrix[i][l] = 0;
            else graph1->adjacent_matrix[i][l] = MAX_LENGTH;
        }
    }
    FILE *fp = fopen("./graph.txt","r");
    fscanf(fp,"%d %d",&graph1->point,&graph1->edge);
    for(int i=0;i<graph1->edge;i++){
        int node1,node2,value;
        fscanf(fp,"%d %d %d",&node1,&node2,&value);
        graph1->adjacent_matrix[node1][node2] = value;
        graph1->adjacent_matrix[node2][node1] = value;
    }
    fclose(fp);
    print1(graph1);
}

//邻接矩阵的打印
void print2(Graph2*graph2){
    graph2 = graph2->next;
    int tmp[MAX+1][MAX+1];
    for(int i=1;i<MAX+1;i++){
        for(int l=1;l<MAX+1;l++){
            if(i==l) tmp[i][l] = 0;
            else tmp[i][l] = MAX_LENGTH;
        }
    }
    while(graph2){
        tmp[graph2->start][graph2->end] = graph2->value;
        tmp[graph2->end][graph2->start] = graph2->value;
        graph2 = graph2->next;
    }
    printf("\n邻接矩阵形式输出：\n");
    for(int i=0;i<MAX+1;i++) {
        if(i==0)printf(" @");
        else printf("%6d",i);
    }
    printf("\n");
    for(int i=1;i<MAX+1;i++){
        printf("%2d",i);
        for(int l=1;l<MAX+1;l++){
            if(tmp[i][l]==MAX_LENGTH) printf("%7s","∞") ;
            else printf("%6d",tmp[i][l]);
        }
        printf("\n");
    }
    printf("\n");
}
//边表的创建
void init_Graph2(Graph2*graph2){//有头结点
    FILE *fp = fopen("./graph.txt","r");
    int edge,point;
    Graph2 *tmp = graph2;
    fscanf(fp,"%d %d\n",&edge,&point);
    while(!feof(fp)){
        Graph2 *node = (Graph2*)malloc(sizeof(Graph2));
        fscanf(fp,"%d %d %d",&node->start,&node->end,&node->value);
        tmp->next = node;
        node->next = NULL;
        tmp = tmp->next;
    }
    print2(graph2);
}

void print_min_tree(Min_tree*min_tree,int len){
    printf("\n最小生成树构建顺序：\n");
    for(int i=0;i<len;i++){
        printf("%2d  --  %d\n",min_tree[i].start,min_tree[i].end);
    }
    printf("\n");
}

//prim最小生成树
void prim(Graph1*graph1,Graph2*graph2){
    Min_tree min_tree[graph1->point];//从下标0开始
    int tree_num=0;//创建最小生成树标记以及生成树的个数
    int flag[graph1->point+1];//标记数组的下标对应图中序号
    Graph2 *tmp = graph2->next;//用于后面的最小生成树寻找对应点
    Graph1 *min_tree_print = (Graph1*)malloc(sizeof(Graph1));
    min_tree_print->point = graph1->point;
    for(int i=0;i<=MAX;i++){
        for(int l=0;l<=MAX;l++)
            min_tree_print->adjacent_matrix[i][l] = MAX_LENGTH;
    }
    int money=0;//造价

    for(int i=0;i<=graph1->point;i++)
        flag[i] = MAX_LENGTH;//创建标记数组与置为无穷大
    int num;
    printf("请输入最小生成树起点：\n");
    scanf("%d",&num);
    for(int i=1;i<=graph1->point;i++){//到每个点的最短距离
        if(graph1->adjacent_matrix[num][i]<flag[i])
            flag[i] = graph1->adjacent_matrix[num][i];
    }
    flag[num] = -1;
    for(int i=1;i<=graph1->point;i++){
        int min = MAX_LENGTH;
        for(int l=1;l<=graph1->point;l++){//找此时生成树中最短的边与终点
            if(flag[l]<min&&flag[l]>0){
                min = flag[l];
                num = l;
            }
        }
        if(min==MAX_LENGTH) break;
        tmp = graph2->next;
        while(tmp){//找最短边的起点和终点
            if(tmp->value==min){
                if(tmp->start==num||tmp->end==num){
                    min_tree[tree_num].end = num;
                    min_tree[tree_num].length = min;
                    if(tmp->start==num) min_tree[tree_num].start = tmp->end;
                    else min_tree[tree_num].start = tmp->start;
                    min_tree_print->adjacent_matrix[min_tree[tree_num].start][min_tree[tree_num].end] = min_tree[tree_num].length;
                    min_tree_print->adjacent_matrix[min_tree[tree_num].end][min_tree[tree_num].start] = min_tree[tree_num].length;
                    money+=min_tree[tree_num].length;
                    tree_num++;
                }
            }
            tmp = tmp->next;
        }
        for(int l=1;l<=graph1->point;l++){//到每个点的最短距离
            if(graph1->adjacent_matrix[num][l]<flag[l])
                flag[l] = graph1->adjacent_matrix[num][l];
        }
        flag[num] = -1;
    }
    min_tree_print->edge = tree_num;
    
    print_min_tree(min_tree,tree_num);
    //邻接链表的打印
    print1(min_tree_print);
    printf("通信网的总造价为：%d(拾万元)\n",money);
}

int cmp(const void *a, const void *b){//快排的cmp函数
    Min_tree *x = (Min_tree*)a,*y = (Min_tree*)b;
    return (*x).length-(*y).length;
}
int find(int x,int dsu[]){//并查集查找根
    if(x!=dsu[x]){
        dsu[x] = find(dsu[x],dsu);
    }
    return dsu[x];
}
//Kruskal最小生成树
void kruskal(Graph1 *graph1,Graph2*graph2){
    Min_tree min_tree[graph1->edge];//从下标0开始
    Graph2*tmp = graph2->next;
    int i=0;
    while(tmp){//初始化最小生成树
        min_tree[i].start = tmp->start;
        min_tree[i].end = tmp->end;
        min_tree[i].length = tmp->value;
        i++;
        tmp = tmp->next;
    }

    int tree_num=0;//最小生成树结点的个数
    Graph1 *min_tree_print = (Graph1*)malloc(sizeof(Graph1));
    min_tree_print->point = graph1->point;
    for(i=0;i<=MAX;i++){//初始化打印的邻接矩阵
        for(int l=0;l<=MAX;l++)
            min_tree_print->adjacent_matrix[i][l] = MAX_LENGTH;
    }
    min_tree_print->point = graph1->point;
    int money=0;//造价

    int dsu[graph1->point+1];//并查集，判断是否要环路
    for(i=0;i<=graph1->point;i++){//并查集初始化
        dsu[i] = i;
    }

    qsort(min_tree,graph1->edge,sizeof(Min_tree),cmp);
    for(i=0;i<graph1->edge;i++){
        if(tree_num==graph1->point-1) break;
        int start = find(min_tree[i].start,dsu);
        int end = find(min_tree[i].end,dsu);
        if(start == end) continue;
        else{
            min_tree[tree_num].start = min_tree[i].start;
            min_tree[tree_num].end = min_tree[i].end;
            min_tree[tree_num].length = min_tree[i].length;
            min_tree_print->adjacent_matrix[start][end] = min_tree[i].length;
            min_tree_print->adjacent_matrix[end][start] = min_tree[i].length;
            money+=min_tree[tree_num].length;
            tree_num++;
        }
    }
    min_tree_print->edge = tree_num;

    print_min_tree(min_tree,tree_num);
    //邻接链表的打印
    print1(min_tree_print);
    printf("通信网的总造价为：%d(拾万元)\n",money);
}



int main(){
    Graph1 graph1;
    Graph2 graph2;
    int flag1=0,flag2=0;
    while(1){
        printf("+------------------------欢迎使用通信线路的敷设应用------------------------+\n");
        printf("|                                                                          |\n");
        printf("|     1-邻接矩阵存储，链表输出                2-边表存储，矩阵输出         |\n");
        printf("|     3-Prim算法+计算造价                     4-Kruskal算法+计算造价       |\n");
        printf("|     5-退出系统                                                           |\n");
        printf("|                                                                          |\n");
        printf("+--------------------------------------------------------------------------+\n");
        int i;
        printf("请输入您的选择：\n");
        scanf("%d",&i);
        if(i==1){
            init_Graph1(&graph1);
            flag1=1;
        }
        else if(i==2){
            init_Graph2(&graph2);
            flag2=1;
        }
        else if(i==3){
            if(flag1==1&&flag2==1) prim(&graph1,&graph2);
            else printf("未初始化邻接矩阵或边表\n");
        }
        else if(i==4){
            if(flag1==1&&flag2==1) kruskal(&graph1,&graph2);
            else printf("未初始化邻接矩阵或边表\n");
        }
        else if(i==5) {
            printf("欢迎下次使用\n");
            break;
        }
        else printf("输入有误，请重新输入\n");
    }
    return 0;
}


