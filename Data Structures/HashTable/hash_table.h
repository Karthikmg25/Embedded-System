

#include<string.h>

// define size for hash table
#define HASH_TABLE_SIZE           10 
// table stores 10 key-value pairs(items)

// structure defined for an item
typedef struct 
{
    char key[30];
    int value;
   
}hash_t;

//declare the global hash table created in .c file
extern hash_t table[HASH_TABLE_SIZE];

//APIs used

/*
fn: initialise table 
*/
void hash_init();

/*
fn: insert an item into the list
*/
void hash_insert(char *key, int value);
/*
 fn: print hash table
*/
void hash_print();
/*
 fn: search an item if it's present
*/
int hash_search(char *key);

