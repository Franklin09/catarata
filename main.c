#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
int main(int argc, char* argv[]){

    //testar se foi passado os argumentos
    if (argc < 5) {
		fprintf(stderr, "A execucao do programa deve atender ao seguinte formato: -i <imageEntrada.ppm> -f <ImagemSaida.ppm>\n\n");
		exit(1);
	}else{

    //armazena a path
    char url[100];
    PPM img = {0};

    //completa com nome do arquivo passado
    strcpy(url,"Img/"); strcat(url, argv[2]);

    //passa caminho para funcao carregar
    img = carregaPPM(url),

    //chama menu e envia os paramentos nescessarios
    menu(url,img,argv[4]);

    }

}
