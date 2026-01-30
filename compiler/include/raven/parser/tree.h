#ifndef INCLUDE_INCLUDE_TREE_H_
#define INCLUDE_INCLUDE_TREE_H_

#include <stddef.h>
#include <stdint.h>

typedef enum {
  TREE_ROOT,

  /* Literals */
  TREE_INT,
  TREE_FLOAT,
  TREE_STRING,
  TREE_BOOL,
  TREE_TAG,
  TREE_ARRAY_LITERAL,
  TREE_MAP_LITERAL,
  TREE_ID,

  /* Types */
  TREE_PRIMITIVE_TYPE,
  TREE_ARRAY_TYPE,
  TREE_MAP_TYPE,
  TREE_FUNC_TYPE,

  /* Declarations */
  TREE_VAR_DECL,
  TREE_FUNC_DECL,
  TREE_STRUCT_DECL,
  TREE_ENUM_DECL,

  /* Statements */
  TREE_IF_STMT,
  TREE_SWITCH_STMT,
  TREE_DEFER_STMT,
  TREE_RETURN_STMT,
  TREE_THROW_STMT,
  TREE_TRY_STMT,
} nodetype_t;

typedef struct tree tree_t;

typedef union type type_t;

struct tree {
  nodetype_t type;
  char *data;
  char *label; // for debug purpose

  union {
    int64_t i;
    float f;
    char *str;
    int bool;
    char *tag;
    char *id;

    tree_t **array_lit;
    struct {
      tree_t *k;
      tree_t *v;
    } **map_lit;

    char *type_id;
    tree_t *array_type;

    struct {
      tree_t *k;
      tree_t *v;
    } map_type;

    struct {
      tree_t **args;
      tree_t *ret;
    } func_type;
  } leaf;
};

tree_t *new_tree(nodetype_t type, const char *data, const char *label);
void free_tree(tree_t *tree);

#endif // INCLUDE_INCLUDE_TREE_H_
