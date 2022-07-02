en un arreglo el acceso es constante, estan guaraddo en zona de memoria con direccion consecutiva
se puede acceder a cualquier elemento en un solo paso.
no es facil hacer crecer un arreglo.
si la cantidad de variable van a ir creciendo y no se sabe exactamente el tamaño se usa listas.
en c no hay lista.

declaracion de un arreglo
int a[5]; afrreglo de 5 elementos empieza desde el 0 al 4

inicializacion de arreglo:
a[1] = 10; accedo al 2 elemento y le asigno 10 
 
lectura
x = a[1];

inicializar todos los elementos del arreglo en 0:
#define N 5;
int a[N];
int i;
i=0;
while (imenor a N){
    a[i]=0;
    i=i+1;
};

ESTRUCTURAS (STRUCT)
conjuntos de variables de diferentes tipos.. es como una tupla
donde cada elemnto tiene un nombre
ejemplo
struct par{
    int fst;
    int snd;
};

declaracion:
struct par dupla;
dupla.fst =3;
dupla.snd=2;
para acceder a los campos de la estructura se hace con un punto

lectura:
x= dupla.fst;
y=dupla.snd;

para sacarse de encima el struct para declarar se hace con typedef

typedef struct{
    int fst;
    int snd;
} par;

luego para declararar
par dupla;
dupla.fst=3;

#include stdio.h

int main(void){
    printf("holamundo!!!/n");
    return 0;
}
