/*
 * LRU Cache Properties
 *
 * 1. store key-value pairs
 * 2. get and put is O(1) on average
 * 3. fixed capacity
 * 4. if full, evict the least recently used pair
 *
 * property 1 and 2 means i need to have an 'hash table'
 * property 3 and 4 means i need to keep track of 'last used time'
 *
 * the challenges:
 * a. implement a hash table that also handles collisions
 * b. implement a linked list
 * c. do it fast (took 2-3 hours)
 */

#include <stdlib.h>

typedef struct kv_pair {
        unsigned int key;
        unsigned int value;

        struct kv_pair* table_next;
        struct kv_pair* table_prev;

        struct kv_pair* list_next;
        struct kv_pair* list_prev;
} kv_pair;

typedef struct {
        kv_pair** table;
        size_t table_size;
        size_t table_capacity;

        kv_pair* head;
        kv_pair* tail;
} LRUCache;

LRUCache* lRUCacheCreate(int capacity)
{
        LRUCache* obj = (LRUCache*)malloc(sizeof(LRUCache));

        obj->table = (kv_pair**)calloc(capacity, sizeof(kv_pair*));
        obj->table_size = 0;
        obj->table_capacity = (size_t)capacity;
        obj->head = NULL;
        obj->tail = NULL;

        return obj;
}

void maintain_lru_list(LRUCache* obj, kv_pair* last_used)
{
        if (obj->head == NULL || obj->tail == NULL) {
                obj->head = last_used;
                obj->tail = last_used;

                last_used->list_next = NULL;
                last_used->list_prev = NULL;
                return;
        }

        // do nothing on on tail
        if (obj->tail == last_used) {
                return;
        }

        if (obj->head == last_used) {
                // move the head to next
                if (last_used->list_next) {
                obj->head = last_used->list_next;
                }
                obj->head->list_prev = NULL;
        } else if (last_used->list_prev) {
                // take out the 'last_used'
                last_used->list_prev->list_next = last_used->list_next;
                if (last_used->list_next) {
                last_used->list_next->list_prev = last_used->list_prev;
                }
        }

        // move 'last_used' to tail
        obj->tail->list_next = last_used;
        last_used->list_prev = obj->tail;
        obj->tail = last_used;
        obj->tail->list_next = NULL;
}

int lRUCacheGet(LRUCache* obj, int key)
{
    size_t bucket = key % obj->table_capacity;

    kv_pair* iter = obj->table[bucket];
    while (iter) {
        if (iter->key == key) {
            maintain_lru_list(obj, iter);
            return iter->value;
        }

        iter = iter->table_next;
    }

    return -1;
}

void lRUCachePut(LRUCache* obj, int key, int value)
{
        size_t bucket = key % obj->table_capacity;

        // find it
        kv_pair* last_used = NULL;

        kv_pair* iter = obj->table[bucket];
        while (iter) {
                if (iter->key == key) {
                break;
                }

                iter = iter->table_next;
        }

        // already exists
        if (iter != NULL) {
                iter->value = value;

                last_used = iter;
                maintain_lru_list(obj, last_used);
                return;
        }
        
        if (obj->table_size == obj->table_capacity) {
                // if table is full, evict the LRU
                kv_pair* lru = obj->head;
                size_t lru_bucket = lru->key % obj->table_capacity;

                // update the list
                obj->head = obj->head->list_next;
                if (obj->head) {
                obj->head->list_prev = NULL;
                }

                // update the table
                if (obj->table[lru_bucket] == lru) {
                obj->table[lru_bucket] = lru->table_next;
                } else if (lru->table_prev) {
                lru->table_prev->table_next = lru->table_next;
                }

                if (lru->table_next) {
                lru->table_next->table_prev = lru->table_prev;
                }

                obj->table_size--;
                free(lru);
        }

        // add new pair
        kv_pair* new_pair = (kv_pair*)malloc(sizeof(kv_pair));

        new_pair->key = key;
        new_pair->value = value;
        new_pair->table_next = NULL;
        new_pair->table_prev = NULL;
        new_pair->list_next = NULL;
        new_pair->list_prev = NULL;

        obj->table_size++;

        if (obj->table[bucket]) {
                new_pair->table_next = obj->table[bucket];
                obj->table[bucket]->table_prev = new_pair;
        }

        obj->table[bucket] = new_pair;

        last_used = new_pair;

        maintain_lru_list(obj, last_used);
}

void lRUCacheFree(LRUCache* obj) {
        // free all pairs
        kv_pair* iter = obj->head;
        while (iter) {
                kv_pair* tmp = iter;
                iter = iter->list_next;

                free(tmp);
        }

        free(obj->table);
        free(obj);
}

/**
 * Your LRUCache struct will be instantiated and called as such:
 * LRUCache* obj = lRUCacheCreate(capacity);
 * int param_1 = lRUCacheGet(obj, key);

 * lRUCachePut(obj, key, value);

 * lRUCacheFree(obj);
*/