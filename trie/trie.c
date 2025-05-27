#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define WORD_DELIM " ,.;:\r\n\t~¡!@#$%^&*()=+[{]}\\|'\"<>/?¿—"
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
        trie_deinit(root->childs[i], depth+1);
    }
    free(root);
}

void trie_print(node_t *const root, uint8_t depth) {
    for (size_t i = 0; i < MAX_CHILD_CNT; i++) {
        if (!root->childs[i]) continue;
        printf("%*c\n", depth * 2 + 1, (char) i);
        trie_print(root->childs[i], depth+1);
    }
}

void trie_valid(node_t *const root, char *prev) {

    size_t n = strlen(prev);
    char name[128] = {};
    strncpy(name, prev, sizeof(name) / sizeof(*name));
    name[n+1] = 0;

    if (root->valid) {
        printf("%s = %u\n", name, root->value);
    }

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
    name[n+1] = 0;

    for (size_t i = 0; i < MAX_CHILD_CNT; i++) {
        if (!root->childs[i]) continue;
        name[n] = (char) i;
        printf("    %s -> %s [label=\"%c\"]\n", n > 0 ? prev : "__root__", name, (char) i);
        trie_dot(root->childs[i], name);
    }
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
        // printf("%s", buff);
        for (char *word = strtok(buff, WORD_DELIM); word; word = strtok(NULL, WORD_DELIM)) {
            if (*word == 0 || *word == '-' || strcmp(word, "-") == 0) continue;
            node_t *node = trie_get(root, word);
            if (node != NULL) node->value++;
            else trie_add(root, word, 1);
        }
    }

    fclose(fp);
    if (line) free(line);

    trie_valid(root, "");
    // printf("digraph Tree {\n");
    // trie_dot(root, "");
    // printf("}\n");

    trie_deinit(root, 0);

    return 0;
}

int8_t type(unsigned char c) {
    // -1 -> invalid
    // 0 -> break character
    // 1 -> valid character
    if (c < 32) return -1;
    if (c <= 47) return 0;
    if (c <= 57) return 1;
    if (c <= 64) return 0;
    if (c <= 90) return 1;
    if (c <= 96) return 0;
    if (c <= 122) return 1;
    if (c <= 126) return 0;
    return -1;
}



/*
32:  
33: !
34: "
35: #
36: $
37: %
38: &
39: '
40: (
41: )
42: *
43: +
44: ,
45: -
46: .
47: /

48: 0
49: 1
50: 2
51: 3
52: 4
53: 5
54: 6
55: 7
56: 8
57: 9

58: :
59: ;
60: <
61: =
62: >
63: ?
64: @

65: A
66: B
67: C
68: D
69: E
70: F
71: G
72: H
73: I
74: J
75: K
76: L
77: M
78: N
79: O
80: P
81: Q
82: R
83: S
84: T
85: U
86: V
87: W
88: X
89: Y
90: Z

91: [
92: \
93: ]
94: ^
95: _
96: `

97: a
98: b
99: c
100: d
101: e
102: f
103: g
104: h
105: i
106: j
107: k
108: l
109: m
110: n
111: o
112: p
113: q
114: r
115: s
116: t
117: u
118: v
119: w
120: x
121: y
122: z

123: {
124: |
125: }
126: ~
*/
