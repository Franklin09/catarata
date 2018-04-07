#ifndef SETUP_H_INCLUDED
#define SETUP_H_INCLUDED
typedef struct {
    int r, g, b;
}PIXELS;

typedef struct {
int c; // n�mero de colunas na imagem
int l; // n�mero de linhas na imagem
char tipo[2]; //tipo do ppm
unsigned int corMax; // valor m�ximo para cada pixel
PIXELS **pixel; // vari�vel para armazenar os pixels da imagem (matriz)
}PPM;

#endif // SETUP_H_INCLUDED
