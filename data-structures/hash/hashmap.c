#include <stdio.h>
#include <stdbool.h>

#include "hashmap.h"
#include "hash.h"
#include "datum.h"

#define key_matches_bucket(k,b) \
    k->size == b->key->size && memcmp(k->data, b->key->data, b->key->size) == 0 

struct bucket * init_bucket(){
    struct bucket * b = (struct bucket *)malloc(sizeof(struct bucket));
    b->val  = 0;
    b->key  = 0;
    b->next = 0;
    return(b);
}

void free_bucket(struct bucket * b){
    if(b){
        if(b->next)        
            free_bucket(b->next);
        free(b);
    }
}

struct hashmap * init_hash(){
    struct hashmap * map = (struct hashmap *)malloc(sizeof(struct hashmap));
    map->index_size = HASH_SIZE;
    map->size = 0;
    map->table = (struct bucket **)malloc(map->index_size * sizeof(struct bucket *));
    // This is essential: checks for missing keys require NULL if unset
    for(size_t i = 0; i < map->index_size; i++)
        map->table[i] = NULL;
    return map;
}

void free_hashmap(struct hashmap * map){
    if(map){
        if(map->table){
            for(int i = 0; i < map->index_size; i++){
                if(map->table[i])
                    free_bucket(map->table[i]);
            }
            free(map->table);
        }
        free(map);
    }
}

void add(struct datum * key, struct datum * data, struct hashmap * map){
    size_t index = hash(key); 
    if(index < map->index_size){
        struct bucket * b = init_bucket(); 
        b->val = data;
        b->key = key;
        b->next = map->table[index];
        map->table[index] = b;
        map->size++;
    } else {
        fprintf(stderr, "Hash key (%lu) is out of bounds\n", index);
        exit(EXIT_FAILURE);
    }
}

struct datum * get(struct datum * key, struct hashmap * map){
    size_t index = hash(key); 
    if(index < map->index_size){
        struct bucket * b = map->table[index];
        while(true){
            if(!b)
                return NULL;      
            if(key_matches_bucket(key, b)){
                return b->val; 
            }
            b = b->next;
        }
    } else {
        fprintf(stderr, "Something is wrong with the hashing algorithm");
        exit(EXIT_FAILURE);
    }
}

void del(struct datum * key, struct hashmap * map){
    size_t index = hash(key); 
    if(index < map->index_size){
        struct bucket * prev_bucket = NULL;
        struct bucket * this_bucket = map->table[index];
        while(true){
            if(!this_bucket)
                break;
            if(key_matches_bucket(key, this_bucket)){
                if(prev_bucket){
                    prev_bucket->next = this_bucket->next;
                } else {
                    map->table[index] = NULL;
                }
                free(this_bucket);
                map->size--;
                break;
            }
            prev_bucket = this_bucket;
            this_bucket = this_bucket->next;
        }
    }
}

void dump(struct hashmap * map){
    for(int i = 0; i < map->index_size; i++){
        struct bucket * b = map->table[i];
        if(b){
            printf("%d ", i);
            while(b){
                printf(".");
                b = b->next;
            }
            printf("\n");
        }
    }
}
