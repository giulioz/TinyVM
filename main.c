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
  if (!readFileLine(file, outSize)) {
    return 0;
  }

  numberType *lines = calloc(*outSize + 3, sizeof(numberType));
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

  printf("section .bss\n");
  printf("registers: resb 128\n\n");

  printf("section .text\n");
  printf("extern _printVal\n");
  printf("default rel\n");
  printf("global _CVMstart\n");
  printf("_CVMstart:\n");

  int programCounter = 0;
  while (programCounter < programLineCount) {
    numberType opcode = program[programCounter];
    numberType p1 = program[programCounter + 1];
    numberType p2 = program[programCounter + 2];

    printf("l%d:\n", programCounter);

    switch (opcode) {
      case HALT:
        printf("ret\n");
        programCounter += 1;
        break;

      case DISPLAY:
        printf("lea rdx, [registers+%d]\n", p1 * NBYTES);
        printf("mov rdi, [rdx]\n");

        printf("push rsp\n");
        printf("add rsp, 16\n");
        printf("and rsp, -16\n");
        printf("call _printVal\n");
        printf("pop rsp\n");
        programCounter += 2;
        break;

      case PRINT_STACK:
        // printf("STACK:\n");
        // for (int i = 0; i < p1; i++) {
        //   printf("  [%d]: %d\n", i, stack[stackPointer - i]);
        // }
        programCounter += 2;
        break;

      case PUSH:
        printf("lea rdx, [registers+%d]\n", p1 * NBYTES);
        printf("mov rax, dword [rdx]\n");
        printf("push rax\n");
        programCounter += 2;
        break;

      case POP:
        printf("pop rax\n");
        printf("lea rdx, [registers+%d]\n", p1 * NBYTES);
        printf("mov [rdx], rax\n");

        programCounter += 2;
        break;

      case MOV:
        printf("lea rdx, [registers+%d]\n", p1 * NBYTES);
        printf("mov dword [rdx], %d\n", p2);
        programCounter += 3;
        break;

      case CALL:
        printf("call l%d\n", p1);
        programCounter += 2;
        break;

      case RET:
        printf("ret\n");
        programCounter += 1;
        break;

      case JMP:
        printf("jmp l%d\n", p1);
        programCounter += 2;
        break;

      case JZ:
        printf("pop rax\n");
        printf("cmp eax, 0\n");
        printf("je l%d\n", p1);
        programCounter += 2;
        break;

      case JPOS:
        printf("pop rax\n");
        printf("cmp eax, 0\n");
        printf("jg l%d\n", p1);
        programCounter += 2;
        break;

      case JNEG:
        printf("pop rax\n");
        printf("cmp eax, 0\n");
        printf("jl l%d\n", p1);
        programCounter += 2;
        break;

      case ADD:
        printf("lea rdx, [registers+%d]\n", p1 * NBYTES);
        printf("mov eax, dword [rdx]\n");
        printf("lea rdx, [registers+%d]\n", p2 * NBYTES);
        printf("mov ebx, [rdx]\n");
        printf("add eax, ebx\n");
        printf("push rax\n");
        programCounter += 3;
        break;

      case SUB:
        printf("lea rdx, [registers+%d]\n", p1 * NBYTES);
        printf("mov eax, dword [rdx]\n");
        printf("lea rdx, [registers+%d]\n", p2 * NBYTES);
        printf("mov ebx, [rdx]\n");
        printf("sub eax, ebx\n");
        printf("push rax\n");
        programCounter += 3;
        break;

      case MUL:
        printf("lea rdx, [registers+%d]\n", p1 * NBYTES);
        printf("mov eax, dword [rdx]\n");
        printf("lea rdx, [registers+%d]\n", p2 * NBYTES);
        printf("mov ebx, [rdx]\n");
        printf("imul eax, ebx\n");
        printf("push rax\n");
        programCounter += 3;
        break;

      case DIV:
        printf("lea rdx, [registers+%d]\n", p1 * NBYTES);
        printf("mov eax, dword [rdx]\n");
        printf("lea rdx, [registers+%d]\n", p2 * NBYTES);
        printf("mov ebx, [rdx]\n");
        printf("idiv eax, ebx\n");
        printf("push rax\n");
        programCounter += 3;
        break;

      default:
        break;
    }
  }

  return EXIT_SUCCESS;
}
