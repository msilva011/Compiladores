#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// possíveis operações matemáticas 
enum Token { Mais, Subtracao, Multiplicacao, Divisao, Indeterminado };

//estrutura de dados para armazenar um único nó em uma lista encadeada de tokens
struct TokenList {
  enum Token token;
  struct TokenList *next;
};

//cria um novo nó na lista encadeada de tokens e adiciona o token especificado
void adicionar_token(enum Token token, struct TokenList **tokens) //ponteiro duplo: **tokens
{
  struct TokenList *node = malloc(sizeof(struct TokenList));
  node->token = token;
  node->next = NULL;
  if (*tokens == NULL) {
    *tokens = node;
  } else {
    struct TokenList *temp = *tokens;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = node;
  }
}
//percorre uma string de entrada caractere por caractere e adiciona um novo token à lista de tokens
void usar_token(char *sinalizador, struct TokenList **tokens) {
  *tokens = NULL;
  while (*sinalizador != '\0') {
    if (*sinalizador == '+') {
      adicionar_token(Mais, tokens);
    } else if (*sinalizador == '-') {
      adicionar_token(Subtracao, tokens);
    } else if (*sinalizador == '*') {
      adicionar_token(Multiplicacao, tokens);
    } else if (*sinalizador == '/') {
      adicionar_token(Divisao, tokens);
    } else if (*sinalizador != ' ') {
      adicionar_token(Indeterminado, tokens);
    }
    sinalizador++;
  }
}
//recebe um ponteiro para o primeiro nó de uma lista de tokens e imprime cada token na lista
void imprimir_tokens(struct TokenList *tokens) {
  printf("\nIdentificação dos Tokens:\n\n");
  while (tokens != NULL) {
    switch (tokens->token) {
    case Mais:
      printf("- Soma\n");
      break;
    case Subtracao:
      printf("- Subtração\n");
      break;
    case Multiplicacao:
      printf("- Multiplicacao\n");
      break;
    case Divisao:
      printf("- Divisao\n");
      break;
    case Indeterminado:
      printf("- Indeterminado\n");
      break;
    }
    tokens = tokens->next;
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  FILE *fp = fopen(argv[0], "r");
  if (fp == NULL) {
    printf("Erro ao abrir arquivo\n");
    return 1;
  }

  // buffer de caracteres usado para armazenar cada linha do       arquivo de entrada 
  char buffer[1024];
  
  //ponteiro para o primeiro nó
  struct TokenList *tokens = NULL;
  while (fgets(buffer, 1024, fp) != NULL) { //le cada linha e armazena num buffer enquanto houver linha a ser lida
    buffer[strcspn(buffer, "\n")] = '\0'; // índice do primeiro caractere de nova linha ("\n") na string e o substitui por um caractere nulo ("\0")
    usar_token(buffer, &tokens);
    imprimir_tokens(tokens);
  }

  fclose(fp);
  return 0;
}