#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

#define max 600
#define numProcess 4
#define numThreads 4


struct Data{
    double height;
    double weight;

};

struct ThreadData{
     struct Data *data;
     int start;//here I am tyring to avoid race condition.
     int end;
     double result;
};


double calculateBmi(double height , double weight);
double naiveApproch(struct Data * data , int size);
void *threadingFunciton(void *thread);


int main()
{
   struct Data data[max];

   FILE *file = fopen("bmi.csv" , "r");
   if(file == NULL)
   {
    printf("Memory allocation failed");
    return 1;
   }

    int nums = 0;

    char line[50];
    char *token;
    fgets(line , sizeof(line) , file);
    int SoSize=0;
    while (fgets(line , sizeof(line) , file) != NULL && nums < max)
    {
        token = strtok(line , ",\n");
        SoSize =0;

        while(token != NULL)
       {
         if(SoSize == 1)
         {
          data[nums].height = atoi(token);
         }
         else if (SoSize == 2)
         {
           data[nums].weight = atoi(token);
         }
         SoSize ++;
         token = strtok(NULL , ",\n");
       }

        nums++;
    }
    fclose(file);


   clock_t startTime = clock();
   double naiveResult = naiveApproch(data , nums);
   double naiveTime = (double)(clock() - startTime) / CLOCKS_PER_SEC;
   printf("Answer  : %lf  Naive Approach : %lf \n" , naiveResult , naiveTime);

   //multiprocess
     pid_t pid;
    int fd[numProcess][2];
    double multiprocessing_results[numProcess] = {0};
    for (int i = 0; i < numProcess; i++) {
        if (pipe(fd[i]) == -1) {
            perror("pipe");
            return 1;
        }
        pid = fork();
        if (pid < 0) {
            perror("fork");
            return 1;
        } else if (pid == 0) {
            close(fd[i][0]);
            double TotalBmi = 0;
            int size = nums / numProcess;
            int start = i * size;
            int end;
            if(i == numProcess - 1)
              end =size;
             else
             end = (i + 1) * size;

            for (int j = start; j < end; j++) {
                TotalBmi += calculateBmi(data[j].height, data[j].weight);
            }
            double result = TotalBmi / (end - start);
            write(fd[i][1], &result, sizeof(double));
            close(fd[i][1]);
            exit(0);
        } else {
            close(fd[i][1]);
            read(fd[i][0], &multiprocessing_results[i], sizeof(double));
            close(fd[i][0]);
            wait(NULL);
        }
    }
    double multiprocessing_result = 0;
    for (int i = 0; i < numProcess; i++) {
        multiprocessing_result += multiprocessing_results[i];
    }
    double multiprocessing_time = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    printf("Answer  : %lf  MultiProcess Approach : %lf \n", multiprocessing_result / numProcess, multiprocessing_time);

   //multithreading
   pthread_t threads[numThreads];
   struct ThreadData thread_data[numThreads];
   int size = nums / numThreads;

   for(int i =0 ; i < numThreads ; i++)
   {
      thread_data[i].data = data;
      thread_data[i].start = i * size;
      if (i == numThreads - 1) {
        thread_data[i].end = nums;
    } else {
        thread_data[i].end = (i + 1) * size;
    }
      pthread_create(&threads[i] , NULL , threadingFunciton , (void *) & thread_data[i]);
   }

   double Multithreading_result =0 ;
   for(int i =0 ; i < numThreads ; i++)
   {
    pthread_join(threads[i] , NULL);
    Multithreading_result += thread_data[i].result;
   }
    double multithreading_time = (double)(clock() - startTime) / CLOCKS_PER_SEC;
    printf("Answer  : %lf  Multithreading Approach : %lf ", Multithreading_result / numThreads, multithreading_time);

   return 0;
}

double calculateBmi(double height , double weight)
{
   double result = weight / (height * height);
   return result;
}

double naiveApproch(struct Data * data , int size)
{
    double TotalBmi = 0;

    for(int i = 0 ; i < size ;i++)
    {
        TotalBmi += calculateBmi(data[i].height , data[i].weight);

    }
    TotalBmi /= size;

    return TotalBmi;
}

void *threadingFunciton(void *thread)
{
    struct ThreadData *ThreadD= (struct ThreadData *)thread ;
    double TotalBmi =0 ;

    for(int i = ThreadD->start; i < ThreadD->end ; i++)
    {
       TotalBmi += calculateBmi(ThreadD->data[i].height , ThreadD->data[i].weight);
    }
    ThreadD->result = TotalBmi / (ThreadD->end - ThreadD->start);
    pthread_exit(NULL);
}
