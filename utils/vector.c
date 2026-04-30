#include <stdlib.h>

#include "vector.h"

StringVector *createVector()
{
    StringVector *vector = (StringVector *)malloc(sizeof(StringVector));
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    return vector;
}

void pushElement(StringVector *vector, char *element)
{
    if (vector->data == NULL)
    {
        vector->data = (char **)malloc(sizeof(char *));
        vector->capacity = 1;
    }
    else if (vector->capacity <= vector->size)
    {
        vector->capacity *= 2;
        vector->data = realloc(vector->data, vector->capacity * sizeof(char *));
    }
    vector->data[vector->size] = element;
    vector->size++;
}

char *getElement(StringVector *vector, int index)
{
    if (vector->size <= index)
    {
        return NULL;
    }
    return vector->data[index];
}

void *modifyElement(StringVector *vector, int index, char *element)
{
    if (vector->size > index)
    {
        vector->data[index] = element;
    }
}

void removeElement(StringVector *vector, int index)
{
    if (vector->size > index)
    {
        for (size_t i = index; i < vector->size - 1; i++)
        {
            vector->data[i] = vector->data[i + 1];
        }
        vector->size--;

        if (vector->size < vector->capacity)
        {
            vector->capacity /= 2;
            vector->data = realloc(vector->data, vector->capacity * sizeof(char *));
        }
    }
}

size_t getSize(StringVector *vector)
{
    return vector->size;
}