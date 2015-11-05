# List.h

STB-style inspired dynamic array. Builds on Sean Barrett's awesome stb stretchy_buffer adding more functionality.
The idea is to store the count and capacity as two integers in the first two slots of the list memory block
from malloc/realloc, and then return to the user base pointer + 2 for the rest of the list stroage.

See Example.cpp for usage code

Public API:

    ListLast(List)
    ListCount(List)
    ListCapacity(List)
    ListFree(List)
    ListClear(List)
    ListPush(List, Item)
    ListInsert(List, Item, Index)
    ListAdd(List, Amount)
    ListFor(List)
    ListForeach(List)
    ListIndexOf(List, Item, Index, Compare)
    ListIfItemExists(List, Item, Index, Compare)
    ListIfItemDoesntExist(List, Item, Index, Compare)
    ListSort(List, Compare)
    ListInsertAscending(List, Item, Compare)
    ListInsertDescending(List, Item, Compare)
    ListRemoveAt(List, Index)
    ListRemove(List, Item, Compare)
