//p1_1212974_TareqziadAbushaib_3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max 51
#define Max_Inside 20

struct ListNode {
    char *town;
    int population;
    struct ListNode *prev;
    struct ListNode *next;
};

struct districtNode {
    char *name;
    int sum;
    struct districtNode *prev;
    struct districtNode *next;
    struct ListNode *headTown;
};

void DeleteList(struct districtNode *head) {
    struct districtNode *p, *temp;
    p = head->next;
    head->next = NULL;
    while (p != NULL) {
        temp = p->next;
        free(p->name);
        struct ListNode *currentTown = p->headTown;
        while (currentTown != NULL) {
            struct ListNode *tempTown = currentTown;
            currentTown = currentTown->next;
            free(tempTown->town);
            free(tempTown);
        }
        free(p);
        p = temp;
    }
}

struct districtNode *MakeEmpty(struct districtNode *head) {
    if (head != NULL)
        DeleteList(head);

    head = (struct districtNode *)malloc(sizeof(struct districtNode));
    if (head == NULL)
        printf("Memory allocation failed");
    head->next = NULL;
    return head;
}

int isEmpty(struct districtNode *head) {
    return head->next == NULL;
}

int isLast(struct districtNode *head, struct districtNode *p) {
    return p->next == NULL;
}

struct districtNode *FindDistrict(struct districtNode *head, char distName[]) {
    struct districtNode *current = head->next;
    while (current != NULL && strcmp(current->name, distName) != 0) {
        current = current->next;
    }
    return current;
}

struct ListNode *FindTown(struct districtNode *head, char townName[]) {
    struct districtNode *current = head;
    struct ListNode *currentTown = current->headTown;
    while (currentTown != NULL && strcmp(currentTown->town, townName) != 0) {
        currentTown = currentTown->next;
    }
    return currentTown;
}

struct districtNode *Insert(struct districtNode *head, char distName[]) {
    struct districtNode *current = head;
    struct districtNode *newNode = (struct districtNode *)malloc(sizeof(struct districtNode));

    newNode->name = (char *)malloc(sizeof(char) * Max_Inside);
    if (newNode == NULL || newNode->name == NULL) {
        printf("Memory allocation failed");
        if (newNode != NULL) free(newNode->name);
        free(newNode);
        return NULL;
    }
    strcpy(newNode->name, distName);
    newNode->sum = 0;
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->headTown = NULL;

    while (current->next != NULL)
        current = current->next;

    current->next = newNode;
    newNode->prev = current;

    return newNode;
}

void InsertTown(struct districtNode *head, char townName[], int population) {
    struct ListNode *current = head->headTown;
    struct ListNode *newNode = (struct ListNode *)malloc(sizeof(struct ListNode));
    if (newNode == NULL) {
        printf("Memory allocation failed");
        return;
    }

    newNode->town = (char *)malloc(sizeof(char) * Max_Inside);
    if (newNode->town == NULL) {
        printf("Memory allocation failed");
        free(newNode);
        return;
    }

    strncpy(newNode->town, townName, Max_Inside);
    newNode->population = population;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (current == NULL) {
        head->headTown = newNode;
    } else {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->prev = current;
    }
    head->sum += population;
}

void LoadInputFile(struct districtNode *head, char **dynamicarray, int *i) {
    FILE *file = fopen("district.txt", "r");
    char *str;
    if (file == NULL) {
        printf("Error opening file \n");
        return;
    }
    char line[50];
    int j = 0;
    while (fgets(line, Max, file) != NULL && j < 26) {
        dynamicarray[j] = (char *)malloc(sizeof(char) * Max);
        if (dynamicarray[j] == NULL)
            printf("Memory allocation failed");
        strcpy(dynamicarray[j], line);
        j++;
    }
    fclose(file);
    *i = j;

    char array[26][Max];
    int size = 0;
    for (int z = 0; z < j; z++) {
        size = 0;
        strcpy(array[z], dynamicarray[z]);
        char *str = strtok(array[z], "|");
        char townName[Max];
        char districtName[Max];
        int population;

        while (str != NULL) {
            size++;
            if (size == 1)
                strcpy(districtName, str);
            else if (size == 2)
                strcpy(townName, str);
            else if (size == 3)
                population = atoi(str);

            str = strtok(NULL, "|");
        }
        struct districtNode *current = FindDistrict(head, districtName);
        if (current == NULL) {
            struct districtNode *newNode = Insert(head, districtName);
            InsertTown(newNode, townName, population);
        } else {
            InsertTown(current, townName, population);
        }
    }
}

void countingSort(char arr[][Max_Inside], int size, int pos) {
    int count[52] = {0};
    char sorted[26][Max_Inside];

    for (int i = 0; i < size; i++) {
        char ch = arr[i][pos];
        int index = 0;
        if (ch >= 'a' && ch <= 'z') {
            index = ch - 'a' + 26;
        } else if (ch >= 'A' && ch <= 'Z') {
            index = ch - 'A';
        }
        count[index]++;
    }

    for (int i = 1; i < 52; i++) {
        count[i] = count[i] + count[i - 1];
    }

    for (int i = 0; i < size; i++) {
        char ch = arr[i][pos];
        int index = 0;
        if (ch >= 'a' && ch <= 'z') {
            index = ch - 'a' + 26;
        } else if (ch >= 'A' && ch <= 'Z') {
            index = ch - 'A';
        }
        strcpy(sorted[count[index] - 1], arr[i]);
        count[index]--;
    }

    for (int i = 0; i < size; i++) {
        strcpy(arr[i], sorted[i]);
    }
}

void RadixSort(struct districtNode *head, struct districtNode *newhead) {
    char array[26][Max_Inside];

    struct districtNode *current = head->next;

    int i = 0;
    while (current != NULL && i < 26) {
        strcpy(array[i], current->name);
        i++;
        current = current->next;
    }
    int size = i;
    int MaxLen = getMax(array, size);
    for (int i = MaxLen - 1; i >= 0; i--) {
        countingSort(array, size, i);
    }

    current = head->next;
    newLinkedList(current, newhead, array, size);
}

int getMax(char array[][Max_Inside], int size) {
    int Maxlen = 0;

    for (int i = 0; i < size; i++) {
        int len = countWithNospaces(array[i]);
        if (len > Maxlen)
            Maxlen = len;
    }
    return Maxlen;
}

int countWithNospaces(char *str) {
    int count = 0;
    while (*str != '\0') {
        if (*str != ' ') {
            count++;
        }
        str++;
    }
    return count;
}

void swap(struct ListNode *p1, struct ListNode *p2) {
    struct ListNode *temp = p1->prev;
    struct ListNode *current = p1->next;

    p2 = p2->next;
    p1->next = p2;
    if (p2 != NULL)
        p2->prev = p1;

    current->next = p1;
    current->prev = temp;
    if (temp != NULL)
        temp->next = current;
}

void Bubblesort(struct districtNode *head) {
    struct districtNode *current = head->next;

    while (current != NULL) {
        struct ListNode *currentTown = current->headTown;
        int swapped = 0;
        int size = 0;

        while (currentTown != NULL) {
            size++;
            currentTown = currentTown->next;
        }

        currentTown = current->headTown;

        for (int i = 0; i < size - 1; i++) {
            struct ListNode *p1 = currentTown;
            struct ListNode *p2 = p1->next;

            for (int j = 0; j < size - i - 1; j++) {
                if (p1->population > p2->population) {
                    swap(p1, p2);
                    swapped = 1;
                }
                p1 = p1->next;
                p2 = p2->next;
            }
        }

        if (!swapped) {
            break;
        }

        current = current->next;
    }
}

void CalculatePopulation(struct districtNode *head) {
    int totalPopulation = 0;
    int maxPopulation = 0;
    int minPopulation = 100000;

    struct districtNode *current = head->next;
    while (current != NULL) {
        struct ListNode *currentTown = current->headTown;
        while (currentTown != NULL) {
            totalPopulation += currentTown->population;
            if (currentTown->population > maxPopulation) {
                maxPopulation = currentTown->population;
            }
            if (currentTown->population < minPopulation) {
                minPopulation = currentTown->population;
            }
            currentTown = currentTown->next;
        }
        current = current->next;
    }

    printf("Total population of Palestine: %d\n", totalPopulation);
    printf("Maximum town population: %d\n", maxPopulation);
    printf("Minimum town population: %d\n", minPopulation);
}

void DeleteTown(struct districtNode *head, char districtName[Max_Inside], char townName[Max_Inside]) {
    struct districtNode *current = head->next;

    while (current != NULL && strcmp(current->name, districtName) != 0) {
        current = current->next;
    }

    if (current != NULL) {
        struct ListNode *currentTown = current->headTown;

        while (currentTown != NULL && strcmp(currentTown->town, townName) != 0) {
            currentTown = currentTown->next;
        }

        if (currentTown != NULL) {
            if (currentTown->prev != NULL) {
                currentTown->prev->next = currentTown->next;
            } else {
                current->headTown = currentTown->next;
            }

            if (currentTown->next != NULL) {
                currentTown->next->prev = currentTown->prev;
            }

            free(currentTown->town);
            free(currentTown);
        } else {
            printf("Town '%s' not found in district '%s'.\n", townName, districtName);
        }
    } else {
        printf("District '%s' not found.\n", districtName);
    }
}

void DeleteDistrict(struct districtNode *head, char districtName[]) {
    struct districtNode *districtToDelete = FindDistrict(head, districtName);
    if (districtToDelete != NULL) {
        struct ListNode *currentTown = districtToDelete->headTown;
        while (currentTown != NULL) {
            struct ListNode *temp = currentTown;
            currentTown = currentTown->next;
            free(temp->town);
            free(temp);
        }
        districtToDelete->prev->next = districtToDelete->next;
        if (districtToDelete->next != NULL) {
            districtToDelete->next->prev = districtToDelete->prev;
        }
        free(districtToDelete->name);
        free(districtToDelete);
    }
}

void newLinkedList(struct districtNode *head, struct districtNode *newhead, char array[][Max_Inside], int size) {
    struct districtNode *current = head;
    struct districtNode *newcurrent = newhead;

    for (int i = 0; i < size; i++) {
        if (FindDistrict(newhead, array[i]) == NULL) {
            newcurrent = Insert(newcurrent, array[i]);
        }
    }
    newcurrent = newhead->next;

    while (newcurrent != NULL) {
        current = head;
        while (current != NULL) {
            if (strcmp(current->name, newcurrent->name) == 0) {
                newcurrent->headTown = current->headTown;
                break;
            }
            current = current->next;
        }
        newcurrent = newcurrent->next;
    }
}
void ChangeTownPopulation(struct districtNode *head) {
    char districtName[Max_Inside];
    char townName[Max_Inside];
    int population;

    printf("Enter the name of the district where the town is located: ");
    scanf("%s", districtName);
    struct districtNode *current = FindDistrict(head, districtName);
    if (current == NULL) {
        printf("District %s not found.\n", districtName);
    } else {
        printf("Enter the name of the town: ");
        scanf("%s", townName);
        struct ListNode *town = FindTown(current, townName);
        if (town == NULL) {
            printf("Town %s not found in district %s.\n", townName, districtName);
        } else {
            printf("Enter the new population for town %s ", townName);
            scanf("%d", &population);
            town->population = population;
            printf("Population of town %s in district %s updated to %d.\n", townName, districtName, population);
        }
    }
}
void printBeforSorting(char **dynamicarray, int i) {
    for (int j = 0; j < i; j++) {
        printf("%s", dynamicarray[j]);
    }
}

void PrintDistricts(struct districtNode *head) {
    struct districtNode *current = head->next;

    while (current != NULL) {
        printf("%s District , Population = %d ", current->name, current->sum);
        struct ListNode *currentTown = current->headTown;
        while (currentTown != NULL) {
            printf("\n%s  , %d", currentTown->town, currentTown->population);
            currentTown = currentTown->next;
        }
        current = current->next;
        printf("\n");
    }
}
void PrintDistrictsWithPop(struct districtNode *head) {
    struct districtNode *current = head->next;

    while (current != NULL) {
        printf("%s District , Population = %d \n", current->name, current->sum);
        current = current->next;

    }
}
void saveToFile(struct districtNode *head, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    struct districtNode *current = head->next;
    while (current != NULL) {
        fprintf(file, "%s District , Population = %d\n", current->name, current->sum);
        struct ListNode *currentTown = current->headTown;
        while (currentTown != NULL) {
            fprintf(file, "%s|%d\n", currentTown->town, currentTown->population);
            currentTown = currentTown->next;
        }
        current = current->next;
    }

    fclose(file);
}

int main() {
    struct districtNode *head;
    head = MakeEmpty(NULL);
    struct districtNode *newhead;
    newhead = MakeEmpty(NULL);
    int choice;
    int i = 0;
    char districtName[Max_Inside];
    char townName[Max_Inside];
    int population;
    struct districtNode *current;
    char **dynamicarray;
    dynamicarray = (char **)malloc(sizeof(char *) * 26);
    if (dynamicarray == NULL)
        printf("Memory allocation failed");

    do {
        printf("\nMenu:\n");
        printf("1. Load the input file (Palestinian districts and their town with population).\n");
        printf("2. Print the loaded information before sorting (as in the input file format)\n");
        printf("3. Sort the districts alphabetically using Radix sort.\n");
        printf("4. Sort the towns for each district based on population in ascending order.\n");
        printf("5. Print the sorted information\n");
        printf("6. Add a new district to the list of sorted districts (and sort the list)\n");
        printf("7. Add a new town to a certain district\n");
        printf("8. Delete a town from a specific district.\n");
        printf("9. Delete a complete district; here, you must delete all towns belonging to that district before deleting it.");
        printf("10. Calculate the population of Palestine, the max and min town population.\n");
        printf("11. Print the districts and their total population (without towns details). \n");
        printf("12. Change the population of a town, after prompting the user to enter the district name. \n");
        printf("13.Save to output file.\n");
        printf("14. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                LoadInputFile(head, dynamicarray, &i);
                printf("Reading file has being done successfully\n");
                break;
            case 2:
                printBeforSorting(dynamicarray, i);
                break;
            case 3:
                RadixSort(head, newhead);
                printf("District sorted using Radix Sort.\n");
                break;
            case 4:
                Bubblesort(head);
                printf("Towns sorted based on population");
                break;
            case 5:
                PrintDistricts(newhead);
                ("Sorted Information!");
                break;
            case 6:
                printf("Please enter the district: ");
                scanf("%s", districtName);
                current = FindDistrict(head, districtName);
                if (current == NULL) {
                    newhead = Insert(head, districtName);
                    RadixSort(head, newhead);
                    printf("District %s added and sorted.\n", districtName);
                } else {
                    printf("The district %s already exists in the list.\n", districtName);
                }
                break;

            case 7:
                printf("Please enter the specific district ");
                scanf("%s", districtName);
                current = FindDistrict(head, districtName);
                if (current == NULL) {
                    printf("There is no district called %s", districtName);
                } else {
                    printf("Please enter the town and the population ");
                    scanf("%s%d", townName, &population);
                    InsertTown(current, townName, population);
                    printf("Town saved to the list.\n");
                }
                break;
            case 8:
                printf("Enter the name of the district from which you want to delete the town: ");
                scanf("%s", districtName);
                printf("Enter the name of the town to be deleted: ");
                scanf("%s", townName);
                DeleteTown(head, districtName, townName);
                break;
            case 9:
                printf("Enter the name of the district to be deleted: ");
                scanf("%s", districtName);
                DeleteDistrict(head, districtName);
                break;
            case 10:
                CalculatePopulation(head);
                break;
            case 11:
                PrintDistrictsWithPop(head);
                break;
            case 12:
                ChangeTownPopulation(head);
                break;
            case 13:
                saveToFile(newhead, "sorted_district.txt");
                printf("Sorted districts have been saved to 'sorted_district.txt'.\n");

                break;
            case 14:
                DeleteList(head);
                free(dynamicarray);
                printf("Leaving the program");
                break;
            default:
                printf("Invalid ! . try again >.\n");
        }
    } while (choice != 14);



    return 0;
}

