#include "tlist.h"
#include <stdio.h>

typedef struct
{
	int n;
	void* l[2];
} IntNode;

void Dump(TLIST* k)
{
    TLIST* n;
    printf("Dump:");
    TLIST_FOREACH(k, n)
        printf("%d ", TLIST_DATA(n, IntNode, l)->n);
    printf("\n");
}

TLIST* Find(TLIST* k, int x)
{
    TLIST* n;
    TLIST_FOREACH(k, n)
    {
        if (x == TLIST_DATA(n, IntNode, l)->n)
            break;
    }
    return n;
}

int Comp(TLIST* x, TLIST* y)
{
    return TLIST_DATA(x, IntNode, l)->n - TLIST_DATA(y, IntNode, l)->n;
}

void TestInsert(void)
{
    IntNode ls[16];
    const int N = (int)(sizeof(ls) / sizeof(ls[0]));
    int i;
    int p;

    for (i = 0; i < N; ++i)
        ls[i].n = i;

    printf("Test insert head\n");
    {
        TLIST_INIT(&ls[0].l);

        p = 0;
        while (++p < N)
            TLIST_INSERT_HEAD(&ls[0].l, &ls[p].l);

        Dump(&ls[0].l);
    }

    printf("Test insert tail\n");
    {
        TLIST_INIT(&ls[0].l);

        p = 0;
        while (++p < N)
            TLIST_INSERT_TAIL(&ls[0].l, &ls[p].l);

        Dump(&ls[0].l);
    }

    printf("\n");
}

void TestRemove(void)
{
    IntNode ls[16];
    const int N = (int)(sizeof(ls) / sizeof(ls[0]));
    int i;
    TLIST* n;

    for (i = 0; i < N; ++i)
        ls[i].n = i;

    printf("Test remove head\n");
    {
        TLIST_INIT(&ls[0].l);
        for (i = 1; i < N; ++i)
            TLIST_INSERT_TAIL(&ls[0].l, &ls[i].l);

        printf("before remove all nodes from head in order\n");

        Dump(&ls[0].l);

        printf("remove all node from head:");

        while (!TLIST_EMPTY(&ls[0].l))
        {
            TLIST_REMOVE_HEAD(&ls[0].l, n);
            printf("%d ", TLIST_DATA(n, IntNode, l)->n);
        }
        printf("\n");

        Dump(&ls[0].l);
    }

    printf("Test remove tail\n");
    {
        TLIST_INIT(&ls[0].l);
        for (i = 1; i < N; ++i)
            TLIST_INSERT_TAIL(&ls[0].l, &ls[i].l);

        printf("before remove all nodes from tail in order\n");

        Dump(&ls[0].l);

        printf("remove all node from tail:");

        while (!TLIST_EMPTY(&ls[0].l))
        {
            TLIST_REMOVE_TAIL(&ls[0].l, n);
            printf("%d ", TLIST_DATA(n, IntNode, l)->n);
        }
        printf("\n");

        Dump(&ls[0].l);
    }

    printf("\n");
}

void TestTransfer(void)
{
    IntNode ls[16];
    const int N = (int)(sizeof(ls) / sizeof(ls[0]));
    int i;
    TLIST *p, *f, *l, *n;

    for (i = 0; i < N; ++i)
        ls[i].n = i;

    printf("Test transfer before\n");
    {
        TLIST_INIT(&ls[0].l);
        for (i = 1; i < N; ++i)
            TLIST_INSERT_TAIL(&ls[0].l, &ls[i].l);

        printf("before insert [5,8) before 1\n");

        Dump(&ls[0].l);

        p = Find(&ls[0].l, 1);
        f = Find(&ls[0].l, 5);
        l = Find(&ls[0].l, 8);

        TLIST_TRANSFER_BEFORE(p, f, l, n);

        printf("after insert [5,8) before 1\n");

        Dump(&ls[0].l);
    }
    
    printf("Test transfer after\n");
    {
        TLIST_INIT(&ls[0].l);
        for (i = 1; i < N; ++i)
            TLIST_INSERT_TAIL(&ls[0].l, &ls[i].l);

        printf("before insert [5,16) after 1\n");

        Dump(&ls[0].l);

        p = Find(&ls[0].l, 1);
        f = Find(&ls[0].l, 5);
        l = Find(&ls[0].l, 16);

        TLIST_TRANSFER_AFTER(p, f, l, n);

        printf("after insert [5,16) after 1\n");

        Dump(&ls[0].l);
    }
    printf("\n");
}

void TestReverse(void)
{
    IntNode ls[16];
    const int N = (int)(sizeof(ls) / sizeof(ls[0]));
    int i;
    TLIST* ta[4];

    TLIST_INIT(&ls[0].l);
    for (i = 0; i < N; ++i)
    {
        ls[i].n = i;
        if (i > 0)
            TLIST_INSERT_TAIL(&ls[0].l, &ls[i].l);
    }

    printf("Test reverse\n");
    printf("before reverse\n");
    Dump(&ls[0].l);

    TLIST_REVERSE(&ls[0].l, ta[0], ta[1], ta[2]);

    printf("after reverse\n");
    Dump(&ls[0].l);

    printf("\n");
}

void TestMerge(void)
{
    IntNode lsa[16];
    const int NA = (int)(sizeof(lsa) / sizeof(lsa[0]));
    IntNode lsb[16];
    const int NB = (int)(sizeof(lsb) / sizeof(lsb[0]));
    int i;
    TLIST* ta[4];

    TLIST_INIT(&lsa[0].l);
    for (i = 0; i < NA; ++i)
    {
        lsa[i].n = i * 2;
        if (i > 0)
            TLIST_INSERT_TAIL(&lsa[0].l, &lsa[i].l);
    }
    TLIST_INIT(&lsb[0].l);
    for (i = 0; i < NB; ++i)
    {
        lsb[i].n = i * 3;
        if (i > 0)
            TLIST_INSERT_TAIL(&lsb[0].l, &lsb[i].l);
    }

    printf("Test merge\n");
    printf("before merge A\n");
    Dump(&lsa[0].l);
    printf("before merge B\n");
    Dump(&lsb[0].l);

    TLIST_MERGE(&lsa[0].l, &lsb[0].l, Comp, ta[0], ta[1], ta[2], ta[3]);

    printf("Test merge\n");
    printf("after merge A\n");
    Dump(&lsa[0].l);
    printf("after merge B\n");
    Dump(&lsb[0].l);

    printf("\n");
}

int main(int argc, char** argv)
{
    TestInsert();
    TestRemove();
    TestTransfer();
    TestReverse();
    TestMerge();

    return 0;
}
