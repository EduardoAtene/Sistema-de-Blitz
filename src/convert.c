#include <stdio.h>
#include <stdlib.h>

// ESTRUTURA / STRUCT QUE IR� RECEBER OS VALOERS DO ARQUIVO.TXT E
// CRIAR ESCREVER EM UM ARQUIVO.BIN
typedef struct{
	char placa[8];
	int ano;
	char marca[10];
	char modelo[15];
	char estado[3];
}ttipo;

// FUN��O PRINCIPAL
int main(void) {
    // ABRIR ARQUIVOS
	FILE *base_bin, *base_txt;
	// Abrir arquivo.txt para leitura ("r")
    base_txt = fopen("..//base//base.txt","r");
    // Abrir arquivo.bin para escrita ("wb")
	base_bin = fopen("..//base//base.bin","wb");

	// Verificar se o arquivo foi aberto corretamente.
	if(base_txt == NULL || base_bin == NULL){
        // erro arquivo bin
        if(base_bin == NULL)
            printf("Erro ao abrir o arquivo: Bin");
        // erro arquivo txt
        else
            printf("Erro ao abrir o arquivo: Txt");
		return -1;
    }

    // variavel que ir� armazenar os dados do arquivo.txt e repassar para o arquivo.bin
	ttipo info;
	while(!feof(base_txt)){
        // LER OS DADOS DO ARQUIVO.TXT E JOGAR PARA VARI�VEL QUE EST�R� ARMAZENANDO A ESTRUTURA TTIPO.
		fscanf(base_txt,"%*c %s %d %s %s %s\n",info.placa,&info.ano,info.marca,info.modelo,info.estado);
        // ESCREVER DADOS/INFORMA��O NO ARQUIVO BASE_BIN.BIN
		fwrite(&info,sizeof(ttipo),1,base_bin);

		// Testar se os arquivos est�o sendo lidos corretamentes.
        // Se quiser remover, fique avontade. Pode tamb�m comentar utilizando o ( // ) ou ( /* HI BITCH */)
		//printf("\n %d Lido : %s %d %s %s %s",a,info.placa,info.ano,info.marca,info.modelo,info.estado);
    }

	// Fechar arquivos
	fclose(base_txt);
	fclose(base_bin);


	return 0;
}
