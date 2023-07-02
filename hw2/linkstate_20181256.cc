#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include "bitset"
#include <vector>
#include <algorithm>

FILE *topologyfile;
FILE *messagesfile;
FILE *changesfile;
FILE *output_ls;

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
    int linked_source[100000];
    int linked_dest[100000];
    int linked_cost[100000];
    int link_cnt=0;

    while (EOF != fscanf(topologyfile, "%d %d %d", &temp1, &temp2,&temp3))
    {
    linked_source[link_cnt]=temp1;
    linked_dest[link_cnt]=temp2;
    linked_cost[link_cnt]=temp3; 
    link_cnt++;
    }
    for(int i=0;i<numofNode;i++){
        int spt_next[200];
       int spt_cost[200];

         for(int j=0;j<numofNode;j++){
            if(j!=i){
                spt_next[j]=-1;
                spt_cost[j]=-999;
            }
            else{
                spt_next[j]=i;
                spt_cost[j]=0;
            }
        }

        for(int j=0;j<link_cnt;j++){
            if(linked_source[j]==i){
                spt_next[linked_dest[j]]=linked_source[j];
                spt_cost[linked_dest[j]]=linked_cost[j];
            }
            if(linked_dest[j]==i){
                spt_next[linked_source[j]]=linked_dest[j];
                spt_cost[linked_source[j]]=linked_cost[j];
            }
        }

        
       std::vector<int> selectedNode;
      selectedNode.push_back(i);
       int tempNode;

        for(int j=1;j<numofNode-1;j++){
            int min=1000000;
            for(int k=0;k<numofNode;k++){
                if(spt_cost[k]<=0||spt_cost[k]>=min)
                continue;
                if(std::find(selectedNode.begin(), selectedNode.end(), k)!=selectedNode.end())
                {
                    continue;}
                min=spt_cost[k];
                tempNode=k;
            }
            selectedNode.push_back(tempNode);

            
        
        for(int k=0;k<link_cnt;k++){
            if(linked_source[k]==tempNode){
                if((spt_next[linked_dest[k]]==-1||spt_cost[tempNode]+linked_cost[k]<spt_cost[linked_dest[k]])&&spt_cost[tempNode]+linked_cost[k]>=0)
            {
            spt_cost[linked_dest[k]]=spt_cost[tempNode]+linked_cost[k];
            spt_next[linked_dest[k]]=tempNode;
                                   }

            }
           else if(linked_dest[k]==tempNode){
                if((spt_next[linked_source[k]]==-1||spt_cost[tempNode]+linked_cost[k]<spt_cost[linked_source[k]])&&spt_cost[tempNode]+linked_cost[k]>=0)

            {spt_cost[linked_source[k]]=spt_cost[tempNode]+linked_cost[k];
            spt_next[linked_source[k]]=tempNode;
                                
            }
            }

        }
        }

    for(int j=0;j<numofNode;j++){
        if(j==i) continue;
        if(spt_cost[j]>=0){
            cost_table[i][j]=spt_cost[j];
            int temp=j;
            while(1){
                if(spt_next[temp]==i) break;
                temp=spt_next[temp];
            }
            next_table[i][j]=temp;
        }
    }

        }



  





    output_ls=fopen("output_ls.txt","wt");
  if (output_ls == NULL)
    {
        printf("Error: open input file.\n");
        return 1;
    }

    for(int i=0;i<numofNode;i++){
        for(int j=0;j<numofNode;j++){
            if(next_table[i][j]!=-1&&cost_table[i][j]!=-999){ //0번 노드부터 순차적으로 연결이 되어있는 노드들 destin, next. cost
                fprintf(output_ls,"%d %d %d\n",j,next_table[i][j],cost_table[i][j]);
            }
        }
         fprintf(output_ls,"\n");
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
            fprintf(output_ls,"from %d to %d cost infinite hops unreachable message %s\n",temp1,temp2,message);
        }
        else{
            fprintf(output_ls,"from %d to %d cost %d hops ",temp1,temp2,cost_table[temp1][temp2]);
            while(temp1!=temp2){
                fprintf(output_ls,"%d ",temp1);
                temp1=next_table[temp1][temp2]; //next
            }
            fprintf(output_ls,"message %s\n",message);
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
fprintf(output_ls,"\n"); 
//changedfile start


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
   
    if(EOF==fscanf(changesfile,"%d %d %d",&temp1,&temp2,&temp3)) break;
       
       
       
       for(int i=0;i<link_cnt;i++){
           if(linked_source[i]==temp1&&linked_dest[i]==temp2){
               linked_cost[i]=temp3;
           }
           else if(linked_source[i]==temp2&&linked_dest[i]==temp1){
               linked_cost[i]=temp3;
           }
           else{
               linked_source[link_cnt]=temp1;
               linked_dest[link_cnt]=temp2;
               linked_cost[link_cnt]=temp3;
               link_cnt++;
           }
       }       




    for(int i=0;i<numofNode;i++){
        int spt_next[200];
       int spt_cost[200];

         for(int j=0;j<numofNode;j++){
            if(j!=i){
                spt_next[j]=-1;
                spt_cost[j]=-999;
            }
            else{
                spt_next[j]=i;
                spt_cost[j]=0;
            }
        }

        for(int j=0;j<link_cnt;j++){
            if(linked_source[j]==i){
                spt_next[linked_dest[j]]=linked_source[j];
                spt_cost[linked_dest[j]]=linked_cost[j];
                if(linked_cost[j]==-999){
                     spt_next[linked_dest[j]]=-1;
                }
            }
            if(linked_dest[j]==i){
                spt_next[linked_source[j]]=linked_dest[j];
                spt_cost[linked_source[j]]=linked_cost[j];
                if(linked_cost[j]==-999){
                     spt_next[linked_source[j]]=-1;
                }
            }
        }

        
       std::vector<int> selectedNode;
      selectedNode.push_back(i);
       int tempNode;

        for(int j=1;j<numofNode-1;j++){
            int min=1000000;
            for(int k=0;k<numofNode;k++){
                if(spt_cost[k]<=0||spt_cost[k]>=min)
                continue;
                if(std::find(selectedNode.begin(), selectedNode.end(), k)!=selectedNode.end())
                {
                    continue;}
                min=spt_cost[k];
                tempNode=k;
            }
            selectedNode.push_back(tempNode);

            
  
        
        for(int k=0;k<link_cnt;k++){
            if(linked_source[k]==tempNode){
                if((spt_next[linked_dest[k]]==-1||spt_cost[tempNode]+linked_cost[k]<spt_cost[linked_dest[k]])&&spt_cost[tempNode]+linked_cost[k]>=0)
            {
            spt_cost[linked_dest[k]]=spt_cost[tempNode]+linked_cost[k];
            spt_next[linked_dest[k]]=tempNode;
                                   }

            }
           else if(linked_dest[k]==tempNode){
                if((spt_next[linked_source[k]]==-1||spt_cost[tempNode]+linked_cost[k]<spt_cost[linked_source[k]])&&spt_cost[tempNode]+linked_cost[k]>=0)

            {spt_cost[linked_source[k]]=spt_cost[tempNode]+linked_cost[k];
            spt_next[linked_source[k]]=tempNode;
                                
            }
            }

        }
        }

    for(int j=0;j<numofNode;j++){
        if(j==i) continue;
        if(spt_cost[j]>=0){
            cost_table[i][j]=spt_cost[j];
            int temp=j;
            while(1){
                if(spt_next[temp]==i) break;
                temp=spt_next[temp];
            }
            next_table[i][j]=temp;
        }
    }

        }
         for(int i=0;i<numofNode;i++){
        for(int j=0;j<numofNode;j++){
            if(next_table[i][j]!=-1&&cost_table[i][j]!=-999){ //0번 노드부터 순차적으로 연결이 되어있는 노드들 destin, next. cost
                fprintf(output_ls,"%d %d %d\n",j,next_table[i][j],cost_table[i][j]);
            }
        }
         fprintf(output_ls,"\n");
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
            fprintf(output_ls,"from %d to %d cost infinite hops unreachable message %s\n",temp1,temp2,message);
        }
        else{
            fprintf(output_ls,"from %d to %d cost %d hops ",temp1,temp2,cost_table[temp1][temp2]);
            while(temp1!=temp2){
                fprintf(output_ls,"%d ",temp1);
                temp1=next_table[temp1][temp2]; //next
            }
            fprintf(output_ls,"message %s\n",message);
        }
    }
    fprintf(output_ls,"\n"); 

    fclose(topologyfile);
    fclose(messagesfile);
  
    }
    fclose(changesfile);
    fclose(output_ls);
    printf("Complete. Output file written to output_ls.txt\n");
 
    return 0;
}