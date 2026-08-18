
#include "hash_table.h"

int main()
{
    //initialise table
    hash_init();

    //Inser items
    hash_insert("karthik", 27);
    hash_insert("Sreelatha", 54);
    hash_insert("karthika", 31);
    hash_insert("Advik", 3);
    hash_insert("Vignesh", 37);
    hash_insert("Sidhisree", 1);

    //print items
    hash_print();

    // search for an item
    if(hash_search("Vignesh"))
    {
        printf("\nKey is present");
    }
    else{
        printf("\nKey is absent");
    }
}