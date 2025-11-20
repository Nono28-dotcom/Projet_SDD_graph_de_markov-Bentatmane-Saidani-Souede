#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "markov.h"

char *getID(int i)
{
    static char buffers[4][10];  // 4 buffers pour éviter l'écrasement
    static int current = 0;
    char *buffer = buffers[current];
    current = (current + 1) % 4;  // Rotation des buffers

    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}