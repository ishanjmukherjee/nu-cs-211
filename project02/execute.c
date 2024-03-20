/*execute.c*/

//
// << WHAT IS THE PURPOSE OF THIS FILE??? >>
// Executing nuPython programs
// << WHAT IS YOUR NAME >> Ishan Mukherjee
// << WHAT SCHOOL IS THIS >> Northwestern University
// << WHAT COURSE IS THIS >> CS 211
// << WHAT QUARTER IS THIS >> Winter 2024
// 
// Starter code: Prof. Joe Hummel
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  // true, false
#include <string.h>
#include <assert.h>
#include <math.h>

#include "programgraph.h"
#include "ram.h"
#include "execute.h"

//
// semantic_error
//
// Prints a semantic error message given the name of the identifier and line number
//

static void semantic_error(char* identifier_name, int line)
{
  printf("**SEMANTIC ERROR: name '%s' is not defined (line %i)\n", identifier_name, line);
}


//
// execute_function_call
//
// Handles (only print for now) function calls, given statement and memory pointers
// 

static bool execute_function_call(struct STMT* statement, struct RAM* memory)
{
  if (statement->types.function_call->parameter == NULL)
  {
    printf("\n");
    return true;
  }
  else if (statement->types.function_call->parameter->element_type == ELEMENT_IDENTIFIER)
  {
    if (ram_read_cell_by_id(memory, statement->types.function_call->parameter->element_value) != NULL)
    {
      printf("%d\n", ram_read_cell_by_id(memory, statement->types.function_call->parameter->element_value)->types.i);
      return true;
    }
    else
    {
      semantic_error(statement->types.function_call->parameter->element_value, statement->line);
      return false;
    }
  }
  else if (statement->types.function_call->parameter->element_type == ELEMENT_INT_LITERAL)
  {
    printf("%d\n", atoi(statement->types.function_call->parameter->element_value));
    return true;
  }
  else if (statement->types.function_call->parameter->element_type == ELEMENT_STR_LITERAL)
  {
    printf("%s\n", statement->types.function_call->parameter->element_value);
    return true;
  }
  else
  {
    semantic_error(statement->types.function_call->parameter->element_value, statement->line);
    return false;
  }
}

//
// update_res_val
//
// Helper function for execute_assignment, which updates the result value given lhs and rhs values 
// and operator, and returns false if invalid operation found or true if executed successfully 
//

static bool update_res_val(struct RAM_VALUE* result_val, struct RAM_VALUE lhs_val, struct RAM_VALUE rhs_val, int op)
{
  // print debugging code:
  // printf("lhs: %i rhs: %i\n", lhs_val.types.i, rhs_val.types.i);
  
  // if-else calculates result based on the operator
  if (op == OPERATOR_PLUS)
  {
    result_val->types.i = lhs_val.types.i + rhs_val.types.i;
  }
  else if (op == OPERATOR_MINUS)
  {
    result_val->types.i = lhs_val.types.i - rhs_val.types.i;
  }
  else if (op == OPERATOR_ASTERICK)
  {
    result_val->types.i = lhs_val.types.i * rhs_val.types.i;
  }
  else if (op == OPERATOR_DIV)
  {
    result_val->types.i = lhs_val.types.i / rhs_val.types.i;
  }
  else if (op == OPERATOR_MOD)
  {
    result_val->types.i = lhs_val.types.i % rhs_val.types.i;
  }
  else if (op == OPERATOR_POWER)
  {
    result_val->types.i = (int) pow(lhs_val.types.i, rhs_val.types.i);
  }
  else
  {
    return false;  
  }
  return true;
}

//
// execute_assignment
//
// Handles assignment of variables, given statement and memory pointers
//

static bool execute_assignment(struct STMT* statement, struct RAM* memory)
{
  struct RAM_VALUE result_val;
  result_val.value_type = RAM_TYPE_INT;
  result_val.types.i = 0; // initializing

  // finding lhs of operator
  struct RAM_VALUE lhs_val;
  lhs_val.value_type = RAM_TYPE_INT;
  if (statement->types.assignment->rhs->types.expr->lhs->element->element_type == ELEMENT_IDENTIFIER)
  {
    if (ram_read_cell_by_id(memory, statement->types.assignment->rhs->types.expr->lhs->element->element_value) != NULL)
    {
      lhs_val.types.i = ram_read_cell_by_id(memory, statement->types.assignment->rhs->types.expr->lhs->element->element_value)->types.i;
    }
    else
    {
      semantic_error(statement->types.assignment->rhs->types.expr->lhs->element->element_value, statement->line);
      return false;
    }
  }
  else if (statement->types.assignment->rhs->types.expr->lhs->element->element_type == ELEMENT_INT_LITERAL)
  {
    lhs_val.types.i = atoi(statement->types.assignment->rhs->types.expr->lhs->element->element_value);
  }
  else
  {
    semantic_error(statement->types.assignment->rhs->types.expr->lhs->element->element_value, statement->line);
    return false;
  }

  // finding rhs of operator and result value
  struct RAM_VALUE rhs_val;
  // if a binary expression
  if (statement->types.assignment->rhs->types.expr->isBinaryExpr)
  {
    rhs_val.value_type = RAM_TYPE_INT;
    rhs_val.types.i = 0; // initializing 
    // if rhs of operator is identifier
    if (statement->types.assignment->rhs->types.expr->rhs->element->element_type == ELEMENT_IDENTIFIER)
    {
      if (ram_read_cell_by_id(memory, statement->types.assignment->rhs->types.expr->rhs->element->element_value) != NULL)
      {
        rhs_val.types.i = ram_read_cell_by_id(memory, statement->types.assignment->rhs->types.expr->rhs->element->element_value)->types.i;
      }
      else
      {
        semantic_error(statement->types.assignment->rhs->types.expr->rhs->element->element_value, statement->line);
        return false;
      }
    }
    // if rhs of operator is int literal
    else if (statement->types.assignment->rhs->types.expr->rhs->element->element_type == ELEMENT_INT_LITERAL)
    {
      rhs_val.types.i = atoi(statement->types.assignment->rhs->types.expr->rhs->element->element_value);
    }
    // if rhs of operator is a secret third thing
    else
    {
      return false;
    }

    // finding result val
    //
    // if updating the result val ended in disaster (aka returned false)
    if (!update_res_val(&result_val, lhs_val, rhs_val, statement->types.assignment->rhs->types.expr->operator))
    {
      // abort abort
      return false;
    }
  }
  // if not a binary expression
  else
  {
    result_val = lhs_val;
  }
  
  ram_write_cell_by_id(memory, result_val, statement->types.assignment->var_name);
  
  return true;
}


//
// Public functions:
//

//
// execute
//
// Given a nuPython program graph and a memory, 
// executes the statements in the program graph.
// If a semantic error occurs (e.g. type error),
// and error message is output, execution stops,
// and the function returns.
//
void execute(struct STMT* program, struct RAM* memory)
{
  struct STMT* stmt = program;
  while (stmt != NULL)
  {
    // assignment
    if (stmt->stmt_type == STMT_ASSIGNMENT) 
    {
      int line = stmt->line;
      char* var_name = stmt->types.assignment->var_name;
      if (!execute_assignment(stmt, memory))
      {
        return;
      }
      stmt = stmt->types.assignment->next_stmt; // advance to next statement
    }
    // function call
    else if (stmt->stmt_type == STMT_FUNCTION_CALL) 
    {
      if (!execute_function_call(stmt, memory))
      {
        return;
      }
      stmt = stmt->types.function_call->next_stmt;
    }
    // pass
    else
    {
      assert(stmt->stmt_type == STMT_PASS);
      stmt = stmt->types.pass->next_stmt;
    }
  }
}
