#include <stdio.h>
//Verificar se o caracter é um número
int verificaNumero (char c) {
    if ((c >= 48) && (c <= 57)) { // entre 48 e 57 estão de 0 a 9 na tabela ASCII
        return 1;
    } else {
        return 0;
    }
}

//Função responsável por fazer a compressão do arquivo.
void Comprimir () {
    char text[100], comprimido[100], qtd[3];
    int tamText = 0, i = 0, j = 0, count = 0, inicio;
    FILE *arq, *cp;
    char *resultRead, *resultWrite;

    //Abre o arquivo que será comprimido para leitura 
    arq = fopen("teste.txt", "rt"); //r = read (lê o txt)
    if (arq == NULL)  // Se houve erro na abertura
    {
       printf("Problemas na abertura do arquivo\n");
       return -1;
    }

    //Abre o arquivo de compressão para escrita
    cp = fopen("teste_comp.txt", "w"); //w = write (escreve o txt do zero) //rle = Run Length Encoding
    if (cp == NULL)  // Se houve erro na abertura
    {
       printf("Problemas na abertura do arquivo\n");
       return -1;
    }

    //Até o final do arquivo que será comprimido
    while (!feof(arq))
    {
        memset(text,0x0,100); //limpa variavel text
        resultRead = fgets(text, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
        tamText = strlen(text);  
        count = 0;
        for (i = 0; i < tamText; i++) {
            //Se o caracter lido é igual ao próximo caracter que será lido incrementa a quantidade desse carcter
            if (text[i] == text[i+1] && i+1 <= tamText) { 
                count++;
            } else {
                //Se o caracter lido é diferente do anterior e o contador de quantidade for maior que zero, faz a compressão
                if (count > 0) {                    
                    CodCompressao(comprimido, count+1, text[i-1]); //Retorna o texto que foi comprimido (ex.: lê BB retorna @2B)
                    count = 0; 
                    //printf("String Comprimida: %s\n", comprimido);
                    resultWrite = fputs(comprimido, cp); //joga o texto que foi comprimido para o novo arquivo comprimido 
                //Se não houve contabilização apenas imprime o que foi lido
                } else {
                    comprimido[0] = text[i];
                    comprimido[1] = '\0';
                    resultWrite = fputs(comprimido, cp); //joga o texto que não teve necessidade de compressão para o novo arquivo 
                }
            }
        }
    }

    //Fecha o arquivo que estava sendo lido.
    fclose(arq);

    if (resultWrite == EOF)
        printf("Erro na Gravacao\n");
    else 
        printf("Arquivo Compactado com Sucesso\n"); 

    //Fecha o arquivo que estava sendo escrito.
    fclose(cp);
}

void Extrair () {
    char text[100], descomprimido[100], aux[4];
    int tamText = 0, i = 0, j = 0, qtd = 0, inicio;
    FILE *arq, *cp;
    char *resultRead, *resultWrite;

    //Abre o arquivo que foi comprimido para leitura 
    arq = fopen("teste_comp.txt", "rt"); //r = read (lê o txt)
    if (arq == NULL)  // Se houve erro na abertura
    {
       printf("Problemas na abertura do arquivo\n");
       return -1;
    }

    //Abre o arquivo que será descomprimido para escrita 
    cp = fopen("teste_descomp.txt", "w"); //w = write (escreve o txt do zero) 
    if (cp == NULL)  // Se houve erro na abertura
    {
       printf("Problemas na abertura do arquivo\n");
       return -1;
    }

    while (!feof(arq))
    {
        memset(text,0x0,100); //limpa variavel text
        resultRead = fgets(text, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
        tamText = strlen(text);  
        for (i = 0; i < tamText; i++) {
            //Se o caracter lido for um @ então o próximo caracter é obrigatoriamente um número e uma letra
            if (text[i] == '@') { 
                aux[0] = text[i+1]; //Pega o número que vem logo após o '@'
                aux[1] = '\0';//Fecha a String
                qtd = atoi(aux); //transforma string em inteiro
                CodDesCompressao(descomprimido, qtd, text[i+2]); //text[i+2] é a posição que temos o caracter que será repetido (@1c)
                //printf("String Descomprimida: %s\n", descomprimido);
                resultWrite = fputs(descomprimido, cp); //joga o texto que foi comprimido para o novo arquivo descomprimido
                i = i + 2; //Já que depois do '@' vem obrigatoriamente um numero e uma letra (Ex.: @1c)
            //Se não, será impresso somente o caracter encontrado.
            } else {
                descomprimido[0] = text[i];
                descomprimido[1] = '\0';
                resultWrite = fputs(descomprimido, cp); //joga o texto que não teve necessidade de descompressão para o novo arquivo 
            }
        }
    }

    //Fecha o arquivo que estava sendo lido.
    fclose(arq);

    if (resultWrite == EOF)
        printf("Erro na Gravacao\n");
    else 
        printf("Arquivo Descompactado com Sucesso\n"); 

    //Fecha o arquivo que estava sendo escrito.
    fclose(cp);
}

int main (void) {
    int continuar=1;

    system("clear");

    do
    {
        printf("\n\tCompactador de Arquivos - Run Length Encoding\n\n");
        printf("1. Comprimir arquivo\n");
        printf("2. Extrair arquivo\n");
        printf("0. Sair\n\n");

        scanf("%d", &continuar); getchar();
        fflush(stdin);

        system("clear");

        switch(continuar)
        {
            case 1:
	        Comprimir();
                break;

            case 2:
                Extrair();
                break;

            case 0:
                //sair();
                break;

            default:
                printf("Digite uma opcao valida\n");
        }
    } while(continuar);

    return 0;
}

//Função responsável por criar o código Run Length Encoding
//Recebe uma string que será usada para devolver o código 
//Recebe um inteiro 'qtdCaracter' que é a quantidade de caracteres encontrados no arquivo original
//Recebe um char 'c' que é a letra encontrada no arquivo original
//Ex.: CodCompressao(comprimido, 15, c);
// Ao final a função retorna "@9c@6c"  
void CodCompressao(char * string, int qtdCaracter, char c) {
    int i = 0, tamText = 0, j = 0, val = 0, count, acumulador;
    char cadeia[100], qtd[3];

    //Divide a quantidade por 9 para que não passe de uma casa decimal a quantidade
    if (qtdCaracter%9 == 0) {
        val = qtdCaracter/9;
    } else {
        val = (qtdCaracter/9)+1;
    }
   
    //printf("Qtd de Caracteres: %d\n", qtdCaracter);
    val = val * 3; //multiplica o valor recebido por 3 para que possam ser impressos o "@9c" e o "@6c"

    count = 0;
    acumulador = 0;
    for (i = 0; i < val; i++) {
        //Quando não for o ultimo será impresso "@9c" e será acumulado esse valor para poder imprimir o resto ao final
        if (i < val - 3) {
            count = 9;
            acumulador = acumulador + count;
        //Quando for o ultimo será impresso o total - a quantidade acumulada "@6c"
        } else {
            count = qtdCaracter - acumulador;
        }      
        sprintf(qtd, "%d", count); //Transforma o contador em caracter //primeiro 9 e depois 6
        cadeia[i] = '@'; // imprime o @
        cadeia[i+1] = qtd[0]; //imprime a quantidade
        cadeia[i+2] = c; //imprime o caracter
        i = i + 2;
    }   
    cadeia[i] = '\0';

    strcpy(string, cadeia); //devolve pra função que chamou o texto "@9c@6c"
    //printf("RESULTADO %s\n", cadeia);
}

//Função responsável por receber uma quantidade e um caracter devolvendo o original do código Run Length Encoding
//Recebe um inteiro que é a quantidade de caracteres que será replicada
//Recebe um char que é o caracter que será replicado
//Ex.: CodDesCompressao(comprimido, 15, c);
// Ao final a função retorna "ccccccccccccccc"  
void CodDesCompressao(char * string, int qtd, char c) {
    int i = 0;
    char aux[100];

    //printf("Quantidade %d de %c\n", qtd, c);
    //Cria a quantidade de letras necessarias
    for (i = 0; i < qtd; i++) {
        aux[i] = c;
    }   
    aux[i] = '\0';
    strcpy(string, aux); //devolve pra função que chamou o texto "ccccccccccccccc"
}
