#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NumberOfSluts 50
#define Max 100
#define InfiniteValue 100000000

struct ListNode{
    char* course;
    struct ListNode* next;
};

struct Graph{
    char* course;
    int Indegree;
    struct ListNode *head;
};
struct Queue{
    struct ListNode* front;
    struct ListNode* rear;
};
struct ListNodeB {
    char* building;
    int weight;
    struct ListNodeB* next;
};

struct GraphB {
    char building[Max];
    struct ListNodeB* head;
};

void CreateBuildingList(struct GraphB *b[]) {
    for (int i = 0; i < NumberOfSluts; i++) {
        b[i] = (struct GraphB*) malloc(sizeof(struct GraphB));
        if (b[i] == NULL) {
            printf("Memory allocation failed\n");
        }

        b[i]->building[0] = '\0';
        b[i]->head = (struct ListNodeB*) malloc(sizeof(struct ListNodeB));
        if (b[i]->head == NULL) {
            printf("Memory allocation failed\n");
        }

        b[i]->head->next = NULL;
    }
}

int FindBuilding(struct GraphB *b[], char str[]) {
    for (int i = 0; i < NumberOfSluts; i++) {
        if (strcmp(b[i]->building, str) == 0) {
            return i;
        }
    }
    return -1;
}

struct ListNodeB* InsertB(struct ListNodeB *head, char *building, int weight) {
    struct ListNodeB* newNode = (struct ListNodeB*) malloc(sizeof(struct ListNodeB));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
    }

    newNode->building = (char*)malloc(sizeof(char) * Max);
    if(newNode->building == NULL) {
        printf("Memory allocation failed\n");
    }

    strcpy(newNode->building, building);
    newNode->weight = weight;
    newNode->next = head->next;
    head->next = newNode;

    return head;
}

void LoadInputFileBuilding(struct GraphB *b[]) {
    FILE *file2 = fopen("input_buildings.txt", "r");
    if (file2 == NULL) {
        printf("Error opening file\n");
        return;
    }

    char line[Max];
    int i = 0;
    while (fgets(line, Max, file2) != NULL && i < NumberOfSluts) {
        line[strcspn(line, "\n")] = '\0';
        char* str = strtok(line, "#");

        int size = 0;
        int indexOfBuilding = -1;
        while (str != NULL) {
            if (size == 0) {
                indexOfBuilding = FindBuilding(b, str);
                if (indexOfBuilding == -1) {
                    strcpy(b[i]->building, str);
                    indexOfBuilding = i;
                }

            } else if (size > 0) {
                char* weightStr = strtok(NULL, "#");
                int weight = atoi(weightStr);
                if (indexOfBuilding == -1) {
                    b[i]->head = InsertB(b[i]->head, str, weight);
                } else {
                    b[indexOfBuilding]->head = InsertB(b[indexOfBuilding]->head, str, weight);
                }
                str = weightStr;
            }
            size++;
            str = strtok(NULL, "#");
        }
        i++;
    }

    fclose(file2);
}
int minDistance(int dist[], int visited[], int n) {
    int min = InfiniteValue;
    int minIndex;
    for (int v = 0; v < n; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            minIndex = v;
        }
    }
    return minIndex;
}

void printPath(int parent[], int j, struct GraphB* buildings[]) {
    if (parent[j] == -1)
        return;
    printPath(parent, parent[j], buildings);
    printf(" -> %s", buildings[j]->building);
}

void dijkstra(struct GraphB* buildings[], int src, int dest, int n , int path) {
    int dist[n];
    int visited[n];
    int parent[n];

    for (int i = 0; i < n; i++) {
        dist[i] = InfiniteValue;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, visited, n);
        visited[u] = 1;

        if (u == dest) {
            break;
        }

        struct ListNodeB* temp = buildings[u]->head->next;
        while (temp != NULL) {
            int v = FindBuilding(buildings, temp->building);
            if (!visited[v] && dist[u] != InfiniteValue && dist[u] + temp->weight < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + temp->weight;
            }
            temp = temp->next;
        }
    }

    if (dist[dest] != InfiniteValue) {
        if(path == 2)
        {
            printf("Shortest path from %s to %s:\n", buildings[src]->building, buildings[dest]->building);
            printf("Distance: %d\n", dist[dest]);
        }
        if(path == 3)
        {
            printf("Shortest path from %s to %s:\n", buildings[src]->building, buildings[dest]->building);
            printf("Distance: %d\n", dist[dest]);
            printf("Path: ");
            printPath(parent, dest, buildings);
            printf("\n");
        }
    }
     else
        {
        printf("No path found from %s to %s\n", buildings[src]->building, buildings[dest]->building);
    }
}

void CreateCourseList(struct Graph *c[])
{
    for(int i =0 ; i < NumberOfSluts ;i++)
    {
        c[i] = (struct Graph*)malloc(sizeof(struct Graph));//each struct is getting memory allocation (course struct).
        if(c[i] == NULL)
            printf("Memory allocation failed");

        c[i]->course = (char*)malloc(sizeof(char)* Max);//also each course variable is getting memory allocation.
        if(c[i]->course == NULL)
        {
            printf("Memory allocation failed");
        }
        c[i]->course[0] = '\0';//initialization.
        c[i]->Indegree = 0;

        c[i]->head = (struct ListNode*)malloc(sizeof(struct ListNode));//each index is getting malloc for head struct.
        if(c[i]->head == NULL)
            printf("Memory allocation");

        c[i]->head->next = NULL;

    }

}
struct ListNode* Insert(struct ListNode *head , char course[])
{

    struct ListNode* current = head;
    struct ListNode* newCourse;

    newCourse = (struct ListNode*)malloc(sizeof(struct ListNode));
    if(newCourse == NULL)
        printf("Memory allocation failed");

    newCourse->course = (char*)malloc(sizeof(char)* Max);
    if(newCourse->course == NULL)
        printf("Memory allocation failed");

    strcpy(newCourse->course , course);
    newCourse->next = NULL;

    while(current->next != NULL)
        current = current->next;

    current->next = newCourse;

    return head;
}

int FindCourse(struct Graph *c[] , char str[])
{
    for(int i = 0 ; i < NumberOfSluts ;i++)
    {
        if(strcmp(c[i]->course , str) == 0)
        {
            return i;
        }
    }
}

void LoadInputFileCourses(struct Graph *c[])
{
    FILE *file = fopen("input_courses.txt", "r");
    char *str;

    if (file == NULL) {
        printf("Error opening file \n");
        return;
    }

    char line[50];
    int i = 0;
    int indexOfPrereq ;
    char temp[Max];
    while (fgets(line, Max, file) != NULL && i < NumberOfSluts)
    {
        line[strcspn(line, "\n")] = '\0';
        char* str = strtok(line , "#");
        int size =0 ;
        while(str != NULL)
        {
            if(size == 0)
            {
                strcpy(c[i]->course,str);
                strcpy(temp , str);
            }
            else if(size > 0)
            {
                c[i]->Indegree++;
                indexOfPrereq = FindCourse(c,str);
                c[indexOfPrereq]->head =Insert(c[indexOfPrereq]->head,temp);
            }
            size++;
            str = strtok(NULL , "#");
        }
        i++;
    }

    fclose(file);


}

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
struct ListNode* InsertQueue(char *course)
{
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));

    if(newNode == NULL)
    {
        printf("Memory allocation failed");
        free(newNode);
    }
    newNode->course = (char*)malloc(sizeof(char)* sizeof(course) +1);
    if(newNode->course == NULL)
    {
        printf("Memory allocation failed");
        free(newNode->course);
    }
    strncpy(newNode->course , course , sizeof(course));
    newNode->course[sizeof(course)] = '\0';
    newNode->next = NULL;

    return newNode;

}

struct Queue* Enque( struct Queue *q , char* course)
{

    struct ListNode* current = InsertQueue(course);

    if( q->rear == NULL)
    {
        q->front = current;
        q->rear = current;
    }
    else
    {
        q->rear->next = current;
        q->rear = current;
    }
    return q;

}
struct Queue* Dequeue(struct Queue *q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return q;
    }

    struct ListNode* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp->course);
    free(temp);
    return q;
}

int IsQueueEmpty(struct Queue *q) {
    return q->front == NULL;
}

char* QueueFront(struct Queue *q) {
    return q->front->course;
}

void TopologicalSort(struct Graph *c[], struct Queue *q, char TopoAlg[NumberOfSluts][Max]) {
    int index = 0;

    for (int i = 0; i < NumberOfSluts; i++) {
        if (c[i]->course[0] != '\0' && c[i]->Indegree == 0) {
            q =Enque(q, c[i]->course);//so the first we are getting the zero indegree for the courses
        }
    }

     while (!IsQueueEmpty(q))//if the queue is not empty then get in the loop
    {
        char* course = QueueFront(q);//queue front return the front(first node). of the queue
        q = Dequeue(q);//then I dequeue the front.
        strcpy(TopoAlg[index], course);//this is gonna be the sorted courses. The zero indegree entry the topoalg.
        index++;

        int courseIndex = FindCourse(c, course);
        struct ListNode* temp = c[courseIndex]->head->next;

        while (temp != NULL)
        {
            int neighborIndex = FindCourse(c, temp->course);
            c[neighborIndex]->Indegree--;
            if (c[neighborIndex]->Indegree == 0) {
                Enque(q, c[neighborIndex]->course);
            }
            temp = temp->next;
        }
    }

}

void printCourses(struct Graph *c[])
{
    for(int i = 0 ;i < NumberOfSluts ; i++)
    {
        if(c[i]->course[0] != '\0')
        {
            printf("%s [%d]: ",c[i]->course , c[i]->Indegree);
            struct ListNode* head = c[i]->head->next;
            while(head != NULL)
            {

                printf("-> %s" , head->course);
                head = head->next;
            }
            printf("\n---------------------\n");
        }
    }
}
void PrintTheSortedCourses(char TopoAlg[NumberOfSluts][Max])
{
       printf("Topologically sorted order of courses:\n");
        for (int i = 0; i < NumberOfSluts; i++) {
                if(TopoAlg[i][0] != '\0')
            printf("%s\n", TopoAlg[i]);
        }
}

int main() {
    struct GraphB* buildings[NumberOfSluts];
    struct Graph *c[NumberOfSluts];
    CreateBuildingList(buildings);
    CreateCourseList(c);
    struct Queue *q;
    char TopoAlg[NumberOfSluts][Max] = {0};
    q = CreateQueue(q);

    printCourses(c);
    int choice ;
    char sourceBuilding[Max];
    char destBuilding[Max];


      do
    {
        printf("1. Load the two files.\n");
        printf("2. Calculate the shortest distance between two buildings.\n");
        printf("3. Print the shortest route between two buildings and the total distance.\n");
        printf("4. Sort the courses using the topological sort.\n");
        printf("5. Print the sorted courses.\n");
        printf("6. Exit the application.\n");
        printf("Please enter your choice\n");
        scanf("%d" , &choice);

        switch(choice)
        {
            case 1:
                LoadInputFileBuilding(buildings);
                LoadInputFileCourses(c);
                printf("Loading the files has been done successfully!\n");
                break;
            case 2 :
                printf("Enter the source building: ");
                scanf("%s", sourceBuilding);
                printf("Enter the destination building: ");
                scanf("%s", destBuilding);

                int srcIndex = FindBuilding(buildings, sourceBuilding);
                int destIndex = FindBuilding(buildings, destBuilding);

                if (srcIndex == -1 || destIndex == -1) {
                    printf("Invalid source or destination building.\n");
                    break;
                }
                dijkstra(buildings, srcIndex, destIndex,NumberOfSluts ,2);

                break;
            case 3 :
                printf("Enter the source building: ");
                scanf("%s", sourceBuilding);
                printf("Enter the destination building: ");
                scanf("%s", destBuilding);

                 srcIndex = FindBuilding(buildings, sourceBuilding);
                 destIndex = FindBuilding(buildings, destBuilding);

                if (srcIndex == -1 || destIndex == -1) {
                    printf("Invalid source or destination building.\n");
                    break;
                }
                dijkstra(buildings, srcIndex, destIndex,NumberOfSluts ,3);
                break;
            case 4 :
                TopologicalSort(c,q , TopoAlg);
                printf("Sorting the courses using Topological Sort has been done successfully! \n");
                break;
            case 5 :
                PrintTheSortedCourses(TopoAlg);
                break;
            case 6 :
                printf("Exit the program");
                break;
            default :
                printf("Invalid ! . try again >.\n");

        }

    }while(choice != 6);

    return 0;
}
