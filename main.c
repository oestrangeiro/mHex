//----------------------------------------
// Leitor de hexadecimal
// author: Mateus "oestrangeiro" Almeida
// date: 02/2026
//----------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "src/colors.h"

#define PROGRAM_NAME 	"mHex"
#define SIZE_PER_OFFSET 16

int counterLines = 0;


int main (int argc, char *argv[]){

	char fileName[64];
	unsigned char buffer[16];
	int bytesRead;
	unsigned long offset = 0;

	if(argc == 1){
		printf("Uso ./%s file\n", PROGRAM_NAME);
		return 1;
	}

	strcpy(fileName, argv[1]);

	FILE *filePtr = fopen(fileName, "r");

	if(filePtr == NULL){
		printf("[!] Erro ao ler arquivo '%s'!\n", fileName);
		exit(-1);
	}
	printf(COLOR_RED "[ %s ]\n" COLOR_RESET, fileName);
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
		// if(counterLines % 16 == 0){
		// 	getchar();
		// }
	}

	return 0;
}
