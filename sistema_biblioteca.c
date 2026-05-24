#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>          // ← Adicionado para tolower()
#ifdef _WIN32
#include <windows.h>
#endif

#define TAM_TITULO 120
#define TAM_NOME 80
#define TAM_ISBN 20
#define TAM_CPF 15
#define TAM_AUTOR 80
#define MAX_LIVROS 200
#define MAX_USUARIOS 100

#define RESET "\033[0m"
#define VERDE "\033[32m"
#define VERMELHO "\033[31m"
#define AMARELO "\033[33m"
#define AZUL "\033[34m"
#define CIANO "\033[36m"
#define BRANCO "\033[37m"
#define NEGRITO "\033[1m"
#define ROSA "\033[35m"
#define ROXO "\033[35;1m"

//declarações dos enums e structs necessarios
typedef enum{
    FICCAO,
    NAOFICCAO,
    CIENCIA,
    HISTORIA,
    TECNOLOGIA,
    ARTE
} CategoriaLivros;

typedef enum{
    DISPONIVEL,
    EMPRESTADO,
    RESERVADO
} SituacaoLivro;

typedef struct{
    char nome[TAM_AUTOR ];
    char nacionalidade[TAM_NOME];
    int anoNascimento;
} DadosAutor;

typedef struct{
    int id, ativo;
    char tituloLivro[TAM_TITULO];
    char isbn[TAM_ISBN];
    int anoPublicacao;
    CategoriaLivros categoria;
    SituacaoLivro situacao;
    DadosAutor autor;
} Livro;

typedef struct{
    int idUsuario;
    char nomeUsuario[TAM_NOME];
    char cpf[TAM_CPF];
    char email[TAM_NOME];
    int livroEmprestadoId;
    int ativo;
} Usuario;

//PROTÓTIPOS
const char* categoriaParaTexto(CategoriaLivros categoria);
const char* situacaoParaTexto(SituacaoLivro situacao);

//módulo livros
void cadastrarLivro(Livro acervo[], int *total, int *proximoId);
void listarLivros(Livro acervo[], int total);
void listarLivrosDisponiveis(Livro acervo[], int total);
void buscarLivroPorTitulo(Livro acervo[], int total);
void buscarLivroPorCategoria(Livro acervo[], int total);
void atualizarLivro(Livro acervo[], int total);
void removerLivro(Livro acervo[], int *total);
int encontrarLivroPorId(Livro acervo[], int total, int id);

//módulos usuarios
void cadastrarUsuario(Usuario usuarios[], int *total, int *proximoId);
void imprimirUsuario(Usuario u);
void listarUsuarios(Usuario usuarios[], int total);
void buscarUsuarioPorNome(Usuario usuarios[], int total);
void atualizarUsuario(Usuario usuarios[], int total);
void removerUsuario(Usuario usuarios[], int total, Livro acervo[], int totalLivros);
int encontrarUsuarioPorId(Usuario usuarios[], int total, int id);

//módulo emprestimos
void registrarEmprestimo(Livro acervo[], int totalLivros, Usuario usuarios[], int totalUsuarios);
void registrarDevolucao(Livro acervo[], int totalLivros, Usuario usuarios[], int totalUsuarios);
void listarEmprestimosAtivos(Livro acervo[], int totalLivros, Usuario usuarios[], int totalUsuarios);

//menuss
void menuPrincipal(Livro acervo[], int *totalLivros, int *proximoIdLivro, Usuario usuarios[], int *totalUsuarios, int *proximoIdUsuario);
void menuLivros(Livro acervo[], int *total, int *proximoId);
void menuUsuarios(Usuario usuarios[], int *total, int *proximoId, Livro acervo[], int totalLivros);
void menuEmprestimos(Livro acervo[], int totalLivros, Usuario usuarios[], int totalUsuarios);

//funções auxiliares
void limparTela();
void pausar();
void removerQuebraLinha(char texto[]);
void lerTexto(char mensagem[], char destino[], int tamanho);
int lerInteiro(char mensagem[]);

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    printf("\n%sPressione ENTER para continuar...%s", AMARELO, RESET);
    getchar();
}

void removerQuebraLinha(char texto[]) {
    texto[strcspn(texto,"\n")] = '\0';
}

void lerTexto(char mensagem[], char destino[], int tamanho) {
    printf("%s%s%s", CIANO, mensagem, RESET);
    fgets(destino, tamanho, stdin);
    removerQuebraLinha(destino);
}

int lerInteiro(char mensagem[]) {
    int valor;
    printf("%s%s%s", CIANO, mensagem, RESET);
    scanf("%d", &valor);
    getchar();
    return valor;
}

const char* categoriaParaTexto(CategoriaLivros categoria) {
    switch (categoria) {
        case FICCAO:        return "Ficção";
        case NAOFICCAO:     return "Não Ficção";
        case CIENCIA:       return "Ciência";
        case HISTORIA:      return "Historia";
        case TECNOLOGIA:    return "Tecnologia";
        case ARTE:          return "Arte";
        default:            return "Desconhecida";
    }
}

const char* situacaoParaTexto(SituacaoLivro situacao){
    switch (situacao) {
        case DISPONIVEL:  return "Disponivel";
        case EMPRESTADO:  return "Emprestado";
        case RESERVADO:   return "Reservado";
        default:          return "Desconhecida";
    }
}

void imprimirUsuario(Usuario u){
    printf("ID: %d\n", u.idUsuario);
    printf("Nome: %s\n", u.nomeUsuario);
    printf("CPF: %s\n", u.cpf);
    printf("Email: %s\n", u.email);
    if(u.livroEmprestadoId == 0)
        printf("Empréstimo: Nenhum\n");
    else
        printf("Livro emprestado ID: %d\n", u.livroEmprestadoId);
}

// ==================== FUNÇÕES DE USUÁRIO ====================

void cadastrarUsuario(Usuario usuarios[], int *total, int *proximoId){
    if(*total >= MAX_USUARIOS){
        printf("%sNão é possível cadastrar novos usuários!!%s\n", VERMELHO , RESET);
        return;
    }
    Usuario novo;
    novo.idUsuario = *proximoId;
    novo.ativo = 1;
    novo.livroEmprestadoId = 0;

    printf("\n%s==== Cadastro de Usuário ===%s\n", ROXO,RESET);
    lerTexto("Nome: ",novo.nomeUsuario,TAM_NOME);
    lerTexto("CPF: ",novo.cpf,TAM_CPF);
    lerTexto("Email: ",novo.email,TAM_NOME);

    usuarios[*total] = novo;
    (*total)++;
    (*proximoId)++;
    printf("%sUsuário cadastrado com sucesso!!%s\n", VERDE, RESET);
}

void listarUsuarios(Usuario usuarios[], int total){
    int encontrou = 0;
    printf("\n%s==== Lista de Usuários ===%s\n", ROXO, RESET);
    for (int i = 0; i < total; i++){
        if(usuarios[i].ativo == 1){
            imprimirUsuario(usuarios[i]);
            encontrou = 1;
        }
    }
    if(!encontrou)
        printf("%sNenhum usuário cadastrado!%s\n", VERMELHO, RESET);
}

void buscarUsuarioPorNome(Usuario usuarios[], int total){
    char busca[TAM_NOME];
    int encontrou = 0;
    printf("\n%s=== Buscar Usuario ===%s\n", ROXO, RESET);
    lerTexto("Digite parte do nome: ", busca, TAM_NOME);

    for(int i = 0; i < total; i++){
        if(usuarios[i].ativo == 1 && strstr(usuarios[i].nomeUsuario, busca)!= NULL){
            imprimirUsuario(usuarios[i]);
            encontrou = 1;
        }
    }
    if(!encontrou)
        printf("%sNenhum usuário encontrado!%s\n", VERMELHO, RESET);
}

int encontrarUsuarioPorId(Usuario usuarios[], int total, int id){
    for(int i = 0; i < total; i++){
       if(usuarios[i].ativo == 1 && usuarios[i].idUsuario == id){
          return i;
        }
    }
    return -1;
}

void atualizarUsuario(Usuario usuarios[], int total){
    int id, i;
    printf("\n%s=== Atualizar Usuário ===%s\n", ROXO, RESET);
    listarUsuarios(usuarios, total);
    id = lerInteiro("\nDigite o ID do usuário que queira atualizar: ");
    i = encontrarUsuarioPorId(usuarios, total, id);
    if(i == -1){
        printf("%sUsuário não encontrado!%s\n", VERMELHO, RESET);
        return;
    }
    printf("\n%sDigite os novos dados do usuário%s\n", ROSA , RESET);
    lerTexto("Nome: ", usuarios[i].nomeUsuario, TAM_NOME);
    lerTexto("CPF: ", usuarios[i].cpf, TAM_CPF);
    lerTexto("Email: ", usuarios[i].email, TAM_NOME);
    printf("%sUsuário atualizado com sucesso!!%s\n", VERDE, RESET);
}

void removerUsuario(Usuario usuarios[], int total, Livro acervo[], int totalLivros){
    int id, i;
    printf("\n%s=== Remoção de usuário ===%s\n", ROXO, RESET);
    listarUsuarios(usuarios, total);
    id = lerInteiro("\nDigite o ID do usuário que queira remover: ");
    i = encontrarUsuarioPorId(usuarios, total, id);
    if(i == -1){
        printf("%sUsuário não encontrado!%s\n", VERMELHO, RESET);
        return;
    }
    if (usuarios[i].livroEmprestadoId != 0){
        printf("%sUsuário não pode ser removido!%s\n", VERMELHO, RESET);
        return;
    }
    usuarios[i].ativo = 0;
    printf("%sUsuário removido com sucesso!%s\n", VERDE, RESET);
}

// ==================== FUNÇÕES DE LIVRO ====================

int encontrarLivroPorId(Livro acervo[], int total, int id) {
    for(int i = 0; i < total; i++){
        if(acervo[i].ativo == 1 && acervo[i].id == id){
            return i;
        }
    }
    return -1;
}

void cadastrarLivro(Livro acervo[], int *total, int *proximoId) {
    if (*total >= MAX_LIVROS) {
        printf("\n%sLimite máximo de livros atingido!%s\n", VERMELHO, RESET);
        return;
    }
    Livro novo = {0};
    novo.id = (*proximoId)++;
    novo.situacao = DISPONIVEL;
    novo.ativo = 1;                    // ← Corrigido

    printf("\n%s===== Cadastro de Livro =====%s\n", ROXO, RESET);
    lerTexto("Título: ", novo.tituloLivro, TAM_TITULO);
    lerTexto("ISBN: ", novo.isbn, TAM_ISBN);
    novo.anoPublicacao = lerInteiro("Ano de publicação: ");

    printf("\n%s====== Dados do Autor =======%s\n", ROXO, RESET);
    lerTexto("Nome do autor: ", novo.autor.nome, TAM_AUTOR);
    lerTexto("Nacionalidade: ", novo.autor.nacionalidade, TAM_NOME);
    novo.autor.anoNascimento = lerInteiro("Ano de nascimento: ");

    printf("\n1-Ficção 2-Não Ficção 3-Ciência 4-História 5-Tecnologia 6-Arte\n");
    int cat = lerInteiro("Categoria: ");
    novo.categoria = (CategoriaLivros)(cat - 1);

    acervo[*total] = novo;
    (*total)++;
    printf("\n%sLivro cadastrado com sucesso!%s\n", VERDE, RESET);
}

void listarLivros(Livro acervo[], int total) {
    if (total == 0) {
        printf("\n%sNenhum livro cadastrado.%s\n", VERMELHO, RESET);
        return;
    }
    printf("\n%s======= Lista de Livros =======%s\n\n", ROXO, RESET);
    for (int i = 0; i < total; i++) {
        if(acervo[i].ativo == 0) continue;
        printf("%d. %s\n", i+1, acervo[i].tituloLivro);
        printf(" ISBN : %s\n", acervo[i].isbn);
        printf(" Autor : %s\n", acervo[i].autor.nome);
        printf(" Ano : %d\n", acervo[i].anoPublicacao);
        printf(" Categoria : %s\n", categoriaParaTexto(acervo[i].categoria));
        printf(" Situação : %s\n", situacaoParaTexto(acervo[i].situacao));
        printf(" -----------------------------------\n");
    }
}

// ... (outras funções de livro mantidas iguais)

void listarLivrosDisponiveis(Livro acervo[], int total) {
    int cont = 0;
    printf("\n%s======== Livros Disponíveis =======%s\n\n", ROXO, RESET);
    for (int i = 0; i < total; i++) {
        if (acervo[i].ativo == 1 && acervo[i].situacao == DISPONIVEL) {
            cont++;
            printf("%d. %s\n", cont, acervo[i].tituloLivro);
            printf(" Autor : %s\n", acervo[i].autor.nome);
            printf(" ISBN : %s\n", acervo[i].isbn);
            printf(" Ano : %d\n", acervo[i].anoPublicacao);
            printf(" -----------------------------------\n");
        }
    }
    if (cont == 0)
        printf("%sNenhum livro disponível.%s\n", VERMELHO, RESET);
    else
        printf("\n%sTotal disponível: %d livro(s)%s\n", VERDE, cont, RESET);
}

// ✅ CORRIGIDA - Sem strlwr
void buscarLivroPorTitulo(Livro acervo[], int total) {
    if (total == 0) {
        printf("\n%sNenhum livro cadastrado.%s\n", VERMELHO, RESET);
        return;
    }

    char busca[100];
    int encontrados = 0;

    lerTexto("Digite o título (ou parte): ", busca, 100);

    for(int k = 0; busca[k] != '\0'; k++) 
        busca[k] = tolower(busca[k]);

    printf("\n%s=== Resultados por Título ===%s\n\n", ROXO, RESET);

    for (int i = 0; i < total; i++) {
        if (acervo[i].ativo == 0) continue;

        char tituloTemp[TAM_TITULO];
        strcpy(tituloTemp, acervo[i].tituloLivro);
        for(int k = 0; tituloTemp[k] != '\0'; k++) 
            tituloTemp[k] = tolower(tituloTemp[k]);

        if (strstr(tituloTemp, busca) != NULL) {
            printf("%d. %s\n", encontrados + 1, acervo[i].tituloLivro);
            printf(" ISBN : %s\n", acervo[i].isbn);
            printf(" Autor : %s\n", acervo[i].autor.nome);
            printf(" Situação : %s\n", situacaoParaTexto(acervo[i].situacao));
            printf(" -----------------------------------\n");
            encontrados++;
        }
    }

    if (encontrados == 0)
        printf("%sNenhum livro encontrado.%s\n", VERMELHO, RESET);
    else
        printf("%sTotal encontrado: %d%s\n", VERDE, encontrados, RESET);
}

// ✅ CORRIGIDA - Sem strlwr
void buscarLivroPorCategoria(Livro acervo[], int total) {
    if (total == 0) {
        printf("\n%sNenhum livro cadastrado.%s\n", VERMELHO, RESET);
        return;
    }

    char busca[50];
    int encontrados = 0;

    lerTexto("Digite a categoria: ", busca, 50);

    for(int k = 0; busca[k] != '\0'; k++) 
        busca[k] = tolower(busca[k]);

    printf("\n%s=== Resultados por Categoria ===%s\n\n", ROXO, RESET);

    for (int i = 0; i < total; i++) {
        if (acervo[i].ativo == 0) continue;

        char catTemp[30];
        strcpy(catTemp, categoriaParaTexto(acervo[i].categoria));
        for(int k = 0; catTemp[k] != '\0'; k++) 
            catTemp[k] = tolower(catTemp[k]);

        if (strstr(catTemp, busca) != NULL) {
            printf("%d. %s\n", encontrados + 1, acervo[i].tituloLivro);
            printf(" Autor : %s\n", acervo[i].autor.nome);
            printf(" Categoria : %s\n", categoriaParaTexto(acervo[i].categoria));
            printf(" Situação : %s\n", situacaoParaTexto(acervo[i].situacao));
            printf(" -----------------------------------\n");
            encontrados++;
        }
    }

    if (encontrados == 0)
        printf("%sNenhum livro encontrado nessa categoria.%s\n", VERMELHO, RESET);
    else
        printf("%sTotal encontrado: %d%s\n", VERDE, encontrados, RESET);
}

// (As demais funções como atualizarLivro, removerLivro, menus, etc. permanecem iguais às suas)

void atualizarLivro(Livro acervo[], int total) {
    if (total == 0) {
        printf("\n%sNenhum livro cadastrado.%s\n", VERMELHO, RESET);
        return;
    }
    char isbnBuscado[TAM_ISBN];
    int encontrado = -1;
    lerTexto("Digite o ISBN: ", isbnBuscado, TAM_ISBN);
    for (int i = 0; i < total; i++) {
        if (acervo[i].ativo == 1 && strcmp(acervo[i].isbn, isbnBuscado) == 0) {
            encontrado = i;
            break;
        }
    }
    if (encontrado == -1) {
        printf("%sLivro não encontrado!%s\n", VERMELHO, RESET);
        return;
    }
    int op;
    do {
        printf("\n1. Título\n2. Ano\n3. Categoria\n0. Sair\n");
        op = lerInteiro("Opção: ");
        switch(op) {
            case 1: lerTexto("Novo título: ", acervo[encontrado].tituloLivro, TAM_TITULO); break;
            case 2: acervo[encontrado].anoPublicacao = lerInteiro("Novo ano: "); break;
            case 3:
                printf("1-Ficção 2-NãoFicção 3-Ciência 4-História 5-Tecnologia 6-Arte\n");
                int cat = lerInteiro("Nova categoria: ");
                acervo[encontrado].categoria = (CategoriaLivros)(cat-1);
                break;
            case 0: printf("%sAtualização concluída.%s\n", VERDE, RESET); break;
        }
    } while (op != 0);
}

void removerLivro(Livro acervo[], int *total) {
    if (*total == 0) {
        printf("\n%sNenhum livro cadastrado.%s\n", VERMELHO, RESET);
        return;
    }
    char isbnBuscado[TAM_ISBN];
    int encontrado = -1;
    lerTexto("Digite o ISBN do livro a remover: ", isbnBuscado, TAM_ISBN);
    for (int i = 0; i < *total; i++) {
        if (acervo[i].ativo == 1 && strcmp(acervo[i].isbn, isbnBuscado) == 0) {
            encontrado = i;
            break;
        }
    }
    if (encontrado == -1) {
        printf("%sLivro não encontrado!%s\n", VERMELHO, RESET);
        return;
    }
    printf("\nLivro: %s\n", acervo[encontrado].tituloLivro);
    char conf;
    printf("Confirmar remoção (S/N)? ");
    scanf(" %c", &conf);
    getchar();
    if (conf == 'S' || conf == 's') {
        for (int i = encontrado; i < *total - 1; i++) {
            acervo[i] = acervo[i + 1];
        }
        (*total)--;
        printf("%sLivro removido com sucesso!%s\n", VERDE, RESET);
    } else {
        printf("Remoção cancelada.\n");
    }
}

// ==================== MENUS (mantidos iguais) ====================

void menuLivros(Livro acervo[], int *total, int *proximoId) {
    int opcao;
    do {
        printf("\n%s=== Menu Livros ===%s\n", ROXO, RESET);
        printf("%s─────────────────────────────%s\n", CIANO, RESET);
        printf("%s1%s - [+] Cadastrar livro\n", ROXO, RESET);
        printf("%s2%s - [=] Listar livros\n", ROXO, RESET);
        printf("%s3%s - [=] Listar livros disponíveis\n", ROXO, RESET);
        printf("%s4%s - [?] Buscar por título\n", ROXO, RESET);
        printf("%s5%s - [?] Buscar por categoria\n", ROXO, RESET);
        printf("%s6%s - [*] Atualizar livro\n", ROXO, RESET);
        printf("%s7%s - [x] Remover livro\n", ROXO, RESET);
        printf("%s0%s - [<] Voltar\n", ROXO, RESET);
        printf("%s─────────────────────────────%s\n", CIANO, RESET);
        printf("%sEscolha uma opção:%s\n", ROSA, RESET);
        opcao = lerInteiro("Escolha uma Opção: ");
        limparTela();
        switch (opcao) {
            case 1: cadastrarLivro(acervo, total, proximoId); pausar(); break;
            case 2: listarLivros(acervo, *total); pausar(); break;
            case 3: listarLivrosDisponiveis(acervo, *total); pausar(); break;
            case 4: buscarLivroPorTitulo(acervo, *total); pausar(); break;
            case 5: buscarLivroPorCategoria(acervo, *total); pausar(); break;
            case 6: atualizarLivro(acervo, *total); pausar(); break;
            case 7: removerLivro(acervo, total); pausar(); break;
            case 0: break;
            default: printf("%sOpção inválida!%s\n", VERMELHO, RESET); pausar();
        }
    }while (opcao != 0);
}

// (Os outros menus e funções de empréstimo permanecem iguais ao seu código original)

void menuPrincipal(Livro acervo[], int *totalLivros, int *proximoIdLivro, Usuario usuarios[], int *totalUsuarios, int *proximoIdUsuario){
    int opcao;
    do{
        printf("%s\n", ROXO);
        printf(" ____ ___ ____ _ ___ ___ _____ \n");
        printf(" | __ )|_ _| __ )| | |_ _/ _ \\|_ _|\n");
        printf(" | _ \\ | || _ \\| | | | | | | | | \n");
        printf(" | |_) || || |_) | |___| | |_| | | | \n");
        printf(" |____/|___|____/|_____|___\\___/ |_| \n");
        printf("%s\n", RESET);
        printf("%s1%s - Livros\n", ROXO, RESET);
        printf("%s2%s - Usuários\n", ROXO, RESET);
        printf("%s3%s - Empréstimos\n", ROXO, RESET);
        printf("%s0%s - Sair\n", ROXO, RESET);
        opcao = lerInteiro("Escolha uma Opção: ");
        limparTela();
        switch(opcao){
            case 1: menuLivros(acervo, totalLivros, proximoIdLivro); break;
            case 2: menuUsuarios(usuarios, totalUsuarios, proximoIdUsuario, acervo, *totalLivros); break;
            case 3: menuEmprestimos(acervo, *totalLivros, usuarios, *totalUsuarios); break;
            case 0: printf("%sSaindo...%s\n", VERDE, RESET); break;
            default: printf("%sOpção inválida!%s\n", VERMELHO, RESET); pausar();
        }
    }while(opcao != 0);
}

// Funções de Empréstimo (mantidas iguais)
void registrarEmprestimo(Livro acervo[], int totalLivros, Usuario usuarios[], int totalUsuarios){
    int idLivro, idUsuario, i, j;
    printf("\n%s=== Registrar Empréstimo do Livro ===%s\n", ROXO, RESET);
    idLivro = lerInteiro("ID do livro: ");
    idUsuario = lerInteiro("ID do usuário: ");
    i = encontrarLivroPorId(acervo, totalLivros, idLivro);
    j = encontrarUsuarioPorId(usuarios, totalUsuarios, idUsuario);

    if(i == -1){ printf("%sLivro não encontrado!!%s\n", VERMELHO, RESET); return; }
    if(acervo[i].situacao != DISPONIVEL){ printf("%sLivro não está disponível!!%s\n", VERMELHO, RESET); return; }
    if(j == -1){ printf("%sUsuário não encontrado!!%s\n", VERMELHO, RESET); return; }
    if(usuarios[j].livroEmprestadoId != 0){ printf("%sUsuário já possui um livro emprestado!!%s\n", VERMELHO, RESET); return; }

    acervo[i].situacao = EMPRESTADO;
    usuarios[j].livroEmprestadoId = acervo[i].id;
    printf("%sEmpréstimo registrado com sucesso!!%s\n", VERDE, RESET);
}

void registrarDevolucao(Livro acervo[], int totalLivros, Usuario usuarios[], int totalUsuarios) {
    int idUsuario, i, j;
    printf("\n%s=== Registrar Devolução do Livro ===%s\n", ROXO, RESET);
    idUsuario = lerInteiro("ID do usuário: ");
    i = encontrarUsuarioPorId(usuarios, totalUsuarios, idUsuario);
    if (i == -1) { printf("%sUsuário não encontrado!!%s\n", VERMELHO, RESET); return; }
    if(usuarios[i].livroEmprestadoId == 0){ printf("%sUsuário não possui livro emprestado!%s\n", VERMELHO, RESET); return; }

    j = encontrarLivroPorId(acervo, totalLivros, usuarios[i].livroEmprestadoId);
    acervo[j].situacao = DISPONIVEL;
    usuarios[i].livroEmprestadoId = 0;
    printf("%sDevolução registrada com sucesso!%s\n", VERDE, RESET);
}

void listarEmprestimosAtivos(Livro acervo[], int totalLivros, Usuario usuarios[], int totalUsuarios) {
    int encontrou = 0;
    printf("\n%s=== Empréstimos Ativos ===%s\n", ROXO, RESET);
    for(int i = 0; i < totalUsuarios; i++) {
        if(usuarios[i].ativo == 1 && usuarios[i].livroEmprestadoId != 0){
            int j = encontrarLivroPorId(acervo, totalLivros, usuarios[i].livroEmprestadoId);
            printf("Usuário: %s\n", usuarios[i].nomeUsuario);
            if(j != -1) printf("Livro: %s\n", acervo[j].tituloLivro);
            printf("---\n");
            encontrou = 1;
        }
    }
    if(!encontrou)
        printf("%sNenhum empréstimo ativo!%s\n", VERMELHO, RESET);
}

void menuUsuarios(Usuario usuarios[], int *total, int *proximoId, Livro acervo[], int totalLivros){
    int opcao;
    do{
        printf("\n%s=== Menu Usuários ===%s\n", ROXO, RESET);
        printf("%s─────────────────────────────%s\n", CIANO, RESET);
        printf("%s1%s - [+] Cadastrar um usuário\n", ROXO, RESET);
        printf("%s2%s - [=] Listar usuários\n", ROXO, RESET);
        printf("%s3%s - [?] Buscar um usuário\n", ROXO, RESET);
        printf("%s4%s - [*] Atualizar um usuário\n", ROXO, RESET);
        printf("%s5%s - [x] Remover um usuário\n", ROXO, RESET);
        printf("%s0%s - [<] Voltar\n", ROXO, RESET);
        printf("%s─────────────────────────────%s\n", CIANO, RESET);
        printf("%sEscolha uma opção:%s\n", ROSA, RESET);
        opcao = lerInteiro("Opção: ");
        limparTela();
        switch(opcao){
            case 1: cadastrarUsuario(usuarios, total , proximoId); pausar(); break;
            case 2: listarUsuarios(usuarios, *total); pausar(); break;
            case 3: buscarUsuarioPorNome(usuarios, *total); pausar(); break;
            case 4: atualizarUsuario(usuarios, *total); pausar(); break;
            case 5: removerUsuario(usuarios, *total , acervo, totalLivros); pausar(); break;
            case 0: break;
            default: printf("%sOpção inválida!%s\n", VERMELHO, RESET);
        }
    }while(opcao != 0);
}

void menuEmprestimos(Livro acervo[], int totalLivros, Usuario usuarios[], int totalUsuarios) {
    int opcao;
    do{
        printf("\n%s=== Menu de Empréstimos ===%s\n", ROXO, RESET);
        printf("%s1%s - Registrar empréstimo\n", ROXO, RESET);
        printf("%s2%s - Registrar devolução\n", ROXO, RESET);
        printf("%s3%s - Listar empréstimos ativos\n", ROXO, RESET);
        printf("%s0%s - Voltar\n", ROXO, RESET);
        opcao = lerInteiro("Escolha uma Opção: ");
        limparTela();
        switch(opcao){
            case 1: registrarEmprestimo(acervo, totalLivros, usuarios, totalUsuarios); pausar(); break;
            case 2: registrarDevolucao(acervo, totalLivros, usuarios, totalUsuarios); pausar(); break;
            case 3: listarEmprestimosAtivos(acervo, totalLivros, usuarios, totalUsuarios); pausar(); break;
            case 0: printf("%sVoltando...%s\n", VERDE, RESET); break;
            default: printf("%sOpção inválida!%s\n", VERMELHO, RESET); pausar();
        }
    }while(opcao != 0);
}

int main(){
    setlocale(LC_ALL, "pt_BR.UTF-8");
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    Livro acervo[MAX_LIVROS] = {0};
    Usuario usuarios[MAX_USUARIOS] = {0};
    int totalLivros = 0, totalUsuarios = 0;
    int proximoIdLivro = 1, proximoIdUsuario = 1;

    menuPrincipal(acervo, &totalLivros, &proximoIdLivro, usuarios, &totalUsuarios, &proximoIdUsuario);
    return 0;
}
