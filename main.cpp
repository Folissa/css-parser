#include <iostream>
#include "main.h"

using namespace std;


int main() {

    char input[INPUT_SIZE];
    char *data = (char *) malloc(DATA_SIZE * sizeof(char));
    int sizeOfData = 0, currentIndex = 0, appendingAllowed = 1, parseData = 0;

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
                printString(data);
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
//                int partsTypes[COMMAND_SIZE];
//                recognizePartsTypes(commandParts, partsTypes);

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

    free(data);

    // Initialize data blocks and sections.
//    block *blocks;
//    blocks = (block *) malloc(sizeof(block));
//    section *sections;
//    sections = (section *) malloc(sizeof(section));
//    blocks->sectionArray[0] = sections;
//    blocks->takenSections++;

    // Free the allocated memory.
//    free(sections);
//    free(blocks);

    return 0;
}

// STRING RELATED METHODS

//void recognizePartsTypes(char *commandParts[], int partsTypes[]) {
//    int i = 0;
//    while (i < COMMAND_SIZE) {
//        partsTypes[i] = intOrString(commandParts[i]);
//        i++;
//    }
//}

int intOrString(const char *data) {
    int i = 0;
    while(data[i] != '\0') {
        if (!('0' <= data[i] && data[i] <= '9'))
            return 1;
        i++;
    }
    return 0;
}

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

void freeString(char *data, int *currentIndex) {
    memset(data, '\0', *currentIndex);
    *currentIndex = 0;
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

//void parseData(block *blocks, section *sections, char *data, int size) {
//    int currentIndex = 0;
//    if (!isGlobalAttribute(currentIndex, data)) {
//        if (getLast<block>(blocks) == nullptr) {
//
//        }
//        else if (getLast<block>(blocks)->takenSections <= SECTIONS_PER_BLOCK) {
//
//        }
//    }
//    else {
//
//    }
//}

// LIST RELATED METHODS

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

template <typename type> type addLast(type *firstNode, type *newNode) {
    type *lastNode = getLast(firstNode);
    newNode->prev = lastNode;
    if (lastNode == nullptr)
        return newNode;
    lastNode->next = newNode;
    return  firstNode;
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

