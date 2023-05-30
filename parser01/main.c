#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Token {
  Mais,
  Menos,
  Multiplicacao,
  Potencia,
  Divisao,
  Indeterminado,
  Inteiro,
  Real,
};

struct TokenList {
  enum Token token;
  union {
    int inteiro;
    float real;
  } value;
  struct TokenList *next;
};

void usar_token(const char *sinalizador, struct TokenList **tokens) {
  *tokens = NULL;
  struct TokenList *add_final = NULL;

  while (*sinalizador != '\0') {
    struct TokenList *node = malloc(sizeof(struct TokenList));
    node->next = NULL;

    if (isdigit(*sinalizador) || (*sinalizador == '.' || *sinalizador == ',')) {
      char *endptr;
      char *decimal_sep = strchr(sinalizador, '.');

      if (decimal_sep == NULL) {
        decimal_sep = strchr(sinalizador, ',');
      }

      if (decimal_sep != NULL) {
        float real = strtof(sinalizador, &endptr);
        node->value.real = real;
        node->token = Real;

      } else {
        int inteiro = strtol(sinalizador, &endptr, 10);
        node->value.inteiro = inteiro;
        node->token = Inteiro;
      }
      sinalizador = endptr - 1;
    } else if (*sinalizador == '+') {
      node->token = Mais;
    } else if (*sinalizador == '-') {
      node->token = Menos;
    } else if (*sinalizador == '*') {
      if (*(sinalizador + 1) == '*') {
        node->token = Potencia;
        sinalizador++;
      } else {
        node->token = Multiplicacao;
      }
    } else if (*sinalizador == '/') {
      node->token = Divisao;
    } else if (*sinalizador != ' ') {
      node->token = Indeterminado;
    } else {
      free(node);
      sinalizador++;
      continue;
    }

    if (add_final == NULL) {
      *tokens = node;
    } else {
      add_final->next = node;
    }

    add_final = node;
    sinalizador++;
  }
}

void imprimir_tokens(struct TokenList *tokens) {
  printf("Tokens: ");

  while (tokens != NULL) {
    switch (tokens->token) {
      case Mais:
        printf("Mais, ");
        break;
      case Menos:
        printf("Menos, ");
        break;
      case Multiplicacao:
        printf("Multiplicar, ");
        break;
      case Potencia:
        printf("Potencia, ");
        break;
      case Divisao:
        printf("Dividir, ");
        break;
      case Indeterminado:
        printf("Indeterminado, ");
        break;
      case Inteiro:
        printf("%d (inteiro), ", tokens->value.inteiro);
        break;
      case Real:
        printf("%f (real), ", tokens->value.real);
        break;
    }

    tokens = tokens->next;
  }

  printf("\n");
}

int main(int argc, char *argv[]) {
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("Erro ao abrir o arquivo.\n");
    return 1;
  }

  char buffer[1024];
  struct TokenList *tokens = NULL;

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    buffer[strcspn(buffer, "\n")] = '\0'; // remove o caractere de nova linha
    usar_token(buffer, &tokens);
    imprimir_tokens(tokens);
  }

  fclose(fp);
  return 0;
}