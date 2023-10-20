#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int id = 0;

link novoNo(int item, link l, link r) {
  link t = malloc(sizeof(struct node));
  t->item = item;
  t->l = l;
  t->r = r;
  t->N = 1;
  t->id = id++;
  t->altura = 0; // define altura inicial do no
  return t;
}

// z = nó externo
Tree createTree() {
  Tree t = malloc(sizeof(struct tree));
  t->z = malloc(sizeof(struct node));
  t->raiz = t->z;
  t->z->l = t->z;
  t->z->N = 0;
  t->z->r = t->z;
  return t;
}

int calculaAltura(link h) {
  // Se o nó é nulo a altura é 0
  if (h == NULL) {
    return 0;
  } else {
    // Se não retorna a altura dele
    return h->altura;
  }
}

void corrigeAltura(link h) {
  // procura altura do filho esquerdo
  int alturaL = calculaAltura(h->l);

  // procura altura do filho direito
  int alturaR = calculaAltura(h->r);

  // a altura do no é 1 a mais que a altura do seu filho mais alto
  h->altura = 1 + (alturaL > alturaR ? alturaL : alturaR);
}

link rotR(Tree t, link h) {
  link x = h->l;
  h->l = x->r;
  x->r = h;

  // corrige altura de x e h depois da rotação
  corrigeAltura(h);
  corrigeAltura(x);

  // atualiza numero de nós das subarvores
  x->N = h->N;
  h->N = 1 + h->l->N + h->r->N;

  return x;
}

link rotL(Tree t, link h) {
  link x = h->r;
  h->r = x->l;
  x->l = h;

  // corrige altura de x e h depois da rotação
  corrigeAltura(h);
  corrigeAltura(x);

  // atualiza numero de nós das subarvores
  x->N = h->N;
  h->N = 1 + h->l->N + h->r->N;

  return x;
}

link searchR(Tree t, link h, int query) {
  if (h == t->z) {
    return NULL;
  }
  if (h->item == query)
    return h;
  if (h->item >= query)
    return searchR(t, h->l, query);
  return searchR(t, h->r, query);
}

link search(Tree t, int query) { return searchR(t, t->raiz, query); }

link insertR(Tree t, link h, link n) {
  if (h == t->z) {
    return n;
  }

  if (h->item >= n->item)
    h->l = insertR(t, h->l, n);
  else
    h->r = insertR(t, h->r, n);
  (h->N)++;
  return h;
}

void imprimeEmOrdem(Tree t, link h) {
  if (h == t->z)
    return;
  imprimeEmOrdem(t, h->l);
  printf("<chave: %d N: %d>", h->item, h->N);
  imprimeEmOrdem(t, h->r);
}

void imprimePosOrdem(Tree t, link h) {
  if (h == t->z)
    return;
  imprimePosOrdem(t, h->l);
  imprimePosOrdem(t, h->r);
  printf("<chave: %d N: %d>", h->item, h->N);
}
void imprimePreOrdem(Tree t, link h, int k) {
  if (h == t->z)
    return;
  for (int i = 0; i <= k; i++)
    printf(" . ");
  printf("<chave: %d N: %d>\n", h->item, h->N);
  imprimePreOrdem(t, h->l, k + 1);
  imprimePreOrdem(t, h->r, k + 1);
}

void printnode(char *x, int h) {
  int i;
  for (i = 0; i < h; i++)
    printf("\t");
  printf("%s\n", x);
}

int c = 0;
void imprimeR(Tree a, link t) {
  char s[255];
  if (t == a->z) {
    return;
  }
  imprimeR(a, t->r);
  printf("%d [ label = \"<f0>| <f1> %d (%d) | <f2>\" ]\n", t->id, t->item,
         t->N);
  if (t->l != a->z) {
    printf("%d:f0->%d:f1;\n", t->id, t->l->id);
  } else {
    printf("z%d [ shape=point ];\n", c);
    printf("%d:f0->z%d;\n", t->id, c);
    c++;
  }
  if (t->r != a->z) {
    printf("%d:f2->%d:f1;\n", t->id, t->r->id);
  } else {
    printf("z%d [ shape=point ];\n", c);
    printf("%d:f2->z%d;\n", t->id, c);
    c++;
  }
  imprimeR(a, t->l);
}
void imprime(Tree a, char *s) {
  c = 0;
  printf("digraph %s {\n", s);
  printf("node [shape=record,style=rounded, height=.1]\n");
  imprimeR(a, a->raiz);
  printf("}\n");
}
void imprimeFromNode(Tree a, link h, char *s) {
  c = 0;
  printf("digraph  {\n");
  printf("name [label = \"%s\",shape=plaintext]\n", s);
  printf("node [shape=record,style=rounded, height=.1]\n");
  imprimeR(a, h);
  printf("}\n");
}

// link h é o nó (struct node *h)
link AVLinsertR(Tree t, link h, int item) {

  // se encontramos o no externo estamos no ponto de adicionar um novo nó
  // acontece se primeiro no direto
  if (h == t->z) {
    return novoNo(item, t->z, t->z);
  }

  // se o item a inserir é menor que o item do nó atual nos vamos para a
  // esquerda
  if (item < h->item) {
    h->l = AVLinsertR(t, h->l, item);
  }
  // se não, vamos para a arvore direita
  else {
    h->r = AVLinsertR(t, h->r, item);
  }

  // define o numero de nós da subarvore enraizada na arvore h
  // soma direita e esquerda +1 (porque conta ela também)
  h->N = 1 + h->l->N + h->r->N;

  // atualiza a altura do nó h
  corrigeAltura(h);

  // calcula o fator de balanceamento para ver se desbalanceou h
  // Fator de balancemento = altura esquerda - altura direita
  int balance = calculaAltura(h->l) - calculaAltura(h->r);

  // se fator de balanceamento maior que 1 tem que corrigir
  if (balance > 1) {
    // se o item a ser inserido for menor que o item do filho esquerdo, rotação
    // a direita
    if (item < h->l->item) {
      return rotR(t, h);
    }
    // se não nós fazemos uma rotação a esquerda e depois uma rotação a direita
    // (rotação dupla a direita)
    // rotação a esquerda na subarvore da esquerda
    // rotação a direita na raiz original
    else {
      h->l = rotL(t, h->l);
      return rotR(t, h);
    }
  }

  // se fator de balanceamento menor que -1 tem que corrigir
  if (balance < -1) {
    // se o item a ser inserido for maior que o item do filho a direita, rotação
    // a esquerda
    if (item > h->r->item) {
      return rotL(t, h);
    }
    // se não, fazemos uma rotação a direita e depois uma rotação a esquerda
    // (rotação dupla a esquerda)
    // rotação a direita na subarvore da direita
    // rotação a esquerda na raiz original
    else {
      h->r = rotR(t, h->r);
      return rotL(t, h);
    }
  }

  return h;
}

link insert(Tree t, int item) {
  // chama a AVLinsertR para inserir recursivamente
  t->raiz = AVLinsertR(t, t->raiz, item);
  return t->raiz;
}