/*tests.c*/

//
// << DESCRIPTION >>
// Testing ram.c
// << YOUR NAME >> Ishan Mukherjee
//
// Initial template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"
#include "gtest/gtest.h"

//
// private helper functions:
//

//
// Test case: writing one integer value
//

TEST(memory_module, write_one_int) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);


  //
  // check the memory, does it contain x = 123?
  //
  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[0].value.types.i == 123);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

// //
// // TODO: add many more unit tests
// //

TEST(memory_module, write_one_int_then_invalid_address_read) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  ASSERT_TRUE(memory->capacity == 4);
  ASSERT_TRUE(memory->num_values == 1);

  struct RAM_VALUE* value = ram_read_cell_by_addr(memory, 1);
  ASSERT_TRUE(value == NULL);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, ram_get_addr) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  char* s = (char*) malloc(sizeof(char) * (strlen("x") + 1));
  strcpy(s, "x");
  ASSERT_TRUE(ram_get_addr(memory, s) == -1);
  free(s);

  s = (char*) malloc(sizeof(char) * (strlen("anathem") + 1));
  strcpy(s, "anathem");
  ASSERT_TRUE(ram_get_addr(memory, s) == -1);
  free(s);
  
  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);
  ASSERT_TRUE(memory->num_values == 1);


  //
  // check the memory, does it contain x = 123?
  //
  s = (char*) malloc(sizeof(char) * (strlen("x") + 1));
  strcpy(s, "x");
  ASSERT_TRUE(ram_get_addr(memory, s) == 0);
  free(s);

  s = (char*) malloc(sizeof(char) * (strlen("anathem") + 1));
  strcpy(s, "anathem");
  ASSERT_TRUE(ram_get_addr(memory, s) == -1);
  free(s);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_and_read_empty_memory) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = false;

  success = ram_write_cell_by_addr(memory, i, 0);
  ASSERT_FALSE(success);

  success = ram_write_cell_by_addr(memory, i, 2);
  ASSERT_FALSE(success);

  success = ram_write_cell_by_addr(memory, i, 4);
  ASSERT_FALSE(success);
  
  struct RAM_VALUE* value = ram_read_cell_by_id(memory, "x");
  ASSERT_TRUE(value == NULL);
  ram_free_value(value);

  value = ram_read_cell_by_addr(memory, 0);
  ASSERT_TRUE(value == NULL);
  ram_free_value(value);

  value = ram_read_cell_by_addr(memory, 2);
  ASSERT_TRUE(value == NULL);
  ram_free_value(value);

  value = ram_read_cell_by_addr(memory, 4);
  ASSERT_TRUE(value == NULL);
  ram_free_value(value);

  // //
  // // done test, free memory
  // //
  ram_destroy(memory);
}

TEST(memory_module, write_free_and_read_one_int) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  struct RAM_VALUE* value = ram_read_cell_by_id(memory, "x");
  ASSERT_TRUE(value->value_type == RAM_TYPE_INT);
  ASSERT_TRUE(value->types.i == 123);
  free(value);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_and_read_one_int) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  struct RAM_VALUE* value = ram_read_cell_by_id(memory, "x");
  ASSERT_TRUE(value->value_type == RAM_TYPE_INT);
  ASSERT_TRUE(value->types.i == 123);
  ram_free_value(value);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_and_read_one_ptr) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_PTR;
  i.types.i = 12345;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_PTR);
  ASSERT_TRUE(memory->cells[0].value.types.i == 12345);

  struct RAM_VALUE* value = ram_read_cell_by_id(memory, "x");
  ASSERT_TRUE(value->value_type == RAM_TYPE_PTR);
  ASSERT_TRUE(value->types.i == 12345);
  ram_free_value(value);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_one_str) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_STR;
  i.types.s = "kindergarten";

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);


  //
  // check the memory, does it contain x = 123?
  //
  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(memory->cells[0].value.types.s, "kindergarten") == 0);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);


  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_and_read_one_str) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_STR;
  i.types.s = "kindergarten";

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  struct RAM_VALUE* value = ram_read_cell_by_id(memory, "x");
  ASSERT_TRUE(value->value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(value->types.s, "kindergarten") == 0);
  ram_free_value(value);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_one_str_and_overwrite) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_STR;
  i.types.s = "kindergarten";

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  i.types.s = "school";
  success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  //
  // check the memory, does it contain x = 123?
  //
  ASSERT_TRUE(memory->num_values == 1);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(memory->cells[0].value.types.s, "school") == 0);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);


  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_two_int_and_overwrite) 
{
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE i;

  i.value_type = RAM_TYPE_INT;
  i.types.i = 123;

  bool success = ram_write_cell_by_id(memory, i, "x");
  ASSERT_TRUE(success);

  i.value_type = RAM_TYPE_INT;
  i.types.i = 456;

  success = ram_write_cell_by_id(memory, i, "y");
  ASSERT_TRUE(success);

  i.value_type = RAM_TYPE_INT;
  i.types.i = 789;

  success = ram_write_cell_by_id(memory, i, "y");
  ASSERT_TRUE(success);


  //
  // check the memory, does it contain our ints?
  //
  ASSERT_TRUE(memory->num_values == 2);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[0].value.types.i == 123);
  ASSERT_TRUE(strcmp(memory->cells[0].identifier, "x") == 0);
  ASSERT_TRUE(memory->cells[1].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[1].value.types.i == 789);
  ASSERT_TRUE(strcmp(memory->cells[1].identifier, "y") == 0);


  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_four_int) 
{
  int num_vars = 4;
  int var_vals[] = {123, 456, 789, 10000};
  const char* var_names[] = {"x", "integer", "int", " "};
  int var_types[] = {RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT};
  
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE var;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    var.types.i = var_vals[i];

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(var_name);
  }


  //
  // check the memory, does it contain our ints?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(memory->cells[i].value.types.i == var_vals[i]);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }
  


  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_four_int_then_expand) 
{
  int num_vars = 4;
  int var_vals[] = {123, 456, 789, 10000};
  const char* var_names[] = {"x", "integer", "int", " "};
  int var_types[] = {RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE var;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    var.types.i = var_vals[i];

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(var_name);
  }

  //
  // check the memory, does it contain our ints?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(memory->cells[i].value.types.i == var_vals[i]);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }

  var.value_type = RAM_TYPE_STR;
  var.types.s = (char*) malloc(sizeof(char) * (strlen("competence") + 1));
  strcpy(var.types.s, "competence");
  ram_write_cell_by_id(memory, var, "cool");

  ASSERT_TRUE(memory->capacity == 8);
  ASSERT_TRUE(memory->num_values == 5);

  struct RAM_VALUE* value = ram_read_cell_by_addr(memory, 3);
  ASSERT_TRUE(value->value_type == RAM_TYPE_INT);
  ASSERT_TRUE(value->types.i == var_vals[3]);
  ram_free_value(value);

  value = ram_read_cell_by_addr(memory, 4);
  ASSERT_TRUE(value->value_type == var.value_type);
  ASSERT_TRUE(strcmp(value->types.s, var.types.s) == 0);
  ram_free_value(value);
  free(var.types.s);

  var.value_type = RAM_TYPE_STR;
  var.types.s = (char*) malloc(sizeof(char) * (strlen("you") + 1));
  strcpy(var.types.s, "you");
  ram_write_cell_by_id(memory, var, "cool");

  ASSERT_TRUE(memory->capacity == 8);
  ASSERT_TRUE(memory->num_values == 5);

  value = ram_read_cell_by_addr(memory, 4);
  ASSERT_TRUE(value->value_type == var.value_type);
  ASSERT_TRUE(strcmp(value->types.s, var.types.s) == 0);
  free(var.types.s);
  ram_free_value(value);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_four_str) 
{
  int num_vars = 4;
  const char* var_vals[] = {"pop tart", "stop start", "flop part", "shake smart"};
  const char* var_names[] = {"x", "integer", "int", " "};
  int var_types[] = {RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store our variables
  //
  struct RAM_VALUE var;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    char* s = (char*) malloc(sizeof(char) * (strlen(var_vals[i]) + 1));
    strcpy(s, var_vals[i]);
    var.types.s = s;

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(s);
    free(var_name);
  }
  //
  // check the memory, does it contain our variables?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].value.types.s, var_vals[i]) == 0);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }



  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_and_read_nine_int) 
{
  int num_vars = 9;
  int var_vals[] = {123, 456, 789, 10000, 1, 2, 3, 4, 5};
  const char* var_names[] = {"x", "integer", "int", " ", "a", "b", "c", "h", "l"};
  int var_types[] = {RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE var;

  struct RAM_VALUE* value = NULL;
  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    var.types.i = var_vals[i];

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(var_name);

    for (int j = 0; j < i; j++)
    {
      char* v_name = (char*) malloc(sizeof(char) * (strlen(var_names[j]) + 1));
      strcpy(v_name, var_names[j]);
      value = ram_read_cell_by_id(memory, v_name);
      free(v_name);
      ASSERT_TRUE(value->value_type == RAM_TYPE_INT);
      ASSERT_TRUE(value->types.i == var_vals[j]);
      ram_free_value(value);
    }
  }


  //
  // check the memory, does it contain our ints?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(memory->cells[i].value.types.i == var_vals[i]);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }

  for (int j = 0; j < num_vars; j++)
  {
    char* v_name = (char*) malloc(sizeof(char) * (strlen(var_names[j]) + 1));
    strcpy(v_name, var_names[j]);
    value = ram_read_cell_by_id(memory, v_name);
    free(v_name);
    ASSERT_TRUE(value->value_type == RAM_TYPE_INT);
    ASSERT_TRUE(value->types.i == var_vals[j]);
    ram_free_value(value);
  }

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_and_read_nine_values_of_different_types_then_overwrite) 
{
  int num_vars = 9;
  int var_vals[] = {123, 456, 789, 10000, 0, 42, 345678, 1, 0};
  const char* var_names[] = {"x", "integer", "int", " ", "a", "b", "c", "h", "l"};
  int var_types[] = {RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_INT, RAM_TYPE_NONE, RAM_TYPE_NONE, RAM_TYPE_INT, RAM_TYPE_PTR, RAM_TYPE_BOOLEAN, RAM_TYPE_BOOLEAN};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store the integer 123:
  //
  struct RAM_VALUE var;

  struct RAM_VALUE* value = NULL;
  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    var.types.i = var_vals[i];

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(var_name);

    for (int j = 0; j < i; j++)
    {
      char* v_name = (char*) malloc(sizeof(char) * (strlen(var_names[j]) + 1));
      strcpy(v_name, var_names[j]);
      value = ram_read_cell_by_id(memory, v_name);
      free(v_name);
      ASSERT_TRUE(value->value_type == var_types[j]);
      ASSERT_TRUE(value->types.i == var_vals[j]);
      ram_free_value(value);
    }
  }


  //
  // check the memory, does it contain our ints?
  //
  ASSERT_TRUE(memory->capacity == 16);
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(memory->cells[i].value.types.i == var_vals[i]);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }

  for (int j = 0; j < num_vars; j++)
  {
    char* v_name = (char*) malloc(sizeof(char) * (strlen(var_names[j]) + 1));
    strcpy(v_name, var_names[j]);
    value = ram_read_cell_by_id(memory, v_name);
    free(v_name);
    ASSERT_TRUE(value->value_type == var_types[j]);
    ASSERT_TRUE(value->types.i == var_vals[j]);
    ram_free_value(value);
  }

  var.value_type = RAM_TYPE_STR;
  var.types.s = (char*) malloc(sizeof(char) * (strlen("inter") + 1));
  strcpy(var.types.s, "inter");
  ASSERT_TRUE(ram_write_cell_by_addr(memory, var, 5));
  value = ram_read_cell_by_addr(memory, 5);
  ASSERT_TRUE(value != NULL);
  ASSERT_TRUE(value->value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(value->types.s, "inter") == 0);
  ram_free_value(value);
  free(var.types.s);

  var.value_type = RAM_TYPE_STR;
  var.types.s = (char*) malloc(sizeof(char) * (strlen("open") + 1));
  strcpy(var.types.s, "open");
  ASSERT_TRUE(ram_write_cell_by_addr(memory, var, 8));
  value = ram_read_cell_by_addr(memory, 8);
  ASSERT_TRUE(value != NULL);
  ASSERT_TRUE(value->value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(value->types.s, "open") == 0);
  ram_free_value(value);
  free(var.types.s);

  var.value_type = RAM_TYPE_STR;
  var.types.s = (char*) malloc(sizeof(char) * (strlen("cauterize") + 1));
  strcpy(var.types.s, "cauterize");
  ASSERT_FALSE(ram_write_cell_by_addr(memory, var, 9));
  free(var.types.s);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_five_str) 
{
  int num_vars = 5;
  const char* var_vals[] = {"pop tart", "stop start", "flop art", "shake smart", "top fart"};
  const char* var_names[] = {"x", "integer", "int", "lop", "lacrosse"};
  int var_types[] = {RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store our variables
  //
  struct RAM_VALUE var;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    char* s = (char*) malloc(sizeof(char) * (strlen(var_vals[i]) + 1));
    strcpy(s, var_vals[i]);
    var.types.s = s;

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(s);
    free(var_name);

    for (int j = 0; j < i; j++)
    {
      ASSERT_TRUE(memory->cells[j].value.value_type == var_types[j]);
      ASSERT_TRUE(strcmp(memory->cells[j].value.types.s, var_vals[j]) == 0);
      char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[j]) + 1));
      strcpy(var_name, var_names[j]);
      ASSERT_TRUE(strcmp(memory->cells[j].identifier, var_name) == 0);
      free(var_name);
    }
  }
  
  //
  // check the memory, does it contain our variables?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].value.types.s, var_vals[i]) == 0);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }



  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_five_str_then_overwrite_with_none) 
{
  int num_vars = 5;
  const char* var_vals[] = {"pop tart", "stop start", "flop art", "shake smart", "top fart"};
  const char* var_names[] = {"x", "integer", "int", "lop", "lacrosse"};
  int var_types[] = {RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store our variables
  //
  struct RAM_VALUE var;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    char* s = (char*) malloc(sizeof(char) * (strlen(var_vals[i]) + 1));
    strcpy(s, var_vals[i]);
    var.types.s = s;

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(s);
    free(var_name);

    for (int j = 0; j < i; j++)
    {
      ASSERT_TRUE(memory->cells[j].value.value_type == var_types[j]);
      ASSERT_TRUE(strcmp(memory->cells[j].value.types.s, var_vals[j]) == 0);
      char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[j]) + 1));
      strcpy(var_name, var_names[j]);
      ASSERT_TRUE(strcmp(memory->cells[j].identifier, var_name) == 0);
      free(var_name);
    }
  }

  //
  // check the memory, does it contain our variables?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].value.types.s, var_vals[i]) == 0);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }

  var.value_type = RAM_TYPE_NONE;
  var.types.i = 0;
  char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[2]) + 1));
  strcpy(var_name, var_names[2]);
  ASSERT_TRUE(ram_write_cell_by_id(memory, var, var_name));
  ASSERT_TRUE(strcmp(memory->cells[2].identifier, var_name) == 0);
  free(var_name);
  ASSERT_TRUE(memory->cells[2].value.value_type == RAM_TYPE_NONE);
  ASSERT_TRUE(memory->cells[2].value.types.i == 0);
  ASSERT_TRUE(memory->num_values == 5);

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_seven_str) 
{
  int num_vars = 7;
  const char* var_vals[] = {"pop tart", "stop start", "flop art", "shake smart", "top fart", "apple", "banana"};
  const char* var_names[] = {"x", "integer", "int", "lop", "lacrosse", "tennis", "football"};
  int var_types[] = {RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store our variables
  //
  struct RAM_VALUE var;

  char* s = NULL;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    s = (char*) malloc(sizeof(char) * (strlen(var_vals[i]) + 1));
    strcpy(s, var_vals[i]);
    var.types.s = s;

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(s);
    free(var_name);

    for (int j = 0; j < i; j++)
    {
      ASSERT_TRUE(memory->cells[j].value.value_type == var_types[j]);
      ASSERT_TRUE(strcmp(memory->cells[j].value.types.s, var_vals[j]) == 0);
      char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[j]) + 1));
      strcpy(var_name, var_names[j]);
      ASSERT_TRUE(strcmp(memory->cells[j].identifier, var_name) == 0);
      free(var_name);
    }
  }

  //
  // check the memory, does it contain our variables?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].value.types.s, var_vals[i]) == 0);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }



  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_seven_str_and_overwrite) 
{
  int num_vars = 7;
  const char* var_vals[] = {"pop tart", "stop start", "flop art", "shake smart", "top fart", "apple", "banana"};
  const char* var_names[] = {"x", "integer", "int", "lop", "lacrosse", "tennis", "football"};
  int var_types[] = {RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  //
  // we want to store our variables
  //
  struct RAM_VALUE var;

  char* s = NULL;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    s = (char*) malloc(sizeof(char) * (strlen(var_vals[i]) + 1));
    strcpy(s, var_vals[i]);
    var.types.s = s;

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(s);
    free(var_name);
  }

  var.value_type = RAM_TYPE_INT;
  var.types.i = 123;
  bool success = ram_write_cell_by_id(memory, var, "integer");
  ASSERT_TRUE(success);
  
  var.value_type = RAM_TYPE_BOOLEAN;
  var.types.i = 1;
  success = ram_write_cell_by_addr(memory, var, 0);
  ASSERT_TRUE(success);

  var.value_type = RAM_TYPE_STR;
  var.types.s = "kind";
  success = ram_write_cell_by_addr(memory, var, 2);
  ASSERT_TRUE(success);

  ASSERT_TRUE(strcmp(memory->cells[0].identifier, var_names[0]) == 0);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_BOOLEAN);
  ASSERT_TRUE(memory->cells[0].value.types.i == 1);

  ASSERT_TRUE(strcmp(memory->cells[1].identifier, var_names[1]) == 0);
  ASSERT_TRUE(memory->cells[1].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[1].value.types.i == 123);

  ASSERT_TRUE(strcmp(memory->cells[2].identifier, var_names[2]) == 0);
  ASSERT_TRUE(memory->cells[2].value.value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(memory->cells[2].value.types.s, "kind") == 0);

  
  //
  // check the memory, does it contain our variables?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 3; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].value.types.s, var_vals[i]) == 0);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_and_read_seven_str_and_overwrite) 
{
  int num_vars = 7;
  const char* var_vals[] = {"pop tart", "stop start", "flop art", "shake smart", "top fart", "apple", "banana"};
  const char* var_names[] = {"x", "integer", "int", "lop", "lacrosse", "tennis", "football"};
  int var_types[] = {RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  //
  // we want to store our variables
  //
  struct RAM_VALUE var;

  char* s = NULL;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    s = (char*) malloc(sizeof(char) * (strlen(var_vals[i]) + 1));
    strcpy(s, var_vals[i]);
    var.types.s = s;

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(s);
    free(var_name);
  }

  var.value_type = RAM_TYPE_INT;
  var.types.i = 123;
  bool success = ram_write_cell_by_id(memory, var, "integer");
  ASSERT_TRUE(success);

  var.value_type = RAM_TYPE_BOOLEAN;
  var.types.i = 1;
  success = ram_write_cell_by_addr(memory, var, 0);
  ASSERT_TRUE(success);

  var.value_type = RAM_TYPE_STR;
  var.types.s = "kind";
  success = ram_write_cell_by_addr(memory, var, 2);
  ASSERT_TRUE(success);

  ASSERT_TRUE(strcmp(memory->cells[0].identifier, var_names[0]) == 0);
  ASSERT_TRUE(memory->cells[0].value.value_type == RAM_TYPE_BOOLEAN);
  ASSERT_TRUE(memory->cells[0].value.types.i == 1);

  ASSERT_TRUE(strcmp(memory->cells[1].identifier, var_names[1]) == 0);
  ASSERT_TRUE(memory->cells[1].value.value_type == RAM_TYPE_INT);
  ASSERT_TRUE(memory->cells[1].value.types.i == 123);

  ASSERT_TRUE(strcmp(memory->cells[2].identifier, var_names[2]) == 0);
  ASSERT_TRUE(memory->cells[2].value.value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(memory->cells[2].value.types.s, "kind") == 0);

  //
  // check the memory, does it contain our variables?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 3; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].value.types.s, var_vals[i]) == 0);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }

  struct RAM_VALUE* value = ram_read_cell_by_id(memory, "x");
  ASSERT_TRUE(value->value_type == RAM_TYPE_BOOLEAN);
  ASSERT_TRUE(value->types.i == 1);
  ram_free_value(value);

  value = ram_read_cell_by_addr(memory, 0);
  ASSERT_TRUE(value->value_type == RAM_TYPE_BOOLEAN);
  ASSERT_TRUE(value->types.i == 1);
  ram_free_value(value);

  value = ram_read_cell_by_id(memory, "integer");
  ASSERT_TRUE(value->value_type == RAM_TYPE_INT);
  ASSERT_TRUE(value->types.i == 123);
  ram_free_value(value);

  value = ram_read_cell_by_addr(memory, 1);
  ASSERT_TRUE(value->value_type == RAM_TYPE_INT);
  ASSERT_TRUE(value->types.i == 123);
  ram_free_value(value);

  value = ram_read_cell_by_id(memory, "int");
  ASSERT_TRUE(value->value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(value->types.s, "kind") == 0);
  ram_free_value(value);

  value = ram_read_cell_by_addr(memory, 2);
  ASSERT_TRUE(value->value_type == RAM_TYPE_STR);
  ASSERT_TRUE(strcmp(value->types.s, "kind") == 0);
  ram_free_value(value);

  for (int i = 3; i < num_vars; i++)
  {
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    value = ram_read_cell_by_id(memory, var_name);
    free(var_name);
    ASSERT_TRUE(value->value_type == RAM_TYPE_STR);
    ASSERT_TRUE(strcmp(value->types.s, var_vals[i]) == 0);
    ram_free_value(value);

    value = ram_read_cell_by_addr(memory, i);
    ASSERT_TRUE(value->value_type == RAM_TYPE_STR);
    ASSERT_TRUE(strcmp(value->types.s, var_vals[i]) == 0);
    ram_free_value(value);
  }

  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_nine_str) 
{
  int num_vars = 9;
  const char* var_vals[] = {"pop tart", "stop start", "flop art", "shake smart", "top fart", "shop cart", "pineapple", "blueberry", "raspberry"};
  const char* var_names[] = {"x", "integer", "int", "lop", "lacrosse", "hockey", "football", "maria", "ram"};
  int var_types[] = {RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR, RAM_TYPE_STR};

  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store our variables
  //
  struct RAM_VALUE var;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    char* s = (char*) malloc(sizeof(char) * (strlen(var_vals[i]) + 1));
    strcpy(s, var_vals[i]);
    var.types.s = s;

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(s);
    free(var_name);

    for (int j = 0; j < i; j++)
    {
      ASSERT_TRUE(memory->cells[j].value.value_type == var_types[j]);
      ASSERT_TRUE(strcmp(memory->cells[j].value.types.s, var_vals[j]) == 0);
      char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[j]) + 1));
      strcpy(var_name, var_names[j]);
      ASSERT_TRUE(strcmp(memory->cells[j].identifier, var_name) == 0);
      free(var_name);
    }
  }
  
  //
  // check the memory, does it contain our variables?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].value.types.s, var_vals[i]) == 0);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }



  //
  // done test, free memory
  //
  ram_destroy(memory);
}

TEST(memory_module, write_thirty_three_str) 
{
  int num_vars = 33;
  const char* var_vals[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "1a", "2a", "3a", "4a", "5a", "6a", "7a", "8a", "9a", "10a", "11a", "12a", "13a", "14a", "15a", "16a", "17"};
  const char* var_names[] = {"1a", "2a", "3a", "4a", "5a", "6a", "7a", "8a", "9a", "10a", "11a", "12a", "13a", "14a", "15a", "16a", "1aa", "2aa", "3aa", "4aa", "5aa", "6aa", "7aa", "8aa", "9aa", "10aa", "11aa", "12aa", "13aa", "14aa", "15aa", "16aa", "17a"};
  int var_types[num_vars];
  for (int i = 0; i < num_vars; i++)
  {
    var_types[i] = RAM_TYPE_STR;
  }
  //
  // create a new memory:
  //
  struct RAM* memory = ram_init();

  ASSERT_TRUE(memory != NULL);
  ASSERT_TRUE(memory->cells != NULL);
  ASSERT_TRUE(memory->num_values == 0);
  ASSERT_TRUE(memory->capacity == 4);

  //
  // we want to store our variables
  //
  struct RAM_VALUE var;

  for (int i = 0; i < num_vars; i++)
  {
    var.value_type = var_types[i];
    char* s = (char*) malloc(sizeof(char) * (strlen(var_vals[i]) + 1));
    strcpy(s, var_vals[i]);
    var.types.s = s;

    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    bool success = ram_write_cell_by_id(memory, var, var_name);
    ASSERT_TRUE(success);
    free(s);
    free(var_name);
  }

  //
  // check the memory, does it contain our variables?
  //
  ASSERT_TRUE(memory->num_values == num_vars);
  for (int i = 0; i < num_vars; i++)
  {
    ASSERT_TRUE(memory->cells[i].value.value_type == var_types[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].value.types.s, var_vals[i]) == 0);
    char* var_name = (char*) malloc(sizeof(char) * (strlen(var_names[i]) + 1));
    strcpy(var_name, var_names[i]);
    ASSERT_TRUE(strcmp(memory->cells[i].identifier, var_name) == 0);
    free(var_name);
  }



  //
  // done test, free memory
  //
  ram_destroy(memory);
}