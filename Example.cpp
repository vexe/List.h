
//
// Example program
//

#include "DynamicArray.h"
#include <cassert>

// A functor macro, lets us define local functions and be able to pass them as function pointers arguments to other functions
// technically for a functor, overloading the () operator to call the function is enough
// but we need another conversion operator to implicity convert the functor to the right function pointer type so that we can pass it around
#define FUNC(ReturnType, Name, Params, Body) \
    struct { \
        ReturnType operator() Params Body \
        typedef ReturnType(*Name##fun_t) Params; \
        static ReturnType _fun_ Params Body \
        operator Name##fun_t() { return _fun_; } \
    } Name \

int main()
{
    int* Nums = 0;
    ListPush(Nums, int, 12);
    ListPush(Nums, int, 34);
    ListPush(Nums, int, 56);
    ListPush(Nums, int, 78);

    assert(ListCount(Nums) == 4);

    ListInsert(Nums, int, 4, 0);
    ListInsert(Nums, int, 8, ListCount(Nums));
    ListInsert(Nums, int, 16, 3);

    assert(ListCount(Nums) == 7);

    FUNC(int, CompareInt, (int x, int y), { return x - y; });
    ListInsertAscending(Nums, int, 45, CompareInt);
    ListInsertDescending(Nums, int, 33, CompareInt);

    ListRemove(Nums, 56, CompareInt);
    ListRemove(Nums, 100, CompareInt);
    ListRemoveAt(Nums, 3);

    assert(ListCount(Nums) == 7);

    FUNC(int, CompareSort, (const void* x, const void* y), { return *(int*)x - *(int*)y; });
    ListSort(Nums, int, CompareSort);

    int Index;
    ListIndexOf(Nums, 33, Index, CompareInt);

    ListIfItemExists(Nums, 45, Index, CompareInt)
        printf("34 exists");

    ListIfItemDoesntExist(Nums, 4, Index, CompareInt)
        assert(0);

    ListFor(Nums)
        printf("FOR Item[%d] = %d;\n", i, Nums[i]);

    ListForeach(Nums)
        printf("FOREACH it = %d, index = %d", it, i);

    ListClear(Nums);

    assert(ListCount(Nums) == 0);

    ListFree(Nums);

    return 0;
}