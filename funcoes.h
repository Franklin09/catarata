#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED
#include "setup.h"
int **alocaMatriz(int l, int c);
PPM ppmLimpa(int c, int l);
PPM carregaPPM(char * url);
void criaPPM(PPM novaImg,char *url);
PPM filtroSobel(PPM img);
PPM converteCinza(PPM img);
PPM filtroGaussiano(PPM img);
PPM imgBinaria(PPM img);
void menu(char *url,PPM imgMenu, char *nomeOut);
#endif // FUNCOES_H_INCLUDED
