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
}