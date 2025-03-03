#include <tsh.h>

using namespace std;
//VERSION 22 WORKING 92.6 SCORE; PASSES ALL TEST CASES
//JOE PAOLA, 3/3/25, CS 377
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
    input_line = read_input(); 
    parse_input(input_line, process_list);
    status = run_commands(process_list);  
    is_quit = status; 

  } while(is_quit == false && input_line != nullptr); //ensures that the loop runs until the user quits and maintains valid input

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

  size_t inputlen = 0; //size of the input
  size_t templen = 0; //size of temp buffer
  char *input = NULL; //input buffer
  char tempArr[MAX_LINE]; //temp buffer


  int count = 0; 
  //loops through input collecing it 81 characters at a time until a newline is encountered
  while (fgets(tempArr, MAX_LINE, stdin)) {
    size_t templen = strlen(tempArr);
    char *new_input = (char*) realloc(input, inputlen + templen + 1);
    if (new_input == NULL) {
        printf("MEM NOT ALLOCATED REALLOC\n");
        free(input);
        return NULL;
    }
    input = new_input;
    strcpy(input + inputlen, tempArr); //append next batch of input to the buffer 
    inputlen += templen;
    
    //breaks if newLine is encountered and EOF 
    if (templen > 0 && tempArr[templen - 1] == '\n') {
        break;
    }
}

  return input;
}

/**
 * @brief
 * removes the new line char of the end in cmd. 
 */
void senetize(char* cmd) {
  if (cmd == nullptr){return;}

  //iterates through cmd and replaces new line char with null char
  for (char *c = cmd; *c != '\0'; ++c){
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
  
  senetize(cmd); //remove NL characters
  
  if (cmd == nullptr){
    cerr << "Error : Receieved Null Command Input." << endl;
    return;
  }

  size_t cmdLen = strlen(cmd); 
  char* tempWord = (char*)malloc((cmdLen + 1)  * sizeof(char)); //allocate memory for tempWord based on cmd length
  int tempIndex = 0; 



  for (int i = 0; i<cmdLen; ++i)
  {
    char ch = cmd[i]; //current character in command

    //Handles spaces in CMD
    if (ch == ' '){
    //Add null terminator to tempWord to distinguish between words
    if (tempIndex > 0){
      tempWord[tempIndex] = '\0';
        //ensures process exists for assignment 
        if (!currProcess){
          currProcess = new Process(pipe_in_val, 0);
          process_list.push_back(currProcess);

        }
        currProcess->add_token(tempWord);

        //terminates parsing if user quit command encountered
        if (isQuit(currProcess)){
          free(tempWord);
          return; 
        }
        tempIndex = 0; //reset tempWord index for next word 
      }
    }
    
    //Handles pipes in CMD
    else if (ch == '|'){
      //add null terminator to tempWord to distinguish between words
      if (tempIndex > 0){
        tempWord[tempIndex] = '\0';
        //ensures process exists for assignment
        if (!currProcess){
          currProcess = new Process(pipe_in_val, 0);
          process_list.push_back(currProcess);         
          }
        currProcess->add_token(tempWord); //push word to process

        //terminates parsing if user quit command encountered
        if (isQuit(currProcess)){
          free(tempWord);
          return; 
        }
        tempIndex = 0; 
      }

      //sets pipe_out flag for current process 
      if (currProcess){
        currProcess->pipe_out = 1;  
      }
      currProcess = new Process(1, 0); //create a new process that will need to recieve a pipe_in_val
      process_list.push_back(currProcess); //add process to list 

      pipe_in_val = 1; 
    }

    //Handles semicolons in CMD
    else if (ch == ';')
    {
      //add null terminator to tempWord to distinguish between words
      if (tempIndex > 0){
        tempWord[tempIndex] = '\0';
        //ensures process exists for assignment
        if (!currProcess){
          currProcess = new Process(pipe_in_val, 0);
          process_list.push_back(currProcess);
      }
        currProcess->add_token(tempWord); //add token to process
        //terminates parsing if user quit command encountered
        if (isQuit(currProcess)){
          free(tempWord);
          return; 
        }
        tempIndex = 0; //reset tempWord index for next word
      }

      //adds semicolon to token to signal end of batch command
      if (currProcess){
        currProcess->add_token(";");
      }

      currProcess = nullptr; //set current process to null so that a new process is created
      pipe_in_val = 0; 
      
    }
    //when a delimiter is not encountered, add the character to tempWord so it can create a complete command
    else 
    {
      tempWord[tempIndex++] = ch; 
    }

  }
  //cleanup for unprocessed commands left at end of input, generate new process for them
  if (tempIndex > 0){
    tempWord[tempIndex] = '\0';
    if (!currProcess){
      currProcess = new Process(pipe_in_val, 0);
      process_list.push_back(currProcess);
    }
    currProcess->add_token(tempWord);
    if (isQuit(currProcess)){
      free(tempWord);
      return; 
    }
    tempIndex = 0;
    
  }

  free(tempWord); //free memory allocated for tempWord

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
  //scans through token list to see if any tokens are "quit", returns boolean value depending on success
  for (int j = 0; j < 25; ++j){
    if (p->cmdTokens[j] != nullptr){
      //cout << "Token: " << p->cmdTokens[j] << endl;
      if (strcmp(p->cmdTokens[j], "quit") == 0) {
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
  bool is_quit = false; //flag for quit command
  int size = command_list.size();
  vector<pid_t> pids; //list of pids for child processes
  Process *prev = nullptr;

  int pPipe[2], nPipe[2]; //pipe descriptors for previous and next processes
  bool has_prevPipe = false;  //boolean to determine if there is a previous pipe

  //for (Process *p : command_list)
  for (int p = 0; p < size; p++)
  {
    //cout << p->cmdTokens[1] << endl;
    //iterator  
    std::list<Process*>::iterator it = command_list.begin();
    std::advance(it, p);
    Process* proc = *it;    
    

    bool flush_after = false;
    int token_count = 0;
    //iterates through command tokens and strips out semicolons to prevent exec errors
    while(proc->cmdTokens[token_count] != nullptr)
    {
      token_count++;
    } if(token_count > 0 && strcmp(proc->cmdTokens[token_count - 1], ";") == 0)
    {
      proc->cmdTokens[token_count - 1] = nullptr;
      flush_after = true;
    }

    //checks if quit command is encountered, setting flag to true
    if(isQuit(proc)){  
      is_quit = true; 
      break; 
    }

    
    char *command = proc->cmdTokens[0]; //command is always the first token
    char *tempArg[25] = {}; //temp array to store command arguments
    int tempIndex = 0; 

    //loop to assign argument char pointers to tempArg
    for (char **c = proc->cmdTokens; *c != nullptr; ++c)
    {
      tempArg[tempIndex++] = *c;  
    }
    tempArg[tempIndex] = nullptr; 


    bool needs_pipeOut = (proc->pipe_out == 1);  //sets pipeout flag based on process attribute 
    //start pipe creation if needed 
    if (needs_pipeOut){
      if(pipe(nPipe) < 0)
          {
            perror("PIPE FAILED");
            return false; 
          }
        //else printf("PIPE CREATED\n");
      }

    //fork child processes to execute commands
    pid_t pid = fork();
    //if returns negative value fork failed
    if (pid == -1){
      perror("FORK FAILED");
      return false;
    }

    //child process
    if (pid == 0){
      //checks if quit command is encountered, exits from child without executing command
      if(isQuit(proc)) {
        exit(EXIT_SUCCESS); 
      }

      //if there is a previous pipe, set up input pipe
      if (proc->pipe_in == 1 && has_prevPipe){
            dup2(pPipe[0], STDIN_FILENO); 
            close(pPipe[0]);
            close(pPipe[1]); 
        }
        //if there is a next pipe, set up output pipe
        if (needs_pipeOut){
          dup2(nPipe[1], STDOUT_FILENO); 
          close(nPipe[0]);
          close(nPipe[1]); 
      }
        //DEBUGGING CODE
        //cout << "Executing command: " << command << "\n";
        //for (int i = 0; tempArg[i] != nullptr; ++i) {
          //cout << "Arg[" << i << "]: " << tempArg[i] << "\n";
        //}

        //executes command with arguments, error handling if exec fails
        fflush(stdout);
        execvp(command, tempArg); 
        perror("EXEC FAILED");
        exit(EXIT_FAILURE);
    }

    //parent process
    else {
    //adds process id to stack for future waiting
    pids.push_back(pid); 
    
    //closes pipes if they are not needed
    if(has_prevPipe){
      close(pPipe[0]);
      close(pPipe[1]);
    }

    //sets up previous pipe for next process & boolean logic
    if (needs_pipeOut){
      pPipe[0] = nPipe[0];
      pPipe[1] = nPipe[1];
      has_prevPipe = true;
    }
    else has_prevPipe = false; 
    }
    //waits for child processes to finish if flush_after flag is set indicating a ; and batch processing required
    if (flush_after) {
      for (pid_t p : pids) {
        int status; 
        waitpid(p, &status, 0);
      }
      pids.clear();
      has_prevPipe = false;
    }
  }


//waits for all child processes to finish before returning
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
  //sets pipe_in and pipe_out attributes to input values, fills cmdTokens with null pointers
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
  //frees all tokens in cmdTokens and sets them to null
  for (int j = 0; j < 25; j++){
    if (cmdTokens[j] != nullptr){
      free(cmdTokens[j]);
      cmdTokens[j] = nullptr;
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
    //tokens are copied from input to cmdTokens in object
    cmdTokens[i] = strdup(tok);
    i++;
  }
  else {
    std::cerr << "ERR:TOKEN LIST IS FULL" << endl;
  }
}
