#include<stdio.h>
#include<string.h>
#include"hash_table.h"

// create a hash table
hash_t table[HASH_TABLE_SIZE];

//initialise table 
void hash_init()
{
    for(int i=0; i<HASH_TABLE_SIZE; i++)
    {
         table[i].key[0] = '\0';
         table[i].value = 0;
    }
}

// define hash function to map key and value
int hash(char *key)
{
    //map strings to array indices
    // function adds all ASCII characters in string and maps that value to a valid index
    int val=0;
    while(*key)
    {
        val+=*key;
        key++;
    } 
    int index = val % HASH_TABLE_SIZE;

    return index;
}
/*
fn: insert an item into the list
*/
void hash_insert(char *key, int value)
{
    // find the index using hash function and store key and value at that index of table
    int index = hash(key);
    strcpy(table[index].key, key);
    table[index].value = value;
}
/*
 fn: print hash table
*/
void hash_print()
{
    printf("\n          HASH TABLE          \n");
    printf("________________________________\n");
    printf("\nindex      key         value\n");
    //print only if item is present
    for(int i=0;i<HASH_TABLE_SIZE;i++)
    {
        
        printf("\n");
        if(table[i].key[0] != '\0')
        {
            printf("\n");
            printf("%d         %s             %d",i,table[i].key,table[i].value);
        }
        else{
            printf("\n          EMPTY");
        }
       
    }
    
}
/*
 fn: search an item if it's present
*/
int hash_search(char *key)
{
    //find the index of key and search if key is present at that index
    int index = hash(key);
    if(strcmp(table[index].key, key) == 0)
    {
        //key is present in the table
        return 1;
    }
        return 0;
}
