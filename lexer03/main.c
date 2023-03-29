#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// possíveis operações matemáticas  
enum Token { Mais, Subtracao, Multiplicacao, Divisao, Potenciacao, Indeterminado,Numero };

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
//verifica se um caractere é um dígito
int eh_digito(char c) {
  return c >= '0' && c <= '9';
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
      //cria-se um if para validar se há um asterisco em seguida, se houver é potenciação, se não é multiplicação
      if (*(sinalizador+1) == '*') {
        adicionar_token(Potenciacao, tokens);
        sinalizador++; // avança para o próximo asterisco
      } else {
        adicionar_token(Multiplicacao, tokens);
      }
    } else if (*sinalizador == '/') {
      adicionar_token(Divisao, tokens);
    } else if (eh_digito(*sinalizador)) {
      int ponto = 0;
      char numero[1024] = "";
      int i = 0;
      while (eh_digito(*sinalizador) || (*sinalizador == '.' && !ponto)) {
        if (*sinalizador == '.') {
          ponto = 1;
        }
        numero[i] = *sinalizador;
        sinalizador++;
        i++;
      }
      adicionar_token(Numero, tokens);
      printf("%s\n", numero);
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
    case Potenciacao:
      printf("- Potenciação\n");
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
  FILE *fp = fopen("input.txt", "r");
  if (fp == NULL) {
    printf("Erro ao abrir arquivo\n");
    return 1;
  }

  // buffer de caracteres usado para armazenar cada linha do arquivo de entrada 
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