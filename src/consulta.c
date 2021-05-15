#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

//CHAVE HASH
#define KEY 9973

/*
    MAIOR LISTA QUE CONT�RA NESSA TABELA HASH � DE 17576.
    Porque? A gente ir� utilizar os numeros que cont�m na placa para consutar/destribuir na tabela hash.
    Sendo assim, j� que a placa cont�m 3 letras no alfabeto, o maior caso para conter os dados seria 26*26*26.
    J� que tem 26 possibilidadades de encaixar as letrar.
    26*26*26 = 26� = 17576
*/

// REGISTRO / STRUCT > GUARDAR NA TABELA HASK.
// PLACA = PLACA DO CARRO // ENTRADA DO USU�RIO
// RRN = POSI��O DO CARRO NO ARQUIVO.
typedef struct{
	char placa[8];
	int rrn;
}tpos;

// REGISTRO / STRUCT > GUARDA NA VARI�VEL
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

// DECLARA��O DA TABELA HASH.
list *tabela_hash;


// CONVERTE OS QUATRO NUMERO DA PLACA EM NUMEROS INTEIROS
// SER� USADO PARA ENCONTRAR A CHAVE DA TABELA HASH.
// POR MEIO DO NUMERO DA PLACA (N�O INCLUI AS LETRAS) O RESTO DA DIVIS�O POR (KEY) DEFINIDA L� EM CIMA
// POR MEIO DO RESTO, SER� O INDICE DA TABEL HASH
int conv_int(char *vet){
    int m,c,d,u;
    m = (((int) vet[3])- 48) * 1000;
    c = ((((int) vet[4])- 48) * 100) + m;
    d = ((((int) vet[5])- 48)* 10) + c;
    u = ((((int) vet[6]) - 48) * 1) + d;

    //verificar se a convers�o est� sendo efetuada corretamente
    /*
    printf("             M= %d",m);
    printf("             C=%d",c);
    printf("             D=%d",d);
    printf("             u=%d",u);
    */

    //retornar numero da placara
    return u;
}

// FUN��O HASH
// ELA VAI PEGAR O RESTO DA DIVIS�O DOS 4 NUMEROS INTEIROS DA PLACA
// OU SEJA, A PARTIR DO RESTO DA DIVIS�O, VAI DAR INDICE NA TABELA HASH
// .
// A CHAVE/KEY ESCOLHIDA FOI O N�MERO PRIMO 9973.
// Para o menor tamanho da lista que vai conter na tabela, ser� 0, com 1 milh�o de dados.
// Para o maior tamanho da lista ser� de 26�, ou seja, 17576.
// Se n�o ouver placas com n�meros iguais e ser ditribuida uniformamente, o menor caso ser� de 100 e o maior de 101.
int hash(int numero, int key){
    int aux = key%numero;
    return aux;
}

// VERIFICAR SE AS DUAS PLACAS S�O IGUAIS
int verif_placa(tpos placa_c,list placa_base){
    //verificar se a placa � igual ou n�o. se for igual vai retorna 0, sen�o, -1.
    int a;
    for(a=0;a<7;a++){
        if(placa_c.placa[a] != placa_base->objeto.placa[a])
            return -1;
    }
    return 0;
}

// CRIAR TABELA COM KEY POSIS�O
// ALOCA��O DINAMICA*
// CADA POSI��O DA TABELA � UM PONTEIRO PARA OUTRO PONTEIRO QUE APONTA PARA A ESTRUTURA LISTA
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

    //Verificar se essa placa j� est� cadastrada
    // Verificar se tem dados repetidos. Se tiver n�o vai adicionar na tabela
    list t;
    for(t = tabela_hash[indice]; t!=NULL; t = t->prox){
        int test;
        // retorna 0 siginifca que a placa j� � existente
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

    //LEITURA  X = numero posi��o na base.bin
    fseek(base_bin,((x-1)*sizeof(ttipo)),SEEK_SET);
    fread(&dados,sizeof(ttipo),1,base_bin);

    // imprimir na tela os dados do usu�rio
    printf("carro %s %d %s %s %s eh roubado!\n",
            dados.placa,dados.ano,dados.marca,dados.modelo,dados.estado);
    //fechar arquivo
    fclose(base_bin);
}


int procurar_tabela(tpos dados,int indice){
    list aux;
    int y;
    for(aux=tabela_hash[indice];aux!=NULL; aux = aux->prox){
        //verificar se a placa � igual ou n�o. se for igual vai retorna 0, sen�o, -1.
        y = verif_placa(dados,aux);
        if(y==0){
            // retorna o numero da rrn, ou seja, a posi��o da base.bin dos dados da placa encontrada
            return aux->objeto.rrn;
        }
    }
    // n�o achou a placa na tabela
    return -1;
}

int consutar_tabela(tpos dado_p){
    // X = RNN = Posi��o da linha na base.bin
    int x;
    int indice,n_placa;

    // converter o numero da placa para inteiro e fazer o processo de hashing
    // no caso, ver a chave/indice na tabela hash
    n_placa = conv_int(dado_p.placa);
    // calcular aonde se encontra o indice atr�ves da entrada da placa.
    indice = hash(n_placa,KEY);
    x = procurar_tabela(dado_p,indice);
    // Caso retorna -1, siginifica que n�o encontrou a placa, ou seja, n�o consta na lista
    if(x==-1){
        printf("carro %s nao consta na lista\n",dado_p.placa);
    }
    // sen�o, vai imprimir as informa��es da placa.
    else{
        imprimir(x);
    }
}

//      FUN��O PRINCIPAL
int main(void) {
    // ARQUIVO
	FILE *base_bin;
	// INDICE_POSI��O_BIN
	int indice_bin=1;
	// AUX
	ttipo base;
	// AUX PARA INCLUIR NA TABELA HASH. IR� ARMAZENA A ESTRUTURA. NESSA ESTRUTA CONT�M A
	// POSI��O NO ARQUIVO BIN (rnn) E O N�MERO DA PLAA
	tpos base_i;

	// ABRIR ARQUIVO BIN
	base_bin = fopen("base.bin","rb");

	// VERIFICAR SE FOI ABERTO COM SUCESSO OU GERO ALGUM ERRO. TALVEZ N�O EXISTA O ARQUIVO E RETORNA NULL / ERRO
	if(base_bin == NULL){
        printf("Erro ao abrir o arquivo bin�rio");
        return -1;
	}

    //CRIAR TABELA
    criar_tabela();

    printf("CARREGANDO BASE DA DADOS..");

    // Ler o arquivo bae_bin
	while(!feof(base_bin)){
        // Ler o arquivo base_bin e armazena na vari�vel aux base
		fread(&base,sizeof(ttipo),1,base_bin);

        // Armazenar a string base.placa[aux] (PLACA) que cont�m o numero da placara
        // e armazena na estrura/struct tpos que ir� aramazena na tabela
		int aux;
		for(aux=0;aux<7;aux++)
            base_i.placa[aux] = base.placa[aux];
        // Armazena a posi��o.
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
