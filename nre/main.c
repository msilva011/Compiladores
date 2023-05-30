#include <stdio.h>
#include <stdlib.h>

#define NEANDER_MAGIC_NUMBER 42
#define MEMORY_SIZE 256 // tam max

// mafic =42 , o acumulador,contador(PC) e memória.
typedef struct
{
  unsigned char magicNumber;
  unsigned char accumulator;
  unsigned char pc;
  unsigned char memory[MEMORY_SIZE];
} NeanderState;

// ponteiro neander state
void executeNeander(NeanderState *state)
{
  while (state->pc < MEMORY_SIZE)
  {
    unsigned char instruction = state->memory[state->pc];

    switch (instruction)
    {
    case 0x00: // STA
      state->accumulator = state->memory[state->pc + 1];
      state->pc += 2;
      break;
    case 0x10: // LDA
      state->accumulator = state->memory[state->memory[state->pc + 1]];
      state->pc += 2;
      break;
    case 0x20: // ADD
      state->accumulator += state->memory[state->memory[state->pc + 1]];
      state->pc += 2;
      break;
    case 0x30: // OR
      state->accumulator |= state->memory[state->memory[state->pc + 1]];
      state->pc += 2;
      break;
    case 0x40: // AND
      state->accumulator &= state->memory[state->memory[state->pc + 1]];
      state->pc += 2;
      break;
    case 0x50: // NOT
      state->accumulator = ~state->accumulator;
      state->pc++;
      break;
    case 0x60: // JMP
      state->pc = state->memory[state->pc + 1];
      break;
    case 0x70: // JN
      if ((state->accumulator & 0x80) != 0)
        state->pc = state->memory[state->pc + 1];
      else
        state->pc += 2;
      break;
    case 0x80: // JZ
      if (state->accumulator == 0)
        state->pc = state->memory[state->pc + 1];
      else
        state->pc += 2;
      break;
    case 0xF0: // HLT
      return;
    default:
      printf("Invalid instruction: %02X\n", instruction);
      return;
    }
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Arquivo: %s <filename>\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  FILE *file = fopen(filename, "rb");
  if (file == NULL)
  {
    printf("Erro na abertura do arquivo: %s\n", filename);
    return 1;
  }

  NeanderState state;
  fread(&state, sizeof(NeanderState), 1, file);
  fclose(file);

  if (state.magicNumber != NEANDER_MAGIC_NUMBER)
  {
    printf("Formato de arquivo invalido\n");
    return 1;
  }

  executeNeander(&state);

  printf("Acumulador: %02X\n", state.accumulator);
  printf("PC: %02X\n", state.pc);
  printf("Flag N: %s\n", (state.accumulator & 0x80) ? "true" : "false");
  printf("Flag Z: %s\n", (state.accumulator == 0) ? "true" : "false");

  printf("Mapa de Memória:\n");
  for (int i = 0; i < MEMORY_SIZE; i++)
  {
    printf("%02X ", state.memory[i]);
    if ((i + 1) % 16 == 0)
    {
      printf("\n");
    }
  }
  return 0;
}