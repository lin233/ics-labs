/*
 *
 * Student Name:Li Shan
 * Student ID:516030910175
 *
 */
#include "cachelab.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>


int E;  //every set has E lines
int s;  //set counts S=2^s
int b;  
int hits = 0;
int misses = 0;
int evicts = 0;


//a cache has S sets, each set has E lines. 
//tag-set-block
//t-s-b

typedef struct{
  int valid;              // 0 or 1
  unsigned long long tag; //tag
  int  block_offset;    //block_offset
  int usetime;          //usetime,It indicates the time of access, 
                        //because as long as the relative time is expressed, it is OK to use numbers for convenience. 
                        //From small to large, it means that time goes from far to near,

                        //and the smallest number is the earliest. 
                        //To select the best replacement line when judging replacement, use LRU
} line_t;

typedef struct{   //each set has E lines
  line_t* lines;
} set_t;

typedef struct{ //each cache has S (S=2^s) sets
  set_t* sets;
} cache_t;

cache_t newCache(long long S,int E){//Initialize the cache and its internal set, line structure
  cache_t cache;
  set_t NewSet;
  line_t NewLine;
  cache.sets = (set_t *)malloc(S * sizeof(set_t)); //each cache S set

  for(int i = 0; i < S; i++){
  
    NewSet.lines = (line_t *)malloc(E * sizeof(line_t));//each set E lines
    cache.sets[i]=NewSet;

    for(int j = 0; j < E; j++){
      NewLine.valid = 0;  
      NewLine.tag = 0;
      NewLine.block_offset = 0;
      NewLine.usetime = 0;
      NewSet.lines[j] = NewLine;  
    }
  }
  return cache;
}



void handleCache(cache_t cache,unsigned long long addr){
  int t = 64 - (s + b); // m=64

  unsigned long long tag = addr >> (s + b); // tag-set-block 
  unsigned long long setNum = ((addr << t) >> (t+b));  

  set_t curset = cache.sets[setNum];
  
  int hitflag = 0;
  int fullflag = 1;

  int mostearly = curset.lines[0].usetime;  //indicate the earliest visit time
  int mostlate = curset.lines[0].usetime;   //indicate last time visit 
  int mostEarlyLineNum = 0; //indicate line which has the earliest visit time
  int freelineNum;    //find a free line if it exists
  

  //find the earliest visit line to replace
  for(int i = 0; i < E ;i++){
    line_t curline = curset.lines[i];
    int findfree = 0;

    if(curline.usetime < mostearly){
      mostearly = curline.usetime;
      mostEarlyLineNum = i;     //the line 
    }
    if(curline.usetime > mostlate){
      mostlate = curline.usetime;
    }
    
    //it shows it has freeline
    //using "findfree" when there has more than one freeline to avoid Repeated assignment
    if((curline.valid == 0) && (!findfree)){
      findfree = 1;
      fullflag = 0;
      freelineNum = i;
    }
  }
  

  for(int i = 0; i < E; i++){
    line_t curline = curset.lines[i];
    if((curline.valid == 1) && (curline.tag == tag)){
      hitflag = 1;   //hit
      hits++;
      mostlate ++ ;     //last time +1
      curline.usetime = mostlate ; //update the time
      curset.lines[i]=curline;   //because the time is updated,so the line need to update
    }
  }

  if(!hitflag){   //missed
    misses++;
    
    line_t line;
    line.valid = 1;
    line.tag = tag;
    mostlate++;
    line.usetime = mostlate;

    if(fullflag){ //if full,need replace
      evicts++;   
      curset.lines[mostEarlyLineNum] = line;
    }
    else{//not full can use freeline
      curset.lines[freelineNum] = line;
    }
  }

}

void Help()
{
  printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
         "-h: Optional help ﬂag that prints usage info\n" 
         "-v: Optional verbose ﬂag that displays trace info\n"
         "-s <s>: Number of set index bits (S = 2s is the number of sets)\n"
         "-E <E>: Associativity (number of lines per set)"
         "-b <b>: Number of block bits (B = 2b is the block size)\n"
         "-t <tracefile>: Name of the valgrind trace to replay\n"
         "Examples:\n"
         "./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
         "./csim-ref -v -s 4 -E 1 -b 4 -t traces/yi.trace\n"
        );

}


int main(int argc, char **argv)
{ 

  char *tracefile;
  
  char op;
  while ( (op=getopt(argc,argv,"s:E:b:t:vh"))!= -1){
     switch (op){
     case 's':
       s = atoi(optarg);
       break;
     case 'E':
       E = atoi(optarg);
       break;
     case 'b':
       b = atoi(optarg);
       break;
     case 't':
       tracefile = optarg;
       break;
     case 'v':
       break;
     case 'h':
       Help();
       exit(0);
     default:
       Help();
       exit(1);
     }
  }

  FILE *trace = fopen(tracefile,"r");

  long long S = 1 << s; //S = 2^s

  cache_t cache = newCache(S,E);
  

	char op2;
	unsigned long long addr;
  int size;

  if (trace != NULL){
    while(fscanf(trace," %c %llx,%d",&op2,&addr,&size) == 3)
      switch(op2){
        case 'I':
          break;
        case 'L': case 'S':
          handleCache(cache,addr);
          break;
        case 'M':
          handleCache(cache,addr);
          handleCache(cache,addr);
          break;
        default:
          break;
    }
  }
  fclose(trace);
  printSummary(hits, misses, evicts);
  return 0;
}
