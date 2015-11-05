#pragma once

#ifndef LIST_H
#define LIST_H

// STB-style inspired dynamic array. Builds on stb stretchy_buffer adding more functionality
// The idea is to store the count and capacity as two integers in the first two slots of the list memory block
// from malloc/realloc, and then return to the user base pointer + 2 for the rest of the list stroage.

// TODO: a ListAlloc/Create function that allows you to create a new list with a specified capacity?
// TODO: Maybe store a pointer to the comparison function besides the capacity/count when creating a new list?

// Returns the last element of the list
#define ListLast(List) (List[_ListCount(List) - 1])

// Returns how many elements are currently in the list
#define ListCount(List) (List ? _ListCount(List) : 0)

// Returns the current capacity of the list. The list would regrow when count >= capacity
#define ListCapacity(List) (List ? _ListCapacity(List) : 0)

// Frees the the entire memory allocated by the list, including the count and capacity data
#define ListFree(List) ((List) ? free(_ListHeader(List)), 1 : 0)

// Clears (sets to zero) the entire list. Sets count to 0, but keeps capacity untouched. (Does not 'free' the list)
#define ListClear(List) { int Count = ListCount(List); for(int i = 0; i < Count; i++) ListRemoveAt(List, 0); }

// Adds the specified item of type T to the end of the list (regrows the list if necessary)
#define ListPush(List, Item) (_ListCheckGrow(List, 1), List[_ListCount(List)++] = Item)

// Adds 'Amount' of uninitialized items to the list (grows the list by 'Amount') and returns a pointer to the first added element
#define ListAdd(List, Amount) (_ListCheckGrow(List, Amount), _ListCount(List) += Amount, &List[_ListCount(List)-Amount])

// A handy little list for loop macro. Uses 'i' as the indexer name
#define ListFor(List) for(int i = 0; i < ListCount(List); i++)

// Calls ListIndexOf to check if 'Item' exists in 'List', stores the index in 'Index' and inserts an if statement so you could add the follow-up code in brackets
#define ListIfItemExists(List, Item, Index, Compare) ListIndexOf(List, Item, Index, Compare); if (Index != -1)
#define ListIfItemDoesntExist(List, Item, Index, Compare) ListIndexOf(List, Item, Index, Compare); if (Index == -1)

// Uses qsort to sort the list using the speicified comparison function (must take two const void* parameters and return int)
#define ListSort(List, Compare) qsort(List, ListCount(List), sizeof(decltype(*List)), Compare)

// Inserts the speicified item to the list in an ascending/descending order
#define ListInsertAscending(List, Item, Compare) _ListInsertOrdered(List, Item, Compare(it, Item) > 0)
#define ListInsertDescending(List, Item, Compare) _ListInsertOrdered(List, Item, Compare(it, Item) < 0)

// A handy little foreach loop that gives you access to the index (i) and value (it) of the current iterating item
// We take advantage of C++'s 'auto' for type inferance to assign 'it' to the value of the current element
#define ListForeach(List)\
    for(int i = 0, Toggle = 1;\
        Toggle && i < ListCount(List);\
        i++, Toggle = !Toggle)\
    for(auto &it = (List)[i]; Toggle; Toggle = !Toggle)

// Inserts 'Item' at 'Index' in 'List'
#define ListInsert(List, Item, Index) {\
    _ListCheckGrow(List, 1);\
    for(int _LI = ListCount(List); _LI > Index; _LI--)\
        List[_LI] = List[_LI - 1];\
    List[Index] = Item;\
    _ListCount(List)++;\
}

// Stores the index of 'Item' in 'Index' using the speicifed comparison function
// Use it like this: Int Index; ListIndexOf(List, Item, Index, Fn);
// I found no way to write this macro in a way that would let me do: int Index = ListIndexOf(List, Item, Fn);
#define ListIndexOf(List, Item, Index, Compare) { \
    Index = -1;\
    for (int i = 0; i < ListCount(List); i++)\
    {\
        if (Compare(List[i], Item) == 0)\
        {\
            Index = i;\
            break;\
        }\
    }\
}

// Removes the item in the list at 'Index' by setting it to zero
// NOTE this does NOT free the memory of the element because it might not be a pointer,
// it's up to you to do that if you wanted to before removing the item from the list
#define ListRemoveAt(List, Index) { \
    if (List)\
    {\
        for (int i = Index; i < _ListCount(List) - 1; i++)\
            List[i] = List[i + 1];\
        ListLast(List) = { 0 }; \
        _ListCount(List)--;\
    }\
}

// Removes the specified 'Item' from the list if found, using the specified comparison function
#define ListRemove(List, Item, Compare) { \
    int Index;\
    ListIndexOf(List, Item, Index, Compare);\
    if (Index != -1)\
        ListRemoveAt(List, Index);\
}

#define _ListHeader(List) ((int*)(List) - 2)
#define _ListCapacity(List) (_ListHeader(List)[0])
#define _ListCount(List) (_ListHeader(List)[1])
#define _ListCheckGrow(List, Increment) (!(List) || _ListCount(List) + Increment >= _ListCapacity(List) ?\
                                         ((List) = (decltype(List))_ListGrow((List), Increment, sizeof(decltype(*List)))) : 0)

//((List) = (T*)_ListGrow((List), Increment, sizeof(T))) : 0)

static void* _ListGrow(void* List, int Increment, int ItemSize)
{
    int* Result;
    if (List)
    {
        int Need = Increment + _ListCount(List);
        int DoubleCapacity = _ListCapacity(List) * 2;
        int NewCapacity = (Need > DoubleCapacity) ? Need : DoubleCapacity;
        Result = (int*)realloc(_ListHeader(List), ItemSize * NewCapacity + 2 * sizeof(int));
        if (!Result)
        {
            printf("Memory reallocation failed for dynamic array");
            return 0;
        }
        Result[0] = NewCapacity;
    }
    else
    {
        int NewCapacity = Increment < 4 ? 4 : Increment;
        Result = (int*)malloc(ItemSize * NewCapacity + 2 * sizeof(int));
        if (!Result)
        {
            printf("Memory allocation failed for dynamic array");
            return 0;
        }
        Result[0] = NewCapacity;
        Result[1] = 0;
    }
    return Result + 2;
}

#define _ListInsertOrdered(List, Item, Comparison) {\
    int _LIA = 0, Count = ListCount(List);\
    for(; _LIA < Count; _LIA++)\
    {\
        auto it = List[_LIA];\
        if (Comparison)\
        {\
            ListInsert(List, Item, _LIA);\
            break;\
        }\
    }\
    if (_LIA == Count)\
        ListInsert(List, Item, Count);\
}
#endif
