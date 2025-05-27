#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORD_DELIM " ,.;:\r\n\t~¡!@#$%^&*()=+[{]}\\|'\"<>/?¿—«»"
#define MAX_CHILD_CNT 0x100
#define MAX_WORD_LEN 32

typedef struct node {
    uint8_t key;
    uint8_t valid;
    uint32_t value;
    struct node *childs[MAX_CHILD_CNT];
} node_t;

typedef struct word_stats {
    char word[MAX_WORD_LEN];
    uint32_t count;
    struct word_stats *next;
} word_stats_t;

word_stats_t *word_stat_init() {
    word_stats_t *new = (word_stats_t *) malloc(sizeof(word_stats_t));
    memset(new->word, 0, MAX_WORD_LEN);
    new->next = NULL;
    return new;
}

word_stats_t *word_stats(word_stats_t *prev_stat, node_t *const root, char *prev_name) {
    size_t n = strlen(prev_name);
    char name[MAX_WORD_LEN] = {};
    strncpy(name, prev_name, sizeof(name) / sizeof(*name));
    name[n + 1] = 0;

    if (root->valid) {
        word_stats_t *new = word_stat_init();
        if (new == NULL) {
            printf("failed to reserve space for word stat\n");
            return prev_stat;
        }
        strncpy(new->word, name, MAX_WORD_LEN);
        new->count = root->value;
        prev_stat->next = new;
        prev_stat = new;
        // printf("new word: %s (%u)\n", new->word, new->count);
    }

    for (size_t i = 0; i < MAX_CHILD_CNT; i++) {
        if (!root->childs[i]) continue;
        name[n] = (char) i;
        prev_stat = word_stats(prev_stat, root->childs[i], name);
    }

    return prev_stat;
}

void word_stats_print(word_stats_t *stat) {
    while (stat != NULL) {
        printf("%-*s %u\n", MAX_WORD_LEN, stat->word, stat->count);
        stat = stat->next;
    }
}

int word_stat_cmp(word_stats_t *a, word_stats_t *b) {
    if (a->count == b->count) return strcmp(a->word, b->word);
    return a->count < b->count;
}

word_stats_t *merge_sorted(word_stats_t *a, word_stats_t *b) {
    word_stats_t dummy = { .next = NULL }, *tail = &dummy;
    while (a && b) {
        if (word_stat_cmp(a, b) <= 0) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

word_stats_t *merge_sort(word_stats_t *head) {
    if (!head || !head->next) return head;  // 0 or 1 element → already sorted

    // 1) split in half
    word_stats_t *slow = head, *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    word_stats_t *mid = slow->next;
    slow->next = NULL;

    // 2) sort each half
    word_stats_t *left  = merge_sort(head);
    word_stats_t *right = merge_sort(mid);

    // 3) merge & return
    return merge_sorted(left, right);
}

// TODO: malloc null checks
node_t *trie_init(uint8_t key, uint32_t value) {
    node_t *node = (node_t *) malloc(sizeof(node_t));
    node->key = key;
    node->valid = 0;
    node->value = value;
    memset(node->childs, 0, sizeof(node->childs));
    // printf("trie_init(%c, %u) ->\tnode = %p\n", key, value, node);
    return node;
}

node_t *trie_add(node_t *const root, const char *const key, const uint32_t value) {
    if (root == NULL || key == NULL) return NULL;

    const unsigned char *k = (unsigned char *) key;
    node_t *prev = NULL;
    node_t *curr = root;

    while (*k) {
        if (curr->childs[*k] == NULL) curr->childs[*k] = trie_init(*k, 0);
        prev = curr;
        curr = curr->childs[*k];
        k++;
    }

    curr->valid = 1;
    curr->value = value;

    return curr;
}

node_t *trie_get(node_t *const root, const char *const key) {
    if (root == NULL || key == NULL) return NULL;

    const unsigned char *k = (unsigned char *) key;
    node_t *prev = NULL;
    node_t *curr = root;

    while (*k) {
        if (curr->childs[*k] == NULL) return NULL;
        prev = curr;
        curr = curr->childs[*k];
        k++;
    }

    if (!curr->valid) return NULL;
    return curr;
}

// TODO: make this not recursive
void trie_deinit(node_t *const root, int depth) {
    if (root == NULL) return;
    for (size_t i = 0; i < MAX_CHILD_CNT; i++) {
        if (!root->childs[i]) continue;
        trie_deinit(root->childs[i], depth + 1);
    }
    free(root);
}

void trie_print(node_t *const root, uint8_t depth) {
    for (size_t i = 0; i < MAX_CHILD_CNT; i++) {
        if (!root->childs[i]) continue;
        printf("%*c\n", depth * 2 + 1, (char) i);
        trie_print(root->childs[i], depth + 1);
    }
}

void trie_valid(node_t *const root, char *prev) {
    size_t n = strlen(prev);
    if (n - 2 >= MAX_WORD_LEN) return;
    char name[MAX_WORD_LEN] = {};
    strncpy(name, prev, sizeof(name) / sizeof(*name));
    name[n + 1] = 0;

    if (root->valid) printf("%s = %u\n", name, root->value);

    for (size_t i = 0; i < MAX_CHILD_CNT; i++) {
        if (!root->childs[i]) continue;
        name[n] = (char) i;
        trie_valid(root->childs[i], name);
    }
}

void trie_dot(node_t *const root, char *prev) {
    size_t n = strlen(prev);
    char name[128] = {};
    strncpy(name, prev, sizeof(name) / sizeof(*name));
    name[n + 1] = 0;

    for (size_t i = 0; i < MAX_CHILD_CNT; i++) {
        if (!root->childs[i]) continue;
        name[n] = (char) i;
        printf("    %s -> %s [label=\"%c\"]\n", n > 0 ? prev : "__root__", name, (char) i);
        trie_dot(root->childs[i], name);
    }
}

void word_stats_deinit(word_stats_t *stat) {
    if (stat == NULL) return;
    word_stats_deinit(stat->next);
    free(stat);
}

int main(int argc, char **argv) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if ((fp = fopen("el_quijote_ascii.txt", "r")) == NULL) return 1;

    node_t *root = trie_init(0, 0);

    size_t line_cnt = 0;
    char buff[128]; // we know input lines will never exceed this
    while ((read = getline(&line, &len, fp)) > 0) {
        line_cnt++;

        memcpy(buff, line, read);
        buff[read] = 0;
        for (char *word = strtok(buff, WORD_DELIM); word; word = strtok(NULL, WORD_DELIM)) {
            if (*word == 0 || *word == '-' || strcmp(word, "-") == 0) continue;
            node_t *node = trie_get(root, word);
            if (node != NULL) node->value++;
            else trie_add(root, word, 1);
        }
    }

    fclose(fp);
    if (line) free(line);

    word_stats_t *stats = word_stat_init();
    word_stats(stats, root, "");
    stats = merge_sort(stats);
    word_stats_print(stats);
    word_stats_deinit(stats);

    // trie_valid(root, "");
    // printf("digraph Tree {\n");
    // trie_dot(root, "");
    // printf("}\n");

    trie_deinit(root, 0);

    return 0;
}
