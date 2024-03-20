/*execute.c*/

//
// Student: Ishan Mukherjee
// Course: CS 211
// Term: Winter 2024
//
// Executes nuPython program, given as a Program Graph.
// 
// Solution by Prof. Joe Hummel
// Northwestern University
// CS 211
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
// Private functions:
//

//
// get_element_value
//
// Given a basic element of an expression --- an identifier
// "x" or some kind of literal like 123 --- the value of 
// this identifier or literal is returned via the reference 
// parameter. Returns true if successful, false if not.
//
// Why would it fail? If the identifier does not exist in 
// memory. This is a semantic error, and an error message is 
// output before returning.
//
static bool get_element_value(struct STMT* stmt, struct RAM* memory, struct ELEMENT* element, struct RAM_VALUE* value)
{
  if (element->element_type == ELEMENT_INT_LITERAL) {

    char* literal = element->element_value;

    value->value_type = RAM_TYPE_INT;
    value->types.i = atoi(literal);
  }
  else if (element->element_type == ELEMENT_REAL_LITERAL)
  {
    char* literal = element->element_value;

    value->value_type = RAM_TYPE_REAL;
    value->types.d = atof(literal);
  }
  else if (element->element_type == ELEMENT_TRUE || element->element_type == ELEMENT_FALSE)
  {
    char* literal = element->element_value;
    value->value_type = RAM_TYPE_BOOLEAN;
    if (strcmp(literal, "True") == 0)
    {
      value->types.i = 1;
    }
    else
    {
      assert(strcmp(literal, "False") == 0);
      value->types.i = 0;
    }
    
  }
  else if (element->element_type == ELEMENT_STR_LITERAL)
  {
    char* literal = element->element_value;

    value->value_type = RAM_TYPE_STR;
    value->types.s = literal;
  }
  else {
    //
    // identifier => variable
    //
    // old code: 
    assert(element->element_type == ELEMENT_IDENTIFIER);

    char* var_name = element->element_value;

    struct RAM_VALUE* ram_value = ram_read_cell_by_id(memory, var_name);

    if (ram_value == NULL) {
      printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", var_name, stmt->line);
      return false;
    }

    value->value_type = ram_value->value_type;
    if (value->value_type == RAM_TYPE_INT || value->value_type == RAM_TYPE_BOOLEAN)
    {
      value->types.i = ram_value->types.i;
    }
    else if (value->value_type == RAM_TYPE_REAL)
    {
      value->types.d = ram_value->types.d;
    }
    else if (value->value_type == RAM_TYPE_STR)
    {
      value->types.s = ram_value->types.s;
    }
  }

  return true;
}


//
// get_unary_value
//
// Given a unary expr, returns the value that it represents.
// This could be the result of a literal 123 or the value
// from memory for an identifier such as "x". Unary values
// may have unary operators, such as + or -, applied.
// This value is "returned" via the reference parameter.
// Returns true if successful, false if not.
//
// Why would it fail? If the identifier does not exist in 
// memory. This is a semantic error, and an error message is 
// output before returning.
//
static bool get_unary_value(struct STMT* stmt, struct RAM* memory, struct UNARY_EXPR* unary, struct RAM_VALUE* value)
{
  //
  // we only have simple elements so far (no unary operators):
  //
  assert(unary->expr_type == UNARY_ELEMENT);

  struct ELEMENT* element = unary->element;

  bool success = get_element_value(stmt, memory, element, value);

  return success;
}

//
// operator_plus
//
// Helper function to handle addition between lhs and rhs 
//
static bool operator_plus(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.i = lhs->types.i + rhs.types.i;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->types.d = lhs->types.d + rhs.types.d;
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_REAL;
    double lhs_real_val = lhs->types.i;
    lhs->types.d = lhs_real_val + rhs.types.d;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.d = lhs->types.d + rhs.types.i;
  }
  else if (lhs->value_type == RAM_TYPE_STR && rhs.value_type == RAM_TYPE_STR)
  {      
    char* concat = malloc(sizeof(char) * (strlen(lhs->types.s) + strlen(rhs.types.s) + 1));
    strcpy(concat, lhs->types.s);
    strcat(concat, rhs.types.s);

    lhs->types.s = concat;
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_minus
//
// Helper function to handle subtraction between lhs and rhs 
//
static bool operator_minus(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.i = lhs->types.i - rhs.types.i;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->types.d = lhs->types.d - rhs.types.d;
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_REAL;
    double lhs_real_val = lhs->types.i;
    lhs->types.d = lhs_real_val - rhs.types.d;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.d = lhs->types.d - rhs.types.i;
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_asterisk
//
// Helper function to handle multiplcation between lhs and rhs 
//
static bool operator_asterisk(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.i = lhs->types.i * rhs.types.i;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->types.d = lhs->types.d * rhs.types.d;
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_REAL;
    double lhs_real_val = lhs->types.i;
    lhs->types.d = lhs_real_val * rhs.types.d;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.d = lhs->types.d * rhs.types.i;
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_power
//
// Helper function to handle power operation between lhs and rhs 
//
static bool operator_power(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.i = pow(lhs->types.i, rhs.types.i);
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->types.d = pow(lhs->types.d, rhs.types.d);
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_REAL;
    double lhs_real_val = lhs->types.i;
    lhs->types.d = pow(lhs_real_val, rhs.types.d);
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.d = pow(lhs->types.d, rhs.types.i);
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_mod
//
// Helper function to handle modulus operation between lhs and rhs 
//
static bool operator_mod(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.i = lhs->types.i % rhs.types.i;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->types.d = fmod(lhs->types.d, rhs.types.d);
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_REAL;
    double lhs_real_val = lhs->types.i;
    lhs->types.d = fmod(lhs_real_val, rhs.types.d);
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.d = fmod(lhs->types.d, rhs.types.i);
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_div
//
// Helper function to handle division between lhs and rhs 
//
static bool operator_div(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.i = lhs->types.i / rhs.types.i;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->types.d = lhs->types.d / rhs.types.d;
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_REAL;
    double lhs_real_val = lhs->types.i;
    lhs->types.d = lhs_real_val / rhs.types.d;
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->types.d = lhs->types.d / rhs.types.i;
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_equal
//
// Helper function to equality operator between lhs and rhs 
//
static bool operator_equal(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i == rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d == rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i == rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d == rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_STR && rhs.value_type == RAM_TYPE_STR)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (strcmp(lhs->types.s, rhs.types.s) == 0)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_not_equal
//
// Helper function to inequality operator between lhs and rhs 
//
static bool operator_not_equal(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i != rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d != rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i != rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d != rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_STR && rhs.value_type == RAM_TYPE_STR)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (strcmp(lhs->types.s, rhs.types.s) != 0)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_lt
//
// Helper function to "less than" operator between lhs and rhs 
//
static bool operator_lt(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i < rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d < rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i < rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d < rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_STR && rhs.value_type == RAM_TYPE_STR)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (strcmp(lhs->types.s, rhs.types.s) < 0)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_lte
//
// Helper function to "less than or equal to" operator between lhs and rhs 
//
static bool operator_lte(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i <= rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d <= rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i <= rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d <= rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_STR && rhs.value_type == RAM_TYPE_STR)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (strcmp(lhs->types.s, rhs.types.s) <= 0)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_gt
//
// Helper function to "greater than" operator between lhs and rhs 
//
static bool operator_gt(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i > rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d > rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i > rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d > rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_STR && rhs.value_type == RAM_TYPE_STR)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (strcmp(lhs->types.s, rhs.types.s) > 0)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else
  {
    return false;
  }
  return true;
}

//
// operator_gte
//
// Helper function to "greater than or equal to" operator between lhs and rhs 
//
static bool operator_gte(struct RAM_VALUE* lhs, struct RAM_VALUE rhs)
{
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i >= rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d >= rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_REAL)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.i >= rhs.types.d)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_REAL && rhs.value_type == RAM_TYPE_INT)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (lhs->types.d >= rhs.types.i)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else if (lhs->value_type == RAM_TYPE_STR && rhs.value_type == RAM_TYPE_STR)
  {
    lhs->value_type = RAM_TYPE_BOOLEAN;
    if (strcmp(lhs->types.s, rhs.types.s) >= 0)
    {
      lhs->types.i = 1;
    }
    else
    {
      lhs->types.i = 0;
    }
  }
  else
  {
    return false;
  }
  return true;
}

//
// execute_binary_expr
//
// Given two values and an operator, performs the operation
// and updates the value in the lhs (which can be updated
// because a pointer to the value is passed in). Returns
// true if successful and false if not.
//
static bool execute_binary_expr(struct RAM_VALUE* lhs, int operator, struct RAM_VALUE rhs)
{
  assert(operator != OPERATOR_NO_OP);

  //
  // perform the operation:
  //
  switch (operator)
  {
  case OPERATOR_PLUS:
    if (!(operator_plus(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_MINUS:
    if (!(operator_minus(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_ASTERISK:
    if (!(operator_asterisk(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_POWER:
    if (!(operator_power(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_MOD:
    if (!(operator_mod(lhs, rhs)))
    {
      return false;
    }
    
    break;

  case OPERATOR_DIV:
    if (!(operator_div(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_EQUAL:    
    if (!(operator_equal(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_NOT_EQUAL:
    if (!(operator_not_equal(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_LT:
    if (!(operator_lt(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_LTE:
    if (!(operator_lte(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_GT:
    if (!(operator_gt(lhs, rhs)))
    {
      return false;
    }
    break;

  case OPERATOR_GTE:
    if (!(operator_gte(lhs, rhs)))
    {
      return false;
    }
    break;
    
  default:
    //
    // did we miss something?
    //
    printf("**EXECUTION ERROR: unexpected operator (%d) in execute_binary_expr\n", operator);
    return false;
  }

  return true;
}

//
// is_zeros
//
// Checks if a string is all zeros 
//
static bool is_zeros(char* num)
{
  for (int i = 0; i < strlen(num); i++)
  {
    if (num[i] != '0' && num[i] != '.')
    {
      return false;
    }
  }
  return true;
}

//
// compute_value
// 
// Stores the result of an expression in the value passed by address, returning success
// boolean
//
static bool compute_value(struct STMT* stmt, struct RAM* memory, struct RAM_VALUE* value)
{
  struct VALUE_EXPR* expr = malloc(sizeof(struct VALUE_EXPR));
  if (expr == NULL)
  {
    return false;
  }
  if (stmt->stmt_type == STMT_WHILE_LOOP)
  {
    struct STMT_WHILE_LOOP* while_loop = stmt->types.while_loop;
    expr = while_loop->condition;
  }
  else
  {
    assert(stmt->stmt_type == STMT_ASSIGNMENT);

    struct STMT_ASSIGNMENT* assign = stmt->types.assignment;
    expr = assign->rhs->types.expr;
  }
  //
  // we always have a LHS:
  //
  assert(expr->lhs != NULL);
  
  bool success = get_unary_value(stmt, memory, expr->lhs, value);

  if (!success)  // semantic error? If so, return now:
    return false;

  //
  // do we have a binary expression?
  //
  if (expr->isBinaryExpr)
  {    
    assert(expr->rhs != NULL);  // we must have a RHS
    assert(expr->operator != OPERATOR_NO_OP);  // we must have an operator

    struct RAM_VALUE rhs_value;

    success = get_unary_value(stmt, memory, expr->rhs, &rhs_value);

    if (!success) {  // semantic error? If so, return now:
      return false;
    }

    //
    // perform the operation, updating value:
    //
    bool success = execute_binary_expr(value, expr->operator, rhs_value);

    if (!success) {
      printf("**SEMANTIC ERROR: invalid operand types (line %i)\n", stmt->line);
      return false;
    }
  }

  return true;
}



//
// execute_assignment
//
// Executes an assignment statement, returning true if 
// successful and false if not (an error message will be
// output before false is returned, so the caller doesn't
// need to output anything).
// 
// Examples: x = 123
//           y = x ** 2
//
static bool execute_assignment(struct STMT* stmt, struct RAM* memory)
{
  struct STMT_ASSIGNMENT* assign = stmt->types.assignment;

  char* var_name = assign->var_name;

  //
  // no pointers yet:
  //
  assert(assign->isPtrDeref == false);
  
  if (assign->rhs->value_type == VALUE_FUNCTION_CALL)
  {
    struct VALUE_FUNCTION_CALL* func_call = assign->rhs->types.function_call;
    if (strcmp(func_call->function_name, "input") == 0)
    {
      if (func_call->parameter != NULL) 
        {
          // assumed that element_value is always a string literal
          char* element_value = func_call->parameter->element_value;

          printf("%s", element_value);
        }

        char line[256];
        fgets(line, sizeof(line), stdin);
        // delete EOL chars from input:
        line[strcspn(line, "\r\n")] = '\0';

        struct RAM_VALUE value;
        value.value_type = RAM_TYPE_STR;
        char* inp_val = malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(inp_val, line);
        value.types.s = inp_val;

        bool success = ram_write_cell_by_id(memory, value, var_name);
        return success;
    }
    else if (strcmp(func_call->function_name, "int") == 0)
    {
      struct RAM_VALUE value;
      value.value_type = RAM_TYPE_INT;
      char* stored_str_value = ram_read_cell_by_id(memory, func_call->parameter->element_value)->types.s;
      int conv_int_value = atoi(stored_str_value);
      if (conv_int_value == 0 && !is_zeros(stored_str_value))
      {
        printf("**SEMANTIC ERROR: invalid string for int() (line %i)\n", stmt->line);
        return false;
      }
      else
      {
        value.types.i = conv_int_value;
        bool success = ram_write_cell_by_id(memory, value, var_name);
        return success;
      }
    }
    else
    {
      assert(strcmp(func_call->function_name, "float") == 0);
      
      struct RAM_VALUE value;
      value.value_type = RAM_TYPE_REAL;
      char* stored_str_value = ram_read_cell_by_id(memory, func_call->parameter->element_value)->types.s;
      double conv_float_value = atof(stored_str_value);
      if (conv_float_value == 0 && !is_zeros(stored_str_value))
      {
        printf("**SEMANTIC ERROR: invalid string for float() (line %i)\n", stmt->line);
        return false;
      }
      else
      {
        value.types.d = conv_float_value;
        bool success = ram_write_cell_by_id(memory, value, var_name);
        return success;
      }
    }
  }
  else
  {
    assert(assign->rhs->value_type == VALUE_EXPR);

    struct RAM_VALUE value;

    bool success = compute_value(stmt, memory, &value);
    if (!success)
    {
      return false;
    }

    //
    // write the value to memory:
    //
    success = ram_write_cell_by_id(memory, value, var_name);

    return success;
  }
  
}

//
// execute_print
//
// Helper function for execute_function_call which handles printing
//
static bool execute_print(struct STMT* stmt, struct STMT_FUNCTION_CALL* call, struct RAM* memory)
{
  char* function_name = call->function_name;
  assert(strcmp(function_name, "print") == 0);

  if (call->parameter == NULL) {
    printf("\n");
  }
  else {
    //
    // we have a parameter, which type of parameter?
    // Note that a parameter is a simple element, i.e.
    // identifier or literal (or True, False, None):
    //
    char* element_value = call->parameter->element_value;

    if (call->parameter->element_type == ELEMENT_STR_LITERAL) {
      printf("%s\n", element_value);
    }
    else {
      struct RAM_VALUE value;


      bool success = get_element_value(stmt, memory, call->parameter, &value);

      if (!success)
        return false;

      if (value.value_type == RAM_TYPE_INT)
      {
        printf("%d\n", value.types.i);
      }
      else if (value.value_type == RAM_TYPE_REAL)
      {
        printf("%f\n", value.types.d);
      }
      else if (value.value_type == RAM_TYPE_STR)
      {
        printf("%s\n", value.types.s);
      }
      else if (value.value_type == RAM_TYPE_BOOLEAN)
      {
        if (value.types.i == 0)
        {
          printf("False\n");
        }
        else
        {
          printf("True\n");
        }
      }
    }
  }//else

  return true;
}

//
// execute_function_call
//
// Executes a function call statement, returning true if 
// successful and false if not (an error message will be
// output before false is returned, so the caller doesn't
// need to output anything).
// 
// Examples: print()
//           print(x)
//           print(123)
//
static bool execute_function_call(struct STMT* stmt, struct RAM* memory)
{
  struct STMT_FUNCTION_CALL* call = stmt->types.function_call;

  //
  // for now we are assuming it's a call to print:
  //
  char* function_name = call->function_name;

  if (strcmp(function_name, "print") == 0)
  {
    return execute_print(stmt, call, memory);
  }
  else
  {
    return false;
  }
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
// an error message is output, execution stops,
// and the function returns.
//
void execute(struct STMT* program, struct RAM* memory)
{
  struct STMT* stmt = program;

  //
  // traverse through the program statements:
  //
  while (stmt != NULL) {

    if (stmt->stmt_type == STMT_ASSIGNMENT) {

      bool success = execute_assignment(stmt, memory);

      if (!success)
        return;

      stmt = stmt->types.assignment->next_stmt;  // advance
    }
    else if (stmt->stmt_type == STMT_FUNCTION_CALL) {

      bool success = execute_function_call(stmt, memory);

      if (!success)
        return;

      stmt = stmt->types.function_call->next_stmt;
    }
    else if (stmt->stmt_type == STMT_WHILE_LOOP)
    {
      struct RAM_VALUE value;
      value.value_type = RAM_TYPE_BOOLEAN;
      bool success = compute_value(stmt, memory, &value);
      if (!success)
      {
        return;
      }
      
      if (value.value_type == RAM_TYPE_BOOLEAN && value.types.i == 1)
      {
        stmt = stmt->types.while_loop->loop_body;
      }
      else
      {
        stmt = stmt->types.while_loop->next_stmt;
      }
    }
    else {
      assert(stmt->stmt_type == STMT_PASS);

      //
      // nothing to do!
      //

      stmt = stmt->types.pass->next_stmt;
    }
  }//while
  
  //
  // done:
  //
  return;
}
