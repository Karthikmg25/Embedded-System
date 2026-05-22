#include <stdio.h>

/* Implementing a circular Queue using array */
// - Rear wraps around to reuse free space at front
// - Provides efficient memory utilization

// Define size of the queue
#define MAX 5

// Define structure for queue
typedef struct 
{
    int data[MAX];
    int front, rear;
}queue;


int main()
{
    queue Queue={.data={}, .front= -1, .rear= -1};
    
    // Queue operations:
    void Enqueue(queue* pQ, int data);
    int Dequeue(queue* pQ);
    void Display_Queue(queue* pQ);

    // Enqueue Operations:
    Enqueue(&Queue, 100);
    Enqueue(&Queue, 200);
    Enqueue(&Queue, 300);
    Enqueue(&Queue, 400);
    Enqueue(&Queue, 500);
    Display_Queue(&Queue);

    // Dequeue operations:
    // - Removes elements from front
    // - Creates free space at front end
    Dequeue(&Queue);
    Display_Queue(&Queue);


    // Circular queue advantage:
    // - Rear wraps around to reuse free space at front
    Enqueue(&Queue, 600);
    Display_Queue(&Queue);

}

// Enqueue operation:
// - Insertion of new elements at rear end
// - Rear wraps around to use free space at front
void Enqueue(queue* pQ, int data)
{
    // Edge case 1: Queue is full
    if((pQ->rear + 1)%MAX ==  pQ->front)
    {
        printf("\nQueue is Full: No more insertions");
        return;
    }
    // Edge Case 2: Queue is Empty
    // - Adding first element
    if(pQ->front == -1)
    {
       pQ->front = pQ->rear =0;

       pQ->data[pQ->front] = data;
      
       return;
    }
    // Normal case
    // - Elemnts are added at rear end
    pQ->rear = (pQ->rear+1)%MAX;
    pQ->data[pQ->rear] = data; 
}
// Dequeue operation:
// - removal of elements from front end
int Dequeue(queue* pQ)
{
    // Edge case 1: queue is empty
    if(pQ->front == -1)
    {
        printf("\nQueue is empty: No elements to remove");
        return -1;
    }
    int data = pQ->data[pQ->front];
    // Edge case 2: removing last element from queue
    // reset pointers (front and rear) to -1
    if(pQ->front == pQ->rear)
    {
        pQ->front = pQ->rear = -1;

        return data;
    }
    // Normal case:
    pQ->front = (pQ->front+1)%MAX;
    return data;
}
// Display Queue
// - Display elemnts form front to rear 
// - break after printing rear element
void Display_Queue(queue* pQ)
{
    // Edge case 1: queue is empty
    if(pQ->front == -1)
    {
        printf("\nQueue is empty: No elements to display");
        return ;
    }
      printf("\n");
    int i= pQ->front;

    while(1)
    {
        printf("%d ", pQ->data[i]);

        if( i == pQ->rear)
        {
            break;
        }

        i= (i+1)%MAX;
    }

}