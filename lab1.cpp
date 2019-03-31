#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

static void * func1_thread(void *arg){
    while(!(* (bool*) arg)){
        cout << "1" << flush;
        sleep(1);
    }
    void* exit_code = (void*) 8;
    pthread_exit(exit_code);
}

static void * func2_thread(void *arg){
    while(!(* (bool*) arg)){
        cout << "2" << flush;
        sleep(1);
    }
    void* exit_code = (void*) 9;
    pthread_exit(exit_code);
}

int main(int argc, char *argv[])
{	
    bool thread1_ended = 0;
    bool thread2_ended = 0;
    int exit_thread_code1 = 0;
    int exit_thread_code2 = 0;
    pthread_t thread1;
    pthread_t thread2;

    if ( pthread_create( &thread1, NULL, func1_thread, &thread1_ended ))
            return 1;
    if ( pthread_create( &thread2, NULL, func2_thread, &thread2_ended ) )
            return 1;

    getchar();
    thread1_ended = 1;
    thread2_ended = 1;

    if ( pthread_join( thread1, (void**) &exit_thread_code1 ) )
            return 1;
    cout<< exit_thread_code1;
    if ( pthread_join( thread2, (void**) &exit_thread_code2 ) )
            return 1;
    cout<< exit_thread_code2;

    return 0;
}
