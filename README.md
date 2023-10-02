# C-bernets
collegeproject

O objetivo deste programa e emular um escalonamento de Pods para Workers, 
Por isso se tera uma classe chamada Worker que so tem metricas de espaco disponivel
e uma classe chamada Pod que simula os trabalhos que ela precisa separar para escalonar a um worker
```
std::vector < Pod > p_vec = {
    Pod (3),
    Pod (2)
  };

std::vector < Worker > w_vec = {
  Worker (4000, 16),
  Worker (3000, 8)
};
```
Para introduzir aleatoriedade no programa descomente o srand() no comeco da funcao main
pode se introduzir mais aleatoriedade quanto ao escalonamento de pods pondo um 
random() % N_Teto + N_Chao em seu construtor

#Compilando o programa
Para compilar o programa e preciso de Make, se nao tiver make pode se instalar utilizando o comando
```
sudo apt install make
```
Utilize o comando make na presenca de um Makefile e ira iniciar um script e se tera um programa ELF no diretorio
onde foi iniciado o programa
