/*ram.c*/

//
// << DESCRIPTION >>
// Implementation of the RAM functions 
// << YOUR NAME >> Ishan Mukherjee
//
// Template: Prof. Joe Hummel
// Northwestern University
// CS 211
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // true, false
#include <string.h>
#include <assert.h>

#include "ram.h"

//
// Private functions:
//

//
// expand_memory
// 
// Doubles memory capacity
//
static bool expand_memory(struct RAM* memory)
{
  assert(memory != NULL);
  memory->capacity = memory->capacity * 2;
  struct RAM_CELL* temp_cells = (struct RAM_CELL*) malloc(sizeof(struct RAM_CELL) * memory->capacity);

  for (int i = 0, N = memory->capacity; i < N; i++)
  {
    temp_cells[i].identifier = NULL;
    temp_cells[i].value.value_type = RAM_TYPE_NONE;
    temp_cells[i].value.types.i = 0;
  }

  for (int i = 0, N = memory->num_values - 1; i < N; i++)
  {
    // copy identifier from cell to temp_cell
    assert(memory->cells[i].identifier != NULL);
    temp_cells[i].identifier = memory->cells[i].identifier;

    // copy value from cell to temp_cell
    int val_type = memory->cells[i].value.value_type;
    temp_cells[i].value.value_type = val_type;
    if (val_type == RAM_TYPE_BOOLEAN || val_type == RAM_TYPE_INT || val_type == RAM_TYPE_PTR || val_type == RAM_TYPE_NONE)
    {
      temp_cells[i].value.types.i = memory->cells[i].value.types.i;
    }
    else if (val_type == RAM_TYPE_REAL)
    {
      temp_cells[i].value.types.d = memory->cells[i].value.types.d;
    }
    else
    {
      assert(val_type == RAM_TYPE_STR);
      temp_cells[i].value.types.s = memory->cells[i].value.types.s;
    }


  }

  free(memory->cells);
  memory->cells = temp_cells;
  return true;
}

//
// Public functions:
//

//
// ram_init
//
// Returns a pointer to a dynamically-allocated memory
// for storing nuPython variables and their values. All
// memory cells are initialized to the value None.
//
struct RAM* ram_init(void)
{
  struct RAM* memory = (struct RAM*) malloc(sizeof(struct RAM));
  memory->num_values = 0;
  memory->capacity = 4;

  memory->cells = (struct RAM_CELL*) malloc(memory->capacity * sizeof(struct RAM_CELL));
  for (int i = 0, N = memory->capacity; i < N; i++)
  {
    memory->cells[i].identifier = NULL;
    memory->cells[i].value.value_type = RAM_TYPE_NONE;
    memory->cells[i].value.types.i = 0;
  }

  return memory;
}


//
// ram_destroy
//
// Frees the dynamically-allocated memory associated with
// the given memory. After the call returns, you cannot
// use the memory.
//
void ram_destroy(struct RAM* memory)
{
  if (memory == NULL)
  {
    return;
  }
  for (int i = 0, N = memory->num_values; i < N; i++)
  {
    free(memory->cells[i].identifier);
    if (memory->cells[i].value.value_type == RAM_TYPE_STR)
    {
      free(memory->cells[i].value.types.s);
    }
  }
  free(memory->cells);
  free(memory);
}


//
// ram_get_addr
// 
// If the given identifier (e.g. "x") has been written to 
// memory, returns the address of this value --- an integer
// in the range 0..N-1 where N is the number of values currently 
// stored in memory. Returns -1 if no such identifier exists 
// in memory. 
// 
// NOTE: a variable has to be written to memory before you can
// get its address. Once a variable is written to memory, its
// address never changes.
//
int ram_get_addr(struct RAM* memory, char* identifier)
{
  assert(memory != NULL);
  for (int i = 0, N = memory->num_values; i < N; i++)
  {
    if (strcmp(memory->cells[i].identifier, identifier) == 0)
    {

      return i;
    }
  }
  return -1;
}


//
// ram_read_cell_by_addr
//
// Given a memory address (an integer in the range 0..N-1), 
// returns a COPY of the value contained in that memory cell.
// Returns NULL if the address is not valid.
//
// NOTE: this function allocates memory for the value that
// is returned. The caller takes ownership of the copy and 
// must eventually free this memory via ram_free_value().
//
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
struct RAM_VALUE* ram_read_cell_by_addr(struct RAM* memory, int address)
{
  if (memory == NULL || address < 0 || address > memory->num_values - 1)
  {
    return NULL;
  }
  struct RAM_VALUE* value = (struct RAM_VALUE*) malloc(sizeof(struct RAM_VALUE));
  int value_type = memory->cells[address].value.value_type;

  value->value_type = value_type;
  if (value_type == RAM_TYPE_INT || value_type == RAM_TYPE_BOOLEAN || value_type == RAM_TYPE_PTR || value_type == RAM_TYPE_NONE)
  {
    value->types.i = memory->cells[address].value.types.i;
  }
  else if (value_type == RAM_TYPE_REAL)
  {
    value->types.d = memory->cells[address].value.types.d;
  }
  else
  {
    assert(value_type == RAM_TYPE_STR);
    char* str_value = (char*) malloc((strlen(memory->cells[address].value.types.s) + 1) * sizeof(char));
    strcpy(str_value, memory->cells[address].value.types.s);
    value->types.s = str_value; 
  }

  return value;
}


// 
// ram_read_cell_by_id
//
// If the given identifier (e.g. "x") has been written to 
// memory, returns a COPY of the value contained in memory.
// Returns NULL if no such identifier exists in memory.
//
// NOTE: this function allocates memory for the value that
// is returned. The caller takes ownership of the copy and 
// must eventually free this memory via ram_free_value().
//
struct RAM_VALUE* ram_read_cell_by_id(struct RAM* memory, char* identifier)
{
  if (memory == NULL)
  {
    return NULL;
  }
  int address = ram_get_addr(memory, identifier);
  if (address == -1)
  {
    return NULL;
  }
  return ram_read_cell_by_addr(memory, address);
}


//
// ram_free_value
//
// Frees the memory value returned by ram_read_cell_by_id and
// ram_read_cell_by_addr.
//
void ram_free_value(struct RAM_VALUE* value)
{
  if (value == NULL)
  {
    return;
  }
  if (value->value_type == RAM_TYPE_STR)
  {
    free(value->types.s);
  }
  free(value); 
  
}


//
// ram_write_cell_by_addr
//
// Writes the given value to the memory cell at the given 
// address. If a value already exists at this address, that
// value is overwritten by this new value. Returns true if 
// the value was successfully written, false if not (which 
// implies the memory address is invalid).
// 
// NOTE: if the value being written is a string, it will
// be duplicated and stored.
// 
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
bool ram_write_cell_by_addr(struct RAM* memory, struct RAM_VALUE value, int address)
{
  if (address < 0 || address > memory->num_values - 1)
  {
    return false;
  }
  if (address > memory->capacity - 1)
  {
    assert(expand_memory(memory));
  }
  int value_type = value.value_type;

  // if a string already exists at the address cell, free it first
  if (memory->cells[address].value.value_type == RAM_TYPE_STR)
  {
    free(memory->cells[address].value.types.s);
  }

  // assign new value to address cell
  if (value_type == RAM_TYPE_INT || value_type == RAM_TYPE_BOOLEAN || value_type == RAM_TYPE_PTR || value_type == RAM_TYPE_NONE)
  {
    memory->cells[address].value.types.i = value.types.i;
  }
  else if (value_type == RAM_TYPE_REAL)
  {
    memory->cells[address].value.types.d = value.types.d;
  }
  else
  {
    assert(value_type == RAM_TYPE_STR);
    char* str_value = (char*) malloc((strlen(value.types.s) + 1) * sizeof(char));  
    strcpy(str_value, value.types.s);
    memory->cells[address].value.types.s = str_value; 
  }
  memory->cells[address].value.value_type = value_type;
  return true;
}


//
// ram_write_cell_by_id
//
// Writes the given value to a memory cell named by the given
// identifier. If a memory cell already exists with this name,
// the existing value is overwritten by the given value. Returns
// true since this operation always succeeds.
// 
// NOTE: if the value being written is a string, it will
// be duplicated and stored.
// 
// NOTE: a variable has to be written to memory before its
// address becomes valid. Once a variable is written to memory,
// its address never changes.
//
bool ram_write_cell_by_id(struct RAM* memory, struct RAM_VALUE value, char* identifier)
{
  int address = ram_get_addr(memory, identifier);
  if (address == -1)
  {
    address = memory->num_values;
    memory->num_values++;
  }
  
  assert(ram_write_cell_by_addr(memory, value, address));
  char* id = (char*) malloc((strlen(identifier) + 1) * sizeof(char));
  strcpy(id, identifier);
  free(memory->cells[address].identifier);
  memory->cells[address].identifier = id;
  
  return true;
  
}


//
// ram_print
//
// Prints the contents of memory to the console.
//
void ram_print(struct RAM* memory)
{
  printf("**MEMORY PRINT**\n");

  printf("Capacity: %d\n", memory->capacity);
  printf("Num values: %d\n", memory->num_values);
  printf("Contents:\n");

  for (int i = 0; i < memory->num_values; i++)
  {
    printf(" %d: %s, ", i, memory->cells[i].identifier);

    if (memory->cells[i].value.value_type == RAM_TYPE_INT)
    {
      printf("int, %d", memory->cells[i].value.types.i);
    }
    else if (memory->cells[i].value.value_type == RAM_TYPE_REAL)
    {
      printf("real, %lf", memory->cells[i].value.types.d);
    }
    else if (memory->cells[i].value.value_type == RAM_TYPE_BOOLEAN)
    {
      if (memory->cells[i].value.types.i == 0)
      {
        printf("boolean, False");
      }
      else
      {
        assert(memory->cells[i].value.types.i == 1);
        printf("boolean, True");
      }
    }
    else if (memory->cells[i].value.value_type == RAM_TYPE_STR)
    {
      printf("str, '%s'", memory->cells[i].value.types.s);
    }
    else
    {
      assert(memory->cells[i].value.value_type == RAM_TYPE_NONE);
      printf("none, None");
    }    

    printf("\n");
  }

  printf("**END PRINT**\n");
}
