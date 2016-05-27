#if !defined(NULL)
#define NULL 0
#endif

typedef struct list list;
list *new_list();
void free_list(list *l);
int size(list *l);

void push(list *l, void *elem);
void push_first(list*l, void *elem);
void *pop(list *l);
void *pop_first(list *l);
void *pop_last(list *l);
void *peek_first(list *l);