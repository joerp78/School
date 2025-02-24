#include <tsh.h>

using namespace std;

/**
 * @brief
 * Helper function to print the PS1 pormpt.
 *
 * Linux has multiple promt levels
 * (https://wiki.archlinux.org/title/Bash/Prompt_customization): PS0 is
 * displayed after each command, before any output. PS1 is the primary prompt
 * which is displayed before each command. PS2 is the secondary prompt displayed
 * when a command needs more input (e.g. a multi-line command). PS3 is not very
 * commonly used 
 */
void display_prompt() { cout << "$ " << flush; }

/**
 * @brief Cleans up allocated resources to prevent memory leaks.
 *
 * This function deletes all elements in the provided list of Process objects,
 * clears the list, and frees the memory allocated for the input line.
 *
 * @param process_list A pointer to a list of Process pointers to be cleaned up.
 * @param input_line A pointer to the dynamically allocated memory for user
 * input. This memory is freed to avoid memory leaks.
 */
void cleanup(list<Process *> &process_list, char *input_line) {
  for (Process *p : process_list) {
    delete p;
  }
  process_list.clear();
  free(input_line);
  input_line = nullptr;
}

/**
 * @brief Main loop for the shell, facilitating user interaction and command
 * execution.
 *
 * This function initializes a list to store Process objects, reads user input
 * in a loop, and executes the entered commands. The loop continues until the
 * user decides to quit.
 *
 * @note The function integrates several essential components:
 *   1. Displaying the shell prompt to the user.
 *   2. Reading user input using the read_input function.
 *   3. Parsing the input into a list of Process objects using parse_input.
 *   4. Executing the commands using run_commands.
 *   5. Cleaning up allocated resources to prevent memory leaks with the cleanup
 * function.
 *   6. Breaking out of the loop if the user enters the quit command.
 *   7. Continuously prompting the user for new commands until an exit condition
 * is met.
 *
 * @warning
 *  Initialize Necessary Variables:
 *      Declare variables for storing the list of Process objects, the input
 * line, and a flag for quitting. Read User Input in a Loop: Utilize a while
 * loop to continuously read user input until a termination condition is met.
 * Check for a valid input line and skip empty lines. Clean Up Resources: After
 * executing the commands, clean up allocated resources using the cleanup
 *      function to avoid memory leaks.
 *  Check for Quit Command:
 *      Determine if the user entered the quit command. If so, set the exit flag
 * to true and break out of the loop. Repeat the Process: If the user did not
 * quit, continue prompting for input by displaying the prompt again.
 *  Considerations:
 *      Handle edge cases such as empty input lines or unexpected errors
 * gracefully. Ensure proper error handling and informative messages for the
 * user.
 */
void run() {
  list<Process *> process_list;
  char *input_line;
  bool is_quit = false;
  bool status; 
  
  do {
    display_prompt();
    //cout << "tsh> " ;
    input_line = read_input(); 
    //cout << "RUN INPUT:" << input_line << endl; 
    parse_input(input_line, process_list);
    status = run_commands(process_list);  
    is_quit = status; 
    sleep(1);
  } while(is_quit == false && input_line != NULL);

  cleanup(process_list, input_line);
}

/**
 * @brief Reads input from the standard input (stdin) in chunks and dynamically
 *        allocates memory to store the entire input.
 *
 * This function reads input from the standard input or file in chunks of size MAX_LINE.
 * It dynamically allocates memory to store the entire input, resizing the
 * memory as needed. The input is stored as a null-terminated string. The
 * function continues reading until a newline character is encountered or an
 * error occurs.
 *
 * @return A pointer to the dynamically allocated memory containing the input
 * string. The caller is responsible for freeing this memory when it is no
 * longer needed. If an error occurs or EOF is reached during input, the
 * function returns NULL.
 *
 * @warning Ensure that the memory allocated by this function is freed using
 * free() to avoid memory leaks.
 */
char *read_input() {

  size_t inputlen = 0; 
  size_t templen = 0;
  char *input = NULL;
  char tempArr[MAX_LINE];

  /*input = (char*) malloc(MAX_LINE * sizeof(char));
  if (input == NULL){
    printf("MEM NOT ALLOCATED INITIALLY\n");
    return NULL; 
  }
  
  clearerr(stdin);*/ 

  int count = 0; 
  do{
    size_t templen = strlen(tempArr); 
    if (fgets(tempArr, MAX_LINE, stdin) == nullptr){
      return input; 
    }
    //cout << templen << endl;
    //for (size_t i = 0; i < templen; i++) {
      //printf("%c", tempArr[i]);
    //}
    //printf("\n"); 

    //if(tempArr[templen - 1] == '\n'){
      //break;
    //}

    input = (char*) realloc(input, (inputlen + templen+ 1));
    if (input == NULL) {
      printf("MEM NOT ALLOCATED REALLOC\n");
      return NULL;
    }
    strcpy(input + inputlen, tempArr);
    inputlen += templen; 
    //cout << input << endl;
    //break; 
  } while (templen == MAX_LINE - 1 && tempArr[MAX_LINE - 2] != '\n');

  /*  
  for (size_t i = 0; i < inputlen; i++) {
    printf("%c", input[i]);
  }
  printf("\n"); 

  

  if (input[0] == '\n') {
    free(input);
    return NULL;
}

  if (inputlen == 0) {
    free(input);
    return NULL;
}

  char *temp = (char*) realloc(input, (inputlen + 1) * sizeof(char));
  if (temp == NULL){
    printf("MEM NOT ALLOCATED REALLOC\n");
    free(input);
    return NULL; 
  }
  //input = temp; 
  
  //printf("%d\n",inputlen);
  //printf("%s\n",input);*/


  return input;
}

/**
 * @brief
 * removes the new line char of the end in cmd. 
 */
void senetize(char* cmd) {
  if (cmd == nullptr){return;}
  //cout << cmd << endl;
  for (char *c = cmd; *c != '\0'; ++c){
    //printf("%c\n", *c); 
    if (*c == '\n'){
        *c = '\0'; 
    }
  }
}


/**
 * Parses the given command string and populates a list of Process objects.
 *
 * This function takes a command string and a reference to a list of Process
 * pointers. It tokenizes the command based on the delimiters "|; " and creates
 * a new Process object for each token. The created Process objects are added to
 * the provided process_list. Additionally, it sets pipe flags for each Process
 * based on the presence of pipe delimiters '|' in the original command string.
 *
 * @param cmd The command string to be parsed.
 * @param process_list A reference to a list of Process pointers where the
 * created Process objects will be stored.
 *
 * Hints for students:
 * - The 'delimiters' we need to handle are '|', ';', and '[space]' 
 * - 'pipe_in_val' is a flag indicating whether the current Process should take
 *      input from a previous Process (1 if true, 0 if false).
 * - 'currProcess' is a pointer to the current Process being created and added
 *      to the list.
 * - When a delim is a space it calls currProcess->add_token()
 * - When the delimiter is a ";" or "|", a new Process object is created with
 *       relevant information, and the pipe flags are set based on the presence
 *       of '|' in the original command.
 * - The created Process objects are added to the process_list.
 */

void parse_input(char *cmd, list<Process *> &process_list) {
  int pipe_in_val = 0;
  Process *currProcess = nullptr;
  //cout << cmd << endl;
  senetize(cmd);
  //cout << cmd << endl;
  
  //printf("HERE");
  if (cmd == nullptr){
    cerr << "Error : Receieved Null Command Input." << endl;
    return;
  }

  
 
  size_t cmdLen = strlen(cmd);; 
    
  char *tempWord = (char*) malloc((cmdLen + 1)  * sizeof(char));
  int tempIndex = 0; 


  //cout << cmdLen << endl; 

  for (int i = 0; i<cmdLen; ++i)
  {
    if (cmd[i] == ' '){
      //cout << "found SPACE" << endl; 
    if (tempIndex >0){
      tempWord[tempIndex++] = '\0';
        if (!currProcess){
          currProcess = new Process(pipe_in_val, 0);
          process_list.push_back(currProcess);
          //cout << "found SPACE, creating Process" << endl; 

        }
        //tempWord[tempIndex++] = '\0'; 
        currProcess->add_token(tempWord);
        tempIndex = 0;
        //cout << "found SPACE, adding Word" << endl; 
      }
    }
    else if (cmd[i] == '|'){
      if (currProcess){
        currProcess->pipe_out = 1; 
      }
      //cout << "found |" << endl; 
      currProcess = new Process(1, 0);
      process_list.push_back(currProcess);
      //cout << "found |, creating Process" << endl; 

      pipe_in_val = 1; 
      currProcess = nullptr;

    }
    else if (cmd[i] == ';')
    {
      //cout << "found ;" << endl; 
      
      currProcess = new Process(0, 0);
      process_list.push_back(currProcess);
      //cout << "found ;, creating Process" << endl; 

      pipe_in_val = 0; 
      currProcess = nullptr;
    }
    else 
    {
      tempWord[tempIndex++] = cmd[i]; 
    }

  }
  if (tempIndex > 0){
    //tempWord[tempIndex++] = '\0';
    if (!currProcess){
      currProcess = new Process(pipe_in_val, 0);
      process_list.push_back(currProcess);
      //cout << "creating Process" << endl; 

    }
  
    //cout << tempWord << endl;
    int wordLen = strlen(tempWord);
    tempWord[tempIndex++] = '\0'; 
    currProcess->add_token(tempWord);
    //cout << "adding Word" << endl; 
  }

  free(tempWord);

}


/**
 * Check if the given command represents a quit request.
 *
 * This function compares the first token of the provided command with the
 * string "quit" to determine if the command is a quit request.
 *
 * Parameters:
 *   - p: A pointer to a Process structure representing the command.
 *
 * Returns:
 *   - true if the command is a quit request (the first token is "quit").
 *   - false otherwise.
 */
bool isQuit(Process *p) {
  for (int j = 0; j < 25; ++j){
    if (p->cmdTokens[j] != nullptr){
      //cout << "Token: " << p->cmdTokens[j] << endl;
      if (strcmp(p->cmdTokens[j], "quit") == 0) {
        //cout << endl; 
        return true;
      }
    }
  }
  return false; 
}

/**
 * @brief Execute a list of commands using processes and pipes.
 *
 * This function takes a list of processes and executes them sequentially,
 * connecting their input and output through pipes if needed. It handles forking
 * processes, creating pipes, and waiting for child processes to finish.
 *
 * @param command_list A list of Process pointers representing the commands to
 * execute. Each Process object contains information about the command, such as
 *                     command tokens, pipe settings, and file descriptors.
 *
 * @return A boolean indicating whether a quit command was encountered during
 * execution. If true, the execution was terminated prematurely due to a quit
 * command; otherwise, false.
 *
 * @details
 * The function iterates through the provided list of processes and performs the
 * following steps:
 * 1. Check if a quit command is encountered. If yes, terminate execution.
 * 2. Create pipes and fork a child process for each command.
 * 3. In the parent process, close unused pipes, wait for child processes to
 * finish if necessary, and continue to the next command.
 * 4. In the child process, set up pipes for input and output, execute the
 * command using execvp, and handle errors if the command is invalid.
 * 5. Cleanup final process and wait for all child processes to finish.
 *
 * @note
 * - The function uses Process objects, which contain information about the
 * command and pipe settings.
 * - It handles sequential execution of commands, considering pipe connections
 * between them.
 * - The function exits with an error message if execvp fails to execute the
 * command.
 * - Make sure to properly manage file descriptors, close unused pipes, and wait
 * for child processes.
 * - The function returns true if a quit command is encountered during
 * execution; otherwise, false.
 *
 * @warning
 * - Ensure that the Process class is properly implemented and contains
 * necessary information about the command, such as command tokens and pipe
 * settings.
 * - The function relies on proper implementation of the isQuit function for
 * detecting quit commands.
 * - Students should understand the basics of forking, pipes, and process
 * execution in Unix-like systems.
 */

bool run_commands(list<Process *> &command_list) 
{
  bool is_quit = false;
  int size = command_list.size();
  //pid_t pids[size];
  vector<pid_t> pids;
  Process *prev = nullptr;

  int pPipe[2], nPipe[2];
  bool has_prevPipe = false;  

  //for (Process *p : command_list)
  for (int p = 0; p < size; p++)
  {
    //cout << p->cmdTokens[1] << endl; 
    std::list<Process*>::iterator it = command_list.begin();
    std::advance(it, p);
    Process* proc = *it;    
    
    if(isQuit(proc)) return (is_quit = true); 

    if (!proc || !proc->cmdTokens[0]) {
      //cerr << "Invalid command at index " << p << endl;
      continue;
  }
    //cout << command << endl; 
    
    char *command = proc->cmdTokens[0];
    char *tempArg[25] = {};
    int tempIndex = 0; 

    for (char **c = proc->cmdTokens; *c != nullptr; ++c)
    {
      tempArg[tempIndex++] = *c;  
      //cout << *c << endl;     
    }
    tempArg[tempIndex] = nullptr; 


    bool needs_pipeOut = (proc->pipe_out == 1);  
    if (needs_pipeOut){
      if(pipe(nPipe) < 0)
          {
            perror("PIPE FAILED");
            return false; 
          }
        //else printf("PIPE CREATED\n");
      }

    pid_t pid = fork();
    if (pid == -1){
      perror("FORK FAILED");
      return false;
    }

    //cout << command << endl;
    //cout << tempArg[0] << endl; 
        // Debugging: Print the command and arguments
        //cout << "Executing command: " << command << "\n";
        for (int i = 0; tempArg[i] != nullptr; ++i) {
          //cout << "Arg[" << i << "]: " << tempArg[i] << "\n";
        }    


    if (pid == 0){
      if (proc->pipe_in == 1 && has_prevPipe){
            dup2(pPipe[0], STDIN_FILENO); 
            close(pPipe[0]);
            close(pPipe[1]); 
        }
    
        if (needs_pipeOut){
          dup2(nPipe[1], STDOUT_FILENO); 
          close(nPipe[0]);
          close(nPipe[1]); 
      }

        // Debugging: Print the command and arguments
        //cout << "Executing command: " << command << "\n";
        for (int i = 0; tempArg[i] != nullptr; ++i) {
          //cout << "Arg[" << i << "]: " << tempArg[i] << "\n";
        }
        fflush(stdout);
        execvp(command, tempArg); 
        perror("EXEC FAILED");
        exit(EXIT_FAILURE);
    }
    /*
    if (p->pipe_out == 1){
      close(fd[1]);
    }
    if (p->pipe_in == 1){
      close(fd[0]);
    }
    */
    else {
    pids.push_back(pid); 

    if(has_prevPipe){
      close(pPipe[0]);
      close(pPipe[1]);
    }

    if (needs_pipeOut){
      pPipe[0] = nPipe[0];
      pPipe[1] = nPipe[1];
      has_prevPipe = true;
    }
    else has_prevPipe = false; 


    }
}



for (pid_t pid : pids){
  int status; 
  waitpid(pid, &status, 0);

  if (WIFEXITED(status)) {
    //printf("Process %d exited with status %d\n", pid, WEXITSTATUS(status));
  } 
  else {
    fprintf(stderr, "Process %d terminated abnormally\n", pid);
  }
}
  command_list.clear(); 
  return is_quit;
}

/** bs
 * @brief Constructor for Process class.
 *
 * Initializes a Process object with the provided command string, input pipe,
 * and output pipe.
 *
 * @param _pipe_in The input pipe descriptor.
 * @param _pipe_out The output pipe descriptor.
 */
Process::Process(int _pipe_in, int _pipe_out) {
  pipe_in = _pipe_in;
  pipe_out = _pipe_out;
  i = 0;
  for (int j = 0; j < 25; j++){
    cmdTokens[j] = nullptr; 
  }
}

/**
 * @brief Destructor for Process class.
 */
Process::~Process() {
  for (int j = 0; j < 25; j++){
    if (cmdTokens[j] != nullptr){
      delete[] cmdTokens[j];
    }
  }
}

/**
 * @brief add a pointer to a command or flags to cmdTokens
 * 
 * @param tok 
 */
void Process::add_token(char *tok) {
  if (i < 25){
    cmdTokens[i] = new char [strlen(tok) + 1];
    strcpy(cmdTokens[i], tok); 
    i++;
  }
  else {
    std::cerr << "ERR:TOKEN LIST IS FULL" << endl;
  }
}

      /* if (p->pipe_in == 1 && has_prevPipe) {
          dup2(pPipe[0], STDIN_FILENO); 
        }
        if (needs_pipeOut) {
            dup2(nPipe[1], STDOUT_FILENO); 
        }

        close(pPipe[0]); close(pPipe[1]); 
        close(nPipe[0]); close(nPipe[1]);   

        if (p->pipe_in == 1 & p->pipe_out == 1){
            dup2(fd[0],0); //STDIN replaced by IN Pipe
            dup2(fd[1],1); //STDOUT replaced by OUT Pipe
        }
            else if(p->pipe_in = 1){
              printf("PIPE IN TRUE\n");
              dup2(fd[0],0); //STDIN replaced by IN Pipe
              close(fd[1]);
            }

            else if(p->pipe_out = 1){
              printf("PIPE OUT TRUE\n");
              dup2(fd[1],1); //STDOUT replaced by OUT Pipe
              close(fd[0]); 
            }*/ 