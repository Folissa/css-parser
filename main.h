#ifndef CSS_MAIN_H
#define CSS_MAIN_H

#include <iostream>
#include <string.h>
#include <stdlib.h>

#define SECTIONS_PER_BLOCK 8
#define INPUT_SIZE 1024
#define DATA_SIZE 1048576
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

/*
 * BLOCKS                     * ----------------- *
 *                            |                   |
 * SECTIONS                   [ * , * , * , * ] - [ * , * , * , * ]
 *                             | | | | | | | |     | | | | | | | |
 * SELECTORS                   * | * | * | * |     * | * | * | * |
 * ATTRIBUTES                    * - * - * - *       * - * - * - *
 *
 * NOTE: ASTERISKS ARE NODES OF RESPECTIVE LISTS
 */

typedef struct block block;

typedef struct section section;

typedef struct selector selector;

typedef struct attribute attribute;

typedef struct block {
    section *sectionArray[SECTIONS_PER_BLOCK] = {};
    unsigned int takenSections = 0;
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
void freeData(char *data, int *length);

//
void appendToBuffer(const char *input, char *data, int *sizeOfData, int *currentIndex);

//
void appendCommand(const char *input, char *command, int *currentIndex);

// DATA PARSING RELATED METHODS

//
int countCommandSeparators(const char *input);

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

//
char *getAttributeValueBySelector(section *sections, const char *selectorToFind, const char *attributeToFind);

// LIST RELATED METHODS

//
template <typename type> void printList(type *firstNode);

//
void printBlocks(block *blocks);

//
void printSections(section *sections);

//
void printSelectors(selector *selectors);

//
void printAttributes(attribute *attributes);

//
attribute *getAttribute(section *searchedSection, const char *attributeToFind);

//
selector *getSelector(section *searchedSection, const char *selectorToFind);

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

// Returns the length of a list.
template <typename type> int getListLength(type *firstNode);

// Returns the node at a specific location of a list.
template <typename type> type *getAtPosition(type *firstNode, int position);

// Returns the last node of a list.
template <typename type> type *getLast(type *firstNode);

//
template <typename type> type *addLast(type *firstNode, type *newNode);

//
section *removeSectionNode(section *sections, section *sectionToDelete);

//
attribute *removeAttributeNode(section *section, attribute *attributeToDelete);

//
block *removeBlockNode(block *blocks, block *blockToDelete);

//
block *removeLastBlockNode(block *blocks);

//
template <typename type> void deleteList(type **headReference);

//
section *deleteSelectorsAttributes(section *sections);

#endif //CSS_MAIN_H