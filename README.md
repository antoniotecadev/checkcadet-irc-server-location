# CheckCadet IRC Server Location

Um servidor IRC (Internet Relay Chat) robusto desenvolvido em C++, pronto para suportar múltiplas conexões simultâneas com arquitetura escalável.

## Pré-requisitos

### Para rodar com Docker (Recomendado)
- Docker
- Docker Compose

### Para compilar nativamente
- C++ 17 ou superior
- Compilador C++ (g++ ou clang++)

## Como rodar o projeto

### Opção 1: Com Docker (Recomendado)

#### Build e iniciar
```bash
make build
```

#### Iniciar containers já construídos
```bash
make up
```

#### Ver logs em tempo real
```bash
make logs
```

#### Ver status dos containers
```bash
make ps
```

#### Parar o servidor
```bash
make down
```

### Opção 2: Compilação Nativa

#### Compilar
```bash
make all
```

#### Executar o servidor
```bash
./ircserv
```

#### Limpar binários
```bash
make clean      # Remove arquivos .o
make fclean     # Remove binários compilados
make re         # Recompila tudo do zero
```

## Gerenciamento do Servidor

| Comando | Descrição |
|---------|-----------|
| `make build` | Constrói a imagem Docker |
| `make up` | Inicia os containers |
| `make down` | Para os containers |
| `make start` | Inicia containers parados |
| `make stop` | Para containers em execução |
| `make restart` | Reinicia os containers |
| `make logs` | Exibe logs em tempo real |
| `make ps` | Mostra status dos containers |
| `make docker-clean` | Remove imagens e volumes Docker não utilizados |

## Acesso ao Dashboard

Quando o servidor estiver rodando com Docker, você pode acessar o dashboard de logs em:

```
http://localhost:8080
```

Esse dashboard permite visualizar logs de todos os containers em tempo real.

## Configuração de Rede

- **Porta do Servidor IRC**: 6667
- **Rede Docker**: `irc_network`
- **Limite de conexões simultâneas**: 65535 (C10K ready)

## Arquivos principais

```
.
├── Makefile                    # Automatização de build e Docker
├── docker-compose.yml          # Configuração dos containers
├── docker/Dockerfile           # Definição da imagem Docker
├── srcs/
│   ├── main.cpp               # Entrada do programa
│   ├── server/server.cpp      # Lógica do servidor IRC
│   └── network/ClientConnection.cpp  # Gerenciamento de conexões
└── includes/                   # Headers (.hpp)
```

## Troubleshooting

### Porta 6667 já está em uso
```bash
# Para o container em execução
make down

# Ou mude a porta no docker-compose.yml
```

### Ver erros do servidor
```bash
make logs
```

### Resetar tudo
```bash
make down
make docker-clean
make build
```

## Contato / Autores

- António Teca aka ateca (ateca@student.42luanda.com)
- António Sebastião Pedro aka ansebast (ansebast@student.42luanda.com)
