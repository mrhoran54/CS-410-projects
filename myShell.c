#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_COUNT 100

void interruptHandler(int);
int isSeq(char *);
void pipes(int,int,char*);

void interruptHandler(int signum){ //close foreground process. 

        printf("this is me interrupting your foreground process");

}

int isSeq(char *token){ //checks last character to determine if the command is in a sequence of commands (i.e. ends with ';')
        if (token && *token){
                if (token[strlen(token)-1] == ';')
                        return 1;
                        }
        return 0;
}


void pipes(int commands, int pipecount,char *token){

     int i,j,k,c=0;
        int m = 0;
  //code from lecture to handle piping
     int fd[MAX_COUNT][2];
     int pid[MAX_COUNT];
     char *com[2];
     char *carr[20];
        char *cmd1;

      cmd1 = strtok(token," \n");

        while(cmd1){
if( strcmp("|", cmd1) != 0){

                        carr[m] = cmd1;
                        m++;
                }

        cmd1 = strtok(NULL," \n");

        }

      while(1){


        for (j = 0; j< pipecount; j++){

                pipe(fd[j]);
        }

       for (i = 0; i < commands; i++){


        if((pid[i]=fork()) == 0){ // child 

                if(i==0){ //first command

                dup2(fd[i][1], STDOUT_FILENO);
                close(fd[i][0]);

                }
        }


          else if(i == pipecount - 1){ //last command

          //redirect pipe to std-in

            if(pipecount){

              dup2(fd[i-1][0], STDIN_FILENO);
              close(fd[i-1][1]); //write end
               }
          }
          else{ // nested command ie not last command

              //redirect both stdin & stdout
              if(pipecount){

              dup2(fd[i-1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);
                close(fd[i-1][1]);
                close(fd[i][0]);

              }
              }
       }

    //execute command in child
    com[0] = carr[c];
 com[1] = NULL;
        c++;

    execvp(com[0], com); // dont close fds on execve

    pipecount--;
    if(!pipecount){

        for (k  =0; k < commands; k++)

                waitpid(pid[k]);

        }
        }
}

int main(int argc, char *argv[]){

pid_t child;
 char *token,x, *cmd1, *cmd2, *cmd3;
char * command[2],carr[20];
 char cmd[200];
int fd,commands,pipecount,i=0;
int check = 0;
char new_str[200];

int fpfd[3];
char *ars[1];

char copy[MAX_COUNT];

 while(1){

 signal(SIGINT, interruptHandler);

   printf("myshell> ");

   if(fgets(cmd, sizeof(cmd), stdin) == NULL){
  break;
   }


   // ********************************argv*******************
   // functionality for pipe code : COUNT THE PIPES


   token = strtok(cmd, " ");

   // functionality for pipe code : COUNT THE PIPES

   //token = strtok(cmd," ");
        commands = 0;
        pipecount = 0;

   while(token){

     if(strcmp(token, "|") == 0){
       pipecount++;
     }
        else{
                //carr[commands] = token;
                commands++;
        }

     token = strtok(NULL, " ");

   }
         if (pipecount > 0){

          pipes(commands, pipecount, cmd);
        break;
     }
   //***************************************************

        cmd1 = strtok(cmd," \n");

   while(cmd1){ //command 1? no check for second command
    if(isSeq(cmd1)){
          command[0] = cmd1;
          command[1] = NULL;

        //   check = 1;

          if ((child = fork()) < 0){ // make child process
            printf("fork error\n");
            break;
          }
          else if (child == 0){
            execvp(command[0], command);
            command[0] = NULL;

          }
          else{
            if (waitpid(child, NULL, 0) != child){

              printf("waitpid error\n");
              return;

            }
        }

        cmd1 = strtok(NULL," \n");
        continue;
     }

    else{

        cmd2 = strtok(NULL," \n");
        if(cmd2 == NULL){


          command[0] = cmd1;
          command[1] = NULL;


          if ((child = fork()) < 0){ // make child process
                                                                                                 218,1-8       56%
 printf("fork error\n");
            break;
          }
          else if (child == 0){
            execvp(command[0], command);
            command[0] = NULL;

          }
          else{
            if (waitpid(child, NULL, 0) != child){

              printf("waitpid error\n");
              return;

            }
          }

                break;
        }


        if ((strcmp(cmd2, ">") == 0 || (strcmp(cmd2, "1>") == 0))){

                command[0] = cmd1;
                command[1] = NULL;

                cmd3 = strtok(NULL," \n");
//add error check

                // redirect stdout of first command with dup2)       

                if (child = fork() < 0){ // make child process
                        printf("fork error\n");
                        return;
                }

                if (child == 0){
                        if (( fd = open (cmd3, O_WRONLY | O_CREAT , 0666)) == -1){
                        fprintf(stderr, "mysh error: can't open %s\n", cmd3);
                }
                                                                                                 258,1-8       68%
    dup2(fd,1);
                close(fd);

                        execvp(command[0], command);
                        command[0] = NULL;
                }
                else{
                        if (waitpid(child, NULL, 0) != child){

                                 printf("waitpid error\n");
                                return;
                                }
                        }
                }
        // if statement for Redirect stdin of the cmd to the file with the name "input_file.

        else if (strcmp(cmd2, "<") == 0){

                command[0] = cmd1;
                command[1] = NULL;

                cmd3 = strtok(NULL," ");

                if (child = fork() < 0){ // make child process
                  printf("fork error\n");
                  return;
                }


                else if (child == 0){

                if (( fd = open (cmd3, O_RDONLY)) == -1){

                        fprintf(stderr, "mysh error: can't open %s\n", token);
                }

                dup2(fd,0);
                close(fd);

                execvp(command[0], command);
command[0] = NULL;


                }
                else{
                  if (waitpid(child, NULL, 0) != child){

                    printf("waitpid error\n");
                    return;
                  }
                }


        }


        //Redirect stderr of the cmd to the file with the name "output_file".
        else if (strcmp(cmd2, "2>") == 0){

             command[0] = cmd1;
                command[1] = NULL;

                cmd3 = strtok(NULL," ");
          if (child = fork() < 0){ // make child process
            printf("fork error\n");
            return;
          }


          else if (child == 0){
                if (( fd = open (cmd3, O_WRONLY | O_CREAT, 0666)) == -1){
                        fprintf(stderr, "mysh error: can't open %s\n", cmd3);
          }

           dup2(fd,2);
           close(fd);

                execvp(command[0], command);
            command[0] = NULL;

          }
 else{
            if (waitpid(child, NULL, 0) != child){

              printf("waitpid error\n");
                      return;
            }
          }

        } //?


   }//else bracket after is Seq(token)
} //whiletoken bracket
} // while 1 bracket

}// main bracket

                                                                          
