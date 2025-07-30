//Tareq Ziad Abushaib 1212974
#include <stdio.h>
#include <stdlib.h>
#define max 200

struct ListNode{
    char* str;
    struct ListNode *next;
};

struct Queue{
    struct ListNode* front;
    struct ListNode* rear;
};

typedef struct ListNode* PtrToNode;
typedef PtrToNode Stack;


struct ListNode *MakeEmpty(struct ListNode *head) {
    if ( head != NULL)
        DeleteList(head);

    head = (struct ListNode *)malloc(sizeof(struct ListNode));
    if (head == NULL)
        printf("Memory allocation failed");
    head->next = NULL;
    return head;
}

void DeleteList(struct ListNode **head)
{
    struct ListNode* current = (*head)->next;
    struct ListNode* temp;

    while(current != NULL)
    {
        temp = current->next;
        free(current->str);
        free(current);
        current = temp;
    }
    free(current);

}

int isEmpty(struct ListNode *head) {
    return head->next == NULL;
}

int isLast(struct ListNode *head) {
    return head->next == NULL;
}
struct ListNode* newNodeList(struct ListNode *newNode ,char *str)
{
    newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    if(newNode == NULL)
    {
        printf("Memory allocation failed");
        free(newNode);
    }

    newNode->str = (char*)malloc(sizeof(char)* sizeof(str) );
    if(newNode->str == NULL)
    {
        printf("Memory allocation failed");
        free(newNode->str);
    }

    strncpy(newNode->str , str , sizeof(str) );
    newNode->str[sizeof(str)] = '\0';
    newNode->next = NULL;
    return newNode;
}
void InserLastNode(struct ListNode **head , char* str)
{
    struct ListNode* newNode = newNodeList(newNode ,str);

    struct ListNode* current = *head;

    if(current == NULL)
    {
        current->next = newNode;
    }

    while(current->next != NULL)
        current = current->next;

    current->next = newNode;

}
void InsertMiddle(struct ListNode ** head  , char *str , char *strBefore)
{
    struct ListNode* newNode = newNodeList(newNode , str);
    struct ListNode* current = (*head)->next;
    if(strBefore == NULL)
    {
        newNode->next = current;
        (*head)->next = newNode;

    }
    else
    {
         struct ListNode* temp ;
        while(current->next != NULL)
        {
            temp = current;

            if(strcmp(current->str , strBefore) == 0 )
            {
                temp = current->next;
                current->next = newNode;
                newNode->next = temp;

            }
            current = current->next;
        }
    }

}
int Delete(struct ListNode** head ,Stack Undo , char *str)
{
    struct ListNode* current = (*head)->next;
    struct ListNode* temp = *head;

    while(current != NULL )
    {
        if( strcmp(current->str , str ) == 0)
        {
            temp->next = current->next;
            free(current->str);
            free(current);
            break;
        }
        temp = current;
        current = current->next;
    }

    if(current == NULL)
    {
        printf("String not found");
        return 1;
    }

    PushUndo(Undo , str);
}
//-------------------------------queue
struct Queue* CreateQueue(struct Queue *q)
{
    q = (struct Queue*)malloc(sizeof(struct Queue));
    if(q == NULL)
    {
        printf("Memory allocation failed");
        free(q);
    }
    q->front = NULL;
    q->rear = NULL;
    return q;
}
struct ListNode* InsertQueue( char *str)
{
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));

    if(newNode == NULL)
    {
        printf("Memory allocation failed");
        free(newNode);
    }
    newNode->str = (char*)malloc(sizeof(char)* sizeof(str) +1);
    if(newNode->str == NULL)
    {
        printf("Memory allocation failed");
        free(newNode->str);
    }
    strncpy(newNode->str , str , sizeof(str));
    newNode->str[sizeof(str)] = '\0';
    newNode->next = NULL;

    return newNode;

}

void Enque( struct Queue **q ,Stack Undo,struct ListNode* head , char* str , int num , char* strBefore)
{
    struct ListNode* current = InsertQueue(str);

    if((*q)->rear == NULL)
    {
        (*q)->front = current;
        (*q)->rear = current;
    }
    else
    {
        (*q)->rear->next = current;
        (*q)->rear = current;
    }

    if( num == -1)
    {
        InserLastNode(head  , str);
    }
    else
        InsertMiddle(head , str , strBefore);

    PushUndo(Undo, str);

}

void Dequeue(struct Queue **q )
{
    if(*q == NULL)
        printf("Queue is empty");

    struct ListNode* head = (*q)->front;
    struct ListNode* current= head->next;

    (*q)->front = current;
    free(head->str);
    free(head);

}
Stack CreateStack(Stack s )
{
    s = (Stack)malloc(sizeof(struct ListNode));

    if(s == NULL)
    {
        printf("Memory allocation failed");
        free(s);
    }

    s->next = NULL;
    return s;
}
Stack newStackNode(Stack newNode, int sizeStr)
{
    newNode = (Stack)malloc(sizeof(struct ListNode));
    if(newNode == NULL)
    {
        printf("Memory allocation failed");
        free(newNode);
    }

    newNode->str = (char*)malloc(sizeof(char) * sizeStr );
    if(newNode->str == NULL)
    {
        printf("Memory allocation failed");
        free(newNode->str);
    }
    return newNode;
}
void PushUndo(Stack* Undo , char* str)
{
    Stack newNode ;
    int sizeStr = sizeof(str);
    newNode = newStackNode(newNode , sizeStr);

    strncpy(newNode->str , str , sizeStr);
    newNode->str[sizeStr] = '\0';


    newNode->next = *Undo;

    *Undo = newNode;


}
void PopUndo(Stack *Undo , Stack Redo)
{
    Stack current = *Undo;//takes the first node .
    Stack temp = current->next;//takes the second node
    int sizeStr = sizeof(current->str);//size of first node
    char str[sizeStr] ;

    strcpy(str , current->str);

    free(current->str);
    free(current);

    *Undo = temp;

    PushRedo(Redo , str);
}

void PushRedo(Stack* Redo , char* str)
{
    Stack newNode ;
    int sizeStr = sizeof(str);
    newNode = newStackNode(newNode , sizeStr);

    strncpy(newNode->str , str , sizeStr);
    newNode->str[sizeStr] = '\0';

    newNode->next = *Redo;

    *Redo = newNode;

}
void PopRedo(Stack* Redo , Stack Undo)
{
    Stack current = *Redo;
    Stack temp = current->next;
    int SizeStr = sizeof(current->str);

    char str[SizeStr] ;
    strcpy(str , current->str);

    free(current->str);
    free(current);

    *Redo = temp;

    PushUndo(Undo , str);
}
void LoadInputFile(struct Queue *q ,struct ListNode *head, Stack Undo)
{
    FILE *file = fopen("stack.txt", "r");
    if (file == NULL) {
        printf("Error opening file \n");
    }

    char line[max];
    char* token;

    while (fscanf(file , "%s" , line) == 1)
    {
       Enque(q , Undo , head , line , -1 , NULL);

    }

    fclose(file);

}
void printLinkedList(struct ListNode** head)
{
    struct ListNode *current = (*head)->next;

    while(current != NULL)
    {
        printf("%s " , current->str);
        current = current->next;
    }
}

void printStack(Stack s)
{
    Stack tempStack = CreateStack(NULL);

    while (s->next != NULL)
    {
        printf("%s\n", s->str);

        Stack tempNode = CreateStack(NULL);
        tempNode->str = s->str;
        tempNode->next = tempStack;
        tempStack = tempNode;

        s = s->next;
    }

    while (tempStack != NULL)
    {
        Stack tempNode = tempStack;
        tempStack = tempStack->next;

        tempNode->next = s;
        s = tempNode;
    }
}


void saveToFile(struct ListNode* head) {

    FILE *file = fopen("output.txt" , "w");

    struct ListNode *current = head->next;
    while (current != NULL) {
        fprintf(file, "%s ", current->str );
        current = current->next;
    }

    fclose(file);
}

void FreeList(struct ListNode** head) {
    struct ListNode* current = *head;

    while (current != NULL) {
        struct ListNode* temp = current;
        current = current->next;
        free(temp->str);
        free(temp);
    }
}

void FreeQueue(struct Queue* q) {
    struct ListNode* current = q->front;
    while (current != NULL) {
        struct ListNode* temp = current;
        current = current->next;
        free(temp->str);
        free(temp);
    }
    free(q);
}

int main()
{
    struct ListNode *head = MakeEmpty(NULL);
    struct Queue *q= CreateQueue(q);
    Stack Undo = CreateStack(NULL);
    Stack Redo = CreateStack(NULL);

    int choice;
    char str[max];
    char strBefore[max];
    int num;


    do {
        printf("\nMenu:\n");
        printf("1. Load the input file which contains the initial text.\n");
        printf("2. Print the loaded text.\n");
        printf("3. Insert strings to the text.\n");
        printf("4. Remove strings from the text. \n");
        printf("5. Perform Undo operation\n");
        printf("6. Perform Redo operation\n");
        printf("7. Print the Undo Stack and the Redo stack \n");
        printf("8. Save the updated text to the output file.\n");
        printf("9. Exit.\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                LoadInputFile(&q, &head,&Undo);
                printf("Text has been loaded successfully\n");
                break;
            case 2:
                printLinkedList(&head);
                break;
            case 3:
                printf("Please enter -1 if the string at the end \n");
                printf("Enter 0 if it's in the middle\n");
                printf("Enter 1 if it's in the beginning\n");
                scanf("%d" , &num);
                if(num == 0)
                {
                    printf("\nPlease enter the String");
                    scanf("%s" , str);
                    printf("Please enter the string before the string you want to add it\n");
                    scanf("%s", strBefore);
                    Enque(&q , &Undo ,&head , str , num , strBefore);
                }
                else{
                    printf("\nPlease enter the string");
                    scanf("%s" , str);
                    Enque(&q , &Undo ,&head , str , num , NULL);
                }
                printf("Insertion of string has been added successfully\n");
                break;
            case 4:
                printf("Please enter the string you Want to delete\n");
                scanf("%s" , str);
                num =Delete(&head , &Undo ,  str);
                if(num == 1)
                {
                    printf("\nDeletion of string has been done successfully");
                }
                break;
            case 5:
                PopUndo(&Undo , &Redo);
                break;
            case 6:
                PopRedo(&Redo , &Undo);
                break;
            case 7:
                printf("Undo Stack :\n");
                printStack(Undo);
                printf("-------------");
                printf("\n");
                printf("Redo Stack :\n");
                printStack(Redo);
                break;
            case 8:
                saveToFile(head);
                printf("The Text has been added to file");
                break;
            case 9:
                printf("Exit");
                break;
            default:
                printf("Invalid ! . try again.\n");
        }
    } while (choice != 9);

    DeleteList(&head);

    return 0;
}

