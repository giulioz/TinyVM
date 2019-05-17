#include <stdio.h>
#include <stdlib.h>

#define ENDLINE '\n'
#define TRUE 1
#define FALSE 0

typedef int32_t numberType;
#define NBYTES 4
#define STACKSIZE (64 * 1024) * NBYTES
#define NREGISTERS 32

#define HALT 0
#define DISPLAY 1
#define PRINT_STACK 2
#define PUSH 10
#define POP 11
#define MOV 12
#define CALL 20
#define RET 21
#define JMP 22
#define JZ 23
#define JPOS 24
#define JNEG 25
#define ADD 30
#define SUB 31
#define MUL 32
#define DIV 33

int readFileLine(FILE *file, numberType *output) {
  char buffer[10] = {0};
  char *bufferPointer = buffer;
  char temp;
  int waiting = TRUE;
  int reading = FALSE;
  while (fread(&temp, sizeof(char), 1, file)) {
    if (temp == ENDLINE && sscanf(buffer, "%d", output) &&
        bufferPointer > buffer) {
      return TRUE;
    } else if (((temp >= '0' && temp <= '9') || temp == '-') &&
               (reading || waiting)) {
      *bufferPointer = temp;
      bufferPointer++;
      waiting = FALSE;
      reading = TRUE;
    } else {
      if (!waiting) {
        reading = FALSE;
      }
    }
  }

  if (sscanf(buffer, "%d", output) && bufferPointer > buffer) {
    return TRUE;
  }

  return FALSE;
}

numberType *readProgram(FILE *file, numberType *outSize) {
  numberType programLineCount = 0;
  if (!readFileLine(file, &programLineCount)) {
    return 0;
  }

  numberType *lines = calloc(NBYTES + 3, sizeof(numberType));
  numberType *linePointer = lines;
  numberType temp;
  while (readFileLine(file, &temp)) {
    *linePointer = temp;
    linePointer++;
  }

  return lines;
}

int main(int argc, const char **argv) {
  FILE *readingFile = fopen("test.cvm", "r");
  numberType programLineCount = 0;
  numberType *program = readProgram(readingFile, &programLineCount);

  numberType stack[STACKSIZE] = {0};
  numberType registers[NREGISTERS] = {0};
  unsigned int stackPointer = 0;
  unsigned int programCounter = 0;
  int running = TRUE;

  while (running) {
    numberType opcode = program[programCounter];
    numberType p1 = program[programCounter + 1];
    numberType p2 = program[programCounter + 2];

    switch (opcode) {
      case HALT:
        running = FALSE;
        break;

      case DISPLAY:
        printf("PRINT: %d\n", registers[p1]);
        programCounter += 2;
        break;

      case PRINT_STACK:
        printf("STACK:\n");
        for (int i = 0; i < p1; i++) {
          printf("  [%d]: %d\n", i, stack[stackPointer - i]);
        }
        programCounter += 2;
        break;

      case PUSH:
        stack[stackPointer] = registers[p1];
        stackPointer++;
        programCounter += 2;
        break;

      case POP:
        stackPointer--;
        registers[p1] = stack[stackPointer];
        programCounter += 2;
        break;

      case MOV:
        registers[p1] = p2;
        programCounter += 3;
        break;

      case CALL:
        stack[stackPointer] = programCounter + 2;
        stackPointer++;
        programCounter = p1;
        break;

      case RET:
        stackPointer--;
        programCounter = stack[stackPointer];
        break;

      case JMP:
        programCounter = p1;
        break;

      case JZ:
        stackPointer--;
        p2 = stack[stackPointer];
        if (p2 == 0) {
          programCounter = p1;
        } else {
          programCounter += 2;
        }
        break;

      case JPOS:
        stackPointer--;
        p2 = stack[stackPointer];
        if (p2 > 0) {
          programCounter = p1;
        } else {
          programCounter += 2;
        }
        break;

      case JNEG:
        stackPointer--;
        p2 = stack[stackPointer];
        if (p2 < 0) {
          programCounter = p1;
        } else {
          programCounter += 2;
        }
        break;

      case ADD:
        stack[stackPointer] = registers[p1] + registers[p2];
        stackPointer++;
        programCounter += 3;
        break;

      case SUB:
        stack[stackPointer] = registers[p1] - registers[p2];
        stackPointer++;
        programCounter += 3;
        break;

      case MUL:
        stack[stackPointer] = registers[p1] * registers[p2];
        stackPointer++;
        programCounter += 3;
        break;

      case DIV:
        stack[stackPointer] = registers[p1] / registers[p2];
        stackPointer++;
        programCounter += 3;
        break;

      default:
        break;
    }
  }

  return EXIT_SUCCESS;
}
