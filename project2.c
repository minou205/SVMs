#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "svm.h"

#define maxL 1024
#define Nt 4

const char *s_word[]={"free","win","offer","click","money","urgent"};
const int s_num=6;


int count_word(const char *mail){
    int x=0;
    for(int i=0;i<s_num;i++){
        if(strstr(mail,s_word[i])!=NULL){
            x++;
        }
    }
    return x;
}

void prepare_data(struct svm_node *nodes,int x,int l){
    nodes[0].index=1;//num of the special words
    nodes[0].value=x;

    nodes[1].index=2;//mail length
    nodes[1].value=l;

    nodes[2].index=-1;
}


void read_file(const char *fname,char *content){
    FILE *f=fopen(fname,"r");
    if (!f){
        printf("ERROR of reading %s\n",fname);
        exit(1);
    }
    fgets(content,maxL,f);
    fclose(f);
}

int main() {

const char *train_files[Nt]={"spam1.txt","spam2.txt","mail1.txt","mail2.txt"};
int train_fr[Nt]={1, 1, 0, 0}; //Spam = 1 and mail = 0

struct svm_problem prob;//training data
struct svm_parameter param;//lab settings
struct svm_node *x_space;//norms

prob.l=Nt;
prob.y=(double *)malloc(Nt*sizeof(double));
prob.x=(struct svm_node **)malloc(Nt*sizeof(struct svm_node *));
x_space=(struct svm_node *)malloc(Nt*3*sizeof(struct svm_node));

char content[maxL];
int keyword_count,l,index=0;

for (int i=0;i<Nt;i++){
    read_file(train_files[i], content);
    keyword_count=count_word(content);
    l=strlen(content);

    prob.y[i]=train_fr[i];
    prob.x[i]=&x_space[index];
    prepare_data(&x_space[index],keyword_count,l);
    index+=3;
}

param.svm_type=C_SVC;
param.kernel_type=LINEAR;
param.C=1.0;
param.cache_size=100;
param.eps=1e-3;
param.shrinking=1;

struct svm_model *model=svm_train(&prob,&param);

char test_file[]="new_mail.txt";
read_file(test_file,content);

keyword_count=count_word(content);
l=strlen(content);

struct svm_node test_node[3];
prepare_data(test_node,keyword_count,l);

double result=svm_predict(model,test_node);


if (result==1){
    printf("Spam");
}else{
    printf("Not Spam");
}

svm_free_and_destroy_model(&model);
free(prob.y);
free(prob.x);
free(x_space);

return 0;
}
