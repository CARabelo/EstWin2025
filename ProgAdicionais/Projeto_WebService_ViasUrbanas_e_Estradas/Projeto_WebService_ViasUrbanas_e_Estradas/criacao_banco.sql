drop database if exists db_vias_ue;
create database db_vias_ue;
use db_vias_ue;

create table clientes (
	codigo int not null auto_increment primary key,
    nome varchar(50),
    endereco varchar(200),
    telefone varchar(15),
    cgc varchar(18),
    contato varchar(30),
    cpf varchar(14),
    ramo varchar(200),
    status varchar(30),
    obs varchar(200),
    email varchar(300),
    data_cadastro date
);

create table chaves (
	chave varchar(36) not null unique,
    chave_md5 varchar(36),
    status int not null,
    codigo_cliente int not null,
    codigo_programa int not null,
    obs varchar(200),
    data_criacao date not null,
    data_ult_atualizacao date
);

alter table chaves add foreign key (codigo_cliente) references clientes(codigo);

create table chaves_virgens (
    num_chave int not null auto_increment primary key,
    chave_virgem varchar(50) unique,
    usada boolean
);

create table usuarios_pagseguro (
    VendedorEmail varchar(255),
    TransacaoID varchar(32),
    Referencia varchar(255),
    Extras  decimal(10,2),
    TipoFrete varchar(2),
    ValorFrete decimal(10,2),
    Anotacao varchar(250),
    DataTransacao datetime,
    TipoPagamento varchar(30),
    StatusTransacao varchar(30),
    CliNome varchar(100),
    CliEmail varchar(255),
    CliEndereco varchar(200),
    CliNumero varchar(10),
    CliComplemento varchar(100),
    CliBairro varchar(100),
    CliCidade varchar(100),
    CliEstado varchar(2),
    CliCEP varchar(10),
    CliTelefone varchar(16),
    ProdID_x varchar(100),
    ProdDescricao_x varchar(100),
    ProdValor_x decimal(10,2),
    ProdQuantidade_x int,
    ProdFrete_x decimal(10,2),
    NumItens int
);

#EXEMPLOS
insert into clientes (nome, endereco, telefone, cgc, contato, cpf, ramo, status, email, data_cadastro) values
("Fulano", "rua 1", "2735350789", "27971197000162", "Fulano", "62468071000", "Industria", "0", "fulano@fulano.com.br", now()),
("Cicrano", "rua 2", "27997418372", "16667908000193", "Cicrano", "43100892089", "Industria", "0", "Cicrano@Cicrano.com.br", now());

insert into chaves (chave, status, codigo_cliente, codigo_programa, data_criacao) values 
(uuid(), 0, 1, 1, now()), 
(uuid(), 0, 1, 1, now()), 
(uuid(), 0, 1, 1, now()),
(uuid(), 0, 2, 1, now()),
(uuid(), 0, 2, 1, now());

update chaves set chave_md5 = md5(chave);