//Michelle Maquilon
//N01564103
//Assignment 7


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 50
#define MAX_WORD_LENGTH 30
#define MAX_FORBIDDEN_WORDS 100
#define MAX_TIME_LENGTH 8

typedef struct {
    char word[MAX_WORD_LENGTH];
} Dictionary;

typedef struct {
    char forbiddenWord[MAX_WORD_LENGTH];
} ForbiddenWords;

typedef struct {
    int hour;
    int minute;
    char period[2];
} Time;

typedef struct {
    int numWords;
    char message[MAX_WORDS][MAX_WORD_LENGTH];
} TextMessage;

// Function to clone a struct
TextMessage cloneTextMessage(const TextMessage* source) {
    TextMessage destination;  // Create a new instance of the struct
    // Use memcpy to copy the entire struct
    memcpy(&destination, source, sizeof(TextMessage));
    return destination;
}

int isForbiddenWord(char* word, ForbiddenWords* forbiddenWords, int numForbiddenWords) {
    for (int i = 0; i < numForbiddenWords; ++i) {
        if (strcmp(word, forbiddenWords[i].forbiddenWord) == 0) {
            return 1; // It's a forbidden word
        }
    }
    return 0; // It's not a forbidden word
}

int isValidWord(char* word, Dictionary* dictionary, int numWords) {
    for (int i = 0; i < numWords; ++i) {
        if (strcmp(word, dictionary[i].word) == 0) {
            return 1; // It's a valid word
        }
    }
    return 0; // It's not a valid word
}

void toLowerCase(char* str) {
    for (int i = 0; str[i]; ++i) {
        str[i] = tolower(str[i]);
    }
}


void processTextMessages(FILE* file) {
    int numWords, numForbiddenWords, numMessages;
    fscanf(file, "%d", &numWords);

    Dictionary dictionary[numWords];
    
    for (int i = 0; i < numWords; ++i) {
        fscanf(file, "%s", dictionary[i].word);
    }

    fscanf(file, "%d", &numForbiddenWords);

    ForbiddenWords forbiddenWords[numForbiddenWords];

    for (int i = 0; i < numForbiddenWords; ++i) {
        fscanf(file, "%s", forbiddenWords[i].forbiddenWord);
    }

    fscanf(file, "%d", &numMessages);

    for (int i = 0; i < numMessages; ++i) {
        printf("Message #%d: ", i + 1);

        Time messageTime;
        fscanf(file, "%d:%d %s", &messageTime.hour, &messageTime.minute, messageTime.period);

        TextMessage textMessage;

        fscanf(file, "%d", &textMessage.numWords);
        for (int j = 0; j < textMessage.numWords; ++j) {
            fscanf(file, "%s", textMessage.message[j]);
        }

        TextMessage clonedTextMessage = cloneTextMessage(&textMessage);

        if ((strcmp(messageTime.period, "AM") == 0 && (messageTime.hour >= 1 && messageTime.hour <= 6))) {
            int consecutiveLoveYou = 0;
            int misspelledWords = 0;
            int containsForbiddenWord = 0;

            for (int j = 0; j < textMessage.numWords; ++j) {
                toLowerCase(textMessage.message[j]);
            }

            for (int j = 0; j < textMessage.numWords; ++j) {
                if (j < textMessage.numWords - 2 &&
                    strcmp(textMessage.message[j], "i") == 0 &&
                    strcmp(textMessage.message[j + 1], "love") == 0 &&
                    strcmp(textMessage.message[j + 2], "you") == 0) {
                    consecutiveLoveYou = 1;
                    break;
                }
            }

            for (int j = 0; j < textMessage.numWords; ++j) {
                if (!isValidWord(textMessage.message[j], dictionary, numWords)) {
                    misspelledWords++;
                }
            }

            for (int j = 0; j < textMessage.numWords; ++j) {
                if (isForbiddenWord(textMessage.message[j], forbiddenWords, numForbiddenWords)) {
                    containsForbiddenWord = 1;
                    break;
                }
            }

            if (consecutiveLoveYou || misspelledWords >= 3 || containsForbiddenWord) {
                printf("FAILED TO SEND.\n");
            } else {
                for (int j = 0; j < textMessage.numWords; ++j) {
                    printf("%s", clonedTextMessage.message[j]);
                    if (j < textMessage.numWords - 1) {
                        printf(" ");
                    }
                }
                printf("\n");
            }
        } else {
            for (int j = 0; j < textMessage.numWords; ++j) {
                printf("%s", clonedTextMessage.message[j]);
                if (j < textMessage.numWords - 1) {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }
}

int main() {
    FILE* file = fopen("textmsg.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    processTextMessages(file);

    fclose(file);
    return 0;
}