#include <stdio.h>
#include <string.h>

// TODO: Definir um padrão para a posição final do cursor do arquivo
// e documentar o comportamento das funções que manipulam o FILE *.

typedef struct Livro Livro;

int lAdd(FILE *fp);
int lList(FILE *fp);
int lUpdate(FILE *fp);
int lRemove(FILE *fp);

Livro lSearch(FILE *fp);
Livro lCreate(int id);
int show(Livro l);
long fileSize(FILE *fp);
                              //l = livro
int getValue();

void cleanStdin(void);

int readLine(char *buffer, int len);
int readInt(char *buffer, int len, int *n);

struct Autor {
  char nome[100];
};

struct Livro{
  int id;
  char titulo[100];
  struct Autor autor[3];
  int a_len; // quantidade de autores
  int ano;
};

int main(void){
  FILE *fp = fopen("livros.dat", "r+b");

  if(fp == NULL){
    fp = fopen("livros.dat", "w+b");
  }
  int op;

  do{
    printf("\nMenu do catalogo de livros\n");
    printf(" \n1 - adicionar livro\n 2 - listar\n 3 - buscar\n 4 - atualizar\n 5 - remover\n 0 - sair\n");

    op = getValue();

    switch(op){
      case 1:
        lAdd(fp);
        break;
      case 2:
        lList(fp);
        break;
      case 3:
        show(lSearch(fp));
        break;
      case 4:
        lUpdate(fp);
        break;
      case 5:
        lRemove(fp);
        break;
      case 0:
        printf("bye\n");
        break;
      default:
        printf("Opcao invalida\n");
        break;
    }
  }while(op != 0);

  fclose(fp);
}

int getValue(){
  int value = getc(stdin);
  int c;

  while((c = getc(stdin)) != '\n' && c != EOF);

  return value - '0';
}

// Deixa o cursor no final do arquivo

long fileSize(FILE *fp){
  if(fseek(fp, 0, SEEK_END) != 0)
    return -1;

  return ftell(fp);
}

int lAdd(FILE *fp){
  int id = 1;

  if(fp == NULL){
    printf("arquivo nao pode ser aberto\n");
    return 1;
  }

  long t = fileSize(fp);

  if(t > 0){
    Livro u_r;
    fseek(fp, -sizeof(Livro), SEEK_END);
    size_t lido = fread(&u_r, sizeof(Livro), 1, fp);

    if(lido < 1) {
      printf("Erro ao ler registro");
      return 1;
    }

    id = u_r.id + 1; // u_r => ultimo registro

  }else if(t == -1){
    printf("erro ao ler tanho do arquivo\n");
    return 1;
  }

  Livro l = lCreate(id);

  fwrite(&l, sizeof(Livro), 1, fp);

  return 0;
}

int lList(FILE *fp){
  Livro registo;
  rewind(fp);

  printf("| Id | Titulo | Autores | Ano |");

  while(fread(&registo, sizeof(Livro), 1, fp) == 1){
    if(registo.id == -1) continue;

    printf("\n| %d | %s | ", registo.id, registo.titulo);
    for(int i = 0; i < registo.a_len; i++){
      printf("%s", registo.autor[i].nome);
      if(i != registo.a_len-1) printf(", ");
    }

    printf("| %d | \n", registo.ano);
  }

  return 0;
}

int lUpdate(FILE *fp){
 Livro livro = lSearch(fp);

 if(livro.id == -1) return 1;

  printf("digite enter (deixa em branco), se nao quiser alter\n");

  Livro new_livro = lCreate(livro.id);

  if(strlen(new_livro.titulo) != 0)
    strcpy(livro.titulo, new_livro.titulo);
  if(new_livro.ano != -1)
    livro.ano = new_livro.ano;
  if(strlen(new_livro.autor[0].nome) > 0){
    livro.a_len = new_livro.a_len;

    for(int i = 0; i < new_livro.a_len; i++){
      strcpy(livro.autor[i].nome, new_livro.autor[i].nome);
    }
  }

  fseek(fp, -sizeof(Livro), SEEK_CUR);

  size_t result = fwrite(&livro, sizeof(Livro), 1, fp);

  if(result != 1) return 1;

  return 0 ;
}

int lRemove(FILE *fp){
  Livro livro = lSearch(fp);
  Livro l_removido = {-1};
  char op[5];

  if(livro.id == -1) return 1;

  printf("eliminar?: Y/n: \n");
  readLine(op, sizeof(op));

  if(strcmp(op, "n") == 0)
      return 1;
  if(strcmp(op, "y") == 0 || strlen(op) == 0){
    fseek(fp, -sizeof(Livro), SEEK_CUR);
    size_t result = fwrite(&l_removido, sizeof(Livro), 1, fp);

    if(result != 1) return 1;
  }

  return 0;
}

Livro lSearch(FILE *fp){
  Livro l;
  Livro erro = {-1};
  char titulo[100];
  int encontado = 0;

  rewind(fp);

  printf("digite o titulo do livro para pesquisar\n");
  readLine(titulo, sizeof(titulo));

  while(fread(&l, sizeof(l), 1, fp) == 1){
    if(l.id == -1) continue;
    if(strcmp(l.titulo, titulo) == 0) {
      encontado = 1;
      break;
    }
  }

  if(encontado == 1) {
    return l;
  }else {
    if(feof(fp)){
      printf("registo nao existe\n");
    }else if(ferror(fp)){
      printf("erro durante a leitura do arquivo\n");
    }
  }

  return erro;
}

int show(Livro l){
  if(l.id == -1){
    printf("Livro nao encontado\n");
  }else{
    printf("| Id | Titulo | Autores | Ano |");

    printf("\n| %d | %s | ", l.id, l.titulo);
    for(int i = 0; i < l.a_len; i++){
      printf("%s", l.autor[i].nome);
      if(i != l.a_len-1) printf(", ");
    }

    printf("| %d |\n", l.ano);
  }

  return 0;
}

Livro lCreate(int id){
  Livro l;
  char entrada_ano[5];
  char entrada_autor[4];
  int n_autor = 1;

  l.id = id;
  l.ano = -1;

  printf("digite o titulo do livro: ");
  readLine(l.titulo, sizeof(l.titulo));

  printf("digite o ano de publicacao: ");
  readInt(entrada_ano, sizeof(entrada_ano), &l.ano);

  printf("digite o numero de autores (ate 3): ");
  readInt(entrada_autor, sizeof(entrada_autor), &n_autor);

  if(n_autor > 3) n_autor = 3;
  if(n_autor < 1) n_autor = 1;

  l.a_len = n_autor;
  for(int i = 0; i < n_autor; i++){
    printf("digite o nome do autor %d: ", i+1);
    readLine(l.autor[i].nome, sizeof(l.autor[i].nome));
  }

  return l;
}

int readLine(char *buffer, int len){
  if(fgets(buffer, len, stdin) != NULL){
    if(strchr(buffer, '\n') == NULL)
      cleanStdin();

    buffer[strcspn(buffer, "\n")] = '\0';
    return 0;
  }
  return 1;
  }

int readInt(char *buffer, int len, int *n){
  readLine(buffer, len);

  if(sscanf(buffer, "%d", n) == 1)
    return 0;

  return 1;
}

void cleanStdin(void){
  int ch;

  while((ch = getchar()) != '\n' && ch != EOF);
}
