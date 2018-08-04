#include <stdio.h>
#include <string.h>
#define MAXLINE 1000
int main(int argc, char **argv){
    FILE *inputFile = fopen(argv[1], "r");
    char line[MAXLINE];
    char *pattern = "(";
    while(fgets(line, MAXLINE, inputFile) != NULL){
        char *token = strtok(line, pattern);
        //printf("%s\n", token);
        while(token){
            printf("%s\n", token);
            token = strtok(NULL, pattern);
        }
    }
}