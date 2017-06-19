#include "unit_test_util.h"
#include "SPArrayList.h"
#include <stdbool.h>

#define SIZE 10

static bool spArrayListCreateTest(){
    SPArrayList *al = spArrayListCreate(0);
    ASSERT_FALSE(al);
    al = spArrayListCreate(-5);
    ASSERT_FALSE(al);
    al = spArrayListCreate(SIZE);
    ASSERT_TRUE(al);
    ASSERT_TRUE(spArrayListSize(al) == 0);
    ASSERT_TRUE(spArrayListMaxCapacity(al) == SIZE);
    spArrayListDestroy(al);
    return true;
}

static bool spArrayListCopyTest(){
    ASSERT_FALSE(spArrayListCopy(NULL));

    SPArrayList *al = spArrayListCreate(SIZE);
    SPArrayList *alc = spArrayListCopy(al);
    ASSERT_TRUE(alc && spArrayListSize(alc) == 0 && spArrayListMaxCapacity(alc) == SIZE);
    spArrayListDestroy(alc);

    spArrayListAddFirst(al, 7);
    spArrayListAddLast(al, 5);
    ASSERT_TRUE(alc = spArrayListCopy(al));
    ASSERT_TRUE(spArrayListSize(alc) == 2);
    ASSERT_TRUE(spArrayListMaxCapacity(alc) == SIZE);
    ASSERT_TRUE(spArrayListGetFirst(alc) == 7);
    ASSERT_TRUE(spArrayListGetLast(alc) == 5);

    spArrayListDestroy(al);
    spArrayListDestroy(alc);
    return true;
}

static bool spArrayListClearTest(){
    ASSERT_TRUE(spArrayListClear(NULL) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    SPArrayList *al = spArrayListCreate(SIZE);
    spArrayListAddFirst(al, 1);
    ASSERT_TRUE(spArrayListClear(al) == SP_ARRAY_LIST_SUCCESS);
    ASSERT_TRUE(spArrayListSize(al) == 0);
    ASSERT_TRUE(spArrayListMaxCapacity(al) == SIZE);

    for(int i = 0; i < SIZE; i++){
        spArrayListAddFirst(al, i);
    }
    ASSERT_TRUE(spArrayListClear(al) == SP_ARRAY_LIST_SUCCESS);
    ASSERT_TRUE(spArrayListSize(al) == 0);
    ASSERT_TRUE(spArrayListMaxCapacity(al) == SIZE);

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListAddAtTest(){
    ASSERT_TRUE(spArrayListAddAt(NULL, 0, 0) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    SPArrayList *al = spArrayListCreate(SIZE);
    ASSERT_TRUE(spArrayListAddAt(al, 1, -1) == SP_ARRAY_LIST_INVALID_ARGUMENT);
    ASSERT_TRUE(spArrayListAddAt(al, 1, 1) == SP_ARRAY_LIST_INVALID_ARGUMENT);
    ASSERT_TRUE(spArrayListAddAt(al, 1, SIZE+1) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    ASSERT_TRUE(spArrayListAddAt(al, 2, 0) == SP_ARRAY_LIST_SUCCESS);
    ASSERT_TRUE(spArrayListAddAt(al, 3, 1) == SP_ARRAY_LIST_SUCCESS);
    ASSERT_TRUE(spArrayListAddAt(al, 1, 0) == SP_ARRAY_LIST_SUCCESS);
    ASSERT_TRUE(spArrayListSize(al) == 3);
    for(int i = 0; i < 3; i++){
        ASSERT_TRUE(spArrayListGetAt(al, i) == i+1);
    }
    for(int i = 0; i < SIZE-3; i++){
        spArrayListAddLast(al, 42);
    }
    ASSERT_TRUE(spArrayListAddAt(al, 5, 5) == SP_ARRAY_LIST_FULL);

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListAddFirstTest(){
    ASSERT_TRUE(spArrayListAddFirst(NULL, 0) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    SPArrayList *al = spArrayListCreate(SIZE);
    for(int i = 0; i < SIZE; i++){
        ASSERT_TRUE(spArrayListAddFirst(al, i) == SP_ARRAY_LIST_SUCCESS);
    }
    ASSERT_TRUE(spArrayListAddFirst(al, 9) == SP_ARRAY_LIST_FULL);
    for(int i = 0; i < SIZE; i++){
        ASSERT_TRUE(spArrayListGetAt(al, SIZE-i-1) == i);
    }

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListAddLastTest(){
    ASSERT_TRUE(spArrayListAddLast(NULL, 0) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    SPArrayList *al = spArrayListCreate(SIZE);
    for(int i = 0; i < SIZE; i++){
        ASSERT_TRUE(spArrayListAddLast(al, i) == SP_ARRAY_LIST_SUCCESS);
    }
    ASSERT_TRUE(spArrayListAddLast(al, 9) == SP_ARRAY_LIST_FULL);
    for(int i = 0; i < SIZE; i++){
        ASSERT_TRUE(spArrayListGetAt(al, i) == i);
    }

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListRemoveAtTest(){
    ASSERT_TRUE(spArrayListRemoveAt(NULL, 0) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    SPArrayList *al = spArrayListCreate(SIZE);
    ASSERT_TRUE(spArrayListRemoveAt(al, -1) == SP_ARRAY_LIST_INVALID_ARGUMENT);
    ASSERT_TRUE(spArrayListRemoveAt(al, 1) == SP_ARRAY_LIST_INVALID_ARGUMENT);
    ASSERT_TRUE(spArrayListRemoveAt(al, SIZE+1) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    spArrayListAddFirst(al, 5);
    ASSERT_TRUE(spArrayListRemoveAt(al, 0) == SP_ARRAY_LIST_SUCCESS);
    ASSERT_TRUE(spArrayListSize(al) == 0 && spArrayListMaxCapacity(al) == SIZE);

    for(int i = 0; i < SIZE; i++){
        spArrayListAddFirst(al, 4);
    }
    for(int i = 0; i < SIZE; i++){
        ASSERT_TRUE(spArrayListRemoveAt(al, spArrayListSize(al)-1) == SP_ARRAY_LIST_SUCCESS);
    }
    ASSERT_TRUE(spArrayListSize(al) == 0);
    for(int i = 0; i < SIZE; i++){
        spArrayListAddLast(al, i);
    }
    for(int i = 0; i < 5; i++){
        ASSERT_TRUE(spArrayListRemoveAt(al, 0) == SP_ARRAY_LIST_SUCCESS);
    }
    ASSERT_TRUE(spArrayListSize(al) == SIZE-5);
    for(int i = 5; i < SIZE; i++){
        ASSERT_TRUE(spArrayListGetAt(al, i-5) == i);
    }

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListRemoveFirstTest(){
    ASSERT_TRUE(spArrayListRemoveFirst(NULL) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    SPArrayList *al = spArrayListCreate(SIZE);
    ASSERT_TRUE(spArrayListRemoveFirst(al) == SP_ARRAY_LIST_EMPTY);

    for(int i = 0; i < SIZE; i++){
        spArrayListAddLast(al, i);
    }
    for(int i = 0; i < 5; i++){
        ASSERT_TRUE(spArrayListRemoveFirst(al) == SP_ARRAY_LIST_SUCCESS);
    }
    ASSERT_TRUE(spArrayListSize(al) == SIZE-5);
    for(int i = 5; i < SIZE; i++){
        ASSERT_TRUE(spArrayListGetAt(al, i-5) == i);
    }

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListRemoveLastTest(){
    ASSERT_TRUE(spArrayListRemoveLast(NULL) == SP_ARRAY_LIST_INVALID_ARGUMENT);

    SPArrayList *al = spArrayListCreate(SIZE);
    ASSERT_TRUE(spArrayListRemoveLast(al) == SP_ARRAY_LIST_EMPTY);

    for(int i = 0; i < SIZE; i++){
        spArrayListAddLast(al, i);
    }
    for(int i = 0; i < 5; i++){
        ASSERT_TRUE(spArrayListRemoveLast(al) == SP_ARRAY_LIST_SUCCESS);
    }
    ASSERT_TRUE(spArrayListSize(al) == SIZE-5);
    for(int i = 0; i < SIZE-5; i++){
        ASSERT_TRUE(spArrayListGetAt(al, i) == i);
    }

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListGetAtTest(){
    SPArrayList *al = spArrayListCreate(SIZE);
    spArrayListAddFirst(al, 0);
    ASSERT_TRUE(spArrayListGetAt(al, 0) == 0);

    for(int i = 1; i < SIZE; i++){
        spArrayListAddLast(al, i);
    }
    for(int i = 0; i < SIZE; i++){
        ASSERT_TRUE(spArrayListGetAt(al, i) == i);
    }

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListGetFirstTest(){
    SPArrayList *al = spArrayListCreate(SIZE);
    spArrayListAddFirst(al, 0);
    ASSERT_TRUE(spArrayListGetFirst(al) == 0);
    spArrayListClear(al);

    for(int i = 1; i < SIZE; i++){
        spArrayListAddLast(al, i);
    }
    ASSERT_TRUE(spArrayListGetFirst(al) == 1);

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListGetLastTest(){
    SPArrayList *al = spArrayListCreate(SIZE);
    spArrayListAddLast(al, 0);
    ASSERT_TRUE(spArrayListGetLast(al) == 0);

    for(int i = 1; i < SIZE; i++){
        spArrayListAddLast(al, i);
    }
    ASSERT_TRUE(spArrayListGetLast(al) == SIZE-1);

    spArrayListDestroy(al);
    return true;
}

static bool spArrayListMaxCapacityTest(){
    SPArrayList *al = spArrayListCreate(SIZE);
    ASSERT_TRUE(spArrayListMaxCapacity(al) == SIZE);
    spArrayListDestroy(al);
    return true;
}

static bool spArrayListSizeTest(){
    SPArrayList *al = spArrayListCreate(SIZE);
    ASSERT_TRUE(spArrayListSize(al) == 0);
    spArrayListAddFirst(al, 0);
    ASSERT_TRUE(spArrayListSize(al) == 1);
    for(int i = 0; i < SIZE-1; i++){
        spArrayListAddLast(al, i);
    }
    ASSERT_TRUE(spArrayListSize(al) == SIZE);
    spArrayListDestroy(al);
    return true;
}

static bool spArrayListIsFullTest(){
    SPArrayList *al = spArrayListCreate(SIZE);
    ASSERT_FALSE(spArrayListIsFull(al));
    spArrayListAddFirst(al, 0);
    ASSERT_FALSE(spArrayListIsFull(al));
    for(int i = 0; i < SIZE-1; i++){
        spArrayListAddLast(al, i);
    }
    ASSERT_TRUE(spArrayListIsFull(al));
    spArrayListDestroy(al);
    return true;
}

static bool spArrayListIsEmptyTest(){
    SPArrayList *al = spArrayListCreate(SIZE);
    ASSERT_TRUE(spArrayListIsEmpty(al));
    spArrayListAddFirst(al, 0);
    ASSERT_FALSE(spArrayListIsEmpty(al));
    for(int i = 0; i < SIZE-1; i++){
        spArrayListAddLast(al, i);
    }
    ASSERT_FALSE(spArrayListIsEmpty(al));
    spArrayListDestroy(al);
    return true;
}

int main(){
    RUN_TEST(spArrayListCreateTest);
    RUN_TEST(spArrayListCopyTest);
    RUN_TEST(spArrayListClearTest);
    RUN_TEST(spArrayListAddAtTest);
    RUN_TEST(spArrayListAddFirstTest);
    RUN_TEST(spArrayListAddLastTest);
    RUN_TEST(spArrayListRemoveAtTest);
    RUN_TEST(spArrayListRemoveFirstTest);
    RUN_TEST(spArrayListRemoveLastTest);
    RUN_TEST(spArrayListGetAtTest);
    RUN_TEST(spArrayListGetFirstTest);
    RUN_TEST(spArrayListGetLastTest);
    RUN_TEST(spArrayListMaxCapacityTest);
    RUN_TEST(spArrayListSizeTest);
    RUN_TEST(spArrayListIsFullTest);
    RUN_TEST(spArrayListIsEmptyTest);
    return 0;
}