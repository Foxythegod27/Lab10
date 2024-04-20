#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count; // to store the number of occurrences of the word
};

// Initializes a trie node
struct TrieNode* createNode() {
    struct TrieNode* pNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    pNode->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;
    return pNode;
}

// Inserts the word to the trie structure
void insert(struct TrieNode *root, char *word) {
    struct TrieNode* pCrawl = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = createNode();
        pCrawl = pCrawl->children[index];
    }
    // Mark last node as leaf node
    pCrawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurrences(struct TrieNode *root, char *word) {
    struct TrieNode* pCrawl = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
    }
    return pCrawl->count;
}

// Deallocates the trie structure
void deallocateTrie(struct TrieNode *root) {
    if (!root)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i])
            deallocateTrie(root->children[i]);
    }
    free(root);
}

// Reads the dictionary from a file and inserts each word into the trie
void readDictionaryAndInsert(char *filename, struct TrieNode *root) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file!\n");
        exit(1);
    }

    char buffer[256];
    while (fscanf(file, "%s", buffer) != EOF) {
        char *word = strdup(buffer); // Allocate memory for each word
        insert(root, word);
        free(word); // Free the allocated memory after insertion
    }

    fclose(file);
}


int main(void) {
    struct TrieNode *root = createNode();

    // Read the dictionary from the file and insert each word into the trie
    readDictionaryAndInsert("dictionary.txt", root);

    // Words to search for occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    deallocateTrie(root);

    return 0;
}
