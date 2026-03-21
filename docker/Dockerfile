# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Dockerfile                                         :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ateca <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/21 15:37:02 by ateca             #+#    #+#              #
#    Updated: 2026/03/21 15:51:19 by ateca            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# --- FASE 1: Build (O Construtor) ---
# AS builder é um nome arbitrário para esta fase, usado para referenciar depois
FROM ubuntu:22.04 AS builder

# Evitar prompts interativos durante a instalação de pacotes
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    g++ \
    make \
    && rm -rf /var/lib/apt/lists/*

# Criar diretório de trabalho
WORKDIR /app

# Copiamos apenas o necessário para compilar primeiro (optimiza cache)
COPY . .

# Compila o binário
RUN make

# --- FASE 2: Runtime (A Imagem Final Leve) ---
FROM ubuntu:22.04

# Criar um usuário do sistema para não rodar como root
RUN useradd -m ircuser

# Criar diretório de trabalho
WORKDIR /home/ircuser

# Copiar APENAS o binário compilado da fase anterior
COPY --from=builder /app/ircserv .

# Ajustar permissões
RUN chown ircuser:ircuser ircserv && chmod +x ircserv

# Mudar para o usuário seguro
USER ircuser

# IRC Port
EXPOSE 6667

# Usamos ENTRYPOINT para o binário e CMD para os argumentos padrão
# Isso permite que o usuário passe uma porta diferente via 'docker run'
ENTRYPOINT ["./ircserv"]
CMD ["6667"]