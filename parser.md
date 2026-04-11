# CheckCadet IRC Server

Este projeto é um servidor IRC desenvolvido em C++ seguindo a norma **RFC 2812**, projetado para alta performance utilizando `epoll` para multiplexação de E/S. O servidor é estruturado de forma que possa ser utilizado como um túnel para execução de comandos remotamente.

## Arquitetura (Engenheiro B)

A implementação do protocolo IRC está dividida em cinco componentes principais, cada um com responsabilidades bem definidas:

1.  **Parser**: Responsável por converter mensagens brutas recebidas via TCP em estruturas manipuláveis (`IRCMessage`). Ele trata prefixos, comandos e parâmetros de acordo com o protocolo IRC.
2.  **UserRegistry**: Gerencia todos os usuários conectados. Mantém o mapeamento entre descritores de ficheiro (FD), nicknames e objetos de usuário, garantindo a unicidade dos nomes.
3.  **ChannelManager**: Responsável pela criação, destruição e gestão de canais. Controla quem são os membros e operadores de cada canal.
4.  **MessageRouter**: Centraliza o envio de mensagens. Ele abstrai a complexidade de enviar mensagens para um usuário específico, para todos os membros de um canal ou em broadcast para todo o servidor.
5.  **CommandDispatcher**: O núcleo de lógica. Recebe as mensagens parsadas e as despacha para os handlers apropriados (NICK, USER, JOIN, PRIVMSG, etc.).

## Funcionamento

O servidor opera em um loop de eventos baseado em **epoll**:
- Quando uma nova conexão chega, o `Server` aceita o cliente e o adiciona ao `UserRegistry`.
- Quando dados são lidos, o `Server` acumula-os num buffer interno por cliente.
- Assim que uma linha completa (terminada em `\r\n`) é recebida:
  - O `Parser` extrai o comando e parâmetros.
  - O `CommandDispatcher` executa a lógica do comando.
  - As respostas são enviadas de volta através do `MessageRouter`.

## Comandos Implementados

Atualmente, os comandos básicos para fluxo de sessão estão implementados:
- `NICK`: Define ou altera o apelido do usuário.
- `USER`: Define informações de usuário (username, realname).
- `JOIN`: Entra em um canal (ex: `JOIN #geral`).
- `PRIVMSG`: Envia mensagens para usuários ou canais.
- `QUIT`: Encerra a conexão.

## Como Testar

### 1. Compilar o Servidor
Execute o comando `make` na raiz do projeto:
```bash
make
```

### 2. Iniciar o Servidor
Execute o binário especificando uma porta:
```bash
./ircserv 6667
```

### 3. Conectar usando Telnet ou Netcat
Em outro terminal:
```bash
nc localhost 6667
```

Sequência de comandos exemplo:
```irc
NICK admin
USER admin 0 * :Administrador
JOIN #test
PRIVMSG #test :Olá a todos!
```

### 4. Conectar usando um Cliente IRC Real
Você também pode usar o `irssi`:
```bash
irssi -c localhost -p 6667 -n admin
```

## Docker
O projeto também inclui suporte a Docker:
- `make build`: Constrói e sobe o container.
- `make logs`: Visualiza os logs do servidor.
- `make stop`: Para os containers.
