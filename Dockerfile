FROM gcc:latest

WORKDIR /minishell
COPY . .

SHELL ["/bin/bash", "-c"]

ENTRYPOINT cd tests/e2e/ && bash interactive.sh
