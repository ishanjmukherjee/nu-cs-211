/*scanner.c*/

//
// << WHAT IS THE PURPOSE OF THIS FILE??? >>
// Scan input to classify as tokens, with suitable token ID, line num and col num
// << WHAT IS YOUR NAME >> Ishan Mukherjee
// << WHAT SCHOOL IS THIS >> Northwestern University
// << WHAT COURSE IS THIS >> CS 211
// << WHAT QUARTER IS THIS >> Winter 2024
// 
// Starter code: Prof. Joe Hummel
//

#include <stdio.h>
#include <stdbool.h>  // true, false
#include <ctype.h>    // isspace, isdigit, isalpha
#include <string.h>   // strcmp
#include <assert.h>   // assert

#include "scanner.h"


//
// collect_identifier
//
// Given the start of an identifier, collects the rest into value
// while advancing the column number.
//
static void collect_identifier(FILE* input, int c, int* colNumber, char* value)
{
  assert(isalpha(c) || c == '_');  // should be start of an identifier

  int i = 0;

  while (isalnum(c) || c == '_')  // letter, digit, or underscore
  {
    value[i] = (char)c;  // store char
    i++;

    (*colNumber)++;  // advance col # past char

    c = fgetc(input);  // get next char
  }

  // at this point we found the end of the identifer, so put
  // that last char back for processing next:
  ungetc(c, input);

  // turn the value into a string:
  value[i] = '\0';  // build C-style string:

  return;
}

//
// collect_real_or_integer_literal
//
// Given the start of a real or integer literal, collects the rest into value
// while advancing the column number.
//

static char collect_real_or_integer_literal(FILE* input, int c, int* colNumber, char* value)
{
  assert(isdigit(c) || c == '.');  // should be start of an identifier
  int num_decimals = 0;
  // return value:
  // '.' if a simple dot
  // 'i' if an int literal
  // 'r' if a real literal
  // default assumption is simple dot
  char is_dot_int_or_real = '.';
  int i = 0;

  while (isdigit(c) || (c == '.' && num_decimals < 1))  // digit or decimal point (and number of decimal points is below 1, since real literals can't have multiple decimal points)
  {
    if (c == '.')
    {
      num_decimals++;
    }
    else
    {
      is_dot_int_or_real = 'i';
    }
    value[i] = (char) c;  // store char
    i++;

    (*colNumber)++;  // advance col # past char

    c = fgetc(input);  // get next char
  }

  // at this point we found the end of the real literal, so put
  // that last char back for processing next:
  ungetc(c, input);

  // turn the value into a string:
  value[i] = '\0';  // build C-style string:

  if (num_decimals > 0 && is_dot_int_or_real != '.')
  {
    is_dot_int_or_real = 'r';
  }

  return is_dot_int_or_real;
}

//
// collect_string_literal
//
// Given the start of a string literal, collects the rest into value
// while advancing the column number.
//

static bool collect_string_literal(char c,FILE* input,int* colNumber, int* lineNumber, char* value)
{
  char start_quote_type = c;
  bool string_terminated = true; 

  c = fgetc(input); // first character of string literal
  (*colNumber)++;

  int i = 0;

  while (c != start_quote_type)  // termination of string literal
  {
    if (c == '\n')
    {
      string_terminated = false;
      (*lineNumber)++;  // next line, restart column:
      (*colNumber) = 1;
      break;
    }

    value[i] = (char) c;  // store char
    i++;

    (*colNumber)++;  // advance col # past char

    c = fgetc(input);  // get next char
  }
  if (string_terminated)
  {
    (*colNumber)++; // accounting for closing quote
  }

  // turn the value into a string:
  value[i] = '\0';  // build C-style string:

  return string_terminated;
}

//
// collect_single_char
//
// Given a single character-long punctuation, collects it into value
// while advancing the column number, and returns the appropriate Token.
//
struct Token collect_single_char(char c, int token_id, struct Token T, int* lineNumber, int* colNumber, char*value)
{
  T.id = token_id;
  T.line = *lineNumber;
  T.col = *colNumber;

  (*colNumber)++;  // advance col # past char

  value[0] = (char)c;
  value[1] = '\0';

  return T;
}

//
// collect_double_char
//
// Given a potential double character-long punctuation, collects it into value
// while advancing the column number, and returns the appropriate Token.
//
struct Token collect_double_char(char c, char second_char, int single_char_token_id, int double_char_token_id, struct Token T, int* lineNumber, int* colNumber, char*value, FILE* input)
{
  //
  // could be single or double char, let's assume single char for now:
  //
  T.id = single_char_token_id;
  T.line = *lineNumber;
  T.col = *colNumber;

  (*colNumber)++;  // advance col # past char

  value[0] = (char) c;
  value[1] = '\0';

  //
  // now let's read the next char and see what we have:
  //
  c = fgetc(input);

  if (c == second_char)  // it's double char
  {
    T.id = double_char_token_id;

    (*colNumber)++;  // advance col # past char

    value[1] = (char) c;
    value[2] = '\0';

    return T;
  }

  //
  // if we get here, then next char did not 
  // form a token, so we need to put the char
  // back to be processed on the next call:
  //
  ungetc(c, input);

  return T;
}


//
// scanner_init
//
// Initializes line number, column number, and value before
// the start of processing the input stream.
//
void scanner_init(int* lineNumber, int* colNumber, char* value)
{
  assert(lineNumber != NULL);
  assert(colNumber != NULL);
  assert(value != NULL);

  *lineNumber = 1;
  *colNumber = 1;
  value[0] = '\0';  // empty string
}


//
// scanner_nextToken
//
// Returns the next token in the given input stream, advancing the line
// number and column number as appropriate. The token's string-based 
// value is returned via the "value" parameter. For example, if the 
// token returned is an integer literal, then the value returned is
// the actual literal in string form, e.g. "456". For an identifer,
// the value is the identifer itself, e.g. "print" or "y". For a 
// string literal such as 'hi class', the value is the contents of the 
// string literal without the quotes.
//
struct Token scanner_nextToken(FILE* input, int* lineNumber, int* colNumber, char* value)
{
  assert(input != NULL);
  assert(lineNumber != NULL);
  assert(colNumber != NULL);
  assert(value != NULL);

  struct Token T;

  //
  // repeatedly input characters one by one until a token is found:
  //
  while (true)
  {
    //
    // Get the next input character:
    //
    int c = fgetc(input);

    //
    // Let's see what we have...
    //

    if (c == EOF || c == '$')  // no more input, return EOS:
    { 
      T.id = nuPy_EOS;
      T.line = *lineNumber;
      T.col = *colNumber;

      value[0] = '$';
      value[1] = '\0';

      return T;
    }
    else if (c == '\n')  // end of line, keep going:
    {
      (*lineNumber)++;  // next line, restart column:
      *colNumber = 1;
      continue;
    }
    else if (isspace(c))  // other form of whitespace, skip:
    {
      (*colNumber)++;  // advance col # past char
      continue;
    }
    else if (c == '(')
    {
      return collect_single_char(c, nuPy_LEFT_PAREN, T, lineNumber, colNumber, value);
    }
    else if (c == ')')
    {
      return collect_single_char(c, nuPy_RIGHT_PAREN, T, lineNumber, colNumber, value);
    }
    // identifier or keyword
    else if (c == '_' || isalpha(c))
    {
      //
      // start of identifier or keyword, let's assume identifier for now:
      //
      T.id = nuPy_IDENTIFIER;
      T.line = *lineNumber;
      T.col = *colNumber;

      collect_identifier(input, c, colNumber, value);

      //
      // TODO: is the identifier a keyword? If so, return that
      // token id instead.
      //
      // array of keywords represented as strings
      char *keywords[] = {"and", "break", "continue", "def", "elif", "else", "False", "for", "if", "in", "is", "None", "not", "or", "pass", "return", "True", "while"};
      // iterate through the keywords array
      for (int i = 0, list_len = sizeof(keywords) / sizeof(keywords[0]); i < list_len; i++)
      {
        if (strcmp(keywords[i], value) == 0)
        {
          // id = (index of "and" keyword in the enum) + (loop iterator)
          T.id = nuPy_KEYW_AND + i;
        }
      }

      return T;
    }
    // * or ** 
    else if (c == '*')
    {
      return collect_double_char(c, '*', nuPy_ASTERISK, nuPy_POWER, T, lineNumber, colNumber, value, input);
    }
    //
    //
    // TODO: all the remaining tokens (punctuation, literals), and
    // also need to handle line comments.
    //
    //
    else if (c == '#')
    {
      while (c != '\n')
      {
        c = fgetc(input);  
      }
      (*lineNumber)++;
      (*colNumber) = 1;
      continue;
    }
    else if (c == '[')
    {
      return collect_single_char(c, nuPy_LEFT_BRACKET, T, lineNumber, colNumber, value);
    }
    else if (c == ']')
    {
      return collect_single_char(c, nuPy_RIGHT_BRACKET, T, lineNumber, colNumber, value);
    }
    else if (c == '{')
    {
      return collect_single_char(c, nuPy_LEFT_BRACE, T, lineNumber, colNumber, value);
    }
    else if (c == '}')
    {
      return collect_single_char(c, nuPy_RIGHT_BRACE, T, lineNumber, colNumber, value);
    }
    else if (c == '+')
    {
      return collect_single_char(c, nuPy_PLUS, T, lineNumber, colNumber, value);
    }
    else if (c == '-')
    {
      return collect_single_char(c, nuPy_MINUS, T, lineNumber, colNumber, value);
    }
    else if (c == '/')
    {
      return collect_single_char(c, nuPy_SLASH, T, lineNumber, colNumber, value);
    }
    else if (c == '%')
    {
      return collect_single_char(c, nuPy_PERCENT, T, lineNumber, colNumber, value);
    }
    else if (c == '&')
    {
      return collect_single_char(c, nuPy_AMPERSAND, T, lineNumber, colNumber, value);
    }
    else if (c == ':')
    {
      return collect_single_char(c, nuPy_COLON, T, lineNumber, colNumber, value);
    }
    // = or ==
    else if (c == '=')
    {
      return collect_double_char(c, '=', nuPy_EQUAL, nuPy_EQUALEQUAL, T, lineNumber, colNumber, value, input);
    }
    // < or <=
    else if (c == '<')
    {
      return collect_double_char(c, '=', nuPy_LT, nuPy_LTE, T, lineNumber, colNumber, value, input);
    }
    // > or >= 
    else if (c == '>')
    {
      return collect_double_char(c, '=', nuPy_GT, nuPy_GTE, T, lineNumber, colNumber, value, input);
    }
    // ! (unknown token) or !=
    else if (c == '!')
    {
      return collect_double_char(c, '=', nuPy_UNKNOWN, nuPy_NOTEQUAL, T, lineNumber, colNumber, value, input);
    }
    // string literals
    else if (c == '"' || c == '\'')
    {
      T.id = nuPy_STR_LITERAL;
      T.line = *lineNumber;
      T.col = *colNumber;

      bool string_terminated = collect_string_literal(c, input, colNumber, lineNumber, value);

      if (!string_terminated)
      {
        printf("**WARNING: string literal @ (%d, %d) not terminated properly\n", T.line, T.col);
      }

      return T;
    }
    // special case of real literals
    else if (c == '.')
    {
      T.id = nuPy_UNKNOWN; // default assumption that "." is unknown token
      T.line = *lineNumber;
      T.col = *colNumber;

      char type_id = collect_real_or_integer_literal(input, c, colNumber, value);
      if (type_id == 'r')
      {
        T.id = nuPy_REAL_LITERAL;
      }

      return T;
    }
    // real or integer literals
    else if (isdigit(c))
    {
      T.id = nuPy_INT_LITERAL; // default assumption
      T.line = *lineNumber;
      T.col = *colNumber;

      char type_id = collect_real_or_integer_literal(input, c, colNumber, value);
      if (type_id == 'r')
      {
        T.id = nuPy_REAL_LITERAL;
      }

      return T;
    }
    // unknown token 
    else
    {
      //
      // if we get here, then char denotes an UNKNOWN token:
      //
      return collect_single_char(c, nuPy_UNKNOWN, T, lineNumber, colNumber, value);
    }

  }//while

  //
  // execution should never get here, return occurs
  // from within loop
  //
}
