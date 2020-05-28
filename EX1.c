#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <malloc/malloc.h>
#include <unistd.h>

#define MAX_BUF 100 
#define TRUE 1
#define FALSE 0
#define MAX_ITEM	100
// void *producer(void*); 
// void *consumer(void*);
//int buffer[MAX_BUF];
int count = 0;
int in = -1, out = -1;
typedef struct Node {
   struct Node* next;
	int data;
	
}node;

typedef struct LinkedList {
   int numOfItems;
   struct Node* head;
   struct Node* tail;
}list;

//list* mylist;
list mylist={0,NULL,NULL};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER; 
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;




void ListInit(struct LinkedList* plist);
int isEmpty(struct LinkedList* plist);
int isFull(struct LinkedList* plist);
void insertItem(struct LinkedList* plist, int data);
int getItem(struct LinkedList* plist);
void *producer (void *v);
void *consumer (void *v);

int main(void){
    
    // list* mylist = (list*)malloc(sizeof(list));
   

  
    int i;
    pthread_t threads[2];
    pthread_create (&threads[0], NULL, producer, NULL);
    pthread_create (&threads[1], NULL, consumer, NULL); 
    for (i=0; i< 2; i++)
        pthread_join(threads[i], NULL);

    return 0;
    
    
}


void ListInit(struct LinkedList* plist){

   plist->numOfItems = 0;
   plist->head = NULL;
   plist->tail = NULL;
}

int isEmpty(struct LinkedList* plist){

	if(plist->numOfItems == 0){
		return TRUE;
	} else {
		return FALSE;
	}
}

int isFull(struct LinkedList* plist){

	if(plist->numOfItems == MAX_ITEM){
		return TRUE;
	} else {
		return FALSE;
	}
}

void insertItem(struct LinkedList* plist, int data) {
   struct Node * newNode = (struct Node*)malloc(sizeof(struct Node));
      newNode->data = data;
      newNode->next = NULL;

   if ( plist->head == NULL ) {
      plist->head = newNode;
      plist->tail = newNode;
   } else {
      plist->tail->next = newNode;
      plist->tail = newNode;
   }

   (plist->numOfItems)++;
}
int getItem(struct LinkedList* plist){

	struct Node* tmpNode;
	int ret;
	
	ret = plist->head->data;

	tmpNode = plist->head;
	plist->head = plist->head->next;
	free(tmpNode);
	(plist->numOfItems)--;

	return ret;
}

void freeAllNode(struct LinkedList* plist) {

	struct Node* tmpNode;
	tmpNode = plist->head;

	while(tmpNode != NULL){
		plist->head = plist->head->next;
		free(tmpNode);
		tmpNode = plist->head;

		(plist->numOfItems)--;
	}
}

void *producer (void *v){
    int i;
    //ListInit(mylist);
    for (i =0; i < 1000; i++)  {
        pthread_mutex_lock(&mutex);
        if (isFull(&mylist)) // 링크의 노드수가 100 (numOfItems == MAX_ITEM) True -> wait.
            pthread_cond_wait(&buffer_has_space, &mutex); 
    
        insertItem(&mylist,random()%1000);
     
        
        // in = in % MAX_BUF;
        // in++;
        // buffer[in] = i; // simple data
        // count++;
    
        pthread_cond_signal(&buffer_has_data); 
        pthread_mutex_unlock(&mutex);
        usleep(100);

    }
    return 0;
}

void *consumer (void *v){
   // ListInit(mylist);
    int i, data;
    for (i =0; i < 1000; i++){
        pthread_mutex_lock(&mutex);
        if (isEmpty(&mylist)) // numOfItems == 0 노드수가 0이면 트루, wait
            pthread_cond_wait(&buffer_has_data, &mutex); 
        // out = out % MAX_BUF;
        // out++;
        // data = buffer[out];
        // count--;
        printf("data = %d\n",getItem(&mylist));
        pthread_cond_signal(&buffer_has_space); 
        pthread_mutex_unlock(&mutex);
        usleep(100);
   }
   return 0;
}

