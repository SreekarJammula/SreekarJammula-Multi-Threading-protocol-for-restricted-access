/*This program contains the solution to the given problem .It applies the classic approach of having a semaphore called mysem control the access to the piazza and two mutex locks capuletlock and montaguelock performing locking when number of people in the piazza is being updated.*/

#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>



typedef struct person {    //declaring the structure to read data into
	char family_type[20];
	char name[20];
	int enter_time;
	int exit_time;
 };
 
static sem_t mysem;                   //static semaphore,mutex and count declarations.
static pthread_mutex_t capuletlock;
static pthread_mutex_t montaguelock;
static int num_capulets=0;
static int num_montagues=0;




void *gang_member(void *args)
{
  
  struct person *myargs;
  myargs=(struct person *)args;
  

  if(myargs->enter_time==0)
  {
    printf("%s %s arrives at time %i \n", myargs->family_type, myargs->name, myargs->enter_time);
  }
  else //sleep until arrival time and then print
  {
  sleep(myargs->enter_time);
  printf("%s %s arrives at time %i \n", myargs->family_type, myargs->name, myargs->enter_time);
  }
      
    
  	if(strcmp(myargs->family_type,"Capulet")==0 )  //section for family capulet
			{
				
        pthread_mutex_lock(&capuletlock); // lock 
        num_capulets += 1;                                                       
	    	printf("%s %s enters the plaza.\n", myargs->family_type, myargs->name);  //critical section
        if(num_capulets==1)
        {
        sem_wait(&mysem); //if its the first capulet, wait on semaphore to claim piazza
        }
        pthread_mutex_unlock(&capuletlock); //unlock 
        
        
                   
        sleep(myargs->exit_time);	// sleep until exit time
			
		  	pthread_mutex_lock(&capuletlock); //lock again for decrement
        num_capulets -= 1;
        if(num_capulets==0)
        {
			     sem_post(&mysem); //if last capulet, then release rights on piazza
        }
				printf("%s %s leaves the plaza. \n",myargs->family_type, myargs->name);
        pthread_mutex_unlock(&capuletlock);
			  
			}	
      
			
		 if (strcmp(myargs->family_type,"Montague")==0)  //for family montagues 
			{
				
					pthread_mutex_lock(&montaguelock); //acquire lock
          num_montagues+= 1;
          if(num_montagues==1) //if its first montague, then claim piazza
           {
           sem_wait(&mysem);
				 	}
          printf("%s %s enters the plaza.\n", myargs->family_type, myargs->name); //critical section
					pthread_mutex_unlock(&montaguelock); //unlock
          
                                              
				   sleep(myargs->exit_time); //sleep until exit time
                                     
				  pthread_mutex_lock(&montaguelock); //acquire lock again
          num_montagues-= 1;
          if(num_montagues==0) //if last montague, then release piazza
           {
           sem_post(&mysem); 
				  	}
          printf("%s %s leaves the plaza.\n", myargs->family_type, myargs->name); //critical section
					pthread_mutex_unlock(&montaguelock); //unlock
					
				
					
				}
	}




int main()
{
int i;
int pid=1;
struct person parray[3];
pthread_t tid[3];
int number_people=0;

sem_init(&mysem, 0, 1);  //initialising mutexes and semaphore
pthread_mutex_init(&capuletlock, NULL);
pthread_mutex_init(&montaguelock, NULL);


printf("reading from file now...\n");   // read data using io redirection
while(fscanf(stdin,"%s %s %d %d",parray[i].family_type, parray[i].name, 
			&parray[i].enter_time, &parray[i].exit_time ) != EOF ) 
	    {
		     number_people++;
	     	i++;
    	} 

     for(i=0;i<number_people;i++)
        {
        
          pid = pthread_create(&tid[i], NULL, &gang_member,(void*) &parray[i]); //creating pthread, one each for the entries available in input
          if(pid!=0)
          {
          
           perror("Cannot create the thread! \n");
          }
        }
        
         for (i = 0; i < number_people; i++)
         {
         pthread_join(tid[i], NULL);     //wait for all threads to join 
         }  
        sem_destroy(&mysem);  //destroying the semaphore
        
			
		return 0;
   
}
 