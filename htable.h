#ifndef __HTABLE_H
#define __HTABLE_H
/************************************************
 * @aurhor Jacky Lau
 * @data   11/01/2012
 *
 ************************************************/


/*===================STRUCT==================================*/
typedef struct HTBALE_INFO {
    char *key;
    char *value;
    struct HTBALE_INFO *prev;
    struct HTBALE_INFO *next;
}HTBALE_INFO;

typedef struct HTBALE {
    int size;
    int used;
    struct HTBALE_INFO **data;
}HTABLE;


/*========================FUNCTION================================*/

HTABLE *htable_create(int size);
void htable_destroy(HTABLE *ht);
unsigned hash_function(const char *s, unsigned size);
void htable_growth(HTABLE *ht);
void htable_link(HTABLE *ht, HTBALE_INFO *element);
void htable_insert(HTABLE *ht, char *key, char *value);
void htable_delete(HTABLE *ht, const char *key);
char *htable_find(HTABLE *ht, const char *key);
HTBALE_INFO *htable_locate(HTABLE *ht, const char *key);
void htable_list_free(HTBALE_INFO **list);
HTBALE_INFO **htable_list(HTABLE *ht);

#endif
