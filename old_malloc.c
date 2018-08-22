#include<stdio.h>

#define MEMSIZE 1000

struct test{

  int a;

  
  int tf()
  {

    return a;

  }
  

};







char mem[MEMSIZE]; // 1000 bytes that currently only store character data types. 

struct m_data{

  struct m_data* next;
  struct m_data* prev;
  int isFree;
  int size;
};


struct m_data* root = (struct m_data*)mem;  // it's more useful to make root global


// it could well be that you reach the end of the linked list without ever finding a suitable fit


// finds first occurrence of suitable slot
// what is the termination condition if there is no suitable space available? If no suitable space is available then when will ultimately have reached the end of the queue. 

struct m_data* find(int m_size)
{
 struct m_data*  p = root;

 while(p->next !=NULL && (p->size <  (m_size + sizeof(struct m_data)) || !p->isFree))
    {
      p = p->next;

    }
 
    return p;

}


// still confused about typecasting pointers into void pointer; what is allowable to return in case of function with void pointer.

void* mymalloc(int m_size)
//void* mymalloc()
{

  //initialisation
  static int init = 0;
  // static  struct m_data* root;
  struct m_data* q;
 

  if(!init)
    {
      init = 1;
      root = (struct m_data*)mem;
      root -> isFree = 1;
      root -> next = NULL;
      root -> prev = NULL;
      root -> size = MEMSIZE - sizeof(struct m_data);
    }

  q = find(m_size); // first suitable block
 
  // set up new m_data block to point at new free space

   struct m_data* newB = (struct m_data*)((char*)q + sizeof(struct m_data) + m_size); // need to type cast to char in order to use pointer arithmetic
   newB -> next = q->next;;//?????
   newB -> prev = q;
   newB -> isFree = 1;
   newB -> size = (q->size - m_size - sizeof(struct m_data));
   printf("newB = %p\n", newB);



  //  printf("%p\n", find(m_size));

  q->isFree = 0;
  q->size = m_size;
  q->next = newB;


  return (void*)(q+1);
  
}




void myfree(void* ptr)
{


  struct m_data* t = (struct m_data*)ptr;
  t->isFree = 1;



  struct m_data* curr = root;

  while(curr->next != NULL)
    {


      if(!(curr -> isFree)|| !(curr -> next -> isFree))
	{
	  curr = curr -> next;
	}
      else
	{
	  //merge

	  curr-> size += curr->size + sizeof(struct m_data) + curr->next->size;
	  curr -> next -> next -> prev = curr;
	  curr -> next = curr->next-> next;

	}



    }




}







int main()
{




  printf("mymalloc() = %p\n ", mymalloc(128));
  printf("mem[24]  %p\n", &mem[24]);
  printf("mem[152] = %p\n", &mem[152]);
  //  printf("mymalloc(32) again = %p\n", mymalloc(32));
  int* p = mymalloc(32);


  myfree(p);;




  return 0;

}
