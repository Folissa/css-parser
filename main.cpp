#include <iostream>
#include "main.h"

using namespace std;

// TODO: Make sure there are no duplicates for attributes in sections.
// TODO: Figure out global attributes.

int main() {
    char input[INPUT_SIZE];
    char *data = (char *) malloc(DATA_SIZE * sizeof(char));
    memset(data, '\0', DATA_SIZE);
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
                sections = dataParser(data, blocks, sections);
                parseData = 0;
                // After we parse all data into according data structures we can erase the data buffer.
                freeString(data, &currentIndex);
            }
            // Invoke the commands.
            if (strcmp(input, "?") == 0) {
                cout << "? == " << getListLength<section>(sections) << endl;
            }
            else {
                // Parse the command.
                char commandParts[COMMAND_SIZE][INPUT_SIZE];
                parseCommand(input, commandParts);

                // Then invoke it.
                if (strcmp(commandParts[SECOND_PART], "S") == 0) {
                    if (intOrString(commandParts[FIRST_PART]) == INT && strcmp(commandParts[THIRD_PART], "?") == 0) {
                        int sectionNumber = atoi(commandParts[FIRST_PART]) - 1;
                        section *requestedSection;
                        requestedSection = getAtPosition<section>(sections, sectionNumber);
                        if (requestedSection != nullptr)
                            cout << commandParts[FIRST_PART] << ",S,? == " << getListLength<selector>(requestedSection->selectorList) << endl;
                        else
                            continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == INT && intOrString(commandParts[THIRD_PART]) == INT) {
                        int sectionNumber = atoi(commandParts[FIRST_PART]) - 1;
                        int selectorNumber = atoi(commandParts[THIRD_PART]) - 1;
                        section *requestedSection;
                        selector *requestedSelector;
                        requestedSection = getAtPosition<section>(sections, sectionNumber);
                        if (requestedSection != nullptr)
                            requestedSelector = getAtPosition<selector>(requestedSection->selectorList, selectorNumber);
                        else
                            continue;
                        if (requestedSelector != nullptr)
                            cout << commandParts[FIRST_PART] << ",S," << commandParts[THIRD_PART] << " == " << requestedSelector->selectorName << endl;
                        else
                            continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == STRING && strcmp(commandParts[THIRD_PART], "?") == 0) {
                        cout << commandParts[FIRST_PART] << ",S,? == " << selectorCounter(sections, commandParts[FIRST_PART]) << endl;
                    }
                }
                else if (strcmp(commandParts[SECOND_PART], "A") == 0) {
                    if (intOrString(commandParts[FIRST_PART]) == INT && strcmp(commandParts[THIRD_PART], "?") == 0) {
                        int sectionNumber = atoi(commandParts[FIRST_PART]) - 1;
                        section *requestedSection;
                        requestedSection = getAtPosition<section>(sections, sectionNumber);
                        if (requestedSection != nullptr)
                            cout << commandParts[FIRST_PART] << ",A,? == " << getListLength<attribute>(requestedSection->attributeList) << endl;
                        else
                            continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == INT && intOrString(commandParts[THIRD_PART]) == STRING) {
                        int sectionNumber = atoi(commandParts[FIRST_PART]) - 1;
                        section *requestedSection;
                        requestedSection = getAtPosition<section>(sections, sectionNumber);
                        // TODO: Optimize this.
                        if (getAttributeValue(requestedSection, commandParts[THIRD_PART]) != nullptr)
                            cout << commandParts[FIRST_PART] << ",A," << commandParts[THIRD_PART] << " == " << getAttributeValue(requestedSection, commandParts[THIRD_PART]) << endl;
                        else
                            continue;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == STRING && strcmp(commandParts[THIRD_PART], "?") == 0) {
                        cout << commandParts[FIRST_PART] << ",A,? == " << attributeCounter(sections,commandParts[FIRST_PART]) << endl;
                    }
                }
                else if (strcmp(commandParts[SECOND_PART], "D") == 0) {
                    if (intOrString(commandParts[FIRST_PART]) == INT && strcmp(commandParts[THIRD_PART], "*") == 0) {
//                        int sectionNumber = atoi(commandParts[FIRST_PART]) - 1;
//                        section *requestedSection;
//                        requestedSection = getAtPosition<section>(sections, sectionNumber);
//                        sections = removeNode<section>(sections);
                        cout << commandParts[FIRST_PART] << ",D,* == deleted" << endl;
                    }
                    else if (intOrString(commandParts[FIRST_PART]) == INT && intOrString(commandParts[THIRD_PART]) == STRING) {
                        cout << commandParts[FIRST_PART] << ",D," << commandParts[THIRD_PART] << " == deleted" << endl;
                    }
                }
                else if (strcmp(commandParts[SECOND_PART], "E") == 0)
                    cout << commandParts[FIRST_PART] << ",E," << commandParts[THIRD_PART] << " == " << endl;
            }
        }
        // Append the data for parsing.
        else
            appendToBuffer(input, data, &sizeOfData, &currentIndex);
    }

    // Free the allocated memory.
    sections = deleteSelectorsAttributes(sections);
    deleteList<section>(&sections);
    deleteList<block>(&blocks);
    free(data);

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

void parseCommand(const char *input, char commandParts[][INPUT_SIZE]) {
    const char separator = ',';
    int commandIndex = 0;
    for (int j = 0; j < COMMAND_SIZE; j++) {
        int i = 0;
        char command[INPUT_SIZE];
        while (input[commandIndex] != separator && input[commandIndex] != '\0') {
            command[i] = input[commandIndex];
            commandIndex++;
            i++;
        }
        commandIndex++;
        command[i] = '\0';
        strcpy(commandParts[j], command);
    }
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

section *dataParser(char *data, block *blocks, section *sections) {
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
            sections = parseSelectors(data, &currentIndex, sections);
            sections = parseAttributes(data, &currentIndex, sections);
        }
        currentIndex++;
    }
    return sections;
}

section *parseSelectors(char *data, int *currentIndex, section *sections) {
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

        if (lastSection->selectorList == nullptr) {
            selector *selectors;
            selectors = createSelectorNode();
            lastSection->selectorList = selectors;

            strcpy(lastSection->selectorList->selectorName, selectorNameTrimmed);
        }

        else {
            selector *newSelector;
            newSelector = createSelectorNode();

            strcpy(newSelector->selectorName, selectorNameTrimmed);
            lastSection->selectorList = addLast<selector>(lastSection->selectorList, newSelector);
        }
        // Skip ',' or '{'.
        (*currentIndex)++;
    }
    return sections;
}

section *parseAttributes(char *data, int *currentIndex, section *sections) {
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
        // Add attribute name and value to the list.

        section *lastSection;
        lastSection = getLast<section>(sections);
        if (lastSection->attributeList == nullptr) {
            attribute *attributes;
            attributes = createAttributeNode();
            lastSection->attributeList = attributes;
            strcpy(lastSection->attributeList->attributeName, trimSpaces(attributeName));
            strcpy(lastSection->attributeList->attributeValue, trimSpaces(attributeValue));
        }
        else {
            attribute *newAttribute;
            newAttribute = createAttributeNode();

            strcpy(newAttribute->attributeName, trimSpaces(attributeName));
            strcpy(newAttribute->attributeValue, trimSpaces(attributeValue));
            lastSection->attributeList = addLast<attribute>(lastSection->attributeList, newAttribute);
        }
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
    return sections;
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

char *getAttributeValue(section *searchedSection, const char *attributeToFind) {
    section *temporary = searchedSection;
    while (temporary->attributeList != nullptr) {
        if (strcmp(temporary->attributeList->attributeName, attributeToFind) == 0) {
            return temporary->attributeList->attributeValue;
        }
        temporary->attributeList = temporary->attributeList->next;
    }
    return nullptr;
}

int attributeCounter(section *sections, const char *attributeToCount) {
    int count = 0;
    section *temporary = sections;
    while (temporary != nullptr) {
        while (temporary->attributeList != nullptr) {
            if (strcmp(temporary->attributeList->attributeName, attributeToCount) == 0) {
                count++;
            }
            temporary->attributeList = temporary->attributeList->next;
        }
        temporary = temporary->next;
    }
    return count;
}

int selectorCounter(section *sections, const char *selectorToCount) {
    int count = 0;
    section *temporary = sections;
    while (temporary != nullptr) {
        while (temporary->selectorList != nullptr) {
            if (strcmp(temporary->selectorList->selectorName, selectorToCount) == 0) {
                count++;
            }
            temporary->selectorList = temporary->selectorList->next;
        }
        temporary = temporary->next;
    }
    return count;
}

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

        sections = createSection(lastBLock, sections);
    }
    else {
        if (lastBLock->takenSections < SECTIONS_PER_BLOCK) {
            section *newSection;
            newSection = createSectionNode();
            sections = addLast<section>(sections, newSection);

            sections = createSection(lastBLock, sections);
        }
        else {
            blocks = addBlock(blocks);
            block *newBlock;
            newBlock = lastBLock->next;

            section *newSection;
            newSection = createSectionNode();
            sections = addLast<section>(sections, newSection);

            sections = createSection(newBlock, sections);
        }
    }
    return sections;
}

section *createSection(block *lastBlock, section *sections) {
    section *lastSection;
    lastSection = getLast<section>(sections);

//    lastSection->selectorList = nullptr;
//    lastSection->attributeList = nullptr;

    lastBlock->sectionArray[lastBlock->takenSections] = lastSection;
    lastBlock->takenSections++;
    return sections;
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

// TODO: Create one type for these two.
selector *createSelectorNode() {
    selector *newSelector;
    newSelector = (selector *)malloc(sizeof(selector));

    memset(newSelector->selectorName, '\0', INPUT_SIZE);
    newSelector->prev = nullptr;
    newSelector->next = nullptr;

    return newSelector;
}

attribute *createAttributeNode() {
    attribute *newAttribute;
    newAttribute = (attribute *)malloc(sizeof(attribute));

    memset(newAttribute->attributeName, '\0', INPUT_SIZE);
    memset(newAttribute->attributeValue, '\0', INPUT_SIZE);
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

void printBlocks(block *blocks) {
    block *temporary = blocks;
    while (temporary != nullptr) {
        cout << "BLOCK" << endl;
        cout << "Taken sections: " << temporary->takenSections << endl;
        temporary = temporary->next;
    }
}

void printSections(section *sections) {
    section *temporary = sections;
    while (temporary != nullptr) {
        cout << "SECTION" << endl;
        printSelectors(temporary->selectorList);
        printAttributes(temporary->attributeList);
        temporary = temporary->next;
    }
}

void printSelectors(selector *selectors) {
    selector *temporary = selectors;
    while (temporary != nullptr) {
        cout << "Selector:" << temporary->selectorName << endl;
        temporary = temporary->next;
    }
}

void printAttributes(attribute *attributes) {
    attribute *temporary = attributes;
    while (temporary != nullptr) {
        cout << "Attribute:" << temporary->attributeName << endl;
        cout << "Value:" << temporary->attributeValue << endl;
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

template <typename type> type *getAtPosition(type *firstNode, int position) {
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

section *deleteSelectorsAttributes(section *sections) {
    section *current = sections;
    section *next;
    while (current != nullptr) {
        next = current->next;
        deleteList<selector>(&(current->selectorList));
        deleteList<attribute>(&(current->attributeList));
        current = next;
    }
    return sections;
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

