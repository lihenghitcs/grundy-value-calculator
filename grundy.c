#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include<stdint.h>
#include<math.h>
#include<getopt.h>
#include<sys/stat.h>
#ifdef __APPLE__
    #include<sys/malloc.h>
#else
    #include<malloc.h>
#endif
#ifdef _WIN32
    #include <direct.h>
    #define mkdir _mkdir
#else
    #include <unistd.h>
#endif

int startN = 5;
int endN = 0;

typedef struct vertex{
    int color; // 0: uncolored 1: blue 2: red
    int position; // i of v_i
    int out; // 0: not the vertex between u and w  1: the vertex between u and w
} V;

V **vertexes;

int **TableTnii;
int **TableTniiB;
int **TableTniitB;
int **TableTniiBtB;
int **TableTniiBtR;
int **TableTniiBB;
int **TableTniiBR;
int **TableTniitBtB;
int **TableTniiBBtR;
int **TableTniiBRtR;
int **TableTniiBBtB;
int **TableTniiBRtB;

int getm(int a){
    int count = 0;
    while ((1 << count) < a){
        count++;
    }
    if (a == (1<<count) - 1) return count;
    return -1;
}

void swap(int *a, int *b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void inittables(){
    int i,j;
    TableTnii = (int **)malloc(endN * sizeof(int *));
    TableTniiB = (int **)malloc(endN * sizeof(int *));
    TableTniitB = (int **)malloc(endN * sizeof(int *));
    TableTniiBtB = (int **)malloc(endN * sizeof(int *));
    TableTniiBtR = (int **)malloc(endN * sizeof(int *));
    TableTniiBB = (int **)malloc(endN * sizeof(int *));
    TableTniiBR = (int **)malloc(endN * sizeof(int *));
    TableTniitBtB = (int **)malloc(endN * sizeof(int *));
    TableTniiBBtR = (int **)malloc(endN * sizeof(int *));
    TableTniiBRtR = (int **)malloc(endN * sizeof(int *));
    TableTniiBBtB = (int **)malloc(endN * sizeof(int *));
    TableTniiBRtB = (int **)malloc(endN * sizeof(int *));
    
    for(i=0;i<endN;i++) {
        TableTnii[i] = (int *)malloc(endN * sizeof(int));
        TableTniiB[i] = (int *)malloc(endN * sizeof(int));
        TableTniitB[i] = (int *)malloc(endN * sizeof(int));
        TableTniiBtB[i] = (int *)malloc(endN * sizeof(int));
        TableTniiBtR[i] = (int *)malloc(endN * sizeof(int));
        TableTniiBB[i] = (int *)malloc(endN * sizeof(int));
        TableTniiBR[i] = (int *)malloc(endN * sizeof(int));
        TableTniitBtB[i] = (int *)malloc(endN * sizeof(int));
        TableTniiBBtR[i] = (int *)malloc(endN * sizeof(int));
        TableTniiBRtR[i] = (int *)malloc(endN * sizeof(int));
        TableTniiBBtB[i] = (int *)malloc(endN * sizeof(int));
        TableTniiBRtB[i] = (int *)malloc(endN * sizeof(int));
    }
    
    for(i=0;i<endN;i++){
        for(j=0;j<endN;j++){
            TableTnii[i][j]=-1;
            TableTniiB[i][j]=-1;
            TableTniitB[i][j]=-1;
            TableTniiBtB[i][j]=-1;
            TableTniiBtR[i][j]=-1;
            TableTniiBB[i][j]=-1;
            TableTniiBR[i][j]=-1;
            TableTniitBtB[i][j]=-1;
            TableTniiBBtR[i][j]=-1;
            TableTniiBRtR[i][j]=-1;
            TableTniiBBtB[i][j]=-1;
            TableTniiBRtB[i][j]=-1;
        }
    }
}

int count_rows(char path[]) {
    int count = 0;
    FILE *file = fopen(path,"r");
    char buffer[8192];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++;
    }
    fclose(file);
    return count;
}

void readTable(char path[],int **table){
    int i,j;
    FILE *f = fopen(path,"r");
    for(i=0;i<startN;i++){
        for(j=0;j<startN;j++){
            if(j != startN-1)fscanf(f,"%d,",&table[i][j]);
            else fscanf(f,"%d",&table[i][j]);
        }
    }
    fclose(f);
}

void writeTable(char path[], int n, int **table){
    int i,j;
    FILE *f = fopen(path,"w");
    for(i=0;i<=n;i++){
        for(j=0;j<=n;j++){
            if(j != n) fprintf(f,"%d,",table[i][j]);
            else fprintf(f,"%d",table[i][j]);
        }
        fprintf(f,"\n");
    }
    fclose(f);
}

void init(int read){
    if(read == 1){
        startN = count_rows("./result/Tnii.csv");
        if(endN == 0){
            endN = startN + 100;
        }
    }
    vertexes = (V **)malloc(endN * sizeof(V *));
    inittables();
    if(read == 0) return;
    readTable("./result/Tnii.csv",TableTnii);
    readTable("./result/TniiB.csv",TableTniiB);
    readTable("./result/TniitB.csv",TableTniitB);
    readTable("./result/TniiBtB.csv",TableTniiBtB);
    readTable("./result/TniiBtR.csv",TableTniiBtR);
    readTable("./result/TniiBB.csv",TableTniiBB);
    readTable("./result/TniiBR.csv",TableTniiBR);
    readTable("./result/TniitBtB.csv",TableTniitBtB);
    readTable("./result/TniiBBtR.csv",TableTniiBBtR);
    readTable("./result/TniiBRtR.csv",TableTniiBRtR);
    readTable("./result/TniiBBtB.csv",TableTniiBBtB);
    readTable("./result/TniiBRtB.csv",TableTniiBRtB);
}

int mex(int A[], int n){
    for(int i = 0; i < n; ++ i)
        while(A[i] >= 0 && A[i] <= n && A[A[i]] != A[i])
            swap(&A[i], &A[A[i]]);
    for(int i = 0; i < n; ++ i)
        if(A[i] != i){
            return i;
        }
    return n;
}

void InitTnii(int n,int i){
    int j;
    V *v;
    for(j = 0;j < n - 2;j++){
        v = (V *)malloc(sizeof(V));
        v->color = 0;
        v->position = j;
        v->out = 0;
        vertexes[j] = v;
    }
    v = (V *)malloc(sizeof(V));
    v->color = 0;
    v->position = i;
    v->out = 1;
    vertexes[n - 2] = v;
    v = (V *)malloc(sizeof(V));
    v->color = 0;
    v->position = i;
    v->out = 1;
    vertexes[n - 1] = v;
}

void printtime(){
    struct timeval tv;
    gettimeofday(&tv,NULL);
    uint64_t sec = tv.tv_sec;
    //time_t sec = time(0);
    uint64_t min = tv.tv_sec/60;
    struct tm cur_tm;
    #if defined(linux) || defined(__APPLE__)
    localtime_r((time_t*)&sec,&cur_tm);
    #endif
    #ifdef __WINDOWS_
    localtime_s(&cur_tm,(time_t*)&sec);
    #endif
    char cur_time[20];
    snprintf(cur_time, 20, "%d-%02d-%02d %02d:%02d:%02d", cur_tm.tm_year+1900, cur_tm.tm_mon+1, cur_tm.tm_mday, cur_tm.tm_hour, cur_tm.tm_min, cur_tm.tm_sec);
    printf("%s",cur_time);
}

int hasblue(int colors[],int size){
    int i;
    for(i=0;i<size;i++){
        if(colors[i] == 1) return 1;
    }
    return 0;
}

int hasred(int colors[],int size){
    int i;
    for(i=0;i<size;i++){
        if(colors[i] == 2) return 1;
    }
    return 0;
}

int Grundy(V *tree[], int n){
    int results[3*n];
    int count = 0;
    int j,k;
    int g1,g2,g3,g4;
    V *newv;
    if(tree[n-1]->out == 0){
        for(j=0;j<n;j++){
            if(tree[j]->color == 0) continue;
            if(j != 0 && j != n-1){
                printf("Error 1");
                exit(1);
            }
        }
        if(tree[0]->color == 0 && tree[n-1]->color != 0){
            return n-1;
        }
        if(tree[0]->color != 0 && tree[n-1]->color == 0){
            return n-1;
        }
        if(tree[0]->color != 0 && tree[0]->color == tree[n-1]->color){
            return 1;
        }
        if(tree[0]->color != 0 && tree[0]->color != tree[n-1]->color){
            return 0;
        }
    }else if(tree[n-1]->out == 1 && tree[n-2]->out == 1){
        int i = tree[n-1]->position;

        int TniiBflag1 = 0;
        if(tree[0]->color != 0) TniiBflag1 = 1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0){
                TniiBflag1 = 0;
                break;
            }
        }
        if(TniiBflag1 == 1){
            if(TableTniiB[n-1][i] != -1) return TableTniiB[n-1][i];
        }

        int TniiBflag2 = 0;
        if(tree[n-3]->color != 0) TniiBflag2 = 1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-3){
                TniiBflag2 = 0;
                break;
            }
        }
        if(TniiBflag2 == 1){
            if(TableTniiB[n-1][n-i-3] != -1) return TableTniiB[n-1][n-i-3];
        }

        int TniitBflag1 = 0;
        if(tree[n-1]->color != 0) TniitBflag1 = 1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-1){
                TniitBflag1 = 0;
                break;
            }
        }
        if(TniitBflag1 == 1){
            if(TableTniitB[n-1][i] != -1) return TableTniitB[n-1][i];
            if(TableTniitB[n-1][n-i-3] != -1) return TableTniitB[n-1][n-i-3];
        }
        
        int TniitBflag2 = 0;
        if(tree[n-2]->color != 0) TniitBflag2 = 1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-2){
                TniitBflag2 = 0;
                break;
            }
        }
        if(TniitBflag2 == 1){
            if(TableTniitB[n-1][i] != -1) return TableTniitB[n-1][i];
            if(TableTniitB[n-1][n-i-3] != -1) return TableTniitB[n-1][n-i-3];
        }

        int TniiBtBflag1 = 0;
        if(tree[0]->color != 0 && tree[0]->color == tree[n-1]->color) TniiBtBflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1){
                TniiBtBflag1 = 0;
                break;
            }
        }
        if(TniiBtBflag1 == 1){
            if(TableTniiBtB[n-2][i] != -1) return TableTniiBtB[n-2][i];
        }

        int TniiBtBflag2 = 0;
        if(tree[0]->color != 0 && tree[0]->color == tree[n-2]->color) TniiBtBflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-2){
                TniiBtBflag2 = 0;
                break;
            }
        }
        if(TniiBtBflag2 == 1){
            if(TableTniiBtB[n-2][i] != -1) return TableTniiBtB[n-2][i];
        }

        int TniiBtBflag3 = 0;
        if(tree[n-3]->color != 0 && tree[n-3]->color == tree[n-1]->color) TniiBtBflag3=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-3 && j!=n-1){
                TniiBtBflag3 = 0;
                break;
            }
        }
        if(TniiBtBflag3 == 1){
            if(TableTniiBtB[n-2][n-i-3] != -1) return TableTniiBtB[n-2][n-i-3];
        }

        int TniiBtBflag4 = 0;
        if(tree[n-3]->color != 0 && tree[n-3]->color == tree[n-2]->color) TniiBtBflag4=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-3 && j!=n-2){
                TniiBtBflag4 = 0;
                break;
            }
        }
        if(TniiBtBflag4 == 1){
            if(TableTniiBtB[n-2][n-i-3] != -1) return TableTniiBtB[n-2][n-i-3];
        }

        int TniiBtRflag1 = 0;
        if((tree[0]->color ^ tree[n-1]->color) == 3) TniiBtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1){
                TniiBtRflag1 = 0;
                break;
            }
        }
        if(TniiBtRflag1 == 1){
            if(TableTniiBtR[n-2][i] != -1) return TableTniiBtR[n-2][i];
        }

        int TniiBtRflag2 = 0;
        if((tree[0]->color ^ tree[n-2]->color) == 3) TniiBtRflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-2){
                TniiBtRflag2 = 0;
                break;
            }
        }
        if(TniiBtRflag2 == 1){
            if(TableTniiBtR[n-2][i] != -1) return TableTniiBtR[n-2][i];
        }

        int TniiBtRflag3 = 0;
        if((tree[n-3]->color ^ tree[n-1]->color) == 3) TniiBtRflag3=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-3 && j!=n-1){
                TniiBtRflag3 = 0;
                break;
            }
        }
        if(TniiBtRflag3 == 1){
            if(TableTniiBtR[n-2][n-i-3] != -1) return TableTniiBtR[n-2][n-i-3];
        }

        int TniiBtRflag4 = 0;
        if((tree[n-3]->color ^ tree[n-2]->color) == 3) TniiBtRflag4=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-3 && j!=n-2){
                TniiBtRflag4 = 0;
                break;
            }
        }
        if(TniiBtRflag4 == 1){
            if(TableTniiBtR[n-2][n-i-3] != -1) return TableTniiBtR[n-2][n-i-3];
        }

        int TniiBBflag1 = 0;
        if(tree[0]->color != 0 && tree[0]->color == tree[n-3]->color) TniiBBflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3){
                TniiBBflag1 = 0;
                break;
            }
        }
        if(TniiBBflag1 == 1){
            if(TableTniiBB[n-2][i] != -1) return TableTniiBB[n-2][i];
            if(TableTniiBB[n-2][n-i-3] != -1) return TableTniiBB[n-2][n-i-3];
        }

        int TniiBRflag1 = 0;
        if((tree[0]->color ^ tree[n-3]->color) == 3) TniiBRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3){
                TniiBRflag1 = 0;
                break;
            }
        }
        if(TniiBRflag1 == 1){
            if(TableTniiBR[n-2][i] != -1) return TableTniiBR[n-2][i];
            if(TableTniiBR[n-2][n-i-3] != -1) return TableTniiBR[n-2][n-i-3];
        }

        int TniitBtBflag1 = 0;
        if(tree[n-1]->color != 0 && tree[n-1]->color == tree[n-2]->color) TniitBtBflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-1 && j!=n-2){
                TniitBtBflag1 = 0;
                break;
            }
        }
        if(TniitBtBflag1 == 1){
            if(TableTniitBtB[n-2][i] != -1) return TableTniitBtB[n-2][i];
            if(TableTniitBtB[n-2][n-i-3] != -1) return TableTniitBtB[n-2][n-i-3];
        }

        int TniitBtRflag1 = 0;
        if((tree[n-1]->color ^ tree[n-2]->color) == 3) TniitBtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-1 && j!=n-2){
                TniitBtRflag1 = 0;
                break;
            }
        }
        if(TniitBtRflag1 == 1){
            return (i%2) ^ ((n-i-3)%2);
        }

        int TniiBBtRflag1 = 0;
        if((tree[0]->color ^ tree[n-1]->color) == 3 && tree[0]->color == tree[n-3]->color) TniiBBtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3 && j!=n-1){
                TniiBBtRflag1 = 0;
                break;
            }
        }
        if(TniiBBtRflag1 == 1){
            if(TableTniiBBtR[n-3][i] != -1) return TableTniiBBtR[n-3][i];
            if(TableTniiBBtR[n-3][n-i-3] != -1) return TableTniiBBtR[n-3][n-i-3];
        }

        int TniiBBtRflag2 = 0;
        if((tree[0]->color ^ tree[n-2]->color) == 3 && tree[0]->color == tree[n-3]->color) TniiBBtRflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3 && j!=n-2){
                TniiBBtRflag2 = 0;
                break;
            }
        }
        if(TniiBBtRflag2 == 1){
            if(TableTniiBBtR[n-3][i] != -1) return TableTniiBBtR[n-3][i];
            if(TableTniiBBtR[n-3][n-i-3] != -1) return TableTniiBBtR[n-3][n-i-3];
        }

        int TniiBRtRflag1 = 0;
        if((tree[0]->color ^ tree[n-3]->color) == 3 && tree[n-3]->color == tree[n-1]->color) TniiBRtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3 && j!=n-1){
                TniiBRtRflag1 = 0;
                break;
            }
        }
        if(TniiBRtRflag1 == 1){
            if(TableTniiBRtR[n-3][i] != -1) return TableTniiBRtR[n-3][i];
            if(TableTniiBRtB[n-3][n-i-3] != -1) return TableTniiBRtB[n-3][n-i-3];
        }

        int TniiBRtRflag2 = 0;
        if((tree[0]->color ^ tree[n-3]->color) == 3 && tree[n-3]->color == tree[n-2]->color) TniiBRtRflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3 && j!=n-2){
                TniiBRtRflag2 = 0;
                break;
            }
        }
        if(TniiBRtRflag2 == 1){
            if(TableTniiBRtR[n-3][i] != -1) return TableTniiBRtR[n-3][i];
            if(TableTniiBRtB[n-3][n-i-3] != -1) return TableTniiBRtB[n-3][n-i-3];
        }

        int TniiBBtBflag1 = 0;
        if(tree[0]->color != 0 && tree[0]->color == tree[n-3]->color && tree[0]->color == tree[n-1]->color) TniiBBtBflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3 && j!=n-1){
                TniiBBtBflag1 = 0;
                break;
            }
        }
        if(TniiBBtBflag1 == 1){
            if(TableTniiBBtB[n-3][i] != -1) return TableTniiBBtB[n-3][i];
            if(TableTniiBBtB[n-3][n-i-3] != -1) return TableTniiBBtB[n-3][n-i-3];
        }

        int TniiBBtBflag2 = 0;
        if(tree[0]->color != 0 && tree[0]->color == tree[n-3]->color && tree[0]->color == tree[n-2]->color) TniiBBtBflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3 && j!=n-2){
                TniiBBtBflag2 = 0;
                break;
            }
        }
        if(TniiBBtBflag2 == 1){
            if(TableTniiBBtB[n-3][i] != -1) return TableTniiBBtB[n-3][i];
            if(TableTniiBBtB[n-3][n-i-3] != -1) return TableTniiBBtB[n-3][n-i-3];
        }

        int TniiBRtBflag1 = 0;
        if((tree[0]->color ^ tree[n-3]->color) == 3 && tree[0]->color == tree[n-1]->color) TniiBRtBflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3 && j!=n-1){
                TniiBRtBflag1 = 0;
                break;
            }
        }
        if(TniiBRtBflag1 == 1){
            if(TableTniiBRtB[n-3][i] != -1) return TableTniiBRtB[n-3][i];
            if(TableTniiBRtR[n-3][n-i-3] != -1) return TableTniiBRtR[n-3][n-i-3];
        }

        int TniiBRtBflag2 = 0;
        if((tree[0]->color ^ tree[n-3]->color) == 3 && tree[0]->color == tree[n-2]->color) TniiBRtBflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-3 && j!=n-2){
                TniiBRtBflag2 = 0;
                break;
            }
        }
        if(TniiBRtBflag2 == 1){
            if(TableTniiBRtB[n-3][i] != -1) return TableTniiBRtB[n-3][i];
            if(TableTniiBRtR[n-3][n-i-3] != -1) return TableTniiBRtR[n-3][n-i-3];
        }

        int TniiBtBtRflag1 = 0;
        if((tree[n-1]->color ^ tree[n-2]->color) == 3 && (tree[0]->color == tree[n-1]->color || tree[0]->color == tree[n-2]->color)) TniiBtBtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1 && j!=n-2){
                TniiBtBtRflag1 = 0;
                break;
            }
        }
        if(TniiBtBtRflag1 == 1){
            return (i-1) ^ ((n-i-3)%2);
        }

        int TniiBtBtRflag2 = 0;
        if((tree[n-1]->color ^ tree[n-2]->color) == 3 && (tree[n-3]->color == tree[n-1]->color || tree[n-3]->color == tree[n-2]->color)) TniiBtBtRflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-3 && j!=n-1 && j!=n-2){
                TniiBtBtRflag2 = 0;
                break;
            }
        }
        if(TniiBtBtRflag2 == 1){
            return (n-i-4) ^ (i%2);
        }

        int TniiBtBtBflag1 = 0;
        if(tree[0]->color != 0 && tree[0]->color == tree[n-1]->color && tree[0]->color == tree[n-2]->color) TniiBtBtBflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1 && j!=n-2){
                TniiBtBtBflag1 = 0;
                break;
            }
        }
        if(TniiBtBtBflag1 == 1){
            if(i == 1) return n-3;
            else if (i == 2 && (n-3)%2 == 1) return 0;
            else if (i == 2 && (n-3)%2 == 0) return 1;
            else{
                int m = getm(i);
                if(m == -1 || m < 2) return n-i-4;
                if((n-3)%(1<<m) != (1<<(m-1))) return n-i-4;
                return n-i-2;
            }
        }

        int TniiBtBtBflag2 = 0;
        if(tree[n-3]->color != 0 && tree[n-3]->color == tree[n-1]->color && tree[n-3]->color == tree[n-2]->color) TniiBtBtBflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-3 && j!=n-1 && j!=n-2){
                TniiBtBtBflag2 = 0;
                break;
            }
        }
        if(TniiBtBtBflag2 == 1){
            if(i == n-4) return n-3;
            else if (i == n-5 && (n-3)%2 == 1) return 0;
            else if (i == n-5 && (n-3)%2 == 0) return 1;
            else{
                int m = getm(n-i-3);
                if(m == -1 || m < 2) return i-1;
                if((n-3)%(1<<m) != (1<<(m-1))) return i-1;
                return i+1;
            }
        }

        int TniiBtRtRflag1 = 0;
        if((tree[0]->color ^ tree[n-1]->color) == 3 && tree[n-1]->color == tree[n-2]->color) TniiBtRtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1 && j!=n-2){
                TniiBtRtRflag1 = 0;
                break;
            }
        }
        if(TniiBtRtRflag1 == 1){
            if(i == 1 || i == 2) return (i-1) * (n-3) + (2 - 1.5*i)*(1 + ((n-3)%2==0?1:-1));
            else return (n-i-4) ^ 1;
        }

        int TniiBtRtRflag2 = 0;
        if((tree[n-3]->color ^ tree[n-1]->color) == 3 && tree[n-1]->color == tree[n-2]->color) TniiBtRtRflag2=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=n-3 && j!=n-1 && j!=n-2){
                TniiBtRtRflag2 = 0;
                break;
            }
        }
        if(TniiBtRtRflag2 == 1){
            i = n - i - 3;
            if(i == 1 || i == 2) return (i-1) * (n-3) + (2 - 1.5*i)*(1 + ((n-3)%2==0?1:-1));
            else return (n-i-4) ^ 1;
        }

        int TniiBBtBtBflag1 = 0;
        if(tree[0]->color != 0 && tree[0]->color == tree[n-1]->color && tree[0]->color == tree[n-2]->color && tree[0]->color == tree[n-3]->color) TniiBBtBtBflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1 && j!=n-2 && j!=n-3){
                TniiBBtBtBflag1 = 0;
                break;
            }
        }
        if(TniiBBtBtBflag1 == 1){
            if(i > ((n-3)/2)) i = n-i-3;
            if(i == 2 && ((n-4) % 2 == 0 || (n-4)%4 == 3)) return n-5;
            else if(i == 2 && (n-4)%4 == 1) return n-7;
            else return 1;
        }

        int TniiBRtBtBflag1 = 0;
        if((tree[0]->color ^ tree[n-3]->color) == 3 && tree[0]->color == tree[n-1]->color && tree[0]->color == tree[n-2]->color) TniiBRtBtBflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1 && j!=n-2 && j!=n-3){
                TniiBRtBtBflag1 = 0;
                break;
            }
        }
        if(TniiBRtBtBflag1 == 1){
            if(i > ((n-3)/2)){
                i = n - i - 3;
                if (i == 1) return n-5;
                else return 0;
            }else{
                if(i == 2 && ((n-4)%2 == 0 || (n-4)%4 == 1)) return n-5;
                else if(i == 2 && (n-4)%4 == 3) return n-8;
                else return 0;
            }
        }

        int TniiBRtRtRflag1 = 0;
        if((tree[0]->color ^ tree[n-3]->color) == 3 && tree[n-3]->color == tree[n-1]->color && tree[n-3]->color == tree[n-2]->color) TniiBRtRtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1 && j!=n-2 && j!=n-3){
                TniiBRtRtRflag1 = 0;
                break;
            }
        }
        if(TniiBRtRtRflag1 == 1){
            if(i > ((n-3)/2)){
                i = n - i - 3;
                if(i == 2 && ((n-4)%2 == 0 || (n-4)%4 == 1)) return n-5;
                else if(i == 2 && (n-4)%4 == 3) return n-8;
                else return 0;
            }else{
                if (i == 1) return n-5;
                else return 0;
            }
        }

        int TniiBBtRtRflag1 = 0;
        if((tree[0]->color ^ tree[n-1]->color) == 3 && tree[0]->color == tree[n-3]->color && tree[n-1]->color == tree[n-2]->color) TniiBBtRtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1 && j!=n-2 && j!=n-3){
                TniiBBtRtRflag1 = 0;
                break;
            }
        }
        if(TniiBBtRtRflag1 == 1){
            if(i == 1 || i == n-4) return n-5;
            else return 1;
        }

        int TniiBBtBtRflag1 = 0;
        if((tree[n-1]->color ^ tree[n-2]->color) == 3 && tree[0]->color != 0 && tree[n-3]->color != 0) TniiBBtBtRflag1=1;
        for(j=0;j<n;j++){
            if(tree[j]->color != 0 && j!=0 && j!=n-1 && j!=n-2 && j!=n-3){
                TniiBBtBtRflag1 = 0;
                break;
            }
        }
        if(TniiBBtBtRflag1 == 1){
            return (i - 1) ^ (n-i-4);
        }
        
        for(j=0;j<n;j++){
            if(j==0 || j == n-3){
                int jcolor = tree[j]->color;
                if(jcolor != 0) continue;
                int nearcolor = tree[j==0?j+1:j-1]->color;
                if(nearcolor != 1){
                    tree[j]->color = 1;
                    results[count++] = Grundy(tree,n);
                    tree[j]->color = 0;
                }
                if(nearcolor != 2){
                    tree[j]->color = 2;
                    results[count++] = Grundy(tree,n);
                    tree[j]->color = 0;
                }
            }else if(j == n-2 || j == n-1){
                int jcolor = tree[j]->color;
                if(jcolor != 0) continue;
                int nearcolor = tree[i]->color;
                if(nearcolor != 1){
                    tree[j]->color = 1;
                    results[count++] = Grundy(tree,n);
                    tree[j]->color = 0;
                }
                if(nearcolor != 2){
                    tree[j]->color = 2;
                    results[count++] = Grundy(tree,n);
                    tree[j]->color = 0;
                }
            }else{
                int jcolor = tree[j]->color;
                if(jcolor != 0) continue;
                int nearred;
                int nearblue;
                if(j == i){
                    int colors[4] = {tree[j-1]->color,tree[j+1]->color,tree[n-1]->color,tree[n-2]->color};
                    nearred = hasred(colors,4);
                    nearblue = hasblue(colors,4);
                }else{
                    int colors[2] = {tree[j-1]->color,tree[j+1]->color};
                    nearred = hasred(colors,2);
                    nearblue = hasblue(colors,2);
                }
                if(nearblue != 1){
                    tree[j]->color = 1;
                    if(j == i){
                        V *temp1[j+1],*temp2[n-j-2],*temp3[2],*temp4[2];
                        for(k=0;k<=j;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k;
                            newv->out = tree[k]->out;
                            temp1[k] = newv;
                        }
                        g1 = Grundy(temp1,j+1);
                        for(k=0;k<=j;k++){
                            free(temp1[k]);
                        }

                        for(k=j;k<=n-3;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k-j;
                            newv->out = tree[k]->out;
                            temp2[k-j] = newv;
                        }
                        g2 = Grundy(temp2,n-j-2);
                        for(k=0;k<=n-j-3;k++){
                            free(temp2[k]);
                        }

                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[j]->color;
                        newv->position = 0;
                        newv->out = 0;
                        temp3[0] = newv;

                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[n-1]->color;
                        newv->position = 1;
                        newv->out = 0;
                        temp3[1] = newv;
                        g3 = Grundy(temp3,2);
                        for(k=0;k<=1;k++){
                            free(temp3[k]);
                        }

                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[j]->color;
                        newv->position = 0;
                        newv->out = 0;
                        temp4[0] = newv;

                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[n-2]->color;
                        newv->position = 1;
                        newv->out = 0;
                        temp4[1] = newv;
                        g4 = Grundy(temp4,2);
                        for(k=0;k<=1;k++){
                            free(temp4[k]);
                        }
                    }else if(j < i){
                        V *temp1[j+1],*temp2[n-j];
                        for(k=0;k<=j;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k;
                            newv->out = tree[k]->out;
                            temp1[k] = newv;
                        }
                        g1 = Grundy(temp1,j+1);
                        for(k=0;k<=j;k++){
                            free(temp1[k]);
                        }

                        for(k=j;k<n;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = (k==n-1 || k == n-2)? i-j : k-j;
                            newv->out = tree[k]->out;
                            temp2[k-j] = newv;
                        }
                        g2 = Grundy(temp2,n-j);
                        for(k=0;k<=n-j-1;k++){
                            free(temp2[k]);
                        }
                        g3 = 0;
                        g4 = 0;
                    }else{
                        V *temp1[j+3],*temp2[n-j-2];
                        for(k=0;k<=j;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k;
                            newv->out = tree[k]->out;
                            temp1[k] = newv;
                        }
                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[n-1]->color;
                        newv->position = tree[n-1]->position;
                        newv->out = tree[n-1]->out;
                        temp1[j+1] = newv;
                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[n-2]->color;
                        newv->position = tree[n-2]->position;
                        newv->out = tree[n-2]->out;
                        temp1[j+2] = newv;
                        g1 = Grundy(temp1,j+3);
                        for(k=0;k<=j+2;k++){
                            free(temp1[k]);
                        }

                        for(k=j;k<n-2;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k-j;
                            newv->out = tree[k]->out;
                            temp2[k-j] = newv;
                        }
                        g2 = Grundy(temp2,n-j-2);
                        for(k=0;k<=n-j-3;k++){
                            free(temp2[k]);
                        }
                        g3 = 0;
                        g4 = 0;
                    }
                    //results[count++] = Grundy(tree,n);
                    results[count++] = g1 ^ g2 ^ g3 ^ g4;
                    tree[j]->color = 0;
                }
                if(nearred != 1){
                    tree[j]->color = 2;
                    if(j == i){
                        V *temp1[j+1],*temp2[n-j-2],*temp3[2],*temp4[2];
                        for(k=0;k<=j;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k;
                            newv->out = tree[k]->out;
                            temp1[k] = newv;
                        }
                        g1 = Grundy(temp1,j+1);
                        for(k=0;k<=j;k++){
                            free(temp1[k]);
                        }

                        for(k=j;k<=n-3;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k-j;
                            newv->out = tree[k]->out;
                            temp2[k-j] = newv;
                        }
                        g2 = Grundy(temp2,n-j-2);
                        for(k=0;k<=n-j-3;k++){
                            free(temp2[k]);
                        }

                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[j]->color;
                        newv->position = 0;
                        newv->out = 0;
                        temp3[0] = newv;

                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[n-1]->color;
                        newv->position = 1;
                        newv->out = 0;
                        temp3[1] = newv;
                        g3 = Grundy(temp3,2);
                        for(k=0;k<=1;k++){
                            free(temp3[k]);
                        }

                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[j]->color;
                        newv->position = 0;
                        newv->out = 0;
                        temp4[0] = newv;

                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[n-2]->color;
                        newv->position = 1;
                        newv->out = 0;
                        temp4[1] = newv;
                        g4 = Grundy(temp4,2);
                        for(k=0;k<=1;k++){
                            free(temp4[k]);
                        }
                    }else if(j < i){
                        V *temp1[j+1],*temp2[n-j];
                        for(k=0;k<=j;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k;
                            newv->out = tree[k]->out;
                            temp1[k] = newv;
                        }
                        g1 = Grundy(temp1,j+1);
                        for(k=0;k<=j;k++){
                            free(temp1[k]);
                        }

                        for(k=j;k<n;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = (k==n-1 || k == n-2)? i-j : k-j;
                            newv->out = tree[k]->out;
                            temp2[k-j] = newv;
                        }
                        g2 = Grundy(temp2,n-j);
                        for(k=0;k<=n-j-1;k++){
                            free(temp2[k]);
                        }
                        g3 = 0;
                        g4 = 0;
                    }else{
                        V *temp1[j+3],*temp2[n-j-2];
                        for(k=0;k<=j;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k;
                            newv->out = tree[k]->out;
                            temp1[k] = newv;
                        }
                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[n-1]->color;
                        newv->position = tree[n-1]->position;
                        newv->out = tree[n-1]->out;
                        temp1[j+1] = newv;
                        newv = (V *)malloc(sizeof(V));
                        newv->color = tree[n-2]->color;
                        newv->position = tree[n-2]->position;
                        newv->out = tree[n-2]->out;
                        temp1[j+2] = newv;
                        g1 = Grundy(temp1,j+3);
                        for(k=0;k<=j+2;k++){
                            free(temp1[k]);
                        }

                        for(k=j;k<n-2;k++){
                            newv = (V *)malloc(sizeof(V));
                            newv->color = tree[k]->color;
                            newv->position = k-j;
                            newv->out = tree[k]->out;
                            temp2[k-j] = newv;
                        }
                        g2 = Grundy(temp2,n-j-2);
                        for(k=0;k<=n-j-3;k++){
                            free(temp2[k]);
                        }
                        g3 = 0;
                        g4 = 0;
                    }
                    results[count++] = g1 ^ g2 ^ g3 ^ g4;
                    tree[j]->color = 0;
                }
            }
        }
    }
    return mex(results,count);
}

void CalcTnii(int n){
    int i,result;
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        result = Grundy(vertexes,n);
        printf("Tnii(%d, %d) = %d, ",n,i,result);
        printtime();
        printf("\n");
        TableTnii[n][i] = result;
    }
}

void CalcTniiB(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=n-4;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        result = Grundy(vertexes,n);
        TableTniiB[n-1][i] = result;
    }
    end = clock();
    printf("TniiB(%d, i) completed, time:%.2fs, ",n-1,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniitB(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        vertexes[n-1]->color = 1;
        result = Grundy(vertexes,n);
        TableTniitB[n-1][i] = result;
    }
    end = clock();
    printf("TniitB(%d, i) completed, time:%.2fs, ",n-1,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniiBtB(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=n-4;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        vertexes[n-1]->color = 1;
        result = Grundy(vertexes,n);
        TableTniiBtB[n-2][i] = result;
    }
    end = clock();
    printf("TniiBtB(%d, i) completed, time:%.2fs, ",n-2,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniiBtR(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=n-4;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        vertexes[n-1]->color = 2;
        result = Grundy(vertexes,n);
        TableTniiBtR[n-2][i] = result;
    }
    end = clock();
    printf("TniiBtR(%d, i) completed, time:%.2fs, ",n-2,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniiBB(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        vertexes[n-3]->color = 1;
        result = Grundy(vertexes,n);
        TableTniiBB[n-2][i] = result;
    }
    end = clock();
    printf("TniiBB(%d, i) completed, time:%.2fs, ",n-2,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniiBR(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        vertexes[n-3]->color = 2;
        result = Grundy(vertexes,n);
        TableTniiBR[n-2][i] = result;
    }
    end = clock();
    printf("TniiBR(%d, i) completed, time:%.2fs, ",n-2,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniitBtB(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        vertexes[n-1]->color = 1;
        vertexes[n-2]->color = 1;
        result = Grundy(vertexes,n);
        TableTniitBtB[n-2][i] = result;
    }
    end = clock();
    printf("TniitBtB(%d, i) completed, time:%.2fs, ",n-2,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniiBBtR(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        vertexes[n-3]->color = 1;
        vertexes[n-1]->color = 2;
        result = Grundy(vertexes,n);
        TableTniiBBtR[n-3][i] = result;
    }
    end = clock();
    printf("TniiBBtR(%d, i) completed, time:%.2fs, ",n-3,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniiBRtR(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        vertexes[n-3]->color = 2;
        vertexes[n-1]->color = 2;
        result = Grundy(vertexes,n);
        TableTniiBRtR[n-3][i] = result;
    }
    end = clock();
    printf("TniiBRtR(%d, i) completed, time:%.2fs, ",n-3,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniiBBtB(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        vertexes[n-3]->color = 1;
        vertexes[n-1]->color = 1;
        result = Grundy(vertexes,n);
        TableTniiBBtB[n-3][i] = result;
    }
    end = clock();
    printf("TniiBBtB(%d, i) completed, time:%.2fs, ",n-3,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void CalcTniiBRtB(int n){
    int i,result;
    clock_t start,end;
    start = clock();
    for(i=1;i<=(n-3)/2;i++){
        InitTnii(n,i);
        vertexes[0]->color = 1;
        vertexes[n-3]->color = 2;
        vertexes[n-1]->color = 1;
        result = Grundy(vertexes,n);
        TableTniiBRtB[n-3][i] = result;
    }
    end = clock();
    printf("TniiBRtB(%d, i) completed, time:%.2fs, ",n-3,((double)(end-start)/1000000));
    printtime();
    printf("\n");
}

void calculate(){
    int n;
    const char *folder_path = "result";
    struct stat st = {0};

    if (stat(folder_path, &st) == -1) {
        int status = mkdir(folder_path, 0700);
    }

    for(n=startN;n<endN;n++){
        printf("n = %d:\n",n);
        CalcTniiBBtR(n);
        CalcTniiBRtR(n);
        CalcTniiBBtB(n);
        CalcTniiBRtB(n);
        CalcTniiBtB(n);
        CalcTniiBtR(n);
        CalcTniiBB(n);
        CalcTniiBR(n);
        CalcTniitBtB(n);
        CalcTniiB(n);
        CalcTniitB(n);
        CalcTnii(n);
        printf("\n");
        if((n<=100 && n%50==0) || (n>100 && n<=500 && n%10==0) || n>500 || n==endN-1){
            writeTable("./result/Tnii.csv", n, TableTnii);
            writeTable("./result/TniiB.csv", n, TableTniiB);
            writeTable("./result/TniitB.csv", n, TableTniitB);
            writeTable("./result/TniiBtB.csv", n, TableTniiBtB);
            writeTable("./result/TniiBtR.csv", n, TableTniiBtR);
            writeTable("./result/TniiBB.csv", n, TableTniiBB);
            writeTable("./result/TniiBR.csv", n, TableTniiBR);
            writeTable("./result/TniitBtB.csv", n, TableTniitBtB);
            writeTable("./result/TniiBBtR.csv", n, TableTniiBBtR);
            writeTable("./result/TniiBRtR.csv", n, TableTniiBRtR);
            writeTable("./result/TniiBBtB.csv", n, TableTniiBBtB);
            writeTable("./result/TniiBRtB.csv", n, TableTniiBRtB);
        }
    }

    for(int i=0;i<endN;i++){
        free(TableTnii[i]);
        free(TableTniiB[i]);
        free(TableTniitB[i]);
        free(TableTniiBtB[i]);
        free(TableTniiBtR[i]);
        free(TableTniiBB[i]);
        free(TableTniiBR[i]);
        free(TableTniitBtB[i]);
        free(TableTniiBBtR[i]);
        free(TableTniiBRtR[i]);
        free(TableTniiBBtB[i]);
        free(TableTniiBRtB[i]);
    }
    free(TableTnii);
    free(TableTniiB);
    free(TableTniitB);
    free(TableTniiBtB);
    free(TableTniiBtR);
    free(TableTniiBB);
    free(TableTniiBR);
    free(TableTniitBtB);
    free(TableTniiBBtR);
    free(TableTniiBRtR);
    free(TableTniiBBtB);
    free(TableTniiBRtB);
    free(vertexes);
}

void help(){
    printf("Usage: grundy [-hrn]\n");
    printf("   -h\t\tPrint Help (this message) and exit\n");
    printf("   -r\t\tRead the existing results from ./result/ and continue the calculation from it\n");
    printf("   -n <endN>\tCompute Grundy value until a certain N\n\n");
    printf("Example:\n");
    printf("   grundy -r\t\tRead the existing results, and calculate G(Tnii) from the max n of the result to n=maxn+100\n");
    printf("   grundy -r -n 1000\tRead the existing results, and calculate G(Tnii) from the max n of the result to n=1000\n");
    printf("   grundy -n 1000\tCalculate G(Tnii) from n=5 to n=1000");
}

int main(int argc, char **argv){
    int opt=0;
    int read = 0;
    while((opt=getopt(argc,argv,"hrn:"))!=-1)
    {
        switch(opt)
        {
            case 'h':help();exit(0);break;
            case 'r':read=1;break;
            case 'n':endN = atoi(optarg)+1;break;
            default:help();exit(1);
        }
    }
    init(read);
    printf("startN: %d, endN: %d\n", startN, endN);
    if(endN==0){
        help();
    }
    calculate();
}