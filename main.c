//
// Created by Maryam Yasser on 09/12/2020.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

FILE* MergeFile;


int *arr=NULL;
int arraySize;

typedef struct node {
    int i;
    int j;
} node;

int* readArrayFromFile(){ //READING AND INITIALIZING ARRAY FROM FILE
    MergeFile= fopen("/Users/maryamyasser/Documents/"
                     "College/7th Semester/Lab2_OS/mergeFile.txt","r");


    printf("Array Size: ");
    fscanf(MergeFile,"%d",&arraySize);
    printf("%d \n\n",arraySize);


    printf("Unsorted Array:\n");
    arr=malloc(arraySize* sizeof(int ));
    for(int i=0;i<arraySize;i++){
        fscanf(MergeFile,"%d",&arr[i]);
        printf("%d ",arr[i]);
    }
    printf("\n");
    return arr;
}

void merge(int l,int r,int arr[]) //REGULAR MERGE SORT
{
    int m = (l+r-1)/2;
    int n1 = m - l + 1;
    int n2 = r - m;


    int L[n1], R[n2];


    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    //FIRST INDEX
    int i = 0;

    //SECOND INDEX
    int j = 0;


    int k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}


void * mergesort_t(void *a)
{
    node * Node;
    Node=(node *)a; //SETTING THE ARGUMENTS


    int l = Node->i;
    int r = Node->j;

    if(l<r){ //ONLY IF ARRAY HAS MORE THAN ONE ELEMENT
        int m = (l+r-1)/2;
        node *left=( node *)malloc(sizeof( node));
        node *right=( node *)malloc(sizeof( node));
        //***********CREATING LEFT AND RIGHT NODES FOR NEW THREADS TO WORK ON***********

        left->i=l;
        left->j=m;

        right->i = m+1;
        right->j = r;
        //***********SETTING THE NEW NODES BEGINNING AND END POINTS***********


        pthread_t tidL,tidR;  //THE NEW THREADS


        pthread_create(&tidL, NULL, mergesort_t, (void*)left);
        pthread_create(&tidR, NULL, mergesort_t, (void*)right);

        //CREATING THE NEW THREADS WITH THEIR RESPECTIVE NODES

        pthread_join(tidL, NULL);
        pthread_join(tidR, NULL);

        //MERGING THE FUNCTION ARGUMENTS AFTER RECURSIVELY CALLING AND JOINING THE NEW THREADS
        merge(l,r,arr);
    }
    else{ // JUST RETURN (ARRAY HAS ONE SORTED ELEMENT
    }
    return 0;
}


void printArray(int A[], int size)
{

    printf("\nSorted Array\n");
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

int main()
{

    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    arr=readArrayFromFile(); //READING AND INITIALIZING ARRAY


    node *m=malloc(sizeof(struct node)); //CREATING INITIAL NODE
    m->i = 0;
    m->j= 9;
    pthread_t tid;



    pthread_create(&tid, NULL, mergesort_t, (void*)m); //CREATING INITIAL THREAD

    pthread_join(tid, NULL);

    printArray(arr,arraySize); //PRINTING ARRAY FUNCTION

    clock_gettime(CLOCK_MONOTONIC, &finish);  //GETTING TIME
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("Elapsed Time: %f\n",elapsed);

    pthread_exit(NULL);


}


