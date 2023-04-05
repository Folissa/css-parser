#include <iostream>
#include "main.h"

using namespace std;

int main() {
    char input[INPUT_SIZE];

    char *data = (char *) malloc(DATA_SIZE * sizeof(char));
    memset(data, '\0', DATA_SIZE);
    char *command = (char *) malloc(INPUT_SIZE * sizeof(char));
    memset(command, '\0', INPUT_SIZE);

    int sizeOfData = 0, currentIndex = 0, currentCommandIndex = 0, appendingAllowed = 1, parseData = 0;

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
                freeData(data, &currentIndex);
            }

            // Append command.
            appendCommand(input, command, &currentCommandIndex);

            if (strcmp(trimSpaces(command), "?") == 0) {
                freeData(command, &currentCommandIndex);
                cout << "? == " << getListLength<section>(sections) << endl;
                continue;
            }

            char commandParts[COMMAND_SIZE][INPUT_SIZE];
            int commandSize = countCommandSeparators(command);
            if (commandSize < 2)
                continue;
            else if (commandSize == 2) {
                // Parse the command.
                parseCommand(trimSpaces(command), commandParts);
            }
            else {
                freeData(command, &currentCommandIndex);
                continue;
            }
            freeData(command, &currentCommandIndex);
            if (strcmp(commandParts[SECOND_PART], "S") == 0) {
                if (intOrString(commandParts[FIRST_PART]) == INT && strcmp(commandParts[THIRD_PART], "?") == 0) {
                    int sectionNumber = atoi(commandParts[FIRST_PART]) - 1;
                    section *requestedSection;
                    requestedSection = getAtPosition<section>(sections, sectionNumber);
                    if (requestedSection != nullptr) {
                        cout << commandParts[FIRST_PART] << ",S,? == " << getListLength<selector>(requestedSection->selectorList) << endl;
                    }
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
                // TODO: Error here:
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
                    attribute *requestedAttribute;
                    if (requestedSection != nullptr) {
                        requestedAttribute = getAttribute(requestedSection, commandParts[THIRD_PART]);
                    }
                    else
                        continue;
                    if (requestedAttribute != nullptr)
                        cout << commandParts[FIRST_PART] << ",A," << commandParts[THIRD_PART] << " == " << requestedAttribute->attributeValue << endl;
                    else
                        continue;
                }
                else if (intOrString(commandParts[FIRST_PART]) == STRING && strcmp(commandParts[THIRD_PART], "?") == 0) {
                    cout << commandParts[FIRST_PART] << ",A,? == " << attributeCounter(sections,commandParts[FIRST_PART]) << endl;
                }
            }
            else if (strcmp(commandParts[SECOND_PART], "D") == 0) {
                // TODO: Error here:
                if (intOrString(commandParts[FIRST_PART]) == INT && strcmp(commandParts[THIRD_PART], "*") == 0) {
                    int sectionNumber = atoi(commandParts[FIRST_PART]) - 1;
                    section *requestedSection;
                    requestedSection = getAtPosition<section>(sections, sectionNumber);
                    int sectionsLengthBefore = getListLength<section>(sections);
                    sections = removeSectionNode(sections, requestedSection);
                    int sectionsLengthAfter = getListLength<section>(sections);
                    if (sectionsLengthBefore == sectionsLengthAfter) {
                        continue;
                    }
                    else {
                        blocks->takenSections--;
                        removeLastBlockNode(blocks);
                        cout << commandParts[FIRST_PART] << ",D,* == deleted" << endl;
                    }
                }
                else if (intOrString(commandParts[FIRST_PART]) == INT && intOrString(commandParts[THIRD_PART]) == STRING) {
                    int sectionNumber = atoi(commandParts[FIRST_PART]) - 1;
                    section *requestedSection;
                    requestedSection = getAtPosition<section>(sections, sectionNumber);
                    attribute *requestedAttribute;
                    if (requestedSection != nullptr) {
                        requestedAttribute = getAttribute(requestedSection, commandParts[THIRD_PART]);
                    }
                    else
                        continue;
                    if (requestedAttribute != nullptr) {
                        int attributeListLengthBefore = getListLength<attribute>(requestedSection->attributeList);
                        requestedSection->attributeList = removeAttributeNode(requestedSection, requestedAttribute);
                        int attributeListLengthAfter = getListLength<attribute>(requestedSection->attributeList);
                        if (attributeListLengthBefore == attributeListLengthAfter) {
                            continue;
                        }
                        else {
                            if (requestedSection->attributeList == nullptr) {
                                int sectionsLengthBefore = getListLength<section>(sections);
                                sections = removeSectionNode(sections, requestedSection);
                                int sectionsLengthAfter = getListLength<section>(sections);
                                if (sectionsLengthAfter == sectionsLengthBefore) {
                                    continue;
                                }
                                else {
                                    blocks->takenSections--;
                                    removeLastBlockNode(blocks);
                                }
                            }
                            cout << commandParts[FIRST_PART] << ",D," << commandParts[THIRD_PART] << " == deleted" << endl;
                        }
                    }
                    else
                        continue;
                }
            }
            else if (strcmp(commandParts[SECOND_PART], "E") == 0) {
                char attributeValue[INPUT_SIZE];
                if (getAttributeValueBySelector(sections, commandParts[FIRST_PART], commandParts[THIRD_PART]) != nullptr)
                    strcpy(attributeValue, getAttributeValueBySelector(sections, commandParts[FIRST_PART], commandParts[THIRD_PART]));
                else
                    continue;
                if (getAttributeValueBySelector(sections, commandParts[FIRST_PART], commandParts[THIRD_PART]) != nullptr)
                    cout << commandParts[FIRST_PART] << ",E," << commandParts[THIRD_PART] << " == " << attributeValue << endl;
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
    free(command);

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

void freeData(char *data, int *length) {
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

void appendCommand(const char *input, char *command, int *currentIndex) {
    int j = 0;
    if (*currentIndex - 1 > 0 && command[*currentIndex - 1] != ',') {
        command[*currentIndex] = ' ';
        (*currentIndex) ++;
    }
    while (input[j] != '\0') {
        command[*currentIndex] = input[j];
        (*currentIndex)++;
        j++;
    }
}

// DATA PARSING RELATED METHODS

int countCommandSeparators(const char *input) {
    const char separator = ',';
    int i = 0, count = 0;
    while (input[i] != '\0') {
        if (input[i] == separator)
            count++;
        i++;
    }
    return count;
}

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
    if (data[currentIndex] == '{')
        return 1;
    return 0;
}

section *dataParser(char *data, block *blocks, section *sections) {
    int currentIndex = 0;
    while (data[currentIndex] != '\0') {
        int globalAttribute = isGlobalAttribute(data, currentIndex);
        // Logic for global attribute.
        if (globalAttribute) {
            sections = addSection(blocks, sections);
            sections = parseAttributes(data, &currentIndex, sections);
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
            selector *existingSelector;
            existingSelector = getSelector(lastSection, selectorNameTrimmed);
            if (existingSelector == nullptr) {
                strcpy(newSelector->selectorName, selectorNameTrimmed);
                lastSection->selectorList = addLast<selector>(lastSection->selectorList, newSelector);
            }
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
        char attributeNameTrimmed[INPUT_SIZE];
        strcpy(attributeNameTrimmed, trimSpaces(attributeName));
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
        // Add attribute name and value to the list.

        section *lastSection;
        lastSection = getLast<section>(sections);
        if (lastSection->attributeList == nullptr) {
            attribute *attributes;
            attributes = createAttributeNode();
            lastSection->attributeList = attributes;
            strcpy(lastSection->attributeList->attributeName, attributeNameTrimmed);
            strcpy(lastSection->attributeList->attributeValue, attributeValueTrimmed);
        }
        else {
            attribute *newAttribute;
            newAttribute = createAttributeNode();
            attribute *existingAttribute;
            existingAttribute = getAttribute(lastSection, attributeNameTrimmed);
            if (existingAttribute != nullptr) {
                lastSection->attributeList = removeAttributeNode(lastSection, existingAttribute);
                strcpy(newAttribute->attributeName, attributeNameTrimmed);
                strcpy(newAttribute->attributeValue, attributeValueTrimmed);
                lastSection->attributeList = addLast<attribute>(lastSection->attributeList, newAttribute);
            }
            else {
                strcpy(newAttribute->attributeName, attributeNameTrimmed);
                strcpy(newAttribute->attributeValue, attributeValueTrimmed);
                lastSection->attributeList = addLast<attribute>(lastSection->attributeList, newAttribute);
            }
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

char *getAttributeValueBySelector(section *sections, const char *selectorToFind, const char *attributeToFind) {
    int sectionsLength = getListLength<section>(sections);
    for (int i = sectionsLength - 1; i >= 0; i--) {
        section *lastSection;
        lastSection = getAtPosition<section>(sections, i);
        selector *lastSectionSelectors;
        lastSectionSelectors = lastSection->selectorList;

        int selectorsLength = getListLength<selector>(lastSectionSelectors);
        for (int j = selectorsLength - 1; j >= 0; j--) {
            selector *lastSelector;
            lastSelector = getAtPosition<selector>(lastSectionSelectors, j);
            if (strcmp(lastSelector->selectorName, selectorToFind) == 0) {
                attribute *lastSectionAttributes;
                lastSectionAttributes = lastSection->attributeList;
                int attributesLength = getListLength<attribute>(lastSectionAttributes);
                for (int k = attributesLength - 1; k >= 0; k--) {
                    attribute *lastAttribute;
                    lastAttribute = getAtPosition<attribute>(lastSectionAttributes, k);
                    if (strcmp(lastAttribute->attributeName, attributeToFind) == 0) {
                        return lastAttribute->attributeValue;
                    }
                    if (k == 0 && strcmp(lastAttribute->attributeName, attributeToFind) != 0) {
                        return nullptr;
                    }
                }
            }
        }
    }
    return nullptr;

}

attribute *getAttribute(section *searchedSection, const char *attributeToFind) {
    section *temporary = searchedSection;
    attribute *temporaryAttributeList = temporary->attributeList;
    while (temporaryAttributeList != nullptr) {
        if (strcmp(temporaryAttributeList->attributeName, attributeToFind) == 0) {
            return temporaryAttributeList;
        }
        temporaryAttributeList = temporaryAttributeList->next;
    }
    return nullptr;
}

selector *getSelector(section *searchedSection, const char *selectorToFind) {
    section *temporary = searchedSection;
    selector *temporarySelectorList = temporary->selectorList;
    while (temporarySelectorList != nullptr) {
        if (strcmp(temporarySelectorList->selectorName, selectorToFind) == 0) {
            return temporarySelectorList;
        }
        temporarySelectorList = temporarySelectorList->next;
    }
    return nullptr;
}

int attributeCounter(section *sections, const char *attributeToCount) {
    int count = 0;
    section *temporary = sections;
    while (temporary != nullptr) {
        attribute *temporaryAttributeList = temporary->attributeList;
        while (temporaryAttributeList != nullptr) {
            if (strcmp(temporaryAttributeList->attributeName, attributeToCount) == 0) {
                count++;
            }
            temporaryAttributeList = temporaryAttributeList->next;
        }
        temporary = temporary->next;
    }
    return count;
}

int selectorCounter(section *sections, const char *selectorToCount) {
    int count = 0;
    section *temporary = sections;
    while (temporary != nullptr) {
        selector *temporarySelectorList = temporary->selectorList;
        while (temporarySelectorList != nullptr) {
            if (strcmp(temporarySelectorList->selectorName, selectorToCount) == 0) {
                count++;
            }
            temporarySelectorList = temporarySelectorList->next;
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
            addBlock(blocks);
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

template <typename type> int getListLength(type *firstNode) {
    int length = 0;
    type *temporary = firstNode;
    while (temporary != nullptr) {
        length += 1;
        temporary = temporary->next;
    }
    return length;
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

template <typename type> type *addLast(type *firstNode, type *newNode) {
    type *lastNode = getLast<type>(firstNode);
    newNode->prev = lastNode;
    if (lastNode == nullptr)
        return newNode;
    lastNode->next = newNode;
    return firstNode;
}

section *removeSectionNode(section *sections, section *sectionToDelete) {
    if (sectionToDelete == nullptr)
        return sections;
    if (sectionToDelete->next != nullptr)
        sectionToDelete->next->prev = sectionToDelete->prev;
    if (sectionToDelete->prev == nullptr) {
        deleteList<selector>(&(sectionToDelete->selectorList));
        deleteList<attribute>(&(sectionToDelete->attributeList));
        return sectionToDelete->next;
    }
    sectionToDelete->prev->next = sectionToDelete->next;
    deleteList<selector>(&(sectionToDelete->selectorList));
    deleteList<attribute>(&(sectionToDelete->attributeList));
    return sections;
}

attribute *removeAttributeNode(section *section, attribute *attributeToDelete) {
    if (attributeToDelete == nullptr)
        return section->attributeList;
    if (attributeToDelete->next != nullptr)
        attributeToDelete->next->prev = attributeToDelete->prev;
    if (attributeToDelete->prev == nullptr) {
        memset(attributeToDelete->attributeName, '\0', INPUT_SIZE);
        memset(attributeToDelete->attributeValue, '\0', INPUT_SIZE);
        return attributeToDelete->next;
    }
    memset(attributeToDelete->attributeName, '\0', INPUT_SIZE);
    memset(attributeToDelete->attributeValue, '\0', INPUT_SIZE);
    attributeToDelete->prev->next = attributeToDelete->next;
    return section->attributeList;
}

block *removeLastBlockNode(block *blocks) {
    block *lastBlock = getLast<block>(blocks);
    if (lastBlock->takenSections == 0) {
        if (lastBlock->next != nullptr)
            lastBlock->next->prev = lastBlock->prev;
        if (lastBlock->prev == nullptr)
            return lastBlock->next;
        lastBlock->prev->next = lastBlock->next;
        return blocks;
    }
    return blocks;
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