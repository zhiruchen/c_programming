#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  char* key;
  long val;
} node;

typedef struct {
  node *root;
} btree;


int main(int argc, char const *argv[])
{
    btree* tree = malloc(sizeof(btree));
    tree->root = malloc(sizeof(node));
    tree->root->key = "abc";
    tree->root->val = 100;

    printf("key: %s, val: %ld\n", tree->root->key, tree->root->val);
    return 0;
}
