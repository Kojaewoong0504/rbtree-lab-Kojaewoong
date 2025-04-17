#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree *p = (rbtree *) calloc(1, sizeof(rbtree));
    // TODO: initialize struct if needed
    if (p == NULL) {
        return NULL;
    }
    node_t *nil = (node_t *)calloc(1, sizeof(node_t));
    if (nil == NULL) {
        free(p);
        return NULL;
    }
    nil->color = RBTREE_BLACK;
    nil->key = 0;
    nil->parent = NULL;
    nil->left = NULL;
    nil->right = NULL;

    p->nil = nil;
    p->root = nil;
    return p;
}

void free_child_data(rbtree *t, node_t *root) {
    if (t->nil != root->left) {
        free_child_data(t, root);
    }
    if (t->nil != root->right) {
        free_child_data(t, root);
    }
    free(root);
}


void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    node_t *cur = t->root;
    if (cur != t->nil) {
        free_child_data(t, cur);
    }
    free(t->nil);
    free(t);
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
    // TODO: implement insert
    node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->key = key;
    new_node->color =RBTREE_RED;
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->parent = t->nil;

    node_t *cur = t->root;
    while (t->nil != cur) {
        if (key < cur->key) {
            if (cur->left == t->nil) {
                cur->left = new_node;
                break;
            }
            cur = cur->left;
        }else {
            if (cur->right == t->nil) {
                cur->right = new_node;
                break;
            }
            cur = cur->right;
        }
    }

    new_node->parent = cur;

    if (cur == t->nil) {
        t->root = new_node;
    }

    // RB tree 케이스 처리 함수......

    return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    // TODO: implement find
    node_t *cur = t->root;
    while (t->nil != cur) {
        if (key == cur->key) {
            return cur;
        }else {
            if (key < cur->key) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
    }
    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
    // TODO: implement find
    node_t *cur = t->root;
    while (cur->left != t->nil) {
        cur = cur->left;
    }
    return cur;
}

node_t *rbtree_max(const rbtree *t) {
    // TODO: implement find
    node_t *cur = t->root;
    while (cur->right != t->nil) {
        cur = cur->right;
    }
    return cur;
}

void rb_transplant(rbtree *t, node_t *p, node_t *replacement) {
    if (p->parent == t->nil) {
        t->root = replacement;
    }else if (p == p->parent->left) {
        p->parent->left = replacement;
    }else {
        p->parent->right = replacement;
    }
    replacement->parent = p->parent;
}


node_t *rb_tree_min_subtree(rbtree *t, node_t *start) {
    node_t *cur = start;
    while (cur->left != t->nil) {
        cur = cur->left;
    }
    return cur;
}

int rbtree_erase(rbtree *t, node_t *p) {
    node_t *y = p;
    node_t *replacement;
    color_t original_color = p->color;

    if (p->left == t->nil) {
        replacement = p->right;
        rb_transplant(t, p, replacement);
    } else if (p->right == t->nil) {
        replacement = p->left;
        rb_transplant(t, p, replacement);
    } else {
        y = rb_tree_min_subtree(t, p->right);
        original_color = y->color;
        replacement = y->right;

        if (y->parent != p) {
            rb_transplant(t, y, replacement);
            y->right = p->right;
            y->right->parent = y;
        }
        rb_transplant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }

    if (original_color == RBTREE_BLACK) {
        // rb_delete_fixup(t, replacement);
    }

    free(y);
    return 0;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    return 0;
}
