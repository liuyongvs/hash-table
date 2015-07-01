/************************************************
 * @aurhor Jacky Lau
 * @data   11/01/2012
 *
 ************************************************/

#include"htable.h"
#include<malloc.h>
#include<stdio.h>

void test_htable()
{
    HTABLE * ht = htable_create(10);
   // HTBALE_INFO *h = (HTBALE_INFO *)malloc(sizeof(HTBALE_INFO));
    int i;
    for(i = 0; i < 100; i++)
    {
        char *key = (char *)malloc(16);
        snprintf(key, 16, "key is %d", i);
        char *value = (char *)malloc(16);
        snprintf(value, 16, "value is %d",i);
        htable_insert(ht, key ,value);

        //printf("hash_function(%s): %d\n", key, hash_function(key, ht->size));
    }



    char *search_key = (char *)malloc(16);
//    for(i = 0; i < 20; i++)
//    {
//        snprintf(search_key, 32, "key is %d", i);
//        if(htable_find(ht, search_key) != NULL)
//            printf("%s\n", htable_find(ht, search_key));
//    }

    for(i = 0; i < 10; i++)
    {
        snprintf(search_key, 16, "key is %d", i);
        htable_delete(ht, search_key);
    }
    free(search_key);

    HTBALE_INFO **list = htable_list(ht);
    for(i = 0; i < ht->used; i++)
        printf("%s %s\n", list[i]->key, list[i]->value);
    htable_list_free(list);
    printf("table size %d, used %d\n", ht->size, ht->used);
    htable_destroy(ht);

}

int main()
{
    test_htable();
    return 1;
}
