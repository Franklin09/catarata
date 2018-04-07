#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <string.h>
#include "setup.h"
/********************************************************************************/
//alocar matriz
int **alocaMatriz(int l, int c){
    int i;
    PIXELS **n = {0};
    //linhas
    n = (PIXELS **)calloc(l,sizeof(PIXELS*));
    if(n == NULL){
        printf("Erro ao alocar memoria...\n");
        exit(1);
    }
    //colunas
    for(i = 0; i < l; ++i){
        n[i] = (PIXELS *)calloc(c,sizeof(PIXELS));
        if(n == NULL){
        printf("Erro ao alocar memoria...\n");
        exit(1);
        }
    }
return (n);
}
/********************************************************************************/
PPM ppmLimpa(int l, int c){
PPM img = {0};
img.l = l;
img.c = c;
img.corMax = 255;
img.tipo[2] = "P3\n";
img.pixel =  alocaMatriz(l,c);
return img;
}
/********************************************************************************/
PPM carregaPPM(char *url){

    FILE *arq;
    int i, j;
    PPM img = {0};

  //ler o cabeçalho do arquivo para saber as dimensões da img
    arq = fopen(url,"r");
    //verifica se abriu
    if(arq==NULL){
        printf("Erro ao abrir aquivo!");
        exit(1);
    }
    fgets(&img.tipo, 5, arq); // linha 1 tipo
    fscanf(arq, "%*[^\n]\n");// linha 2 comentario, ignora
    fscanf(arq,"%d %d",&img.c,&img.l);//linha 3 largura x altura
    fscanf(arq,"%d",&img.corMax);//linha 3 cor maxima

    //debug
    printf("\nTIPO:%c%c RES:%i x %i COR MAXIMA:%d \n",img.tipo[0],img.tipo[1],img.c,img.l,img.corMax);

    //alocar matriz de pixel
    img.pixel = alocaMatriz(img.l,img.c);
    printf("carregando imagem...\n");

    //ler matriz de pixel
    for(i=0; i < img.l; i++){
        for(j=0; j< img.c; j++){
           fscanf(arq, "%d %d %d", &img.pixel[i][j].r, &img.pixel[i][j].g, &img.pixel[i][j].b);
        }
    }
    fclose(arq);

   printf("imagem carregada!\n");
   printf("Iniciando modulo de processamento de imagem...Aguarde\n\n");
   Sleep(6000);
   system("cls");

   return img;
}
/********************************************************************************/
void criaPPM(PPM novaImg, char *url) {

    FILE *arq = fopen(url, "w");
    if(arq==NULL){
        printf("Erro ao abrir aquivo!\n");
        exit(1);
    }
    int i, j;
    //escrever cabecalho
    fprintf(arq, "P3\n%");//tipo
    fprintf(arq, "#COMENTARIO: PPM SAIDA\n%");//tipo
    fprintf(arq, "%d %d\n",novaImg.c,novaImg.l);//tamanho
    fprintf(arq, "%d\n",novaImg.corMax);//maxCor
    printf("Escrevendo cabecalho da imagem...\n");

    for(i=0; i < novaImg.l; i++){
        for(j=0; j< novaImg.c; j++){

           fprintf(arq, "%d %d %d\n", novaImg.pixel[i][j].r, novaImg.pixel[i][j].g, novaImg.pixel[i][j].b);
         }
    }

    printf("Imagem gravada com Sucesso! local:%s\n\n",url);
    Sleep(4000);
  fclose(arq);

}
/********************************************************************************/
/*
Para converter qualquer cor em seu nível aproximado de cinza, deve-se primeiro
obter suas primitivas vermelho, verde e azul (da escala RedGreenBlue). Adiciona-se
então 30% do vermelho mais 59% do verde mais 11% do azul, independente da
escala utilizada (0.0 a 1.0, 0 a 255, 0% a 100%.) O nível resultante é o valor de cinza
desejado. Tais porcentagens estão relacionadas à própria sensibilidade visual do
olho humano convencional para as cores primárias.
*/
PPM converteCinza(PPM img){
    //Convertendo imagem em escala de cinza...
    int i,j, l;
    for(i=0; i < img.l; i++){
        for(j=0; j< img.c; j++){

            l = 0.30*img.pixel[i][j].r + 0.59*img.pixel[i][j].g + 0.11*img.pixel[i][j].b;
            img.pixel[i][j].r = l;
            img.pixel[i][j].g = l;
            img.pixel[i][j].b = l;

         }
    }
printf("Convertido em cinza!\n");
Sleep(4000);
return img;
}
/********************************************************************************/
//filtro gaussiano
//http://www.lac.inpe.br/~demisio/univ/pdi/lab2.pdf

PPM filtroGaussiano(PPM img){
int k,l,i,j;
float s=0,d=159;
PPM nova = ppmLimpa(img.l,img.c);

//kernel 5 x 5 gaussiano
int mask[5][5] = {{ 2,  4,  5,  4, 2 },
                 { 4,  9, 12,  9, 4 },
                 { 5, 12, 15, 12, 5 },
                 { 4,  9, 12,  9, 4 },
                 { 2,  4,  5,  4, 2 }};

    //percorre a imagem
    for(k=0; k < img.l; k++){
        for(l=0; l < img.c; l++){

            for(i=-2; i <=2; i++){
                for(j=-2; j <=2; j++){
                        //retira indices negativos
                        if((k+i >= 0 && k+i < img.l) && (l+j >= 0 && l+j < img.c)){
                            s+=(img.pixel[k+i][l+j].r * mask[i+2][j+2]);
                        }

                }
            }
        nova.pixel[k][l].r=s/d;
        nova.pixel[k][l].g=s/d;
        nova.pixel[k][l].b=s/d;
        s=0;
        }

    }

free(img.pixel);
printf("filtro gaussiano aplicado!\n");
Sleep(4000);
 return nova;
}

/********************************************************************************/
//https://rhaylsonsilva.wordpress.com/2012/03/02/processamento-de-imagens-deteccao-de-bordas-de-imagens/
PPM filtroSobel(PPM img){

//cria uma nova imagem limpa e aloca memoria
PPM sobelImg = ppmLimpa(img.l,img.c);
   //filtro sobel 1
    int sobel_x [3] [3] = {{3, 0, -3}, {10, 0, -10}, {3, 0, -3}};
	int sobel_y [3] [3] = {{3, 10, 3}, {0, 0, 0}, {-3, -10, -3}};
   //filtro sobel 2
	//int sobel_x[3][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
	//int sobel_y[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

	int filtro_x;
	int filtro_y;
	int i,j,k,l,count=0;

   //percorre a imagem

	for (i = 0; i < img.l; ++i) {
		for (j = 0; j < img.c; ++j) {

			//convolucao
			filtro_x = 0;
			filtro_y = 0;
			for (k = 0; k < 3; ++k) {
				for (l = 0; l < 3; ++l) {
					//verifica se o pixel estiver na borda
					if ( (i == 0 || j == 0) || (i == img.l - 1 || j == img.c - 1) ) {
						// se tiver entao o valor é 0
						filtro_x += 0;
						filtro_y += 0;
					} else {
						// se não multiplica
						filtro_x += img.pixel[i-1+k][j-1+l].r * sobel_x[k][l];
						filtro_y += img.pixel[i-1+k][j-1+l].r * sobel_y[k][l];
					}
				}
			}

			// aq testamos se o valor do pixel passa do valor maximo 255, se for maior seta 255
			if (sqrt(filtro_x*filtro_x + filtro_y*filtro_y) > img.corMax) {
				sobelImg.pixel[i][j].r = img.corMax;
				sobelImg.pixel[i][j].g = img.corMax;
				sobelImg.pixel[i][j].b = img.corMax;
			} else {
			    //Raiz quadrada de (pixelV2 + pixelH2 )
			    //Feito isso para todos os pixels uma imagem onde as bordas estarão em destaque
				sobelImg.pixel[i][j].r = sqrt(filtro_x*filtro_x + filtro_y*filtro_y);
				sobelImg.pixel[i][j].g = sqrt(filtro_x*filtro_x + filtro_y*filtro_y);
				sobelImg.pixel[i][j].b = sqrt(filtro_x*filtro_x + filtro_y*filtro_y);
			}
		}
	}



printf("Filtro sobel aplicado!\n");
Sleep(4000);
   //retorna a imagem nova com o filtro
return sobelImg;
}
/********************************************************************************/
PPM imgBinaria(PPM img){
   int x, y,limiar=100;

		for (x = 0; x < img.l; x++) {
			for (y = 0; y < img.c; y++) {

				if(img.pixel[x][y].r >= limiar){
                    img.pixel[x][y].r = 255;
                    img.pixel[x][y].g = 255;
                    img.pixel[x][y].b = 255;
				}else{
                    img.pixel[x][y].r = 0;
                    img.pixel[x][y].g = 0;
                    img.pixel[x][y].b = 0;

				}

            }
		}

printf("Imagem binarizada!\n");
Sleep(4000);
return img;
}
//menu ainda para melhorar funcionalidades... :D
/********************************************************************************/
void menu(char *url,PPM imgMenu, char *nomeOut){

 PPM img=imgMenu;
 char sOrN[1],path[100]={0};
 int op=-1;
do{
    if(op==0) return 0;
    system("cls");
    printf("====================UNIVERSIDADE FEDERAL DO RIO GRANDE DO NORTE==============\n");
    printf("====================DISCIPLINA: ITP E PTP====================================\n");
    printf("====================PROFESSOR: ANDRE SOLINO==================================\n");
    printf("====================AUTORES: DAVID FRANKLIN E THALES MARCEL==================\n");
    printf("*****************************************************************************\n");
    printf("    ******                         **      ** **               **          **\n");
    printf("   /*////**                       /**     /**//               /**         /**\n");
    printf("   /*   /**   *****  **********   /**     /** ** *******      /**  ****** /**\n");
    printf("   /******   **///**//**//**//**  //**    ** /**//**///**  ****** **////**/**\n");
    printf("   /*//// **/******* /** /** /**   //**  **  /** /**  /** **///**/**   /**/**\n");
    printf("   /*    /**/**////  /** /** /**    //****   /** /**  /**/**  /**/**   /**// \n");
    printf("   /******* //****** *** /** /**     //**    /** ***  /**//******//******  **\n");
    printf("   ///////   ////// ///  //  //       //     // ///   //  //////  //////  // \n");
    printf("*****************************************************************************\n");
    printf("====================PROGRAMA PARA PROCESSAMENTO DE IMAGENS!==================\n\n");

    printf("Local da Imagem : %s\n\n",url);
    printf(" O que desejas ? Digite a opcao:\n\n");
    printf(" 1 - Transformar a imagem colorida para tons de cinza\n");
    printf(" 2 - Aplicar filtro gaussiano\n");
    printf(" 3 - Aplicar Filtragem com sobel (realcar as arestas)\n");
    printf(" 4 - Binarizar imagem\n");
    printf(" 5 - Salvar imagem\n");
    printf(" 6 - Recarregar imagem Original\n");
    printf(" 0 - Sair do programa\n");
    printf(" Digite opcao desejada:");
    scanf("%d",&op);
    printf("\n");
    switch(op){
        case 1 :
        img = converteCinza(imgMenu);
        menu(url,img,nomeOut);
        break;
        case 2 :
        img = filtroGaussiano(imgMenu);
        menu(url,img,nomeOut);
        break;
        case 3 :
        img = filtroSobel(imgMenu);
        menu(url,img,nomeOut);
        break;
        case 4 :
        img = imgBinaria(imgMenu);
        menu(url,img,nomeOut);
        break;
        case 5 :
        printf("Deseja Salvar Imagem ? S (sim) ou N (nao) : ");
        scanf(" %c",&sOrN);
        	if(sOrN[0] == 'S') {
                if(sizeof(img)<1){
                    printf("Imagem não alterada!\n");
                    break;
                }else{
        	    strcat(path,"Img/");
        	    strcat(path,nomeOut);
                printf("Salvando...%s\n",path);
                criaPPM(img,path);
                menu(url,img,nomeOut);
                break;
                }
            }
            else if (sOrN[0] == 'N') {
                printf("\nNenhuma imagem foi salvada\n");
                menu(url,img,nomeOut);
                break;
            }

            else {
                printf(stderr, "ERRO: Necessario fazer uma das escolhas disponiveis!\n");
                Sleep(3000);
                menu(url,img,nomeOut);
                break;
            }
            case 6 :
            img = carregaPPM(url);
            menu(url,img,nomeOut);
            break;

    }

}while(op);
printf("\nTxau... :D\n");
}



