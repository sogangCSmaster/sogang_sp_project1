/*
 * Include headers to use functions for this project.
 */
#include <stdbool.h>
#include <stdio.h> 
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
/*
 * Define simple function and colors to make program more vivid and colorful.
 */
#define min(x,y) (((x) < (y)) ? (x) : (y))
#define NORMAL  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"
#define HELP  "\x1B[35m"
#define RED  "\x1B[31m"
/*
 * Define nodes(linked list etc..) for history, opcode, hash.
 *
 * I referenced node names, function names, value names in github.
 * 
 *
 */
typedef struct _node{
  char  data[100];
  struct _node* next;
}node;

typedef node* nptr;

typedef struct _list{
  int count;
  nptr head;
}list;

struct Node
{
  char data[100];
  struct Node *next;
};

typedef struct _oplist{
  unsigned int opcode;
  char mnemonic[50];
  struct _oplist* link;
}oplist;

typedef oplist* opptr;
opptr hash_head[20],hash_tail[20],working=NULL;

/*
 * Define functions used in this program.
 */
void printHelp();
void printDirectory();
void dump();
void printList(struct Node *node);
void edit();
int findLastIndex(char* myString, char x);
int findCharNum(char *, char);
int index_return_func(char*s);
void append(struct Node** , char *);
void fill();
void reset();
void handle_error();
void syntax_error();
int opcode_make_hash(char*s);
void init_hash();
void insert_hash();
void make_hash_table();
int find_opcode(char s[]);
void print_oplist();
int commaNum(char s[]);
void executeShellCommand();

/*
 * Define global values to easily build program.
 */
unsigned char mem[65536][16];
unsigned int tmp_s=0,tmp_e=0, address=0, tmp_add=0,tmp_value,tmp_s2, tmp_e2,tmp_value2;
int start= 0, end =0;
char token[3][100];
int totalToken;
bool flag = true;



/*
 * The following codes are Fuctions codes.
 * To make 20131575.c (our main.c) more clean and short, I wrote function codes here...
 *
 */

/*
 * Print error when syntax_error accurs.
 */
void syntax_error(){
  printf("%sSyntaxError: Invalid syntax.\n",RED);
}

/*
 * Return number of commas in a string.
 */
int commaNum(char s[]){
  int cnt=0;
  int i;
  i=0;
  while(i<strlen(s)){
    if(s[i] == ','){
      cnt++;
    }
    i++;
  }
  return cnt;

}


/*
 * Print opcodelist.
 */
void print_oplist(){
  int i;
  i=0;
  while(i<20){
    working= hash_head[i];
    if(working != NULL){
      printf("%d : ",i);
      while(working != NULL ){
        printf("[%s,%X]",working->mnemonic, working->opcode);
        working= working->link;
        if( working != NULL ) 
          printf("->");
        else
          break;
      }
    }
    else if(working==NULL)
      printf("%d : ",i);
    printf("\n");
    i++;
  }
}


/*
 * Find the number opcode. (etc. ADD -> 18)
 */
int find_opcode(char s[]){
  int i;
  int chk=1;

  i=0;
  while(i<20){
    working= hash_head[i];
    while(working!= NULL ){
      if( strcmp(working->mnemonic,s) == 0){
        printf("opcode is %X\n",working->opcode);
        chk = 0;
        break;
      }
      working = working->link;
    }
    i++;
  }
  return chk;

}

/*
 * Create hash table.
 * Sources are in opcode.txt in the same directory.
 */
void make_hash_table(){
  FILE* f;
  unsigned int op_f_file;
  int index;
  char s[100],mne_f_file[50];

  init_hash();
  f = fopen("opcode.txt","r");
  if( !f ){
    printf("%sFileError : Cannot open file.\n",RED);
    flag = false;
    return;
  }
  while( fgets(s,100,f) ){
    sscanf(s,"%X %s %*s",&op_f_file,mne_f_file);
    index = index_return_func(mne_f_file); 
    insert_hash(op_f_file,mne_f_file,index);
  }
fclose(f);
}

/*
 * This function is to insert hash in to a hashtable.
 */
void insert_hash(unsigned int opip,char s[],int index){

  working = (oplist*)malloc(sizeof(oplist));
  working->opcode = opip;
  strcpy(working->mnemonic,s);

  working->link = NULL;

  if(hash_head[index] != NULL){
    hash_tail[index]->link = working; 
    hash_tail[index] = working;
  }

  else if(hash_head[index] == NULL){
    hash_head[index] = working;
    hash_tail[index] = working;
  }
}

/*
 * Hash table initialize function.
 */
void init_hash(){
  int i; 
  i=0;
  while(i<20){
    hash_head[i] = NULL;
    i++;
  }
}

/*
 * get index
 */
int index_return_func(char*s){
  int i,sum;
  int len = strlen(s);
  i = 0;
  sum = 0;
  while(i<len){
    sum += s[i];
    i++;
  }
  return sum%20;
}

/*
 * print error
 */
void handle_error(){
  printf("%sCommandError : Invalid command.\n",RED);
}

/*
 * reset function.
 * if executed, mem array's values are seted to 0
 */
void reset(){
  int i;
  i = 0;
  while(i<65536){
    memset(mem[i],0,sizeof(int)*16);
    i++;
  }
}

/*
 * fill function.
 * get the start, end, value from user and fill value from start to end
 */
void fill(){
  int i,j;
  int tp_s=0, tp_e=0; 

  i=0;
  while(i<strlen(token[0])){
    if( (token[0][i] >='0' && token[0][i] <= '9') || (token[0][i] >='a' && token[0][i] <= 'z') || (token[0][i] >= 'A' && token[0][i] <= 'Z') || token[0][i] == '\t'){
      flag = true;
    }
    else{
      flag = false;
      return;
    }
    i++;
  }
  sscanf(token[0],"%x",&tp_s);
  i=0;
  while(i<strlen(token[1])){
    if( (token[1][i] >='0' && token[1][i] <= '9') || (token[1][i] >='a' && token[1][i] <= 'z') || (token[1][i] >= 'A' && token[1][i] <= 'Z') || token[1][i] == '\t'){
      flag = true;
    }
    else{
      flag = false;
      return;
    }
    i++;
  }
  sscanf(token[1],"%x",&tp_e);

  if( (tp_s > tp_e ) ){
    flag = false;
    printf("%sSyntaxError: Invalid syntax.\n",RED);
    return;
  } 
  if( tp_s < 0 || tp_s >= 16*65536){
    flag = false;
    printf("%sBoundaryError : Value is out of boundary.\n",RED);
    return;
  }
  else{
    tmp_s2 = tp_s;
  }
  if( tp_e < 0 || tp_e >= 16*65536){
    flag= false;
    printf("%sBoundaryError : Value is out of boundary.\n",RED);
    return;
  }
  else{
    tmp_e2 = tp_e;
  }
  i=0;
  while(i<strlen(token[2])){
    if((token[2][i] >= '0' && token[2][i] <= '9')|| (token[2][i] >= 'a' && token[2][i] <= 'z' ) || ( token[2][i]>='A' && token[2][i]<='Z')){
      flag = true;
    }
    else{
      flag= false;
      printf("%sValueError : Input value is not hexa-decimal.\n",RED);
      return;
    }
    i++;
  }
  sscanf(token[2],"%x",&tmp_value2);
  sscanf(token[0],"%x",&tmp_s2);
  sscanf(token[1],"%x",&tmp_e2);

  if( tmp_s2/16 == tmp_e2/16 ){
    i=tmp_s2%16;
    while(i<=tmp_e2%16){
      mem[tmp_s2/16][i] = tmp_value2;
      i++;
    }

  }
  else{
    i=tmp_s2/16;
    while(i<=tmp_e2/16){
      if(i==tmp_s2/16)
      {
        j=tmp_s2%16;
        while(j<16){
          mem[i][j] = tmp_value2;
          j++;
        }
      }
      else if(i== tmp_e2/16){
        j=0;
        while(j<=tmp_e2%16){
          mem[i][j] = tmp_value2;
          j++;
        }
      }
      else{
        j=0;
        while(j<16){
          mem[i][j] = tmp_value2;
          j++;
        }
      }
      i++;
    }
  }

}

/*
 * Edit function. (used as like this : edit 4, 68)
 */
void edit(){
  int i;
  int tmp_ad=0;

  i=0;
  while(i<strlen(token[0])){
    if( (token[0][i] >='0' && token[0][i] <= '9') || (token[0][i] >='a' && token[0][i] <= 'z') || (token[0][i] >= 'A' && token[0][i] <= 'Z') || token[0][i] == '\t'){
      flag = true;
    }
    else{
      flag = false;
      return;
    }
    i++;
  }
  sscanf(token[0],"%x",&tmp_ad);
  if( tmp_ad < 0 || tmp_ad >= 16*65536 ){
    flag = false;
    return;
  }
  else{
    tmp_add = tmp_ad;
  }
  i=0;
  while(i< strlen(token[1])){
    if((token[1][i] >='0' && token[1][i]<='9')||(token[1][i] >= 'a' && token[1][i] <= 'z') || (token[1][i] >= 'A' && token[1][i] <= 'Z')|| token[1][i] == '\t'){
      flag= true;
    }
    else{
      flag= false;
      return;
    }
    i++;
  }
  sscanf(token[1],"%x",&tmp_value);
  mem[tmp_add/16][tmp_add%16] = tmp_value;

}

/*
 * Find last index of a character in a string.
 */
int findLastIndex(char* myString, char x)
{
  int index = -1;
  int i;
  for (i = 0; i < strlen(myString); i++)
    if (myString[i] == x)
      index = i;
  return index;
}

/*
 * Fine the number of a character in a string.
 */
int findCharNum(char *myString, char x){
  int cnt = -1;
  int i;
  for(i=0; i<strlen(myString); i++){
    if(myString[i] == x)
      cnt++;
  }
  return cnt;

}
/*
 * Dump function.
 * print memory, ascii, hexa-decimals with different colors 
 */
void dump(){

  unsigned int i,j;

  if(totalToken == 1){
    start = address, end = start +16*10-1;
    end = min(16*65536-1,end);
    if( end != 16*65536-1 ) 
      address = end+1;
    else
      address = 0;

  }
  else if(totalToken == 2){
    i=0;
    while(i<strlen(token[0])){
      if( (token[0][i] >='0' && token[0][i] <= '9' ) || (token[0][i]>='a' && token[0][i]<='z') || (token[0][i]>='A' && token[0][i] <='Z')){
        flag=true;
      }
      else{
        flag= false;
        printf("%sValueError : Input value is not a hexadecimal.\n",RED);
        return;
      }
      i++;
    }

    sscanf(token[0],"%x",&tmp_s);
    start = tmp_s;
    end = start + 16*10 -1;
    end = min(16*65536-1,end);
    if( end == 16*65536-1)
      address= 0;
    else
      address = end+1;
  }
  else if(totalToken == 4){
    i=0;
    while(i<strlen(token[0])){
      if( (token[0][i] >='0' && token[0][i] <= '9' ) || (token[0][i]>='a' && token[0][i]<='z') || (token[0][i]>='A' && token[0][i] <='Z')|| token[0][i] == '\t'){
        flag=true;
      }
      else{
        flag= false;
        printf("%sValueError : Input value is not a hexadecimal.\n",RED);
        return ;  
      }
      i++;
    }

    sscanf(token[0],"%x",&tmp_s);
    i=0;
    while(i<strlen(token[1])){
      if( (token[1][i] >='0' && token[1][i] <= '9' ) || (token[1][i]>='a' && token[1][i]<='z') || (token[1][i]>='A' && token[1][i] <='Z') || token[0][i] == '\t'){
        flag=true;
      }
      else{
        flag= false;
        printf("%sValueError : Input value is not a hexadecimal.\n",RED);
        return;
      }
      i++;
    }
    sscanf(token[1],"%x",&tmp_e);
    start = tmp_s;
    end = tmp_e;

    address = end+1;
  }
  else{
    flag=false;
    return;
  }

  if( totalToken ==2 ){
    if( start < 0 || start >= 16*65536){
      flag = false;
      printf("%sBoundaryError : Input value is not in boundary.\n",RED);
      return;
    }
  }
  if( totalToken == 4){
    if( start > end || start < 0 || start >= 65536*16 || end <0 || end >= 65536*16 ){
      flag = false;
      printf("%sBoundaryError : Input value is not in boundary.\n",RED);
      return;
    }
  }
  if( start/16== end/16 ){
    printf("%s%05X ",GREEN,start/16);
    j=0;
    while(j<start%16){
      printf("   ");
      j++;
    }
    j= start%16;
    while(j<=end%16){
      if(mem[i][j] == 0)
        printf("%s%02X ",NORMAL, mem[i][j]);
      else
        printf("%s%02X ",RED, mem[i][j]);
      j++;
    }
    j= end%16 +1 ;
    while(j<16){
      printf("   ");
      j++;

    }
    printf("%s;",NORMAL);
    j=0;
    while(j<16){
      if( mem[i][j] < 0x20 && mem[i][j] > 0x7E){
        printf("%s%c",BLUE,(char)mem[i][j]);
      }
      else
        printf("%s.",NORMAL); 
      j++;
    }
    printf("\n");
  }
  else{
    for(i=start/16; i<=end/16; i++){
      printf("%s%05X ",GREEN,i*16);

      if( i== start/16)
      {
        j=0;
        while(j< start%16){
          printf("   ");
          j++;
        }
        j= start%16;
        while(j<16){
          if(mem[i][j] == 0)
            printf("%s%02X ",NORMAL,mem[i][j]);
          else
            printf("%s%02X ",RED,mem[i][j]);
          j++;
        }
      }
      else if(i == end/16)
      {
        j=0;
        while(j<=end%16){
          if(mem[i][j] == 0)
            printf("%s%02X ",NORMAL,mem[i][j]);
          else
            printf("%s%02X ",RED,mem[i][j]);
          j++;
        }
        j=end%16+1;
        while(j<16){
          printf("   ");
          j++;
        }
      }
      else{
        j=0;
        while(j<16){
          if(mem[i][j] == 0)
            printf("%s%02X ",NORMAL,mem[i][j]);
          else
            printf("%s%02X ",RED,mem[i][j]);
          j++;
        }
      }

      printf("%s;",NORMAL);
      j=0;
      while(j<16){
        if( mem[i][j] >= 0x20 && mem[i][j] <= 0x7E ){
          if((i == start/16 && j<start%16) || (i == end/16 && j > end%16 ) ){
            printf(".");
          }
          else
            printf("%s%c",BLUE,mem[i][j]);
        }
        else{
          printf("%s.",NORMAL); 
        }
        j++;
      }
      printf("\n");
    }
  }

}

/*
 * Print File/Folder in current directory.
 * 
 * If File -> font red with "*" at the last of the file name.
 *
 * If Folder -> font blue with "/" at the last of the folder name.
 */
void printDirectory(){
  char *path="./";
  DIR * d = opendir(path);
  struct dirent * dir;
  if(d==NULL) return; 
  while ((dir = readdir(d)) != NULL) 
  {
    if(dir-> d_type != DT_DIR)
      printf("%s%s*\t",NORMAL, dir->d_name);
    else
      printf("%s%s/\t",BLUE, dir->d_name);
  }
  printf("\n");
  closedir(d);

}

/*
 * Print help in pink color.
 */
void printHelp(){
  printf("%sh[elp]\n", HELP);
  printf("%sd[ir]\n", HELP);
  printf("%sq[uit]\n", HELP);
  printf("%shi[story]\n", HELP);
  printf("%sdu[mp] [start, end]\n", HELP);
  printf("%se[dit] address, value\n", HELP);
  printf("%sf[ill] start, end, value\n", HELP);
  printf("%sreset\n", HELP);
  printf("%sopcode mnemonic\n", HELP);
  printf("%sopcodelist\n", HELP);

}


/*
 * Save user input in linked list for history.
 */
void append(struct Node** head_ref, char *cmd)
{
  struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
  struct Node *last = *head_ref;

  if(new_node == NULL)
    printf("%sMemoryError: Cannot allocate memory\n",RED);

  strcpy(new_node->data, cmd);
  new_node->next = NULL;

  if (*head_ref == NULL){
    *head_ref = new_node;
    return;
  }
  while (last->next != NULL)
    last = last->next;
  last->next = new_node;
  return;
}

/*
 * Print history.
 */
void printList(struct Node *node)
{
  int i = 1;
  while (node != NULL){
    printf("%s%d %s\n",NORMAL, i, node->data);
    node = node->next;
    i++;
  }
}

/*
 * CRITICAL!!
 * This function is to execute process which is like shell (ex: python shell).
 * When this function is processed, shell executes whith infinite while loop.
 *
 */
void executeShellCommand(){
  char s[100];
  char cmd[100];
  char tmp[3][100];
  int count;
  int index;
  int numSpace;
  struct Node* head = NULL;
  while(1){
    printf("%ssicsim> ",NORMAL);
    fgets(s,99,stdin);
    if( s[strlen(s)-1] == '\n'){
      s[strlen(s)-1] = '\0';
    }

    sscanf(s,"%s",cmd);
    totalToken = sscanf(s,"%s %[^,\n]%[,\n] %[^,\n]%[,\n] %[^,\n]%[,\n]",cmd,token[0],tmp[0],token[1],tmp[1],token[2],tmp[2]);
    numSpace=findCharNum(s,' ');
    if(strcmp(cmd,"h")==0 || strcmp(cmd,"help") == 0){
      if(totalToken != 1){
        printf("%s\n",s);
        for(count=0;count<=strlen(cmd);count++){
          printf(" ");
        }
        printf("^\n");
        syntax_error();
      }
      else{
        append(&head, s);
        printHelp();
      }
    }
    else if(strcmp(cmd,"hi")==0 || strcmp(cmd,"history")==0 ){
      if(totalToken != 1){
        printf("%s\n",s);
        for(count=0;count<=strlen(cmd);count++){
          printf(" ");
        }
        printf("^\n");
        syntax_error();
      }
      else{
        append(&head, s);
        printList(head);
      }
    }
    else if(strcmp(cmd,"q")==0 || strcmp(cmd,"quit")==0){
      if( totalToken != 1){
        printf("%s\n",s);
        for(count=0;count<=strlen(cmd);count++){
          printf(" ");
        }
        printf("^\n");
        syntax_error();
      }
      else
        break;
    }
    else if(strcmp(cmd,"d")==0 || strcmp(cmd,"dir")== 0){
      if( totalToken !=1 ){
        printf("%s\n",s);
        for(count=0;count<=strlen(cmd);count++){
          printf(" ");
        }
        printf("^\n");
        syntax_error();
      }
      else{
        append(&head, s);
        printDirectory();
      }
    }
    else if(strncmp(cmd,"du ",3)==0 || strcmp(cmd,"du")==0 || strncmp(cmd,"dump ",5)==0 || strcmp(cmd,"dump")==0){
      if( totalToken == 4){
        if( commaNum(s) > 1){
          index = findLastIndex(s,',');
          printf("%s\n",s);
          for(count=0;count<index;count++){
            printf(" ");
          }
          printf("^\n");
          syntax_error();
        }
        else{ 
          dump();
          if( flag == true )
            continue;
        }
      }
      else{
        dump();
        if(flag==false){
          handle_error();
        }
        else{
          continue;
        }
        flag= true;
      }
    }
    else if(strncmp(cmd,"e ",2)==0 || strcmp(cmd,"e")==0 || strncmp(cmd,"edit ",5)==0 || strcmp(cmd,"edit")==0){
      if( commaNum(s) > 1){
        printf("%sSyntaxError: Invalid syntax.\n",RED);

      }
      else {
        if( totalToken != 4){
          handle_error();
        }
        else{
          edit();
          if( flag == false)
            handle_error();
          else 
            continue;
        }
      }
    }
    else if(strncmp(cmd,"f ",2)==0 || strcmp(cmd,"f")==0 || strncmp(cmd,"fill ",5)==0 || strcmp(cmd,"fill")==0){
      if( commaNum(s) > 2 ){
        printf("%sSyntaxError: Invalid syntax.\n",RED);
      }
      else{
        if( totalToken != 6){
          handle_error();
        }
        else{
          fill();
          if( flag == true)
            continue;
        }
      }
    }
    else if(strcmp(cmd,"reset")==0){
      if(totalToken != 1){
        printf("%s\n",s);
        for(count=0;count<=strlen(cmd);count++){
          printf(" ");
        }
        printf("^\n");


        handle_error();

      }
      else
      {
        append(&head, s);
        reset();
      }
    }
    else if(strcmp(cmd,"opcode")==0){
      if(totalToken == 1){
        printf("%s\n",s);
        for(count=0;count<strlen(s);count++)
          printf(" ");
        printf("^\n");
        printf("%sValueError: Expected more values.\n",RED);
      }
      else if(totalToken != 2){
        index = findLastIndex(s,' ');
        printf("%s\n",s);
        for(count=0;count<index;count++){
          printf(" ");
        }
        printf("^\n");
        syntax_error();
      }
      else if(totalToken>2){
        index = findLastIndex(s,' ');
        printf("%s\n",s);
        for(count=0;count<index;count++){
          printf(" ");
        }
        printf("^\n");
        syntax_error();

      }
      else if(totalToken==2 && numSpace>=2){
        index = findLastIndex(s, ' ');
        printf("%s\n",s);
        for(count=0;count<index;count++){
          printf(" ");
        }
        printf("^\n");
        syntax_error();

      }
      else{
        make_hash_table();
        if(find_opcode(token[0])== 1){
          printf("%sMnemonicError: There is no %s mnemonic.\n",RED,token[0]);
        }
        else{
          continue;
        }

      }
    }
    else if(strcmp(cmd,"opcodelist")==0){
      if(totalToken != 1){
        printf("%s\n",s);
        for(count=0;count<=strlen(cmd);count++){
          printf(" ");
        }
        printf("^\n");
        syntax_error();
      }
      else{
        make_hash_table();
        if( flag == true ){
          append(&head, s);
          print_oplist();
        }
      }
    }
    else{
      if( totalToken < 0 ){
        printf("%sCommandError: Invalid command.\n",RED);
      }
      else{
        printf("%sNameError: Name \'%s\' is not defined.\n",RED,s);
      }
    }
    memset(s,'\0',100);
    memset(cmd,'\0',50);
    flag = true;

  }
}
