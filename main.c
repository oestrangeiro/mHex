//----------------------------------------
// Leitor de hexadecimal
// author: Mateus "oestrangeiro" Almeida
// date: 02/2026
//----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

#include "src/colors.h"

#define PROGRAM_NAME 	"mHex"
#define SIZE_PER_OFFSET 16
#define PAGINATOR_DEFAULT_VALUE SIZE_PER_OFFSET

int counterLines = 0;


int main (int argc, char *argv[]){

	char *fileName;
	unsigned char buffer[16];
	int bytesRead;
	unsigned long offset = 0;
	int opt;
	
	// Caso o usuário não informe o valor da paginação, assumo que é 16
	int paginator = PAGINATOR_DEFAULT_VALUE;
	
	/*
		p: recebe um argumento do tipo inteiro, maior que zero.
		Sendo esse número a quantidade de linhas de código hexadecimal
		por página a serem lidos por vez
	*/


	if(argc == 1){
		printf("Uso ./%s -f seu_arquivo\n", PROGRAM_NAME);
		printf("[OPCIONAL] Para paginação, utilize -p seguido de um número\n");
		return 1;
	}

	while( (opt = getopt(argc, argv, "f:p:") ) != -1 ){
		switch(opt){		
			case 'f':
				fileName = optarg;
			break;

			case 'p':
			
				paginator = atoi(optarg);
				
				if(paginator <= 0) {
					fprintf(stderr, "[!] ERRO: Valor de paginação deve ser maior que zero!\n");
					return 1;
				}
				
			break;

			case '?':
				fprintf(stderr, "[!] Erro: Argumentos mal-informados!\n");			
				return 1;
			break;
		}
	}	

	FILE *filePtr = fopen(fileName, "r");

	if(filePtr == NULL){
		fprintf(stderr, "[!] ERRO: Erro ao ler arquivo '%s'!\n%s (CODE: %d)\n", fileName, strerror(errno), errno);
		return 1;
	}
	printf(COLOR_RED "[ %s ]\t\t" COLOR_RESET, fileName);
	printf(COLOR_LIGHT_GREEN "[ Paginação: %d linhas ]\n" COLOR_RESET, paginator);
	printf("OFFSET:     ");
	
	// Coluna de 00 ~ 0F (apenas pra facilitar a visualização)
	for(int i = 0; i < SIZE_PER_OFFSET; ++i){
		i % 2 == 0 ? printf(COLOR_BLUE " %02x" COLOR_RESET, i) : printf(" %02x", i);

	}
	
	printf("\n");
	
	while((bytesRead = fread(buffer, 1, 16, filePtr)) > 0){
		printf(COLOR_CYAN "0x%08lx" COLOR_RESET " | ", offset);

		for(int i = 0; i < SIZE_PER_OFFSET; i++){
			if(i < bytesRead){
				printf("%02x ", buffer[i]);
			}else {
				printf("   ");
			}
		}
		printf(" | ");

		for(int i = 0; i < bytesRead; i++){
			if(isprint(buffer[i])){
				printf(COLOR_GREEN "%c", buffer[i]);	
			}
			else{
				printf("%c", '.');
			}
			printf(COLOR_RESET);
		}
		counterLines++;
		
		offset += SIZE_PER_OFFSET;
		
		printf("\n");

		// Essa instrução faz uma paginação a cada 16 linhas
		// Caso alguém queira usar, só descomentar
		if(counterLines % paginator == 0){
			getchar();
		}
	}

	return 0;
}
