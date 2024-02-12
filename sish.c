#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 100
#define EXIT_FAILURE 1

////////////////////////////////////////////////////////////////////////////////////////////

struct Node {   
  char *data;      

  struct Node *tail;
  struct Node *head;
}; 

struct Node *queueCretated(char *);
struct Node *enQueue(struct Node *, char *);
struct Node *queueDeuque(struct Node *);

void printQueue(struct Node *);
void Queueoffset(struct Node *, int8_t);


void err_exit(char *msg);

////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){

    

    // Variables for "pipe"
    int lastPipe = STDIN_FILENO;
    int fd[2];
    pid_t pid;
        
    //Variables for Queue and Nodes    
    struct Node *queue_tail = NULL;
    struct Node *queue_head = NULL;
    int8_t current_queue_num = 0;
    

    // Variables for "getline()"
    char *string = NULL;
    size_t bufsize = 32;
    size_t character = 0;

    // Variables for Parsing
    char *pillar;
    char *pillarPars;
    char *spacePars;
    char *clean_string;
    
    do{
    // Allocate string to get data
    string = (char *)malloc(bufsize * sizeof(char));  

    //Check to see that "string" got allocated     
    if(string == NULL) {
        err_exit("Unable to allocate string");
        exit(1);
    }
    printf("\nsish> ");
    //Get users input and store it to "character"
    character = getline(&string,&bufsize,stdin); 
    //Make sure to change last char from '\n' to '\0'
    string[character-1] = '\0';

    // Save input user data to Queue, Queue is history
    if(character >= 0){
        if(queue_tail == NULL){                     
        // If "queue_tail" is NULL, create the Queue for history 
            queue_tail = queueCretated(string);
            queue_head = queue_tail;
            current_queue_num++;
        }
        else if ( current_queue_num < MAX){
        // If history is less than 100, save user input to Queue
            queue_tail = enQueue(queue_tail, string);
            current_queue_num++;
        }
        else{
        // If history has more than 100, delete the oldest history and save the new one
            queue_head = queueDeuque(queue_head);   
            current_queue_num--;
            queue_tail = enQueue(queue_tail, string);
            current_queue_num++;
        }
        printQueue(queue_tail);
    }    

    // Assign string to temp_string, temp_string will be use for strtok_r()
    char temp_string[character];
    strcpy(temp_string, string);
    

    // Check for exit, if true free history Queue and string
    if (strcmp(string,"exit") == 0){
        // Loop every node in Queue, and free each node
        for(int j = 0; j < current_queue_num; j++){
            if(queue_head != queue_tail){
                queue_head = queueDeuque(queue_head);   
            }
            else{
                // Free last node
                queue_head = queueDeuque(queue_head);    
                queue_tail = queue_head;
            }
        }
        // Free String then exit
        free(string);
        exit(1);
    }
    
    if (strcmp(string,"history -c") == 0){
        Queueoffset(queue_head, current_queue_num);
    }
    
    // Count number of pillars, this is to see how many commands there are
    int num_pillars = 0;
    for(int x = 0; x < character; x++){
        if(string[x] == '|'){
            num_pillars++;
        }
    }


////////////////////////////////////////////////////////////////////////////////////////////
// TOKENIZING PILLAR ////////////////////////////////////////////////
    
    int i = 0;

    //Loop removes '|' from the whole string
    //Every variable will have a unique command string 
    for (pillar = strtok_r(temp_string, "|", &pillarPars); 
    pillar; 
    pillar = strtok_r(NULL, "|", &pillarPars)){
        

        //Variables will store command and its arguments
        //Every loop will be a new command and argument
        char *command_string = NULL;
        char *com_argument= NULL;
        char *text= NULL;
        char *text1= NULL;
        char *text2= NULL;
        char *text3= NULL;
        char *text4= NULL;
        char *text5= NULL;
        char *text6= NULL;
        char *text7= NULL;
        char *text8= NULL;
        char *text9= NULL;
        char *text10= NULL;
        char *text11= NULL;
        char *text12= NULL;
        

        //Intilaize a int, this is needed for the code to know
        //if a argument and text came with the command
        int j = 0;
        
        //Loop removes " " from "pillar", this is needed to
        //seperate command and argument
        for (clean_string = strtok_r(pillar, " ", &spacePars); 
        clean_string; 
        clean_string = strtok_r(NULL, " ", &spacePars)){    

            // For the first string in "clean_string", store any
            // command string to "command_string"   
            if(j == 0){
                command_string = clean_string;
            }
            //Either command came with arguments, store the arguments
            //to "com_argument, store NULL if theres no arguments"
           else if (j == 1){
                com_argument = clean_string;
            } 
            else if (j == 2) {
            // Check if 3rd argument has a text file, if so save it and so on
                text = clean_string;
            }
            else if (j == 3) {
                text1 = clean_string;
            }
            else if (j == 4) {
                text2 = clean_string;
            }
            else if (j == 5) {
                text3 = clean_string;
            }
            else if (j == 6) {
                text4 = clean_string;
            }
            else if (j == 7) {
                text5 = clean_string;
            }
            else if (j == 8) {
                text6 = clean_string;
            }
            else if (j == 9) {
                text7 = clean_string;
            }
            else if (j == 10) {
                text8 = clean_string;
            }
            else if (j == 11) {
                text9 = clean_string;
            }
            else if (j == 12){
                text10 = clean_string;
            }
            else if (j == 13) {
                text11 = clean_string;
            }
            else{
                text12 = clean_string;
            }
            j++;                     
        }
    

// TOKENIZING PILLAR ////////////////////////////////////////////////
        // Check for cd, if true then chdir
        int cdVar = 0;    
        if (strcmp(command_string,"cd") == 0){
            char *commandList[] = {command_string, com_argument, NULL};
            chdir(commandList[1]);
            cdVar = 1;
        }

        // PIPING //////////////////////////////////////////////////////////

            // If command is not last command
           if(i < num_pillars && cdVar != 1){
            
            // Pipe fd
            pipe(fd);

            // Create 
            pid = fork();
            if (pid == 0) {
                close(fd[0]);
                
                // Changes stdin to last pipe
                if (lastPipe != STDIN_FILENO) {
                    dup2(lastPipe, STDIN_FILENO);
                }
                
                // Changes stdout to current pipe
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                
                // Execute Command
                char *commandList[] = {command_string, com_argument, text, text1, text2, text3, text4, text5, text6, text7, text8, text9, text10, text11, text12, NULL};
                
                execvp(commandList[0], commandList);
                err_exit("execvp failed");
                exit(1);
            }
        	waitpid(pid, NULL, 0);
            close(fd[1]);
            close(lastPipe);
            // Save the read end of the pipe to the next last pipe
            lastPipe = fd[0];

        }
        // If command is last command or only one command
        else if(i == num_pillars && cdVar != 1){

            // Fork a child
            pid = fork();
            if(pid == 0){
                
                // Changes to last pipe
                if (lastPipe != STDIN_FILENO) {
                    dup2(lastPipe, STDIN_FILENO);
                }

                // Executes Last Command
                char *commandList[] = {command_string, com_argument, text, text1, text2, text3, text4, text5, text6, text7, text8, text9, text10, text11, text12, NULL};

                execvp(commandList[0], commandList);
                err_exit("execvp failed");
                exit(1);
            }
            // Wait for child to finish
        	waitpid(pid, NULL, 0);
        }
        // PIPING //////////////////////////////////////////////////////////
        i++;
    }
    free(string);
    string = NULL;
    }while(1);
}









// HITORY CODE ///////////////////////////////////////////////////////////
/*Creates the first Queue*/
struct Node *queueCretated(char *string){
    struct Node *create_queue = (struct Node *) malloc(sizeof(struct Node));
    create_queue->data = (char*) malloc(sizeof(char) * strlen(string));

    strcpy(create_queue->data, string);

    create_queue->tail = NULL;
    create_queue->head = NULL;

    ///////// SEE STRING ///////////////
    // printf("\n(%s)\n",create_queue->data);
    ////////////////////////
    return create_queue;
}

//////////////////////////////////////////
/*Creates the newist and latest queue*/
struct Node *enQueue(struct Node *queue_tail, char *command_string){
    struct Node *new_queue = (struct Node *) malloc(sizeof(struct Node));


    new_queue->data = (char*) malloc(sizeof(char) * strlen(command_string));
    strcpy(new_queue->data, command_string);

    new_queue->tail = NULL;
    new_queue->head = queue_tail;

    queue_tail->tail = new_queue;
    
    ///////// SEE STRING ///////////////
    // printf("\n(%s)\n",new_queue->data);
    /////////////////////////
    return new_queue;
}

//////////////////////////////////////////
/*Deletes the first Queue and deletes the Queue as a whole*/
struct Node *queueDeuque(struct Node *queue_head){

    if (queue_head->tail == NULL) { 
        free(queue_head->data);
        free(queue_head);        
        queue_head = NULL;
    }
    else{
        struct Node *temp = queue_head->tail;

        temp->head = NULL;
        queue_head->tail = NULL;

        free(queue_head->data);
        free(queue_head);

        queue_head = temp;
    }
    return queue_head;
}

//////////////////////////////////////////
/*Print history of code*/
void Queueoffset(struct Node * queuehead, int8_t total_offsets){

    if(total_offsets < MAX){
        printf("\nOffset out of bounds...\n");
    }
    else{
        struct Node *temp = queuehead;
        for(int i = 0; i != total_offsets; i++){
            temp = temp->tail;
        }
        printf("%d %s\n",total_offsets, temp->data);
    }   
}

//////////////////////////////////////////
/*Prints the whatever is store in queue*/
void printQueue(struct Node *queue_tail){

    // Print Queu 
    printf("\nPrinting DATA...");

    if(queue_tail == NULL){
        printf("\nQueue is Empty...\n");
    }
    else{
        struct Node *temp = queue_tail;
        printf("\n");
        while(temp != NULL){
            printf("Data: %s\n", temp->data);
            temp = temp->head;
        }
        printf("\n");
    }
}

//////////////////////////////////////////
void err_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
/////////////////////////////////////////////////////////////////////////////////////////////////