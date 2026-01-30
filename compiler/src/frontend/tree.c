#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raven/error.h"
#include "raven/macros.h"
#include "raven/parser/tree.h"

tree_t *new_tree(nodetype_t type, const char *data, const char *label) {
  tree_t *tree = malloc(sizeof(tree_t));
  tree->type = type;

  SAFECPY(data, tree->data);
  SAFECPY(label, tree->label);

  return tree;
}

void free_tree(tree_t *tree) {
  if (tree) {
    if (tree->data) {
      free(tree->data);
    }
    if (tree->label) {
      free(tree->label);
    }
  }
}
