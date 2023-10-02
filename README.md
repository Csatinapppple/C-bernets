# C-bernets
collegeproject

O objetivo deste programa e emular um escalonamento de trabalhos para Worker pods, 
por isso se tera uma serie de comandos entrelacados entre pipes que simularao os tasks de um job
e tera um algoritimo que se responsabilizara de escalonar esses processos quando tiver memoria livre
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
