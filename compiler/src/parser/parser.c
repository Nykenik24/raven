#include "parser.h"
#include "macros.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

tree_t *parse(token_list_t *tokens) {
  char rootstr[1];
  rootstr[0] = '\0';
  tree_t *root = new_tree(TREE_ROOT, rootstr, 0, "Root");

  for (size_t i = 0; i <= (size_t)tokens->token_num;) {
    add_child(root, parse_declaration(tokens, i));
  }

  return root;
}

int tok_txt_is(token_t *token, token_type_t type) {
  return token->type == type;
}

int tok_type_is(token_t *token, const char *txt) {
  return strcmp(token->txt, txt) == 0;
}

/* TODO */

#define TODO_PARSE_FUNC(name)                                                  \
  tree_t *name(token_list_t *tokens, size_t index) {                           \
    UNUSED(tokens);                                                            \
    UNUSED(index);                                                             \
    TODO("function: " #name);                                                  \
  }

TODO_PARSE_FUNC(parse_declaration);
