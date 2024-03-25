/* UE22CS252A - Data Structures and its Applications.
   Hackathon - Problem Statement: Building a simple message application.
   version 4.3 
   Author: Akilan Elango (PES1UG22CS061) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>	// Including necessary header files.

#define INITIAL_CAPACITY 10 // Defining initial capacity of the message box.

typedef struct Message {
    char sender[50];
    char recipient[50];
    char *content;
    time_t timestamp;
    struct Message *next;
}Message;	// Structure to define a message.

struct MessageBox {
    struct Message *messages;
    size_t size;
    size_t capacity;
}MessageBox;	// Structure to define a message box.

Message *createMessage(const char *sender, const char *recipient, const char *content) {
    Message *newMessage = (Message *)malloc(sizeof(Message));	// Allocating memory to a new message.
    if (newMessage == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }	// Error message if memory allocation has failed.

    strncpy(newMessage->sender, sender, sizeof(newMessage->sender) - 1);
    strncpy(newMessage->recipient, recipient, sizeof(newMessage->recipient) - 1);
    newMessage->content = strdup(content);	// Inputting information of sender, recipient and content.

    if (newMessage->content == NULL) {
        perror("Memory allocation failed");
        free(newMessage);
        exit(EXIT_FAILURE);
    }	// Error message if there is no content.

    newMessage->timestamp = time(NULL);
    newMessage->next = NULL;

    return newMessage;
}	// Function to create a new message.

MessageBox *initializeMessageBox() {
    MessageBox *box = (MessageBox *)malloc(sizeof(MessageBox));	// Allocating memory to a message box.
    if (box == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }	// Error message if memory allocation has failed.

    box->messages = (Message *)malloc(sizeof(Message) * INITIAL_CAPACITY);	// Creating an array of messages in the message box.
    if (box->messages == NULL) {
        perror("Memory allocation failed");
        free(box);
        exit(EXIT_FAILURE);
    }	// Error message if the message array wasn't created.

    box->size = 0;
    box->capacity = INITIAL_CAPACITY;	// Declaring capacity of the message box.

    return box;
}	// Function to initialize a message box.

void resizeMessageBox(MessageBox *box) {
    size_t newCapacity = box->capacity * 2;	// New capacity is double the initial capacity.
    Message *newMessages = (Message *)realloc(box->messages, sizeof(Message) * newCapacity);	// Reallocating message box capacity.

    if (newMessages == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }	// Error message if reallocation has failed.

    box->messages = newMessages;
    box->capacity = newCapacity;	// Redeclaring capacity of the message box.
}	// Function to resize the message box.

void freeMessages(MessageBox *box) {
    for (size_t i = 0; i < box->size; i++) {
        free(box->messages[i].content);
    }

    free(box->messages);
    free(box);
}	// Function to free memory from message box.

void sendMessage(MessageBox *box) {
    char sender[50];
    char recipient[50];
    char content[256];

    printf("Enter sender's name: ");
    scanf("%s", sender);	// Taking sender's name.

    printf("Enter recipient's name: ");
    scanf("%s", recipient);		// Taking recepient's name.

    printf("Enter message content: ");
    scanf(" %[^\n]s", content);	// Taking the message.

    if (box->size == box->capacity) {
        resizeMessageBox(box);
    }	// Resizing the message box if more than ten messages have been sent.

    box->messages[box->size++] = *createMessage(sender, recipient, content);
}	// Function to send messages.

void displayMessages(MessageBox *box) {
    printf("\n----- Messages -----\n");

    for (size_t i = 0; i < box->size; i++) {
        printf("Sender: %s\nRecipient: %s\nContent: %s\nTimestamp: %s\n\n",
               box->messages[i].sender, box->messages[i].recipient, box->messages[i].content,
               ctime(&box->messages[i].timestamp));
    }	// Printing all the sent messages with each message's timestamp.
}	// Function to display the messages.


int compareTimestamps(const void *a, const void *b) {
    return ((Message *)b)->timestamp - ((Message *)a)->timestamp;
}	// Function to compare timestamps.

void sortMessagesByTimestamp(sMessageBox *box) {
    qsort(box->messages, box->size, sizeof(struct Message), compareTimestamps);
}	// Function to sort and display messages in chronological order.

void filterMessagesBySender(MessageBox *box, const char *sender) {
    printf("\n----- Messages from %s -----\n", sender);

    for (size_t i = 0; i < box->size; i++) {
        if (strcmp(box->messages[i].sender, sender) == 0) {
            printf("Sender: %s\nRecipient: %s\nContent: %s\nTimestamp: %s\n\n",
                   box->messages[i].sender, box->messages[i].recipient, box->messages[i].content,
                   ctime(&box->messages[i].timestamp));
        }
    }
}	// Function to filter messages by sender.

int main() {
    MessageBox *messageBox = initializeMessageBox();	// Initializing a message box.

    int choice;	// Flag variable.
    do {
        printf("1. Send a message\n");
        printf("2. Display messages\n");
        printf("3. Sort messages by timestamp\n");
        printf("4. Filter messages by sender\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sendMessage(messageBox);
                break;
            case 2:
                displayMessages(messageBox);
                break;
            case 3:
                sortMessagesByTimestamp(messageBox);
                printf("Messages sorted by timestamp.\n");
                break;
            case 4: {
                char senderToFilter[50];
                printf("Enter sender's name to filter: ");
                scanf("%s", senderToFilter);
                filterMessagesBySender(messageBox, senderToFilter);
                break;
            }
            case 5:
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);	// Displaying a main menu and calling the previous functions based on user choice.

    freeMessages(messageBox);	// Freeing memory from the message box.

    return 0;
}
