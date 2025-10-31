# Minimização de Autômatos Finitos em C++
#### Este projeto é uma implementação acadêmica em C++ da teoria de autômatos finitos, permitindo a criação, manipulação, conversão e minimização de autômatos. 
#### O código é orientado a objetos e está organizado com arquivos de cabeçalho (.h) na pasta include/ e arquivos de implementação (.cpp) na pasta src/, que é a estrutura padrão para projetos em C++.

## Funcionalidades:
#### A classe Automaton implementa as seguintes operações:
#### Criação de Autômatos Finitos (AFD, AFN e AFN-ε).
#### Validação de Palavras: Verifica se uma palavra é aceita (verify_word).
#### Conversão:
#### - AFN-ε para AFN (afn_epsilon_to_afn).
#### - AFN para AFD (afn_to_afd).
#### Minimização de AFD:
#### - Algoritmo de Preenchimento de Tabela (minimize).
#### - Algoritmo de Hopcroft-Moore (minimize_hopcrof_moore_algorithm).
#### Identificação do tipo de autômato (automaton_type).
#### Gerenciamento de estados, alfabeto, transições e estados finais.

## Estrutura de Pastas:
```
/minimizacaoDeAutomatos/
|
|-- include/
|   |-- automaton.h        (Definição da classe)
|
|-- src/
|   |-- automaton.cpp      (Implementação da classe)
|   |-- main.cpp           (Programa principal que usa a classe)
|
|-- README.md
```

## Pré-requisitos:
#### Este projeto não requer a instalação de nenhuma biblioteca externa. A única coisa necessária é um compilador C++ que suporte o padrão C++11 ou mais recente.
### Compilador: g++ (Recomendado)
#### - Para Windows, você pode instalar o MinGW-w64 para ter o g++.
#### - Para Linux/macOS, ele geralmente já vem instalado.

## Como Compilar e Executar:
#### 1. Clone este repositório para a sua máquina.
#### 2. Abra seu terminal (PowerShell, Bash, etc.) e navegue até a pasta raiz do projeto.
#### 3. Execute o seguinte comando para compilar o projeto:
```
g++ src/main.cpp src/automaton.cpp -o output/main.exe -Iinclude -std=c++11
```
#### 4. Após o comando terminar sem erros, o seu main.exe estará pronto. Para executá-lo, rode:
```
.\output\main.exe
```

##### Para mais informações, você pode acessar a nossa documentação
