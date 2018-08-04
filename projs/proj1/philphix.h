#ifndef _PHILPHIX_H
#define _PHILPHIX_H

/* The pointer to store the dictionary for replacement entries */
extern struct HashTable *dictionary;


extern unsigned int stringHash(void *string);

extern int stringEquals(void *s1, void *s2);

extern void readDictionary(char *dictName);

extern void processInput();
extern int containsLineBreaks(char *ss);
extern char* AllButFirstConvertToLowerCase(char *ss);
extern char* lower(char *ss);
extern int isAlphabet(char c);
extern void *FindData(HashTable *table, char *key);

#endif
