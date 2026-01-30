#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raven/colors.h"
#include "raven/error.h"
#include "raven/lexer/lexer.h"
#include "raven/macros.h"
#include "raven/parser/parser.h"
#include "raven/parser/tree.h"
#include "raven/utils.h"

#define currtok (tokens->tokens[*index])
#define tok_txt_is(TXT) (strcmp(currtok->txt, TXT) == 0)
#define tok_type_is(TYPE) (currtok->type == TYPE)
#define expect(TXT)                                                            \
  if (!tok_txt_is(TXT)) {                                                      \
    THROW("Expected token '%s', got '%s'.", TXT, currtok->txt);                \
  }

#define skip()                                                                 \
  do {                                                                         \
    (*index)++;                                                                \
    if (*index >= (size_t)tokens->token_num) {                                 \
      THROW("Out of bounds when parsing. Expected more tokens.");              \
    }                                                                          \
  } while (0)

#define expectskip(TXT)                                                        \
  expect(TXT);                                                                 \
  skip();

tree_t *parse(token_list_t *tokens) {
  char rootstr[1];
  rootstr[0] = '\0';
  tree_t *root = new_tree(TREE_ROOT, rootstr, "Root");
  for (size_t i = 0; i < (size_t)tokens->token_num;) {
    // TODO: main parse loop
  }

  return root;
}

tree_t *parse_declaration(token_list_t *tokens, size_t *index) {
  UNUSED(tokens);
  UNUSED(index);

  TODO("parse_declaration");
}

void display_tree(tree_t *tree, int indent) {
  char *indentstr = "";
  for (size_t i = 0; i < (size_t)indent; i++) {
    strcatchr(indentstr, '\t');
  }
  printf(BLU "%s" CRESET ": " GRN "%s" CRESET, tree->label, tree->data);
  for (size_t i = 0; i < tree->child_count; i++) {
    display_tree(tree->children[i], indent + 1);
  }
}
