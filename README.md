# Implementacao_4b

# Compilar e rodar a implementação
Compile todos arquivos .cpp previamente e rode o arquivo Main.cpp.

## Exemplo de compilação e execução:
### MacOS
g++-14 -std=c++17 *.cpp -o Main && ./Main

### Windows
g++ -o programa *.cpp -std=c++17
./Main

# Formato das Imagens
A imagem deve estar no formato .ppm (P3 - Formato legível)

# Comando Python
Nosso código utiliza um script python. 
- Certifique de possuir o pip instalado para as bibiliotecas corretas serem instaladas.
- Caso você utilize o comando 'python3 arquivo.py' para executar um programa python, você deve alterar a linha 134 do arquivo Grafo.cpp:
    "string command = "python probabilidade-2.py " + json_path;"
    Altere para: "string command = "python3 probabilidade-2.py " + json_path;"