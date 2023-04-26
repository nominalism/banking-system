#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 30

typedef struct Pilha {
  char dados[MAX];
  int topo;
} Pilha;

void inicializarPilha(Pilha *p);
int vazia(Pilha *p);
int cheia(Pilha *p);
void empilhar(Pilha *p, int valor);
char desempilhar(Pilha *p);
int lerTopo(Pilha *p);
int verificarOperadores(char *operacao);

int main(void) {
  Pilha *p = (Pilha *)malloc(sizeof(Pilha));
  inicializarPilha(p);
  char operacao[MAX] = "()}";

  if (verificarOperadores(operacao) == 0)printf("incorreto");
  else printf("correto");
  return 0;
}


void inicializarPilha(Pilha *p) { 
  p->topo = -1; 
}

int vazia(Pilha *p) { 
  if (p->topo == -1)
    return 0;
  return 1;
}


int cheia(Pilha *p) {
  if (p->topo == MAX - 1)
  return 0;
return 1;
  }
void empilhar(Pilha *p, int valor) {
  p->topo++;
  p->dados[p->topo] = valor;
}

char desempilhar(Pilha *p) {
  if (vazia(p) == 1)
    p->topo--;
  return '$';
}

int lerTopo(Pilha *p) { 
  return p->dados[p->topo]; }

int verificarOperadores(char *operacao){
  Pilha *p = (Pilha *)malloc(sizeof(Pilha));
  inicializarPilha(p);
  int i;
  int res = 1;
  char par;
  for(i = 0; res && operacao[i] != '\0'; i++)
   if (operacao[i] == '[' || operacao[i] == '(' || operacao[i] == '<' || operacao[i] == '{') 
     empilhar(p, operacao[i]);
    else if (vazia(p) == 0)
      res = 0;
    else {
      par = lerTopo(p);
      desempilhar(p);
      if(operacao[i] == ']' && par  != '[')
        res = 0;
      if(operacao[i] == ')' && par  != '(')
        res = 0;
      if(operacao[i] == '>' && par != '<')
        res = 0;
      if(operacao[i] == '}' && par  != '{')
        res = 0;
    }
  if (vazia(p) == 1)
    res = 0;
    return res;
}
