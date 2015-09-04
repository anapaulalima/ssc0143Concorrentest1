# ssc0143Concorrentest1


Para compilar:
  gcc -o sequencial trab1.c -lm
  gcc -o paralelo trab1_concorrente.c -lm -lpthread
  
Para rodar:
  bash tests.sh
  
Nele é possível modificar os parametros:
  - número de threads é parâmetro de execução, caso não passado o número utilziado é o de processadores
  - arquivos devem ser usados com redirecionamento de entrada
  - saída do scrip: média entre 10 execuções.
