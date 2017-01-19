#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <math.h>
#include <string.h>

//char * args [8] = {"./multiply", buff_shmid, buf_i, buf_j, buf_a_row, buf_b_col, buf_multual, (char*)NULL};


/* 
The structure for the shared memory structure.
| mat_a | mat_b | mat_c |
*/
int main(int argc, char* argv[]){
  // get arguments
  // argv[1] -- represented as ascii number
  int shmid = atoi(argv[1]);
  int i = atoi(argv[2]);
  int j = atoi(argv[3]);
  int a_row = atoi(argv[4]);
  int b_col = atoi(argv[5]);
  int m = atoi(argv[6]);

  // get shared memory section

  /* 
    The structure for the shared memory structure.
    p
    | mat_a | mat_b | mat_c |
  */
  int *shared_mem;
  if ((shared_mem=(int*)shmat(shmid,0,0))==(int*)-1){
    printf("get shared mem error\n");
    exit(0);
  }
  //set pointer p to the beginning of the shared_mem
  int *p=shared_mem;
  int *process_start = p;
  int process_a_row[m];
  int process_b_col[m];

  p += i*m;  // go to the right row for mat_a
  // put the number into the process_row
  for(int z=0; z<m; z++){
    process_a_row[z] = *p;
    p++;
  }

  // pointer naviage back to the start of mat_a
  p=process_start;
  p += a_row*m; // pointer will pass mat_a and stop at the beginning of mat_b
  p+=j; // pointer goes to the right column of mat_b
  // get the number for the process_col
  for(int z=0; z<m; z++){
    process_b_col[z] = *p;
    p+=b_col;
  }

  // sum the product 
  int sum = 0;
  for(int z=0; z<m; z++){
    sum+=process_a_row[z]*process_b_col[z];
  }

  // assign the value to the correct spot in memory
  p=process_start;
  // go to mat C 
  // pass through the first two matrices 
  p= (a_row*m + m*b_col) + p;

  /* 
    pointer go to the index of mat_c and put in the results 
    The structure for the shared memory structure.
                    p
    | mat_a | mat_b | mat_c |
  */
  p += j+i*b_col;
  // assign the values to mat_c
  *p = sum;
  return 0;
}
