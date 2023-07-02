#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include "bitset"


FILE *topologyfile;
FILE *messagesfile;
FILE *changesfile;
FILE *output_dv;

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "usage: distvec topologyfile messagesfile changesfile\n");
        return 0;
    }
    
    int cost_table[200][200];
    int next_table[200][200];
    int numofNode;

    topologyfile = fopen(argv[1], "r");
    if (topologyfile == NULL)
    {
        printf("Error: open input file.\n");
        return 1;
    }

    fscanf(topologyfile,"%d",&numofNode);

    for(int i=0;i<numofNode;i++){
        for(int j=0;j<numofNode;j++){
            if(i!=j){
                cost_table[i][j]=-999; //cost
                next_table[i][j]=-1; //next
            }
            else{
                cost_table[i][j]=0; //cost
                next_table[i][j]=i; //next
            }
        }
    }

    int temp1,temp2,temp3;
    
    while (EOF != fscanf(topologyfile, "%d %d %d", &temp1, &temp2,&temp3))
    {
       next_table[temp1][temp2]=temp2;
       next_table[temp2][temp1]=temp1;
       cost_table[temp1][temp2]=temp3;
       cost_table[temp2][temp1]=temp3;
        if(temp3==-999){
        next_table[temp1][temp2]=-1;
       next_table[temp2][temp1]=-1;
       }
    }

int flag=1;

while(flag==1)
{
    flag=0;
    for(int i=0;i<numofNode;i++){
        for(int j=0;j<numofNode;j++){
            if(next_table[i][j]!=j||next_table[i][j]==i){
                continue;}
                for(int s=0;s<numofNode;s++){
                if((next_table[i][s]==-1||cost_table[i][j]+cost_table[j][s]<cost_table[i][s])&&cost_table[i][j]+cost_table[j][s]>=0){
                                      
                    cost_table[i][s]=cost_table[i][j]+cost_table[j][s];
                    next_table[i][s]=j;
                    flag=1;
                }
            
            }
            
        }
    }
   
}
    output_dv=fopen("output_dv.txt","wt");
  if (output_dv == NULL)
    {
        printf("Error: open input file.\n");
        return 1;
    }

    for(int i=0;i<numofNode;i++){
        for(int j=0;j<numofNode;j++){
            if(next_table[i][j]!=-1&&cost_table[i][j]!=-999){ //0번 노드부터 순차적으로 연결이 되어있는 노드들 destin, next. cost
                fprintf(output_dv,"%d %d %d\n",j,next_table[i][j],cost_table[i][j]);
            }
        }
         fprintf(output_dv,"\n");
    }

    

    messagesfile = fopen(argv[2], "r");
    if (messagesfile == NULL)
    {
        printf("Error: open input file.\n");
        return 1;
    }

    char message[5000];

    while (EOF != fscanf(messagesfile, "%d %d %[^\n]s\n", &temp1, &temp2,message))
    {

        if(next_table[temp1][temp2]==-1||cost_table[temp1][temp2]==-999){
            fprintf(output_dv,"from %d to %d cost infinite hops unreachable message %s\n",temp1,temp2,message);
        }
        else{
            fprintf(output_dv,"from %d to %d cost %d hops ",temp1,temp2,cost_table[temp1][temp2]);
            while(temp1!=temp2){
                fprintf(output_dv,"%d ",temp1);
                temp1=next_table[temp1][temp2]; //next
            }
            fprintf(output_dv,"message %s\n",message);
        }
    }
    


    fclose(topologyfile);
    fclose(messagesfile);



    changesfile = fopen(argv[3], "r");
    if (changesfile == NULL)
    {
        printf("Error: open input file.\n");
        return 1;
  
    }

//changedfile start


    fprintf(output_dv,"\n"); //space line align

    while(1){
    topologyfile = fopen(argv[1], "r");
    if (topologyfile == NULL)
    {
        printf("Error: open input file.\n");
        return 1;
    }

    fscanf(topologyfile,"%d",&numofNode);

    for(int i=0;i<numofNode;i++){
        for(int j=0;j<numofNode;j++){
            if(i!=j){
                cost_table[i][j]=-999; //cost
                next_table[i][j]=-1; //next
            }
            else{
                cost_table[i][j]=0; //cost
                next_table[i][j]=i; //next
            }
        }
    }

    int temp1,temp2,temp3;
    
    while (EOF != fscanf(topologyfile, "%d %d %d", &temp1, &temp2,&temp3))
    {
       next_table[temp1][temp2]=temp2;
       next_table[temp2][temp1]=temp1;
       cost_table[temp1][temp2]=temp3;
       cost_table[temp2][temp1]=temp3;
        if(temp3==-999){
        next_table[temp1][temp2]=-1;
       next_table[temp2][temp1]=-1;
       }
    }

//changed node

if(EOF==fscanf(changesfile,"%d %d %d",&temp1,&temp2,&temp3)) break;
       
       next_table[temp1][temp2]=temp2;
       next_table[temp2][temp1]=temp1;
       cost_table[temp1][temp2]=temp3;
       cost_table[temp2][temp1]=temp3;
       if(temp3==-999){
        next_table[temp1][temp2]=-1;
       next_table[temp2][temp1]=-1;
       }

int flag=1;

while(flag==1)
{
    flag=0;
    for(int i=0;i<numofNode;i++){
        for(int j=0;j<numofNode;j++){
            if(next_table[i][j]!=j||next_table[i][j]==i){
                continue;}
                for(int s=0;s<numofNode;s++){
                if((next_table[i][s]==-1||cost_table[i][j]+cost_table[j][s]<cost_table[i][s])&&cost_table[i][j]+cost_table[j][s]>=0){
                                      
                    cost_table[i][s]=cost_table[i][j]+cost_table[j][s];
                    next_table[i][s]=j;
                    flag=1;
                }
            
            }
            
        }
    }
   
}

      for(int i=0;i<numofNode;i++){
        for(int j=0;j<numofNode;j++){
            if(next_table[i][j]!=-1&&cost_table[i][j]!=-999){ //0번 노드부터 순차적으로 연결이 되어있는 노드들 destin, next. cost
                fprintf(output_dv,"%d %d %d\n",j,next_table[i][j],cost_table[i][j]);
            }
        }
         fprintf(output_dv,"\n");
    }

    

    messagesfile = fopen(argv[2], "r");
    if (messagesfile == NULL)
    {
        printf("Error: open input file.\n");
        return 1;
    }

    char message[5000];

      while (EOF != fscanf(messagesfile, "%d %d %[^\n]s\n", &temp1, &temp2,message))
    {

        if(next_table[temp1][temp2]==-1||cost_table[temp1][temp2]==-999){
            fprintf(output_dv,"from %d to %d cost infinite hops unreachable message %s\n",temp1,temp2,message);
        }
        else{
            fprintf(output_dv,"from %d to %d cost %d hops ",temp1,temp2,cost_table[temp1][temp2]);
            while(temp1!=temp2){
                fprintf(output_dv,"%d ",temp1);
                temp1=next_table[temp1][temp2]; //next
            }
            fprintf(output_dv,"message %s\n",message);
        }
    }



    fprintf(output_dv,"\n");
    fclose(topologyfile);
    fclose(messagesfile);
  
    }
    fclose(changesfile);
    fclose(output_dv);
    printf("Complete. Output file written to output_dv.txt\n");
 
    return 0;
}