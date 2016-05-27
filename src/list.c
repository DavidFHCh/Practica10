#include <stdlib.h>
#include "list.h"

typedef struct node {
  void *element;
  struct node *next;
  struct node *prev;
} node;

struct list {
  node *head;
  node *tail;
  int size;
};

list *new_list()
{
  list *ptnew_list = malloc(sizeof(list));
  ptnew_list->head = NULL;
  ptnew_list->tail = NULL;
  ptnew_list->size = 0;
  return ptnew_list;
}

void free_list(list *l)
{
  node *ptnode, *ptnext_node;
  for(ptnode = l->head; ptnode != NULL; ptnode = ptnode->next) {
    free(ptnode);
  }
  free(l);
}

int size(list *l)
{
  return l->size;
}

void push(list *l, void *element)
{
  node *ptnew_node;
  ptnew_node = malloc(sizeof(node));
  ptnew_node->element = element;
  ptnew_node->next = NULL;
  ptnew_node->prev = NULL;
  if (l->size == 0) {
    l->head = ptnew_node;
    l->tail = ptnew_node;
  } else {
    l->tail->next = ptnew_node;
    ptnew_node->prev = l->tail;
    l->tail = ptnew_node;
  }
  l->size++;
}

void push_first(list *l, void *element)
{
  node *ptnew_node;
  ptnew_node = malloc(sizeof(node));
  ptnew_node->element = element;
  ptnew_node->next = NULL;
  ptnew_node->prev = NULL;
  if (l->size == 0) {
    l->head = ptnew_node;
    l->tail = ptnew_node;
  } else {
    l->head->prev = ptnew_node;
    ptnew_node->next = l->head;
    l->head = ptnew_node;
  }
  l->size++;
}

void *pop_first(list *l)
{
  node *ptnode;
  void *ptelem;
  ptnode = l->head;
  ptelem = ptnode->element;
  l->head = ptnode->next;
  ptnode->prev = NULL;
  l->size--;
  free(ptnode);
  return ptelem;
}

void *pop_last(list *l)
{
  node *ptnode;
  void *ptelem;
  ptnode = l->tail;
  ptelem = ptnode->element;
  l->tail = ptnode->prev;
  ptnode->next = NULL;
  l->size--;
  free(ptnode);
  return ptelem;
}

void *pop(list *l)
{
  return pop_first(l);
}

void *peek_first(list *l)
{
    return l->head->element;
}