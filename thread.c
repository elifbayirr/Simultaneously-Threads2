#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h> // for sem_wait,sem_post,sem_init and sem_destroy functions 


void show(const char *data){
    int i = 0;
    for(i;i<5;i++){
    // write to STDOUT
        write(STDOUT_FILENO,data,strlen(data));
        sleep (lrand48()%3);
    }
}

sem_t s1, s2, s3, s4, s5, s6; // ı have 6 semaphores for 6 special case..identifiers s1,s2,s3,s4,s5,s6

// sem_wait for used decrease the value of semaphore by 1.If the corresponding semaphore value is greather than 1 the reduction is performed instantly and the func.returns.

// sem_post for used increase the value of semaphore by 1.If the value of the semaphore is already 0 and another process is blocked because it is waiting for the same semaphore

void*A (void *arg)
{
    show ("A1");
    sem_post (&s1);     //atomically increases the number of semaphores
    sem_wait (&s2);      // suspends the calling thread until it has a non-zero number 1 bacause code block a2 should run after code block b1 (second step so s2)
    show ("A2");
    sem_post (&s4);     //atomically increases the number of semaphores
    sem_wait (&s5);     // suspends the calling thread until it has a non-zero number bacause code block a3 should run after code block c2 (fifth step so s5)
    show ("A3");
    sem_post (&s6);     //atomically increases the number of semaphores

    pthread_exit (0);
}


void*B (void *arg)
{
    show ("B1");
    sem_post (&s2);      //atomically increases the number of semaphores
    show ("B2");
    sem_post (&s3);     //atomically increases the number of semaphores
    sem_wait (&s4);     // suspends the calling thread until it has a non-zero number bacause code block b3 should run after code block a2 (fourth step so s4)
    show ("B3");
    sem_wait (&s6);     // for last block excute step 6
    show ("B4");

    pthread_exit (0);
}


void*C (void *arg)
{
    sem_wait (&s1);     // suspends the calling thread until it has a non-zero number bacause code block c1 should run after code block a1 (fist step so s1)
    show ("C1");
    sem_wait (&s3);     // suspends the calling thread until it has a non-zero number bacause code block c2 should run after code block b2 (third step so s3)
    show ("C2");
    sem_post (&s5);     //atomically increases the number of semaphores
    show ("C3");       
    sem_post (&s6);     //atomically increases the number of semaphores
    
    pthread_exit (0);
}


int main (int argc, char*argv[])
{
    pthread_t thread_1, thread_2, thread_3;
    
     //sem_init(&sem,pshared,value) for initialization of the related semaphore and prepares the semaphore object shown
    // sem_init (&sem ,0,count) for used within this process only
    
    sem_init (&s1, 0, 0);  
    sem_init (&s2, 0, 0);
    sem_init (&s3, 0, 0);
    sem_init (&s4, 0, 0);
    sem_init (&s5, 0, 0);
    sem_init (&s6, 0, 0);
    
    pthread_create (&thread_1, NULL, A,  (void *)&thread_1);
    pthread_create (&thread_2, NULL, B,  (void *)&thread_2);
    pthread_create (&thread_3, NULL, C,  (void *)&thread_3);
    
    pthread_join (thread_1, (void**)&thread_1);
    pthread_join (thread_2, (void**)&thread_2);
    pthread_join (thread_3, (void**)&thread_3);
    
    // destroys the unnamed semaphore at the address pointed to by sem (sem_destroy(&sem))
    // destroys the semaphore object, freeing any resources it may have.
    sem_destroy (&s1);
    sem_destroy (&s2);
    sem_destroy (&s3);
    sem_destroy (&s4);
    sem_destroy (&s5);
    sem_destroy (&s6);
    
    return 0;
}