
#include "AsciiArtTool.h"
#include <stdio.h>
#include <stdlib.h>

RLEList asciiArtRead(FILE* in_stream)
{
    if(in_stream == NULL)
    {
        return NULL;
    }
    RLEList list =  RLEListCreate();
    if(list == NULL)
    {
        return NULL;
    }
    char currentValue;

    do {
        currentValue = fgetc(in_stream);
        if(RLEListAppend(list , currentValue) != RLE_LIST_SUCCESS)
        {
            RLEListDestroy(list);
            return NULL;
        }
    }while (currentValue != EOF);
    return list;
}

RLEListResult asciiArtPrint(RLEList list, FILE *out_stream)
{
    if(list == NULL || out_stream == NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    int size = RLEListSize(list);
    for (int i = 0; i < size; ++i)
    {
        RLEListResult result = RLE_LIST_SUCCESS;
        RLEListResult* ptr = &result;
        char currentValue = RLEListGet(list , i , ptr);
        if((*ptr) != RLE_LIST_SUCCESS)
        {
            return result;
        }
        char* ptr2 = &currentValue;
        fprintf(out_stream , ptr2);
    }
    return RLE_LIST_SUCCESS;
}

//need to erase ptr and put &result in the function call
RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream)
{
    if(list == NULL || out_stream == NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEListResult result = RLE_LIST_SUCCESS;
    RLEListResult* ptr = &result;
    char* listString = RLEListExportToString(list , ptr);
    if(listString == NULL)
    {
        return RLE_LIST_OUT_OF_MEMORY;
    }
    if(*ptr != RLE_LIST_SUCCESS)
    {
        free(listString);
        return result;
    }
    fprintf(out_stream , listString);
    return RLE_LIST_SUCCESS;
}
