#include <stdio.h>
#include <string.h>
#include "system.h"
#include "nios2.h"
#include <unistd.h>
//defines the performance counter peripheral
#include "altera_avalon_performance_counter.h"

//defines the shared memory
#define shared_mem ONCHIP_MEMORY2_0_BASE

//create a shared memory structure
//variables status_start and status_done will be shared variables used for synchronization.
typedef struct
{
    int M1s[10][10];
    int M2s[10][10];
    int Cs[10][10];
    int status_start;
    int status_done;
}   shared_mem_struct;

int main()
{
    printf("Hello\n");
   //initialize the message buffer location
   shared_mem_struct *message;
   message = (shared_mem_struct*)shared_mem;

  volatile int i,j,k; //i = row, j = column,

  int count;
  int M1[10][10]={  0,0,0,0,0,0,0,0,0,0,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    19,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    19,32,33,34,32,32,33,34,32,32};

  int M2[10][10]={  32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    19,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    32,32,33,34,32,32,33,34,32,32,
                    19,32,33,34,32,32,33,34,32,32};
  int C[10][10]= {0};

  for( i=0; i<10; i++)
  {
    for (j=0; j<10; j++)
    {
        (message)->M1s[i][j]=M1[i][j];
        (message)->M2s[i][j]=M2[i][j];
    }
  }

  (message)->status_start=1;
  (message)->status_done=1;

  PERF_RESET (PERFORMANCE_COUNTER_BASE);            //Reset Performance Counters to 0
  PERF_START_MEASURING (PERFORMANCE_COUNTER_BASE);  //Start the Counter
  PERF_BEGIN (PERFORMANCE_COUNTER_BASE,2);          //Start the overhead counter
   PERF_BEGIN (PERFORMANCE_COUNTER_BASE,1);          //Start the Matrix Multiplication Counter
  PERF_END (PERFORMANCE_COUNTER_BASE,2);            //Stop the overhead counter

 /* count = 0;
  while(count < 1000)
 {
    count++;
    for (i=0;i<=4;i++)
    {
      for (j=0;j<=9;j++)
      {
        (message)-> Cs[i][j] = 0;
         for (k=0;k<=9;k++)
	  {
            (message)->Cs[i][j]+=(message)->M1s[i][k]*(message)->M2s[k][j];
          }
          //printf("%f ", C[i][j]);
      }
    }
  }
  */
    //CPU stops the performance_count and print performance_count result and matrix C from 1000th iteration on the terminal.
  while((message)->status_done!=3) ;
  for (i=0;i<=9;i++)
    {
        for(j=0;j<9;j++)
        {
            printf("%d ",(message)->Cs[i][j]);
        }
        printf("\n");
    }

  PERF_END (PERFORMANCE_COUNTER_BASE,1);            //Stop the Matrix Multiplication Counter
  PERF_STOP_MEASURING (PERFORMANCE_COUNTER_BASE);   //Stop all counters

  perf_print_formatted_report((void *)PERFORMANCE_COUNTER_BASE, ALT_CPU_FREQ, 2,
  "1000 loops","PC overhead");

  return 0;
}
