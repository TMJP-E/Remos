#include <stdlib.h>
#include <string.h>

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
    int length = strlen(element) + 1;
    if (vector->data == NULL)
    {
        vector->capacity = 1;
        vector->data = (char **)malloc(vector->capacity * sizeof(char *));
    }
    else if (vector->capacity <= vector->size)
    {
        vector->capacity *= 2;
        vector->data = (char **)realloc(vector->data, vector->capacity * sizeof(char *));
    }

    vector->data[vector->size] = (char *)calloc(sizeof(char), length + 1);
    memset(vector->data[vector->size], 0, length + 1);
    strcpy(vector->data[vector->size], element);
    vector->size++;
}

char *getElement(StringVector *vector, size_t index)
{
    if (vector->size <= index)
    {
        return NULL;
    }
    return strdup(vector->data[index]);
}

void modifyElement(StringVector *vector, size_t index, char *element)
{
    if (vector->size > index)
    {
        strcpy(vector->data[index], element);
    }
}

void removeElement(StringVector *vector, size_t index)
{
    if (vector->size > index)
    {
        free(vector->data[index]);
        for (size_t i = index; i < vector->size - 1; i++)
        {
            vector->data[i] = vector->data[i + 1];
        }
        vector->size--;

        if (vector->size < vector->capacity / 2)
        {
            vector->capacity /= 2;
            vector->data = realloc(vector->data, vector->capacity * sizeof(char *));
        }
    }
}

void freeVector(StringVector *vector)
{
    for (size_t i = 0; i < vector->capacity; i++)
    {
        free(vector->data[i]);
    }
    free(vector->data);
    free(vector);
}

size_t getSize(StringVector *vector)
{
    return vector->size;
}