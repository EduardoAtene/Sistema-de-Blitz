#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//CHAVE HASH
#define KEY 9973

/*
    MAIOR LISTA QUE CONTÉRA NESSA TABELA HASH É DE 17576.
    Porque? A gente irá utilizar os numeros que contém na placa para consutar/destribuir na tabela hash.
    Sendo assim, já que a placa contém 3 letras no alfabeto, o maior caso para conter os dados seria 26*26*26.
    Já que tem 26 possibilidadades de encaixar as letrar.
    26*26*26 = 26³ = 17576
*/

// REGISTRO / STRUCT > GUARDAR NA TABELA HASK.
// PLACA = PLACA DO CARRO // ENTRADA DO USUÁRIO
// RRN = POSIÇÃO DO CARRO NO ARQUIVO.
typedef struct{
	char placa[8];
	int rrn;
}tpos;

// REGISTRO / STRUCT > GUARDA NA VARIÁVEL
// STRUCT DO ARQUIVO .BIN
typedef struct{
	char placa[8];
	int ano;
	char marca[10];
	char modelo[15];
	char estado[3];
}ttipo;

// REGISTRO / STRUCT > LISTA
typedef struct Lista *list;
struct Lista{
    tpos    objeto;
    list    prox;
};

// DECLARAÇÃO DA TABELA HASH.
list *tabela_hash;


// CONVERTE OS QUATRO NUMERO DA PLACA EM NUMEROS INTEIROS
// SERÁ USADO PARA ENCONTRAR A CHAVE DA TABELA HASH.
// POR MEIO DO NUMERO DA PLACA (NÃO INCLUI AS LETRAS) O RESTO DA DIVISÃO POR (KEY) DEFINIDA LÁ EM CIMA
// POR MEIO DO RESTO, SERÁ O INDICE DA TABEL HASH
int conv_int(char *vet){
    int m,c,d,u;
    m = (((int) vet[3])- 48) * 1000;
    c = ((((int) vet[4])- 48) * 100) + m;
    d = ((((int) vet[5])- 48)* 10) + c;
    u = ((((int) vet[6]) - 48) * 1) + d;

    //verificar se a conversão está sendo efetuada corretamente
    /*
    printf("             M= %d",m);
    printf("             C=%d",c);
    printf("             D=%d",d);
    printf("             u=%d",u);
    */

    //retornar numero da placara
    return u;
}

// FUNÇÃO HASH
// ELA VAI PEGAR O RESTO DA DIVISÃO DOS 4 NUMEROS INTEIROS DA PLACA
// OU SEJA, A PARTIR DO RESTO DA DIVISÃO, VAI DAR INDICE NA TABELA HASH
// .
// A CHAVE/KEY ESCOLHIDA FOI O NÚMERO PRIMO 9973.
// Para o menor tamanho da lista que vai conter na tabela, será 0, com 1 milhão de dados.
// Para o maior tamanho da lista será de 26³, ou seja, 17576.
// Se não ouver placas com números iguais e ser ditribuida uniformamente, o menor caso será de 100 e o maior de 101.
int hash(int numero, int key){
    int aux = key%numero;
    return aux;
}

// VERIFICAR SE AS DUAS PLACAS SÃO IGUAIS
int verif_placa(tpos placa_c,list placa_base){
    //verificar se a placa é igual ou não. se for igual vai retorna 0, senão, -1.
    int a;
    for(a=0;a<7;a++){
        if(placa_c.placa[a] != placa_base->objeto.placa[a])
            return -1;
    }
    return 0;
}

// CRIAR TABELA COM KEY POSISÃO
// ALOCAÇÃO DINAMICA*
// CADA POSIÇÃO DA TABELA É UM PONTEIRO PARA OUTRO PONTEIRO QUE APONTA PARA A ESTRUTURA LISTA
void criar_tabela(){
    int h;
    tabela_hash = (list*) malloc(KEY*sizeof(list));
    for (h = 0; h < KEY; h++)
        tabela_hash[h] = NULL;
}

// INSERIR VALORES NA TABELA.
int inserir_tabela(tpos dado){
    int indice,n_placa;
    n_placa = conv_int(dado.placa);
    indice = hash(n_placa,KEY);

    //Verificar se essa placa já está cadastrada
    // Verificar se tem dados repetidos. Se tiver não vai adicionar na tabela
    list t;
    for(t = tabela_hash[indice]; t!=NULL; t = t->prox){
        int test;
        // retorna 0 siginifca que a placa já é existente
        test = verif_placa(dado,t);
        if(test==0){
            return -1;
        }
    }
    list nova = (Lista*) malloc(sizeof(Lista));
    nova->objeto = dado;
    nova->prox = tabela_hash[indice];
    tabela_hash[indice] = nova;
}

void imprimir(int x){
    FILE *base_bin;
	base_bin = fopen("base.bin","rb");
    ttipo dados;

    //LEITURA  X = numero posição na base.bin
    fseek(base_bin,((x-1)*sizeof(ttipo)),SEEK_SET);
    fread(&dados,sizeof(ttipo),1,base_bin);

    // imprimir na tela os dados do usuário
    printf("carro %s %d %s %s %s eh roubado!\n",
            dados.placa,dados.ano,dados.marca,dados.modelo,dados.estado);
    //fechar arquivo
    fclose(base_bin);
}


int procurar_tabela(tpos dados,int indice){
    list aux;
    int y;
    for(aux=tabela_hash[indice];aux!=NULL; aux = aux->prox){
        //verificar se a placa é igual ou não. se for igual vai retorna 0, senão, -1.
        y = verif_placa(dados,aux);
        if(y==0){
            // retorna o numero da rrn, ou seja, a posição da base.bin dos dados da placa encontrada
            return aux->objeto.rrn;
        }
    }
    // não achou a placa na tabela
    return -1;
}

int consutar_tabela(tpos dado_p){
    // X = RNN = Posição da linha na base.bin
    int x;
    int indice,n_placa;

    // converter o numero da placa para inteiro e fazer o processo de hashing
    // no caso, ver a chave/indice na tabela hash
    n_placa = conv_int(dado_p.placa);
    // calcular aonde se encontra o indice atráves da entrada da placa.
    indice = hash(n_placa,KEY);
    x = procurar_tabela(dado_p,indice);
    // Caso retorna -1, siginifica que não encontrou a placa, ou seja, não consta na lista
    if(x==-1){
        printf("carro %s nao consta na lista\n",dado_p.placa);
    }
    // senão, vai imprimir as informações da placa.
    else{
        imprimir(x);
    }
}

//      FUNÇÃO PRINCIPAL
int main(void) {
    // ARQUIVO
	FILE *base_bin;
	// INDICE_POSIÇÃO_BIN
	int indice_bin=1;
	// AUX
	ttipo base;
	// AUX PARA INCLUIR NA TABELA HASH. IRÁ ARMAZENA A ESTRUTURA. NESSA ESTRUTA CONTÉM A
	// POSIÇÃO NO ARQUIVO BIN (rnn) E O NÚMERO DA PLAA
	tpos base_i;

	// ABRIR ARQUIVO BIN
	base_bin = fopen("base.bin","rb");

	// VERIFICAR SE FOI ABERTO COM SUCESSO OU GERO ALGUM ERRO. TALVEZ NÃO EXISTA O ARQUIVO E RETORNA NULL / ERRO
	if(base_bin == NULL){
        printf("Erro ao abrir o arquivo binário");
        return -1;
	}

    //CRIAR TABELA
    criar_tabela();

    printf("CARREGANDO BASE DA DADOS..");

    // Ler o arquivo bae_bin
	while(!feof(base_bin)){
        // Ler o arquivo base_bin e armazena na variável aux base
		fread(&base,sizeof(ttipo),1,base_bin);

        // Armazenar a string base.placa[aux] (PLACA) que contém o numero da placara
        // e armazena na estrura/struct tpos que irá aramazena na tabela
		int aux;
		for(aux=0;aux<7;aux++)
            base_i.placa[aux] = base.placa[aux];
        // Armazena a posição.
		base_i.rrn = indice_bin;

        // VERIFICAR SE FOI ARMAZENADO
        //printf("\n %d: %s ",base_i.rrn,base_i.placa);


		// INSERIR ESTRUTURA BASE_BIN NA TABEL HASH
		// PAREI AQUI
		inserir_tabela(base_i);

		indice_bin+= 1;
	};
	// FECHAR ARQUIVO.
	fclose(base_bin);

    printf("\nBASE DA DADOS CARREGADA\n");

    while(1){
        tpos dado_consulta;
        int x;
        // ENTRADA DOS VALORES
        scanf("%*c %s",dado_consulta.placa);
        if (dado_consulta.placa[0] != 'c')
            // BUSCA.
            consutar_tabela(dado_consulta);
    }

	return 0;
}
