#ifndef SETUP_H_INCLUDED
#define SETUP_H_INCLUDED
typedef struct {
    int r, g, b;
}PIXELS;

typedef struct {
int c; // número de colunas na imagem
int l; // número de linhas na imagem
char tipo[2]; //tipo do ppm
unsigned int corMax; // valor máximo para cada pixel
PIXELS **pixel; // variável para armazenar os pixels da imagem (matriz)
}PPM;

#endif // SETUP_H_INCLUDED
