#include <stdlib.h>

#include "list.h"
#include "types.h"



// Recursively get length of list
int LispList_count(LispListElement *l)
{
  // TODO what about circular lists of a few or one repeating element(s)?
  // ... -> a -> b -> c -> a -> b ->c -> ... ?
  // Should count the unique patterns, number elems before gets back to start

  // TODO error on size of NULL element
  // not the same as empty list which is list elem with no next and NULL value

  if (l->value == NULL) {
    return 0;
  }

  return LispList_count(l->next) + 1;
}




// Add object $toadd to list $list.
// $list must be a list.
void LispList_add_object_to_list(LispListElement *list, LispObject *toadd)
{
  LispListElement *new = calloc(1, sizeof(LispListElement));
  LispListElement *i = list;

  while (i->value != NULL) 
    i = i->next;
  // now i has the last item in the linked list

  i->next = new;
  i->value = toadd;
}




// new list element
LispListElement *LispList_new_element()
{
  LispListElement *rv = calloc(1, sizeof(LispListElement));
  return rv;
}

