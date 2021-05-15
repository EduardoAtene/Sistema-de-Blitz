# Sistema de Blitz usando tabela Hash
<br>
(convert.c) Considerando o arquivo base.txt (disponível na pasta *base*) foi escrito um programa convert.c que converte a base para o formato binário de registros com os campos e gerar o arquivo base.bin .

*typedef struct{*<br>
	*char placa[8];*
	<br>*int ano;*
	<br>*char marca[10];*
	<br>*char modelo[15];*
	<br>*char estado[3];*  
*}ttipo;*


(consulta.c) Foi implementado um programa que carregue o base.bin em uma hash onde o tratamento de colisões é realizado por cada uma lista encadeada conforme ilustrado na Figura 1.


![](img//Figura1.png)


Entrada: A entrada consiste em uma sequˆencia de placas a serem consultadas na base que sempre comec¸a

com a letra ‘c’ conforme ilustrado a seguir:
<br>
c OFD5325<br>
c OEC1204<br>
c NWZ5965<br>
c NOW7292<br>
c OCD7946<br>
c MTC9949<br>
c NVM1038<br>
c NRQ1086<br>
c OLN6083<br>
c OEC1204<br>
c OLV2474<br>
c NWZ5965<br>
c NOW7292<br>
c OCD7946<br>
c MTC9949<br>
c NVM1038<br>
c NRQ1086<br>
c OLN6083<br>
c OCX5847<br>
<br>
Saída: Para cada linha de entrada contém uma linha de saída informando se a placa existe na base de dados. Caso a placa conste na base de dados, o programa irá retornar todas as informações do registro deste carro.

*carro '**placa**' '**ano**' '**marca**' '**modelo**' '**estado**' eh roubado!*

caso o carro esteja na lista de carros roubados. Caso contrário o programa irá escrever:

*carro '**placa**' nao consta na lista*



