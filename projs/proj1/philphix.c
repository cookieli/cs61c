
#include "hashtable.h"
#include "philphix.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXLINE 1000

HashTable *dictionary;

int main(int argc, char **argv){
  if(argc != 2){
    /* Note, you can print everything you want to standard error, it will be
       ignored by the autograder */
    fprintf(stderr, "Specify a dictionary\n");
    return 0;
  }
  fprintf(stderr, "Creating hashtable\n");
  dictionary = createHashTable(2255, &stringHash, &stringEquals);
  fprintf(stderr, "Loading dictionary %s\n", argv[1]);
  readDictionary(argv[1]);
  //fprintf(stdout, "ipsum: %s\n", (char *)findData(dictionary, "fubar"));
  fprintf(stderr, "Dictionary loaded\n");
  fprintf(stderr, "Processing stdin\n");
  processInput();
  return 0;
}


/* Define a hash function */
unsigned int stringHash(void *s){
  char *str = (char *) s;
  /* Printing string is to prevent a compiler warning until you
     actually implement this function */
  //fprintf(stderr, "need to implement stringHash%s\n", string);
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)){
      hash = ((hash << 5) + hash ) + c;
  }

  /* To suppress compiler warning until you implement this function, */
  return hash;
}


int stringEquals(void *s1, void *s2){
    //fprintf(stderr, "You need to implement stringEquals");
  /* To suppress compiler warning until you implement this function */
    char *str1 = (char *)s1;
    char *str2 = (char *)s2;
    while (*str1++ == *str2++){
        if(*str1 == '\0' && *str2 == '\0')
                return 1;
    }
  return 0;
}

void readDictionary(char *name){
  FILE *inputFile = fopen(name, "r");

   /* Printing the address of inputFile is to suppress compiler warning
     until you implement this function */
  //fprintf(stderr, "You need to implement readDictionary %x\n", (unsigned int) inputFile);
  char line[MAXLINE];
  //char *key;
  //char *value;
  char *pattern = " \t";
  while(fgets(line, MAXLINE, inputFile) != NULL){
      char *key = (char *)malloc(sizeof(char) * MAXLINE);
      char *value = (char *)malloc(sizeof(char) * MAXLINE);
      key =strcpy(key,strtok(line, pattern));
      value = strcpy(value,strtok(NULL, pattern));
      for(char *temp = value; (*temp) != '\0';  temp++){
          if(*temp == '\n'){
              *temp = '\0';
              break;
          }
      }
      insertData(dictionary, key, value);
  }
}


void processInput(){
    // fprintf(stderr, "You need to implement processInput\n");
    char **result;
    char line[MAXLINE];
    char *pattern = " ";
    int i;
    while(fgets(line, MAXLINE, stdin) != NULL){
        for(i = 0; i < MAXLINE; i++){
            if(line[i] == '\n'){
                line[i] = '\0';
                break;
            }
        }
        result = (char **)malloc(sizeof(char *) * MAXLINE);
        size_t idx = 0;
        char *token = strtok(line, pattern);
        while (token){
            *(result + idx++) = strdup(token);
            *(result + idx++) = strdup(pattern);
            token = strtok(NULL, pattern);
        }
        *(result + idx) = 0;
        for(i = 0; *(result + i); i++){
            char *temp ;
            if((temp = FindData(dictionary, *(result + i))) || (temp = FindData(dictionary, AllButFirstConvertToLowerCase(*(result + i)))) || (temp = FindData(dictionary, lower(*(result + i))))){
                printf("%s", temp);
            } else {
                printf("%s", *(result+i));
            }
            free(*(result+i));
        }
        free(result);
        printf("\n");
    }
}

char* AllButFirstConvertToLowerCase(char *ss){
    char *s = (char *)malloc(sizeof(char) * (strlen(ss) + 1));
    int i;
    s[0] = ss[0];
    for(i = 1; ss[i] != '\0'; i++){
        if(ss[i] <= 'Z' && ss[i] >= 'A'){
            s[i]  = ss[i] + 'a' - 'A';
        }else {
            s[i] = ss[i];
        }
    }
    s[i] = '\0';
    return s;
}
char* lower(char *ss){
    char *s = (char *)malloc(sizeof(char) * (strlen(ss) + 1));
    int i;
    for(i = 0; ss[i] != '\0'; i++){
        if(ss[i] <= 'Z' && ss[i] >= 'A'){
            s[i] =ss[i] + 'a' - 'A';
        }else{
            s[i] =ss[i];
        }
    }
    s[i] = '\0';
    return s;
}

int containsLineBreaks(char *ss){
    int i;
    for(i = 0; *(ss + i); i++){
        if((*(ss + i)) == '\n' && (*(ss + i + 1)) == '\0'){
            return 1;
        }
    }
    return 0;
}
int isAlphabet(char c){
    if((c < 'z' && c > 'a') || (c < 'Z' && c > 'A'))  return 1;
    return 0;
}
void *FindData(HashTable *table, char *key){
    char *lastOfKey = &key[strlen(key)-1];
    char *temp = strdup(key);
    if(!isAlphabet(*lastOfKey)){
        temp[strlen(temp)-1] = '\0';
        if(findData(table, temp)){
            return strcat((char *)findData(table, temp), lastOfKey);
        }
        return NULL;
    }
    return findData(table, key);
}
