/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL)
        return NULL;
    q->head = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements and the strings? */
    if (q != NULL) {
        if (q->size == 0) {
            free(q);
            return;
        }
        list_ele_t *cur;
        list_ele_t *tmp;
        list_for_each_safe(cur, tmp, q->head) { free(cur); }
        free(q->head);
        /* Free queue structure */
        free(q);
    }
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? */
    if (q == NULL)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL) {
        return false;
    }
    newh->value = strdup(s);
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (q->head == NULL) {
        q->head = newh;
        newh->next = newh;
        newh->prev = newh;
    } else {
        // from new head to left
        newh->prev = q->head->prev;
        // from left to new head
        q->head->prev->next = newh;
        // from old head to new head
        q->head->prev = newh;
        // from new head to old head
        newh->next = q->head;

        q->head = newh;
    }
    q->size++;
    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
        return false;
    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (newt == NULL) {
        return false;
    }
    newt->next = NULL;
    newt->value = strdup(s);
    if (q->head == NULL) {
        q->head = newt;
        newt->next = newt;
        newt->prev = newt;
    } else {
        // from new tail to left
        newt->prev = q->head->prev;
        // from left to new tail
        q->head->prev->next = newt;
        // from old tail to new tail
        q->head->prev = newt;
        // from new tail to old tail
        newt->next = q->head;

        // [note !!!!]
        // the difference between q_insert_head and q_insert_tail is without
        // assigning q->head to the new node.
    }
    q->size++;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
    if (q == NULL || q->size == 0)
        return false;
    list_ele_t *tmp = q->head;
    if (q->size > 1) {
        list_ele_t *left = tmp->prev;
        list_ele_t *right = tmp->next;
        left->next = right;
        right->prev = left;
        q->head = q->head->next;
    } else if (q->size == 1) {
        q->head = NULL;
    }
    int copy_len = bufsize / sizeof(char) - 1;
    if (sp != NULL) {
        memcpy(sp, tmp->value, copy_len);
        sp[copy_len] = '\0';
    }
    free(tmp);
    q->size--;
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
        return 0;
    return q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */

void q_reverse(queue_t *q)
{
    if (q == NULL || q->size <= 1)
        return;
    list_ele_t *tail = q->head->prev;
    list_ele_t *cur;
    list_ele_t *n;
    list_for_each_safe(cur, n, q->head)
    {
        list_ele_t *tmp = cur->next;
        cur->next = cur->prev;
        cur->prev = tmp;
    }
    list_ele_t *tmp = q->head->next;
    q->head->next = q->head->prev;
    q->head->prev = tmp;
    q->head = tail;
}
