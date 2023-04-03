#include <iostream>
#include "main.h"

using namespace std;


int main() {

    char input[INPUT_SIZE];
    char *data = (char *) malloc(DATA_SIZE * sizeof(char));
    int sizeOfData = 0, currentIndex = 0, appendingAllowed = 1, parseData = 0;
    block *blocks = nullptr;
    blocks = addBlock(blocks);
    section *sections = nullptr;

    // Inputs until a whitespace.
    while (cin >> input) {
        // Check if we invoke the commands or input the data to the buffer.
        if (strcmp(input, "????") == 0) {
            appendingAllowed = 0;
            parseData = 1;
            continue;
        }
        else if (strcmp(input, "****") == 0) {
            appendingAllowed = 1;
            continue;
        }

        // Commands handling.
        if (!appendingAllowed) {
            if (parseData) {
                // Data parsing.
//                printString(data);
                dataParser(data, blocks, sections);
                parseData = 0;

                // After we parse all data into according data structures we can erase the data buffer.
                freeString(data, &currentIndex);
            }
            // Invoke the commands.
            if (strcmp(input, "?") == 0) {
                continue;
            }
            else {
                // Parse the command.
                char *commandParts[COMMAND_SIZE];
                parseCommand(commandParts, input);

                // Then invoke it.
                if (strcmp(commandParts[SECOND_PART], "S") == 0) {
                    if (intOrString(commandParts[FIRST_PART]) == INT && strcmp(commandParts[THIRD_PART], "?") == 0) {
                        continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == INT && intOrString(commandParts[THIRD_PART]) == INT) {
                        continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == STRING && strcmp(commandParts[THIRD_PART], "?") == 0) {
                        continue;
                    }
                }
                else if (strcmp(commandParts[SECOND_PART], "A") == 0) {
                    if (intOrString(commandParts[FIRST_PART]) == INT && strcmp(commandParts[THIRD_PART], "?") == 0) {
                        continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == INT && intOrString(commandParts[THIRD_PART]) == STRING) {
                        continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == STRING && strcmp(commandParts[THIRD_PART], "?") == 0) {
                        continue;
                    }
                }
                else if (strcmp(commandParts[SECOND_PART], "D") == 0) {
                    if (intOrString(commandParts[FIRST_PART]) == INT && strcmp(commandParts[THIRD_PART], "*") == 0) {
                        continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == INT && intOrString(commandParts[THIRD_PART]) == STRING) {
                        continue;
                    }
                }
                else if (strcmp(commandParts[SECOND_PART], "E") == 0)
                    continue;
            }
        }
        // Append the data for parsing.
        else
            appendToBuffer(input, data, &sizeOfData, &currentIndex);
    }

    // Free the allocated memory.
    deleteSelectorsAttributes(sections);
    deleteList<section>(&sections);
    deleteList<block>(&blocks);

    return 0;
}

// STRING RELATED METHODS

char *trimSpaces(char *data) {
    // https://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way

    // Trim leading space.
    // Is a char, that *data points to, a space? If yes, move the pointer.
    while (isspace(*data))
        data++;

    // Trim trailing space.
    char *end;
    end = data + strlen(data) - 1;
    while (end > data && isspace(*end))
        end--;

    // Write new null terminator character.
    end[1] = '\0';

    return data;
}

int intOrString(const char *data) {
    int i = 0;
    while(data[i] != '\0') {
        if (!('0' <= data[i] && data[i] <= '9'))
            return 1;
        i++;
    }
    return 0;
}

void freeString(char *data, int *length) {
    memset(data, '\0', *length);
    *length = 0;
}

void appendToBuffer(const char *input, char *data, int *sizeOfData, int *currentIndex) {
    int j = 0;
    while (input[j] > ' ') {
        data[*currentIndex] = input[j];
        (*currentIndex)++;
        j++;
        (*sizeOfData)++;
    }
    data[*currentIndex] = ' ';
    (*currentIndex) ++;
}

void printString(char *data) {
    int i = 0;
    while (data[i] != '\0') {
        cout << data[i];
        i++;
    }
}

// DATA PARSING RELATED METHODS

char **parseCommand(char *commandParts[], char *command) {
    const char separator[SEPARATOR_SIZE] = ",";
    // https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    commandParts[0] = strtok(command, separator);
    int i = 0;
    while( commandParts[i] != nullptr) {
        i++;
        commandParts[i] = strtok(nullptr, separator);
    }
    return commandParts;
}

int isGlobalAttribute(const char *data, int currentIndex) {
    while (data[currentIndex] != '{' && data[currentIndex] != ';') {
        currentIndex++;
    }
    if (data[currentIndex] == '{')
        return 0;
    else if (data[currentIndex] == ';')
        return 1;
    return 0;
}

void dataParser(char *data, block *blocks, section *sections) {
    int currentIndex = 0;
    while (data[currentIndex] != '\0') {
        int globalAttribute = isGlobalAttribute(data, currentIndex);
        // Logic for global attribute.
        if (globalAttribute) {
            currentIndex++;
            continue;
        }
        // Logic for a section.
        else {
            sections = addSection(blocks, sections);
            parseSelectors(data, &currentIndex, sections);
            parseAttributes(data, &currentIndex, sections);
        }
        currentIndex++;
    }
}

void parseSelectors(char *data, int *currentIndex, section *sections) {
    int selectorsCount = countSelectors(data, *currentIndex);
    for (int i = 0; i < selectorsCount; i++) {
        char selectorName[INPUT_SIZE];
        int j = 0;
        while (data[*currentIndex] != ',' && data[*currentIndex] != '{') {
            selectorName[j] = data[*currentIndex];
            (*currentIndex)++;
            j++;
        }
        selectorName[j] = '\0';
        char selectorNameTrimmed[INPUT_SIZE];
        strcpy(selectorNameTrimmed, trimSpaces(selectorName));
        // Add selector to the list.
        section *lastSection;
        lastSection = getLast<section>(sections);

        selector *newSelector;
        newSelector = createSelectorNode();

        strcpy(newSelector->selectorName, selectorNameTrimmed);
        lastSection->selectorList = addLast<selector>(lastSection->selectorList, newSelector);
        cout << selectorNameTrimmed << endl;
        // Skip ',' or '{'.
        (*currentIndex)++;
    }
}

void parseAttributes(char *data, int *currentIndex, section *sections) {
    int attributesCount = countAttributes(data, *currentIndex);
    for (int i = 0; i < attributesCount; i++) {
        char attributeName[INPUT_SIZE];
        int j = 0;
        while (data[*currentIndex] != ':') {
            attributeName[j] = data[*currentIndex];
            (*currentIndex)++;
            j++;
        }
        attributeName[j] = '\0';
        char attributeNameTrimmed[INPUT_SIZE];
        strcpy(attributeNameTrimmed, trimSpaces(attributeName));
        // Add attribute name to the list.
        /// TODO: Put in function
        section *lastSection;
        lastSection = getLast<section>(sections);
        attribute *newAttribute;
        newAttribute = createAttributeNode();
        strcpy(newAttribute->attributeName, attributeNameTrimmed);
        // TODO:
        cout << attributeNameTrimmed << endl;
        // Skip ':'.
        (*currentIndex)++;
        char attributeValue[INPUT_SIZE];
        int k = 0;
        while (data[*currentIndex] != ';' && data[*currentIndex] != '}') {
            attributeValue[k] = data[*currentIndex];
            (*currentIndex)++;
            k++;
        }
        attributeValue[k] = '\0';
        char attributeValueTrimmed[INPUT_SIZE];
        strcpy(attributeValueTrimmed, trimSpaces(attributeValue));
        // Add attribute attributeValue to the list.
        strcpy(newAttribute->attributeValue, attributeValueTrimmed);
        lastSection->attributeList = addLast<attribute>(lastSection->attributeList, newAttribute);
        cout << attributeValueTrimmed << endl;
        // Skip ';'.
        if (i < attributesCount - 1)
            (*currentIndex)++;
        // Skip '}' and anything before.
        else {
            while(data[*currentIndex] != '}')
                (*currentIndex)++;
            (*currentIndex)++;
        }
    }
}

int countSelectors(const char *data, int currentIndex) {
    int selectorsCount = 1;
    while(data[currentIndex] != '{') {
        if (data[currentIndex] == ',')
            selectorsCount++;
        currentIndex++;
    }
    return selectorsCount;
}

int countAttributes(const char *data, int currentIndex) {
    int attributesCount = 0;
    while(data[currentIndex] != '}') {
        if (data[currentIndex] == ':')
            attributesCount++;
        currentIndex++;
    }
    return attributesCount;
}

// LIST RELATED METHODS

block *addBlock(block *blocks) {
    if (blocks == nullptr)
        blocks = createBlockNode();
    else {
        block *newBlock;
        newBlock = createBlockNode();
        blocks = addLast<block>(blocks, newBlock);
    }
    return blocks;
}

section *addSection(block *blocks, section *sections) {
    block *lastBLock;
    lastBLock = getLast<block>(blocks);
    if (sections == nullptr) {
        sections = createSectionNode();

        createSection(lastBLock, sections);
    }
    else {
        if (lastBLock->takenSections < SECTIONS_PER_BLOCK) {
            section *newSection;
            newSection = createSectionNode();
            sections = addLast<section>(sections, newSection);

            createSection(lastBLock, sections);
        }
        else {
            blocks = addBlock(blocks);
            block *newBlock;
            newBlock = lastBLock->next;

            section *newSection;
            newSection = createSectionNode();
            sections = addLast<section>(sections, newSection);

            createSection(newBlock, sections);
        }
    }
    return sections;
}

void createSection(block *lastBlock, section *sections) {
    attribute *attributes;
    attributes = createAttributeNode();
    selector *selectors;
    selectors = createSelectorNode();
    section *lastSection;
    lastSection = getLast<section>(sections);
    lastSection->attributeList = attributes;
    lastSection->selectorList = selectors;
    lastBlock->sectionArray[lastBlock->takenSections] = lastSection;
    lastBlock->takenSections++;
}

block *createBlockNode() {
    block *newBlock;
    newBlock = (block *)malloc(sizeof(block));
    newBlock->takenSections = 0;
    newBlock->prev = nullptr;
    newBlock->next = nullptr;
    return newBlock;
}

section *createSectionNode() {
    section *newSection;
    newSection = (section *)malloc(sizeof(section));
    newSection->selectorList = nullptr;
    newSection->attributeList = nullptr;
    newSection->prev = nullptr;
    newSection->next = nullptr;
    return newSection;
}

selector *createSelectorNode() {
    selector *newSelector;
    newSelector = (selector *)malloc(sizeof(selector));
    newSelector->prev = nullptr;
    newSelector->next = nullptr;
    return newSelector;
}

attribute *createAttributeNode() {
    attribute *newAttribute;
    newAttribute = (attribute *)malloc(sizeof(attribute));
    newAttribute->prev = nullptr;
    newAttribute->next = nullptr;
    return newAttribute;
}

template <typename type> void printList(type *firstNode) {
    type *temporary = firstNode;
    while (temporary != nullptr) {
        cout << temporary->data;
        temporary = temporary->next;
    }
}

template <typename type> int getListLength(type *firstNode) {
    int length = 0;
    type *temporary = firstNode;
    while (temporary != nullptr) {
        length += 1;
        temporary = temporary->next;
    }
    return length;
}

template <typename type> type getFirst(type *firstNode) {
    if (firstNode == nullptr)
        return nullptr;
    return firstNode;
}

template <typename type> type getAtPosition(type *firstNode, int position) {
    type *temporary = firstNode;
    while (temporary != nullptr) {
        if (position == 0)
            return temporary;
        position -= 1;
        temporary = temporary->next;
    }
    return nullptr;
}

template <typename type> type *getLast(type *firstNode) {
    if (firstNode == nullptr)
        return nullptr;
    type *temporary = firstNode;
    while (temporary->next != nullptr)
        temporary = temporary->next;
    return temporary;
}

template <typename type> type addFirst(type *firstNode, type *newNode) {
    newNode->prev = nullptr;
    newNode->next = firstNode;
    if (firstNode != nullptr)
        firstNode->prev = newNode;
    return newNode;
}

template <typename type> type insertBefore(type *firstNode, type *newNode, type *node) {
    if (node == nullptr)
        return firstNode;
    newNode->next = node;
    newNode->prev = node->prev;
    node->prev = newNode;
    if (newNode->prev == nullptr)
        return newNode;
    newNode->prev->next = newNode;
    return firstNode;
}

template <typename type> void insertAfter(type *newNode, type *node) {
    if (node == nullptr)
        return;
    newNode->next = node->next;
    newNode->prev = node;
    if (node->next != nullptr)
        node->next->prev = newNode;
    node->next = newNode;
}

template <typename type> type *addLast(type *firstNode, type *newNode) {
    type *lastNode = getLast<type>(firstNode);
    newNode->prev = lastNode;
    if (lastNode == nullptr)
        return newNode;
    lastNode->next = newNode;
    return firstNode;
}

template <typename type> type removeFirst(type *firstNode) {
    return removeNode(firstNode, firstNode);
}

template <typename type> type removeNode(type *firstNode, type *node) {
    if (node == nullptr)
        return firstNode;
    if (node->next != nullptr)
        node->next->prev = node->prev;
    if (node->prev == nullptr)
        return node->next;
    node->prev->next = node->next;
    return firstNode;
}

template <typename type> void removeAfter(type *node) {
    if (node == nullptr)
        return;
    if (node->next == nullptr)
        return;
    node->next = node->next->next;
    if (node->next != nullptr)
        node->next->prev = node;
}

template <typename type> type removeLast(type *firstNode) {
    type *lastNode = getLast(firstNode);
    return removeNode(firstNode, lastNode);
}

template <typename type> void deleteList(type **headReference) {
    // https://www.geeksforgeeks.org/write-a-function-to-delete-a-linked-list/
    type *current = *headReference;
    type *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *headReference = NULL;
}

void deleteSelectorsAttributes(section *sections) {
    section *current = sections;
    section *next;
    while (current != nullptr) {
        next = current->next;
        deleteList<selector>(&(current->selectorList));
        deleteList<attribute>(&(current->attributeList));
        current = next;
    }
}

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

