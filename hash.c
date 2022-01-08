/*
Name: Usman Zaheer
Class: CIS 2520 (Dr. Stefan Kremer)
This program should sufficently be able to do all assignment functionality
*/

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

struct HashTable *createTable(struct memsys *memsys, unsigned int capacity, unsigned int width, int(*hash)(void *, int), int(*compar)(const void *, const void *)) {
    struct HashTable *table = malloc(sizeof(struct HashTable));
    int mem = MEMNULL;

    if(table == NULL) {
        fprintf(stderr, "Malloc failed\n");
        exit(0);
    }

    table->capacity = capacity;
    table->width = width;
    table->hash = hash;
    table->compar = compar;
    table->nel = 0;
    table->data = memmalloc(memsys, sizeof((sizeof(int) * capacity)));

    if(table->data == MEMNULL) {
        fprintf(stderr, "Memmalloc failed\n");
        exit(0);
    }
    for (int i = 0; i < capacity; i++) {
        setval(memsys, &mem, sizeof(int), (table->data + (i * sizeof(int))));
    }
    return table;
}

void addElement(struct memsys *memsys, struct HashTable *table, int addr) {
    int hashIndex = 0, currAddress = 0, arrayAddress = 0;
    void *data = malloc(table->width);

    if(table->nel == table->capacity) {
        fprintf(stderr, "No avalible space\n");
        free(data);
        exit(0);
    }

    getval(memsys, data, table->width, addr);
    hashIndex = (table->hash)(data, table->capacity);
    while(arrayAddress != MEMNULL) {
        if(hashIndex == table->capacity) {
            hashIndex = 0;
        }
        currAddress = table->data + (hashIndex * sizeof(int));
        getval(memsys, &arrayAddress, sizeof(int), currAddress);
        hashIndex++;    
    }
    setval(memsys, &addr, sizeof(int), currAddress);
    table->nel++;
    free(data);
}

int getElement(struct memsys *memsys, struct HashTable *table, void *key) {
    int hashIndex = 0, currAddress = 0, origin = 0, arrayAddress = 0;
    void *data = malloc(table->width);

    hashIndex = (table->hash)(key, table->capacity);
    origin = hashIndex;
    while(1) {
        if(hashIndex == table->capacity) {
            hashIndex = 0;
        }
        currAddress = table->data + (hashIndex * sizeof(int));
        getval(memsys, &arrayAddress, sizeof(int), currAddress);
        if(arrayAddress == MEMNULL) {
            free(data);
            return MEMNULL;
        }
        getval(memsys, data, table->width, arrayAddress);
        if((table->compar)(data, key) == 0) {
            free(data);
            return arrayAddress;
        }
        hashIndex++;
        if(hashIndex == origin) {
            break;
        }
    }
    free(data);
    return MEMNULL;
}

void freeTable(struct memsys *memsys, struct HashTable *table) {
    memfree(memsys, table->data);
    free(table);
}

int hashAccuracy(struct memsys *memsys, struct HashTable *table) {
    int currAddress = 0, arrayAddress = 0, sumDiff = 0, hashIndex = 0;
    void *data = malloc(table->width);

    for(int i = 0; i < table->capacity; i++) {
        currAddress = table->data + (i * sizeof(int));
        getval(memsys, &arrayAddress, sizeof(int), currAddress);    
        if(arrayAddress != MEMNULL) {
            getval(memsys, data, table->width, arrayAddress);
            hashIndex = (table->hash)(data, table->capacity);
            if(i < hashIndex) {
                sumDiff += i + hashIndex - table->capacity;
            }
            if(i >= hashIndex) {
                sumDiff += i - hashIndex;
            }
        }
    }
    return sumDiff;
}
