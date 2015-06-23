/*
 * Copyright 2015 TaberWalker <taberwalker@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __TLIST_H__
#define __TLIST_H__

typedef void* TLIST[2];

#define TLIST_PREV(p)       (*(TLIST**)&((*(p))[0]))
#define TLIST_NEXT(p)       (*(TLIST**)&((*(p))[1]))
#define TLIST_PREV_NEXT(p)  (TLIST_NEXT(TLIST_PREV(p)))
#define TLIST_NEXT_PREV(p)  (TLIST_PREV(TLIST_NEXT(p)))
#define TLIST_RNEXT(p)      TLIST_PREV(p)

/* init list k */
#define TLIST_INIT(k)       do { TLIST_PREV(k) = TLIST_NEXT(k) = (k); } while(0)
/* return true if list k is empty */
#define TLIST_EMPTY(k)      ((const TLIST *)(k) == (const TLIST *)TLIST_NEXT(k))
/* get the first node of list k */
#define TLIST_BEGIN(k)      (TLIST_NEXT(k))
/* get the next node of list k's tail */
#define TLIST_END(k)        (k)
/* get the reverse first node of list k */
#define TLIST_RBEGIN(k)     (TLIST_PREV(k))
/* get the reverse next node of list k's tail */
#define TLIST_REND(k)       (k)

/* traversal list k by node n */
#define TLIST_FOREACH(k, n)	                            		\
    for ((n) = TLIST_BEGIN(k); (n) != TLIST_END(k); (n) = TLIST_NEXT(n))

/* traversal list k by node n in reverse order */
#define TLIST_REVERSE_FOREACH(k, n)                     		\
    for ((n) = TLIST_RBEGIN(k); (n) != TLIST_REND(k); (n) = TLIST_RNEXT(n))

/* get the pointer to 'type' by the pointer 'ptr' to 'field' in 'type' */
#define TLIST_DATA(ptr, type, field)                    		\
    ((type *)((char *)(ptr) - ((char *)&(((type *)0)->field))))

/* insert node n before node p */
#define TLIST_INSERT_BEFORE(p, n)                       		\
    do                                                  		\
    {                                                   		\
        TLIST_PREV_NEXT(p) = (n);                       		\
        TLIST_PREV(n) = TLIST_PREV(p);                  		\
        TLIST_NEXT(n) = (p);                            		\
        TLIST_PREV(p) = (n);                            		\
    } while (0)

/* insert node n in the tail of list k */
#define TLIST_INSERT_TAIL(k, n)                         		\
    TLIST_INSERT_BEFORE((k), (n))

/* insert node n after node p */
#define TLIST_INSERT_AFTER(p, n)                        		\
    do                                                  		\
    {                                                   		\
        TLIST_NEXT(n) = TLIST_NEXT(p);                  		\
        TLIST_PREV(n) = (p);                            		\
        TLIST_NEXT(p) = (n);                            		\
        TLIST_NEXT_PREV(n) = (n);                       		\
    } while (0)

/* insert node n in the head of list k */
#define TLIST_INSERT_HEAD(k, n)                         		\
    TLIST_INSERT_AFTER((k), (n))

/* remove node n from list */
#define TLIST_REMOVE(n)                                 		\
    do                                                  		\
    {                                                   		\
        TLIST_PREV_NEXT(n) = TLIST_NEXT(n);             		\
        TLIST_NEXT_PREV(n) = TLIST_PREV(n);             		\
    } while (0)

/* remove the front node of list k into node n */
#define TLIST_REMOVE_HEAD(k, n)                         		\
    do                                                  		\
    {                                                   		\
        (n) = TLIST_NEXT(k);                            		\
        TLIST_REMOVE(n);                                		\
    } while (0)

/* remove the tail node of list k into node n */
#define TLIST_REMOVE_TAIL(k, n)                         		\
    do                                                  		\
    {                                                   		\
        (n) = TLIST_PREV(k);                            		\
        TLIST_REMOVE(n);                                		\
    } while (0)

/* insert [f,l) before node p (t is a temp node)*/
#define TLIST_TRANSFER_BEFORE(p, f, l, t)               		\
    do                                                  		\
    {                                                   		\
        if ((p) == (l) || (f) == (l))                   		\
            break;                                      		\
        TLIST_PREV_NEXT(l) = (p);                       		\
        TLIST_PREV_NEXT(f) = (l);                       		\
        TLIST_PREV_NEXT(p) = (f);                       		\
        (t) = TLIST_PREV(p);                            		\
        TLIST_PREV(p) = TLIST_PREV(l);                  		\
        TLIST_PREV(l) = TLIST_PREV(f);                  		\
        TLIST_PREV(f) = (t);                            		\
    } while (0)

/* insert [f,l) after node p (t is a temp node) */
#define TLIST_TRANSFER_AFTER(p, f, l, t)                		\
    do                                                  		\
    {                                                   		\
        if ((f) == (l))                                 		\
            break;                                      		\
        (t) = TLIST_NEXT(p);                            		\
        TLIST_PREV_NEXT(l) = (t);                       		\
        TLIST_PREV_NEXT(f) = (l);                       		\
        TLIST_PREV_NEXT(t) = (f);                       		\
        TLIST_PREV(t) = TLIST_PREV(l);                  		\
        TLIST_PREV(l) = TLIST_PREV(f);                  		\
        TLIST_PREV(f) = (p);                            		\
    } while (0)

/* reverse list k (t1, t2 and t3 are temp node) */
#define TLIST_REVERSE(k, t1, t2, t3)                    		\
    do                                                  		\
    {                                                   		\
        (t1) = TLIST_NEXT(k);                           		\
        (t2) = TLIST_NEXT(t1);                          		\
        if ((k) == (t1) || (k) == (t2))                 		\
            break;                                      		\
        while ((t2) != (k))                             		\
        {                                               		\
            (t1) = (t2);                                		\
            (t2) = TLIST_NEXT(t2);                      		\
            TLIST_TRANSFER_AFTER((k), (t1), (t2), (t3));		\
        }                                               		\
    } while (0)

/* 
merge list a and list b into list a
comp is a compare function,
int (*comp)(TLIST* x, TLIST* y)
    return < 0 if should a before b
    return > 0 if should a after b
t1, t2, t3 and t4 are temp node
*/
#define TLIST_MERGE(a, b, comp, t1, t2, t3, t4)         		\
    do                                                  		\
    {                                                   		\
        (t1) = TLIST_NEXT(a);                           		\
        (t2) = TLIST_NEXT(b);                           		\
        while ((t1) != (a) && (t2) != (b))              		\
        {                                               		\
            if (comp((t2), (t1)) < 0)                       	\
            {                                           		\
                (t3) = TLIST_NEXT(t2);                  		\
                TLIST_TRANSFER_BEFORE((t1), (t2), (t3), (t4));  \
                (t2) = (t3);                            		\
            }                                           		\
            else                                        		\
            {                                           		\
                (t1) = TLIST_NEXT(t1);                  		\
            }                                           		\
        }                                               		\
        if ((t2) != (b))                                		\
            TLIST_TRANSFER_BEFORE((a), (t2), (b), (t4));        \
    } while (0)

#endif // !__TLIST_H__
