#ifndef CSS_MAIN_H
#define CSS_MAIN_H

#include <iostream>
#include <string.h>
#include <stdlib.h>

#define SECTIONS_PER_BLOCK 8
#define INPUT_SIZE 256
#define DATA_SIZE 4096
#define COMMAND_SIZE 3

enum type {
    INT,
    STRING
};

enum part {
    FIRST_PART,
    SECOND_PART,
    THIRD_PART
};

typedef struct block block;

typedef struct section section;

typedef struct selector selector;

typedef struct attribute attribute;

typedef struct block {
    section *sectionArray[SECTIONS_PER_BLOCK] = {};
    int takenSections = 0;
    block *prev = nullptr;
    block *next = nullptr;
} block;

typedef struct section {
    selector *selectorList = nullptr;
    attribute *attributeList = nullptr;
    section *prev = nullptr;
    section *next = nullptr;
} section;

typedef struct selector {
    char selectorName[INPUT_SIZE] = {};
    selector *prev = nullptr;
    selector *next = nullptr;
} selector;

typedef struct attribute {
    char attributeName[INPUT_SIZE] = {};
    char attributeValue[INPUT_SIZE] = {};
    attribute *prev = nullptr;
    attribute *next = nullptr;
} attribute;

// STRING RELATED METHODS

//
char *trimSpaces(char *data);

//
int intOrString(const char *data);

//
void freeString(char *data, int *length);

//
void appendToBuffer(const char *input, char *data, int *sizeOfData, int *currentIndex);

//
void printString(char *data);

// DATA PARSING RELATED METHODS

//
section *parseSelectors(char *data, int *currentIndex, section *sections);

//
section *parseAttributes(char *data, int *currentIndex, section *sections);

//
void parseCommand(const char *input, char commandParts[][INPUT_SIZE]);

//
int isGlobalAttribute(const char *data, int currentIndex);

//
section *dataParser(char *data, block *blocks, section *sections);

//
int countSelectors(const char *data, int currentIndex);

//
int countAttributes(const char *data, int currentIndex);

// LIST RELATED METHODS

//
char *getAttributeValue(section *searchedSection, const char *attributeToFind);

//
int attributeCounter(section *sections, const char *attributeToCount);

//
int selectorCounter(section *sections, const char *selectorToCount);

//
block *addBlock(block *blocks);

//
section *addSection(block *blocks, section *sections);

//
section *createSection(block *block, section *sections);

//
block *createBlockNode();

//
section *createSectionNode();

//
selector *createSelectorNode();

//
attribute *createAttributeNode();

// Prints the data of all the nodes in the list.
template <typename type> void printList(type *firstNode);

//
void printBlocks(block *blocks);

//
void printSections(section *sections);

//
void printSelectors(selector *selectors);

//
void printAttributes(attribute *attributes);

// Returns the length of a list.
template <typename type> int getListLength(type *firstNode);

// Returns the first node of a list.
template <typename type> type getFirst(type *firstNode);

// Returns the node at a specific location of a list.
template <typename type> type *getAtPosition(type *firstNode, int position);

// Returns the last node of a list.
template <typename type> type *getLast(type *firstNode);

//
template <typename type> type addFirst(type *firstNode, type *newNode);

//
template <typename type> type insertBefore(type *firstNode, type *newNode, type *node);

//
template <typename type> void insertAfter(type *newNode, type *node);

//
template <typename type> type *addLast(type *firstNode, type *newNode);

//
template <typename type> type removeFirst(type *firstNode);

//
template <typename type> type *removeNode(type *firstNode, type *node);

//
template <typename type> void removeAfter(type *node);

//
template <typename type> type removeLast(type *firstNode);

//
template <typename type> void deleteList(type **headReference);

//
section *deleteSelectorsAttributes(section *sections);

#endif //CSS_MAIN_H
