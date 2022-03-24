#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#ifdef IMP
#include "reference_stackADT.h"
#else
#include "stackADT.h"
#endif

#include "memtrace.h"

// maximum size of a single command 
#define MAX_INPUT_SIZE 4096
// maximum size of a operation
#define MAX_OP_SIZE 64


void print_command_help();
int process(char *input, Stack dataStack, Stack opStack);
bool is_int(char *);
int runOperation(char *op, Stack dataStack);
int runCloseParen(Stack dataStack, Stack opStack);
bool higherPriority(char *op1, char *op2);

void error_msg_extraData(char *cmd) {
  printf("ERROR: %s: found data left over!\n", cmd);
}

void error_msg_badCommand(char *cmd) {
  printf("ERROR: bad command!\n");
}

void error_msg_opStackNotEmpty(char *cmd) {
  printf("ERROR: %s: not able to process all operations\n", cmd);
}

void error_msg_missingResult(char *cmd) {
  printf("ERROR: %s: no result!\n", cmd);
}

void error_msg_opMissingArgs(char *op) {
  printf("ERROR: op %s: empty stack need two arguments: found none.\n", op);
}

void error_msg_divByZero(void) {
  printf("Error: Division by zero!\n");
}

void error_msg_badOp(char *op) {
  printf("Error: Unrecognized operator: %s!\n", op);
}

int main(int argc, char *argv[]) 
{
  Stack dataStack;
  Stack opStack;
  char *command = NULL;
  int max_input = MAX_INPUT_SIZE;
  int result;

  dataStack = Stack_create();
  opStack = Stack_create();
  //variable to be used for
  int runOp;
  int proc;

  while (true) {
    // Read a command to calculate into memory. Try using fgets().
    // If the command was NULL, break
    command = (char *)malloc(sizeof(char) * max_input); // memory for new command
    char *str = fgets(command, max_input, stdin);
    printf(str);
    if (!str || *str == '\n') {
      free(command);
      break;
    } 
    proc = process(command, dataStack, opStack);
    //if process returns correctly
    if (proc == 0) {
      //while the stack is not empty
      while (!Stack_is_empty(opStack)) {
        //char *opTop = (void*) malloc(sizeof(char));
        //pop top off
        char *opTop = Stack_pop(opStack);
        //call runOperation with the operator at the top of the stack
        runOp = runOperation(opTop, dataStack);
        //push top back
        free(opTop);
        opTop = NULL;
        if (runOp == -1) {
          //if -1 is returned then runOperation failed and we break
          break;
        } 
      }
      //if we get here then runOperation did not fail
      if (runOp != 1 && !Stack_is_empty(opStack)){
        error_msg_opStackNotEmpty(command);
      } else if (runOp == -1) {
        free(command);
        break;
      } else if (Stack_is_empty(dataStack)) {
        //if data stack is empty
        //error_msg_missingResult(command);
      } else {
        //malloc this?
        int *dataTop;
        dataTop = Stack_pop(dataStack);
        result = *dataTop;
        free(dataTop);
        dataTop = NULL;
        if (!Stack_is_empty(dataStack)) {
          error_msg_extraData(command);
        } else {
          printf("= %d\n", result);
        }
      }
    }
    //empty the stacks to prep for next command
    Stack_make_empty(opStack);
    Stack_make_empty(dataStack);
    //must free command at the end of every iteration of the while loop
    free(command);
  }
  Stack_destroy(opStack);
  Stack_destroy(dataStack);
  return 0;
}


int
process(char *command, Stack dataStack, Stack opStack){
  char delim[] = " ";
  //int data = malloc(sizeof(int));
  int *data = ((void *)0);
  char *operation = ((void *)0);
  int rc = 0;
  int openTest = 0;
  int closeTest = 0;

  char* token = strtok(command, delim);
  while (token != ((void *)0)) {
    data = (int*) malloc(sizeof(int));
    operation = (char*) malloc(sizeof(char));

    //token = strtok(((void *)0), delim);
    //if token is a data term push it to data stack
    if (sscanf(token, "%d", data) == 1) {
      Stack_push(dataStack, data);
      //free(data);
      data = NULL;
      //free and set to null
      free(operation);
      operation = NULL;
    } else {
    //if it is a closed parenthesis we need to call runCloseParen()
    free(data);
    data = NULL;
    closeTest = sscanf(token, "%c", operation);
    if (strcmp(")", operation) == 0) {
      free(operation);
      operation = NULL;
      int closeParenTest;
      closeParenTest = runCloseParen(dataStack, opStack);
      if (closeParenTest == -1) {
        rc = -1;
        break;
      }
    } else { 
      if (strcmp("(", operation) == 0) {
        //if it is an open parenthesis, we just push it to the stack
        Stack_push(opStack, operation);
        //free(operation);
        operation = NULL;
    } else {
      //if we get here we know that it is an operator
      while (true) {
        if (Stack_is_empty(opStack)) {
          //if opStack is empty we break
          //free(operation);
          break;
        }
        char *topOp = Stack_pop(opStack);
        //if the operator on top of the stack is of higher priority, then 
        //call runOperation() with the previous operator
        if (strcmp("(", topOp) == 0) {
          Stack_push(opStack, topOp);
          free(topOp); // this was missing
        }
        else if (higherPriority(topOp, operation)) {
          int testRunOp;
          testRunOp = runOperation(topOp, dataStack);
          //have to free top operator
          free(topOp);
          topOp = NULL;
          if (testRunOp == -1) {
            free(operation);
            operation = NULL;
            rc = -1;
            break;
          }
          
        } else {
          //keep everything the way it was
          break;
        }
      }
      

      //push the new operator onto the op stack
      Stack_push(opStack, operation);
      
      //free(operation); //need to free op, was missing
      //operation = NULL;
    }
    }
  }
  token = strtok(NULL, delim);
  //return rc (0) if everything is successful
  //return rc;
  }

  return rc;
}


int
runCloseParen(Stack dataStack, Stack opStack) {
  int rc = 0;
  char *op = ((void *)0);
  //malloc for top op var
  
  while (true) {
    //if the op stack is empty we must break
    if (Stack_is_empty(opStack)) {
      //printf("hello \n");
      rc = -1;
      break;
    }
    char *op = Stack_pop(opStack);
    //store the top operator in a pointer
    if (strcmp("(", op) == 0) {
      free(op);
      op = NULL;
      break;
    } else {
      //if we get here then we are gonna call runOperation() with whatever is on top of the op stack
        int runOpTest;
        runOpTest = runOperation(op, dataStack); 
        if (runOpTest == -1) {
          rc = -1;
          free(op);
          op = NULL;
          break;
        } else {
          free(op);
          op = NULL;
          break;
        }
        //break;
      }
    }
    //if runOperation failed then we return an error
    return rc;
  }

int
getPriority(char* op)
{
  if(!strcmp(op,"*") || !strcmp(op, "/")) return 2;
  if(!strcmp(op,"+") || !strcmp(op, "-")) return 1;
  return 0;
}

_Bool
higherPriority(char *oldOp, char *newOp)
{
  return getPriority(oldOp) >= getPriority(newOp);
}

// runOperation() runs a single operation
int
runOperation(char *op, Stack dataStack)
{
  int data1;
  int data2;
  int result;
  //temporary pointer for result
  int *res_ptr = (int*) malloc(sizeof(int));

  //pop the top two numbers from the data stack
  //if the stack is empty, return the proper error

  if (Stack_is_empty(dataStack)) {
    error_msg_opMissingArgs(op);
    return -1;
  } 
  int *data1_ptr = Stack_pop(dataStack);
  data1 = *data1_ptr;
  //free the pointer
  free(data1_ptr);
  data1_ptr = NULL;
  
  if (Stack_is_empty(dataStack)) {
    error_msg_opMissingArgs(op);
    return -1;
  }
  int *data2_ptr = Stack_pop(dataStack);
  data2 = *data2_ptr;
  //free the pointer
  free(data2_ptr);
  data2_ptr = NULL;

  //begin matching operation
  // addition
  if (*op == '+') {
    result = data2 + data1;
  } 
  //multiplication
  else if (*op == '*') {
    result = data2 * data1;
  } 
  //division
  else if (*op == '/') {
    if (data1 == 0) {
      free(res_ptr);
      error_msg_divByZero();
      return -1;
    }
    result = data2 / data1;
  } 
  //subtraction
  else if (*op == '-') {
    result = data2 - data1;
  } 
  //if we get here then the operator is invalid
  else  {
    //printf(op);
    error_msg_badOp(op);
    return -1;
  }
  //push result to datastack and return 0
  //first have to deal with result pointer though
  *res_ptr = result;
  Stack_push(dataStack, res_ptr);
  //free the pointer
  // res_ptr = NULL;
  // free(res_ptr);
  return 0;
}