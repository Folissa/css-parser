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

// Trim lading and trailing spaces.
char *trimSpaces(char *data);

// Check if is an int or string.
int intOrString(const char *data);

// Empty out temporary data.
void freeData(char *data, int *length);

// Append data to the buffer for parsing.
void appendToBuffer(const char *input, char *data, int *sizeOfData, int *currentIndex);

// Append command to the buffer for parsing.
void appendCommand(const char *input, char *command, int *currentIndex);

// DATA PARSING RELATED METHODS

// Count how many command separators are there to determine if the command is legal.
int countCommandSeparators(const char *input);

// Parse the selectors to data structures.
section *parseSelectors(char *data, int *currentIndex, section *sections);

// Parse the attributes to data structures.
section *parseAttributes(char *data, int *currentIndex, section *sections);

// Parse the command.
void parseCommand(const char *input, char commandParts[][INPUT_SIZE]);

// Check if the attribute is global.
int isGlobalAttribute(const char *data, int currentIndex);

// Parse the data.
section *dataParser(char *data, block *blocks, section *sections);

// Count selectors for parsing.
int countSelectors(const char *data, int currentIndex);

// Count attributes for parsing.
int countAttributes(const char *data, int currentIndex);

// Get attribute within given selector (the last one).
char *getAttributeValueBySelector(section *sections, const char *selectorToFind, const char *attributeToFind);

// LIST RELATED METHODS

// Print out the list.
template <typename type> void printList(type *firstNode);

// Print out the blocks.
void printBlocks(block *blocks);

// Print out the sections.
void printSections(section *sections);

// Print out the selectors.
void printSelectors(selector *selectors);

// Print out the attributes.
void printAttributes(attribute *attributes);

// Find an attribute in a section.
attribute *getAttribute(section *searchedSection, const char *attributeToFind);

// Find a selector in a section.
selector *getSelector(section *searchedSection, const char *selectorToFind);

// Count attribute in all the sections.
int attributeCounter(section *sections, const char *attributeToCount);

// Count selector in all the sections.
int selectorCounter(section *sections, const char *selectorToCount);

// Add a new block.
block *addBlock(block *blocks);

// Add a new section.
section *addSection(block *blocks, section *sections);

// Creates section in a block.
section *createSection(block *block, section *sections);

// Create new block node.
block *createBlockNode();

// Create new section node.
section *createSectionNode();

// Create new selector node.
selector *createSelectorNode();

// Create new attribute node.
attribute *createAttributeNode();

// Returns the length of a list.
template <typename type> int getListLength(type *firstNode);

// Returns the node at a specific location of a list.
template <typename type> type *getAtPosition(type *firstNode, int position);

// Returns the last node of a list.
template <typename type> type *getLast(type *firstNode);

// Add the node at the end of a list.
template <typename type> type *addLast(type *firstNode, type *newNode);

// Remove section node.
section *removeSectionNode(section *sections, section *sectionToDelete);

// Remove attribute node.
attribute *removeAttributeNode(section *section, attribute *attributeToDelete);

// Remove block node.
block *removeBlockNode(block *blocks, block *blockToDelete);

// Remove last block node.
block *removeLastBlockNode(block *blocks);

// Delete the list.
template <typename type> void deleteList(type **headReference);

// Delete selectors and attributes from a section.
section *deleteSelectorsAttributes(section *sections);

#endif //CSS_MAIN_H