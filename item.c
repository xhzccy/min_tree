#include <stdio.h>
#include <stdlib.h>

#define MAX 13
#define MAX_LENGTH 10000
//�ڽӾ���洢
typedef struct {
    int adjacent_matrix[MAX+1][MAX+1];//�ڽӾ����±��Ӧͼ����ţ�0����
    int edge;//����
    int point;//����
}Graph1;

//�߱�洢
typedef struct GRAPH2{
    int start;//���
    int end;//�յ�
    int value;//Ȩֵ
    struct GRAPH2*next;
}Graph2;

//��С�������߽ṹ��
typedef struct {
    int start;//���
    int length;//��
    int end;//�����ӵĵ�
}Min_tree;

//�ڽ�����Ĵ�ӡ
void print1(Graph1*graph1){
    printf("\n�ڽ�������ʽ�����\n");
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
//�ڽӾ��󴴽�
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

//�ڽӾ���Ĵ�ӡ
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
    printf("\n�ڽӾ�����ʽ�����\n");
    for(int i=0;i<MAX+1;i++) {
        if(i==0)printf(" @");
        else printf("%6d",i);
    }
    printf("\n");
    for(int i=1;i<MAX+1;i++){
        printf("%2d",i);
        for(int l=1;l<MAX+1;l++){
            if(tmp[i][l]==MAX_LENGTH) printf("%7s","��") ;
            else printf("%6d",tmp[i][l]);
        }
        printf("\n");
    }
    printf("\n");
}
//�߱�Ĵ���
void init_Graph2(Graph2*graph2){//��ͷ���
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
    printf("\n��С����������˳��\n");
    for(int i=0;i<len;i++){
        printf("%2d  --  %d\n",min_tree[i].start,min_tree[i].end);
    }
    printf("\n");
}

//prim��С������
void prim(Graph1*graph1,Graph2*graph2){
    Min_tree min_tree[graph1->point];//���±�0��ʼ
    int tree_num=0;//������С����������Լ��������ĸ���
    int flag[graph1->point+1];//���������±��Ӧͼ�����
    Graph2 *tmp = graph2->next;//���ں������С������Ѱ�Ҷ�Ӧ��
    Graph1 *min_tree_print = (Graph1*)malloc(sizeof(Graph1));
    min_tree_print->point = graph1->point;
    for(int i=0;i<=MAX;i++){
        for(int l=0;l<=MAX;l++)
            min_tree_print->adjacent_matrix[i][l] = MAX_LENGTH;
    }
    int money=0;//���

    for(int i=0;i<=graph1->point;i++)
        flag[i] = MAX_LENGTH;//���������������Ϊ�����
    int num;
    printf("��������С��������㣺\n");
    scanf("%d",&num);
    for(int i=1;i<=graph1->point;i++){//��ÿ�������̾���
        if(graph1->adjacent_matrix[num][i]<flag[i])
            flag[i] = graph1->adjacent_matrix[num][i];
    }
    flag[num] = -1;
    for(int i=1;i<=graph1->point;i++){
        int min = MAX_LENGTH;
        for(int l=1;l<=graph1->point;l++){//�Ҵ�ʱ����������̵ı����յ�
            if(flag[l]<min&&flag[l]>0){
                min = flag[l];
                num = l;
            }
        }
        if(min==MAX_LENGTH) break;
        tmp = graph2->next;
        while(tmp){//����̱ߵ������յ�
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
        for(int l=1;l<=graph1->point;l++){//��ÿ�������̾���
            if(graph1->adjacent_matrix[num][l]<flag[l])
                flag[l] = graph1->adjacent_matrix[num][l];
        }
        flag[num] = -1;
    }
    min_tree_print->edge = tree_num;
    
    print_min_tree(min_tree,tree_num);
    //�ڽ�����Ĵ�ӡ
    print1(min_tree_print);
    printf("ͨ�����������Ϊ��%d(ʰ��Ԫ)\n",money);
}

int cmp(const void *a, const void *b){//���ŵ�cmp����
    Min_tree *x = (Min_tree*)a,*y = (Min_tree*)b;
    return (*x).length-(*y).length;
}
int find(int x,int dsu[]){//���鼯���Ҹ�
    if(x!=dsu[x]){
        dsu[x] = find(dsu[x],dsu);
    }
    return dsu[x];
}
//Kruskal��С������
void kruskal(Graph1 *graph1,Graph2*graph2){
    Min_tree min_tree[graph1->edge];//���±�0��ʼ
    Graph2*tmp = graph2->next;
    int i=0;
    while(tmp){//��ʼ����С������
        min_tree[i].start = tmp->start;
        min_tree[i].end = tmp->end;
        min_tree[i].length = tmp->value;
        i++;
        tmp = tmp->next;
    }

    int tree_num=0;//��С���������ĸ���
    Graph1 *min_tree_print = (Graph1*)malloc(sizeof(Graph1));
    min_tree_print->point = graph1->point;
    for(i=0;i<=MAX;i++){//��ʼ����ӡ���ڽӾ���
        for(int l=0;l<=MAX;l++)
            min_tree_print->adjacent_matrix[i][l] = MAX_LENGTH;
    }
    min_tree_print->point = graph1->point;
    int money=0;//���

    int dsu[graph1->point+1];//���鼯���ж��Ƿ�Ҫ��·
    for(i=0;i<=graph1->point;i++){//���鼯��ʼ��
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
    //�ڽ�����Ĵ�ӡ
    print1(min_tree_print);
    printf("ͨ�����������Ϊ��%d(ʰ��Ԫ)\n",money);
}



int main(){
    Graph1 graph1;
    Graph2 graph2;
    int flag1=0,flag2=0;
    while(1){
        printf("+------------------------��ӭʹ��ͨ����·�ķ���Ӧ��------------------------+\n");
        printf("|                                                                          |\n");
        printf("|     1-�ڽӾ���洢���������                2-�߱�洢���������         |\n");
        printf("|     3-Prim�㷨+�������                     4-Kruskal�㷨+�������       |\n");
        printf("|     5-�˳�ϵͳ                                                           |\n");
        printf("|                                                                          |\n");
        printf("+--------------------------------------------------------------------------+\n");
        int i;
        printf("����������ѡ��\n");
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
            else printf("δ��ʼ���ڽӾ����߱�\n");
        }
        else if(i==4){
            if(flag1==1&&flag2==1) kruskal(&graph1,&graph2);
            else printf("δ��ʼ���ڽӾ����߱�\n");
        }
        else if(i==5) {
            printf("��ӭ�´�ʹ��\n");
            break;
        }
        else printf("������������������\n");
    }
    return 0;
}


