#include "rbtree.h"
#include <stdio.h>
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
    // 여기를 수정했다.
    nil->parent = nil;
    nil->left = nil;
    nil->right = nil;

    p->nil = nil;
    p->root = nil;
    return p;
}

void free_child_data(rbtree *t, node_t *root) {
    if (root == t->nil) {
        return;
    }
    free_child_data(t, root->left);
    free_child_data(t, root->right);
    free(root);
}


void delete_rbtree(rbtree *t) {
    // TODO: reclaim the tree nodes's memory
    if (t->root != t->nil) {
        free_child_data(t, t->root);
    }
    free(t->nil);  // sentinel node 해제
    free(t);
}

void rb_left_rotation(rbtree *t, node_t *x) {
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rb_right_rotation(rbtree *t, node_t *x) {
    node_t *y = x->left;
    x->left = y->right;
    if (y->right != t->nil){
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil){
        t->root = y;
    }
    else if (x == x->parent->right){
        x->parent->right = y;
    }
    else{
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void rb_insert_fixup(rbtree *t, node_t *target) {
    while (target->parent->color == RBTREE_RED) {
        node_t *parent = target->parent;
        node_t *grandparent = parent->parent;
        if (grandparent->left == parent) {
            // 부모 노드가 할아버지 노드의 왼쪽 자식일 경우
            node_t *uncle = grandparent->right; // 삼촌 노드
            if (uncle->color == RBTREE_RED) { //case 1
                parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                grandparent->color = RBTREE_RED;
                target = grandparent;
            } else{
                if (target == parent->right) {
                    // 부모가 오른쪽 자식이면 left-rotation
                    target = parent;
                    rb_left_rotation(t, target);
                    parent = target->parent;
                }
                parent->color = RBTREE_BLACK;
                grandparent->color = RBTREE_RED;
                //right_rotation
                rb_right_rotation(t, grandparent);
            }
        }
        else {
            // 부모 노드가 할아버지 노드의 오른쪽 자식일 경우
            node_t *uncle = grandparent->left; // 삼촌 노드
            if (uncle->color == RBTREE_RED) { //case 1
                parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                grandparent->color = RBTREE_RED;
                target = grandparent;
            } else{
                if (target == parent->left) {
                    // 부모가 왼쪽 자식이면 right-rotation
                    target = parent;
                    //right_rotation
                    rb_right_rotation(t, target);
                    parent = target->parent;
                }
                parent->color = RBTREE_BLACK;
                grandparent->color = RBTREE_RED;
                //left_rotation
                rb_left_rotation(t, grandparent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
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
    rb_insert_fixup(t, new_node);
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

void rb_delete_fixup(rbtree *t, node_t *target) {
    while (target != t->root && target->color != RBTREE_RED) {
        node_t *node_parent = target->parent;
        node_t *sibling;
        if (target == node_parent->left) {
            // target이 부모의 왼쪽 자식인 경우
            sibling = node_parent->right;
            if (sibling->color == RBTREE_RED) { // case 1
                sibling->color = RBTREE_BLACK;
                target->parent->color = RBTREE_RED;
                rb_left_rotation(t, target->parent);
                sibling = target->parent->right;
            }else {
                // 형제의 색이 검정색일 때 조건문 진입
                // case 2
                if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
                    sibling->color = RBTREE_RED;
                    target = target->parent;
                } else {
                    // case 3 처리
                    if (sibling->right->color == RBTREE_BLACK) {
                        sibling->left->color = RBTREE_BLACK;
                        sibling->color = RBTREE_RED;
                        rb_right_rotation(t,sibling);
                        sibling = node_parent->right;
                    }
                    // case 4 처리
                    sibling->color = node_parent->color;
                    node_parent->color = RBTREE_BLACK;
                    sibling->right->color = RBTREE_BLACK;
                    rb_left_rotation(t, node_parent);
                    target = t->root;

                }
            }

        }
        else {
            // target이 부모의 오른쪽 자식인 경우
            sibling = node_parent->left;
            if (sibling->color == RBTREE_RED) { // case 1
                sibling->color = RBTREE_BLACK;
                target->parent->color = RBTREE_RED;
                rb_right_rotation(t, target->parent);
                sibling = target->parent->left;
            }else {
                // 형제의 색이 검정색일 때 조건문 진입
                // case 2
                if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
                    sibling->color = RBTREE_RED;
                    target = target->parent;
                } else {
                    // case 3 처리
                    if (sibling->left->color == RBTREE_BLACK) {
                        sibling->right->color = RBTREE_BLACK;
                        sibling->color = RBTREE_RED;
                        rb_left_rotation(t,sibling);
                        sibling = node_parent->left;
                    }
                    // case 4 처리
                    sibling->color = node_parent->color;
                    node_parent->color = RBTREE_BLACK;
                    sibling->left->color = RBTREE_BLACK;
                    rb_right_rotation(t, node_parent);
                    target = t->root;
                }
            }
        }
    }
    target->color = RBTREE_BLACK;

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
    } else { // 삭제하려는 노드의 자녀가 둘인 경우
        // 삭제하려는 노드 기준 successor 즉 후임자를 찾는 과정
        y = rb_tree_min_subtree(t, p->right);
        // 삭제되는 노드의 successor의 색을 저장
        original_color = y->color;
        replacement = y->right;
        // successor 노드를 기존 노드에서 분리하기 위한 작업
        if (y->parent == p) {
            replacement->parent = y;
        }
        else {
            rb_transplant(t, y, replacement);
            y->right = p->right;
            y->right->parent = y;
        }
        // 분리된 successor 노드를 삭제하려는 노드에 대체하는 작업
        rb_transplant(t, p, y);
        y->left = p->left;
        y->left->parent = y;
        y->color = p->color;
    }

    if (original_color == RBTREE_BLACK) {
        rb_delete_fixup(t, replacement);
    }
    free(p);
    return 0;
}



void in_order_search(const rbtree *t, key_t *arr, size_t n, int *ptr_i, node_t *p) {
    if (p == t->nil || *ptr_i >= n) {
        return;
    }
    in_order_search(t, arr, n, ptr_i, p->left);
    if (*ptr_i < n) {
        arr[*ptr_i] = p->key;
        *ptr_i += 1;
    }
    in_order_search(t, arr, n, ptr_i, p->right);
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
    // TODO: implement to_array
    node_t *cur = t->root;
    int i = 0;
    int *ptr_i = &i;
    in_order_search(t, arr, n, ptr_i, cur);
    return 0;
}
