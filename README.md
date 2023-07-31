# Operating Systems I

Meu trabalho de implementação referente a 3º nota de Sistemas Operacionais I, professor Francisco José, do curso de Ciência da Computação - UFMA.

O trabalho consiste em implementar um servidor multithread em C que instância uma thread despachadora que ler uma requisição a cada `TEMPO_REQ` em milisegundos de um arquivo com `<num_req>` requisições aleatórias geradas no início do programa e `NUM_THREADS` threads trabalhadoras responsáveis pelo processamento das requisições.

Cada requisição apresenta um `id` auto-incremental, uma `i` quantidade de dígitos de pi que deverar ser calculada pela thread trabalhadora e um `tempo_espera` em milisegundos para que a thread trabalhadora espere antes de gravar os dados da requisição processada e o valor de pi calculado no seu arquivo de saída.

## Servidor Multithread em C
Siga as etapas abaixo para instalação de dependências (bibliotecas), compilação e execução do servidor:
1. ### Instalando o GMP
    Acesse o site do [GMP](https://gmplib.org/) e baixe os arquivos compactados da biblioteca, descompacte os arquivos e no terminal navegue até a pasta descompactada e faça:
    - A Configuração da compilação:
        ```
        ./configure
        ```
    - A Compilação da biblioteca:
        ```
        make
        ```
    - A Verificação da compilação:
        ```
        make check
        ```
    - A Instalação da biblioteca:
        ```
        make install
        ```

2. ### Instalando o MPFR
    Acesse o site do [MPFR](https://www.mpfr.org/mpfr-current/#download) e baixe os arquivos compactados da biblioteca, descompacte os arquivos e no terminal navegue até a pasta descompactada e faça:
    - A Configuração da compilação:
        ```
        ./configure
        ```
    - A Compilação da biblioteca:
        ```
        make
        ```
    - A Verificação da compilação:
        ```
        make check
        ```
    - A Instalação da biblioteca:
        ```
        make install
        ```

3. ### Compilando e Executando o Servidor
    No terminal navegue até a pasta do projeto e faça:
    - A Compilação do servidor:
        ```
        ./build.sh
        ```
    - A Execução do servidor:
        ```
        ./servidor <num_req>
        ```
