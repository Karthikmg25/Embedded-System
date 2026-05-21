#include <stdio.h>

/* Implementing Queue Data structure using Array */

// Define size of array
#define MAX 5

// Define a structure for Queue
// Storing - data, front and rear pointers
typedef struct
{
   int data[MAX];
   int front;
   int rear;
}queue;

int main()
{
    // Declare & initialise Queue:
    queue Queue= { .data={}, .front=-1, .rear = -1};

    // Queue Operations:
    void Enqueue(queue* pQueue, int data);
    int Dequeue(queue* pQueue);
    int Pop(queue* pQueue);
    void Display_Queue(queue* pQueue);
     
    // Enqueue operations:
    Enqueue(&Queue, 30);
    Enqueue(&Queue, 50);
    Enqueue(&Queue, 60);
    Enqueue(&Queue, 80);
    Enqueue(&Queue, 90);

    Display_Queue(&Queue);

    // Inserting more data when queue is full:
    Enqueue(&Queue, 180);

    // Dequeue operations:
    Dequeue(&Queue);
    Dequeue(&Queue);

    Display_Queue(&Queue);

    // Remove all datas
    Dequeue(&Queue);
    Dequeue(&Queue);
    Dequeue(&Queue);

    Display_Queue(&Queue);

    // Dequeue operation when Queue is empty:
    Dequeue(&Queue);


}

                                                       /* Queue Operations  */
                                                       /*********************/


// Enqueue Operation: Insertion of new data
// New data is inserted at rear end
void Enqueue(queue* pQueue, int data )
{
    // Edge case 1: Queue is full
    // - in leniar queue, queue is considered full when rear = MAX-1
    if(pQueue->rear == MAX-1)
    {
        printf("\nQueue is Full: No more insertions possible");
        return;
    }
    // Edge case 2: Queue is empty
    // - data is inserted at front end
    if(pQueue->front == -1)
    {
        pQueue->front++;
        pQueue->rear++;

        pQueue->data[pQueue->front] = data;
        return;
    }
    // Normal case: New data inserted at rear end
    pQueue->data[++pQueue->rear] = data;

}

// Dequeue Operation: removal of data
// Data is removed from the front end
int Dequeue(queue* pQueue)
{
    // Edge case 1: Queue is empty
    if(pQueue->front == -1)
    {
        printf("\nQueue is empty: No more removals possible");
        return 0;
    }
    // Edge case 2: Removing last data from queue
    // - After removal, Queue becomes empty

    int data =  pQueue->data[pQueue->front];
    if(pQueue->front == pQueue->rear)
    {     
        pQueue->front= pQueue->rear = -1;
        return data;
    }

    // Normal case: data is removed from front
    pQueue->front++;
    return data;
}
// Pop operation: returns data at front
int Pop(queue* pQueue)
{
    // Edge case: Queue is empty
    if(pQueue->front == -1)
    {
        printf("\nQueue is empty");
        return 0;
    }
    return pQueue->data[pQueue->front];
} 
// Displat stored data from front to rear
void Display_Queue(queue* pQueue)
{
    // Edge case: Queue is empty
    if(pQueue->front == -1)
    {
        printf("\nQueue is empty: No data to display");
        return;
    }
  
     printf("\n");
    for(int i= pQueue->front;i<= pQueue->rear;i++)
    {
        printf("%d ", pQueue->data[i]);
    }

}