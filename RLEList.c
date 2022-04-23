#include "RLEList.h"
#include <stdio.h>
#include <stdlib.h>
#define EMPTY '*'
#define ILLEGAL_SIZE -1

typedef struct node_t
{
    char value;
    int length;
    struct node_t* next;
}*Node;

struct RLEList_t{
    //TODO: implement
    Node list;
    int size;
};

RLEList RLEListCreate()
{
    RLEList rleList = malloc(sizeof(*rleList));
    if(rleList == NULL)
    {
        return NULL;
    }
    rleList->list = malloc(sizeof(*rleList->list));
    if(rleList->list == NULL)
    {
        free(rleList);
        return NULL;
    }
    rleList->size = 0;
    rleList->list->value = EMPTY;
    rleList->list->length = 0;
    rleList->list->next = NULL;
    return rleList;
}

void RLEListDestroy(RLEList rleList)
{
    if(rleList == NULL)
    {
        return;
    }
    if(rleList->list == NULL)
    {
        free(rleList);
        return;
    }
    Node tmp = rleList->list;
    while(tmp != NULL)
    {
        Node toDelete = tmp;
        tmp = tmp->next;
        free(toDelete);
    }
    free(rleList);
}

RLEListResult RLEListAppend(RLEList list, char value)
{
    if(list == NULL || list->list == NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }

    //move an indicator to the tail of the node
    Node tmp = list->list;
    while(tmp->next != NULL)
    {
        tmp = tmp->next;
    }

    //if the list is empty(no values were appended to this point)
    if(list->size == 0)
    {
        tmp->value = value;
        tmp->length = 1;
        list->size++;
        return RLE_LIST_SUCCESS;
    }

    //the list has been initialized(values have been appended)
    else
    {
        //if the last value is equal to the parameter
        if(value == tmp->value)
        {
            tmp->length++;
            list->size++;
            return RLE_LIST_SUCCESS;
        }
            //the last value is not equal to the parameter
        else
        {
            tmp->next = malloc(sizeof(*tmp->next));
            if(tmp->next == NULL)
            {
                return RLE_LIST_OUT_OF_MEMORY;
            }
            tmp = tmp->next;
            tmp->value = value;
            tmp->length = 1;
            tmp->next = NULL;
            list->size++;
            return RLE_LIST_SUCCESS;
        }
    }
}

int RLEListSize(RLEList list)
{
    if(list == NULL || list->list == NULL)
    {
        return ILLEGAL_SIZE;
    }
    return list->size;
}

//need to swap names of toDelete and tmp
RLEListResult RLEListRemove(RLEList list, int index)
{
    if(list == NULL || list->list == NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(index >= list->size)
    {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }

    int k = 0;
    Node tmp = list->list;
    while(tmp != NULL)
    {
        k += tmp->length;
        if(k > index)
        {
            break;
        }
        tmp = tmp->next;
    }
    //shouldn't get here
    if(tmp == NULL)
    {
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    //there is more than 1 of the value in a sequence(example: aa)
    //no need to delete a node
    if(tmp->length > 1)
    {
        tmp->length--;
        list->size--;
        return RLE_LIST_SUCCESS;
    }
    //the node has a length of 1, needs to be deleted
    Node toDelete = list->list;
    while(toDelete->next != tmp)
    {
        toDelete = toDelete->next;
    }
    toDelete->next = tmp->next;
    free(tmp);
    list->size--;
    return RLE_LIST_SUCCESS;
}

char RLEListGet(RLEList list, int index, RLEListResult *result)
{
    if(list == NULL || list->list == NULL)
    {
        //printf("hello 2\n");
        if(result != NULL)
        {
            *result = RLE_LIST_NULL_ARGUMENT;
        }
        return 0;
    }
    if(index >= list->size)
    {
        //printf("hello \n3");
        if(result != NULL)
        {
            *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        }
        return 0;
    }
    int k = 0;
    Node tmp = list->list;
    while(tmp != NULL)
    {
        k += tmp->length;
        if(k > index)
        {
            break;
        }
        tmp = tmp->next;
    }
    if(tmp == NULL)
    {
        //printf("hello 4\n");
        if(result != NULL)
        {
            *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        }
        return 0;
    }
    if(result != NULL)
    {
        *result = RLE_LIST_SUCCESS;
    }
    return tmp->value;
}

char* RLEListExportToString(RLEList list, RLEListResult* result)
{
    if(result == NULL)
    {
        return NULL;
    }
    if(list == NULL || list->list == NULL)
    {
        *result = RLE_LIST_NULL_ARGUMENT;
        return NULL;
    }
    char* listString = malloc(sizeof(*listString) * (3 * list->size + 1));
    if(listString == NULL)
    {
        *result = RLE_LIST_OUT_OF_MEMORY;
        return NULL;
    }
    Node tmp = list->list;
    int i = 0;
    while (tmp != NULL)
    {
        listString[i] = tmp->value;
        i++;
        listString[i] = (char)tmp->length;
        i++;
        listString[i] = '\n';
        i++;
        tmp = tmp->next;
    }
    listString[i] = '\0';
    return listString;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function)
{
    if(list == NULL || list->list == NULL || map_function == NULL)
    {
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEList newList = RLEListCreate();
    if(newList == NULL)
    {
        return RLE_LIST_OUT_OF_MEMORY;
    }
    Node tmp = list->list;
    while (tmp != NULL)
    {
        for (int i = 0; i < tmp->length; ++i)
        {
            char newValue = map_function(tmp->value);
            RLEListResult appendResult = RLEListAppend(newList , newValue);
            if(appendResult !=  RLE_LIST_SUCCESS)
            {
                RLEListDestroy(newList);
                return appendResult;
            }
        }
    }
    RLEList toDelete = list;
    list = newList;
    RLEListDestroy(toDelete);
    return RLE_LIST_SUCCESS;
}

//implement the functions here