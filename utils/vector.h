#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct StringVector
{
    char **data;
    size_t size;
    size_t capacity;
} StringVector;

StringVector *createVector();
void pushElement(StringVector *vector, char *element);
char *getElement(StringVector *vector, size_t index);
void modifyElement(StringVector *vector, size_t index, char *element);
void removeElement(StringVector *vector, size_t index);
void freeVector(StringVector *vector);
size_t getSize(StringVector *vector);
bool isEmpty(StringVector *vector);

#endif