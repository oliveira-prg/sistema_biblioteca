Sistema de Gerenciamento de Biblioteca em C

O Sistema de Gerenciamento de Biblioteca é um projeto desenvolvido em linguagem C com o objetivo de simular o funcionamento básico de uma biblioteca por meio de uma aplicação em terminal. O sistema foi criado para aplicar conceitos fundamentais de programação estruturada, organização de código e manipulação de dados utilizando recursos da linguagem C.

O projeto permite administrar um acervo de livros, realizar cadastro e gerenciamento de usuários e controlar empréstimos e devoluções de maneira simples e organizada. A aplicação utiliza estruturas (struct) para representar entidades do sistema, enumerações (enum) para controle de categorias e estados, além de funções modularizadas que tornam o código mais organizado, reutilizável e de fácil manutenção.

Entre as funcionalidades implementadas estão o cadastro de livros contendo informações como título, ISBN, ano de publicação, autor e categoria, além de consultas por título e categoria. Também é possível listar livros disponíveis, atualizar dados e remover registros do sistema.

Na parte de gerenciamento de usuários, o sistema permite cadastrar novos usuários, listar registros existentes, realizar buscas por nome, atualizar informações e remover usuários do banco de dados temporário da aplicação.

Além disso, o projeto possui um módulo de empréstimos que controla a disponibilidade dos livros e o relacionamento entre usuários e itens emprestados. O sistema verifica automaticamente regras de negócio como impedir empréstimos de livros indisponíveis ou restringir que um usuário possua mais de um livro emprestado simultaneamente.

Funcionalidades
Cadastro de livros
Listagem de livros cadastrados
Busca de livros por título
Busca de livros por categoria
Atualização de informações dos livros
Remoção de livros
Cadastro de usuários
Listagem de usuários
Busca por nome
Atualização de usuários
Remoção de usuários
Registro de empréstimos
Registro de devoluções
Listagem de empréstimos ativos
Controle automático de disponibilidade
Tecnologias e conceitos utilizados
Linguagem C
Estruturas (struct)
Enumerações (enum)
Vetores
Ponteiros
Manipulação de strings
Modularização por funções
Estruturas condicionais e de repetição
Organização em módulos lógicos
Interface via terminal
Como executar

Compile o projeto utilizando GCC:

Linux :
gcc main.c -o biblioteca
./biblioteca

Windows (MinGW) : 
gcc main.c -o biblioteca.exe
biblioteca.exe

Objetivo do projeto

Este projeto foi desenvolvido com fins acadêmicos e de aprendizado, buscando reforçar conhecimentos relacionados à programação em C, estruturas de dados básicas, organização de sistemas e implementação de regras de negócio em aplicações reais.

Melhorias futuras

Algumas funcionalidades planejadas para versões futuras incluem:

Persistência de dados em arquivos
Integração com banco de dados
Sistema de login
Reserva de livros
Histórico de empréstimos
Aplicação de multas por atraso
Interface gráfica

Desenvolvido por Matheus Barreto
