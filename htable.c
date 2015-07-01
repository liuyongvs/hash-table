/************************************************
 * @aurhor Jacky Lau
 * @data   11/01/2012
 *
 ************************************************/
#include"htable.h"
#include<malloc.h>
#include<string.h>
#include<assert.h>



/********************************************************************************
 *
 *       ______
 *       |size|
 *       |used|
 *       |data|----------->-----
 *                        0 |----|       |----- \
 *                        1 |----|      \|/      \
 *                        2 |----|------->------  \    ---> |----
 *                        3 |----|        |----|   \  /     |----|
 *                        . |----|        |----|    \/      |----|
 *                        . |----|   prev |NULL|    /\----- |----|
 *                        . |----|   next |----|----        |NULL|
 *                    size-1|----|
 *
 *
 *
 **********************************************************************************/
HTABLE *htable_create(int size)
{
    HTABLE *ht = (HTABLE *)malloc(sizeof(HTABLE));
    ht->size =size;
    ht->used = 0;
    ht->data = (HTBALE_INFO **)malloc(size * sizeof(HTBALE_INFO *));
    memset(ht->data, 0, size * sizeof(HTBALE_INFO *));
    return ht;
}

void htable_destroy(HTABLE *ht)
{
    HTBALE_INFO *h;
    HTBALE_INFO *next;
    int i;
    for(i = 0; i < ht->size; i++)
    {
        for(h = ht->data[i]; h; h =next)
        {
            next = h->next;
            free(h->key);
            free(h->value);
            free(h);
        }
    }
    free(ht->data);
    free(ht);
}

unsigned hash_function(const char *s, unsigned size)
{
    unsigned long h = 0;
    unsigned long g;

    /*
     * From the "Dragon" book by Aho, Sethi and Ullman.
     */

    while (*s) {
    h = (h << 4) + *s++;
    if ((g = (h & 0xf0000000)) != 0) {
        h ^= (g >> 24);
        h ^= g;
    }
    }
    return (h % size);
}
//hash function needs to adjust automatically by the hash table size;
//so we need to rehash the key
void htable_growth(HTABLE *ht)
{
    int old_size = ht->size;
    ht->size = 2 * ht->size;
    ht->used = 0;
    HTBALE_INFO ** h = (HTBALE_INFO **)malloc(ht->size *sizeof(HTBALE_INFO *));
    memset(h, 0 , ht->size * sizeof(HTBALE_INFO *));
    HTBALE_INFO **p = ht->data;
    HTBALE_INFO **old_entry = ht->data;
    ht->data =h;

    int i;
    HTBALE_INFO *ptr;
    HTBALE_INFO *next;
    for(i = 0; i < old_size; i++)
    {
        for(ptr = *p++; ptr; ptr = next)
        {
           next = ptr->next;
           htable_link(ht, ptr);
        }

    }
    free(old_entry);
}


void htable_link(HTABLE *ht, HTBALE_INFO *element)
{
    assert(ht);
    assert(element);
    element->next = NULL; //when we are coding, we must set pointer to null
    element->prev = NULL; //especially there are so many malloc and free function
    int index = hash_function(element->key, ht->size);
    if(ht->data[index])
    {
        ht->data[index]->prev = element;
        element->next = ht->data[index];
    }
    ht->data[index] = element;
    ht->used++;
}

void htable_insert(HTABLE *ht, char *key, char *value)
{
    assert(ht);
    assert(key);
    assert(value);
    if(ht->used >= ht->size)
        htable_growth(ht);
    HTBALE_INFO *h = (HTBALE_INFO *)malloc(sizeof(HTBALE_INFO));
    h->key = key;
    h->value = value;
   // h->next = NULL;
   // h->prev =NULL;
    htable_link(ht, h);
}

void htable_delete(HTABLE *ht, const char *key)
{
    HTBALE_INFO *h = htable_locate(ht, key);
    if(h != NULL)
    {
        if(h->prev == NULL)
            ht->data[hash_function(key, ht->size)] = NULL;
        else if(h->next == NULL)
            h->prev->next = NULL;
        else
        {
            h->next->prev = h->prev;
            h->prev->next = h->next;
        }
        free(h->key);
        free(h->value);
        free(h);
        ht->used--;
    }
    else
    {
        printf("do not have this key\n");
    }
}


char *htable_find(HTABLE *ht, const char *key)
{
    int index = hash_function(key, ht->size);
    HTBALE_INFO *h = ht->data[index];
    for(; h && (strcmp(h->key, key) != 0); h = h->next);
    if(h == NULL)
        return NULL;
    return h->value;
}

HTBALE_INFO *htable_locate(HTABLE *ht, const char *key)
{
    int index = hash_function(key, ht->size);
    HTBALE_INFO *h = ht->data[index];
    for(; h && (strcmp(h->key, key) != 0); h = h->next);
    if(h == NULL)
        return NULL;
    return h;
}

HTBALE_INFO **htable_list(HTABLE *ht)
{
    assert(ht);
    HTBALE_INFO **list = (HTBALE_INFO **)malloc(sizeof(*list) *(ht->used +1));
    memset(list, 0 , sizeof(HTBALE_INFO *) *(ht->used +1));
    int i;
    HTBALE_INFO *h;
    int count = 0;
    for(i = 0; i < ht->size; i++)
    {
        for(h = ht->data[i]; h; h = h->next)
        {
            list[count++] = h;
        }
    }
    list[count] = NULL;
    return list;
}
void htable_list_free(HTBALE_INFO **list)
{
    assert(list);
    free(list);
}
