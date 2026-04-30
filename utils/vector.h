#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct StringVector
{
    char **data;
    size_t size;
    size_t capacity;
} StringVector;

void pushElement(StringVector *vector, char *element);
char *getElement(StringVector *vector, int index);
void *modifyElement(StringVector *vector, int index, char *element);
void removeElement(StringVector *vector, int index);
size_t getSize(StringVector *vector);


#endif