#include "t1.h"

#include<stdio.h>
#include<stdlib.h>


unsigned long long sumaVecs(unsigned long long vx, unsigned long long vy) {
    
    unsigned long long mask = 0x00ff00ff00ff00ff, total = 0;

    unsigned long long aux1 = vx & mask;
    unsigned long long aux2 = vy & mask;

    total |= (aux1+aux2)&mask;

    mask<<=8;

    aux1 = vx & mask;
    aux2 = vy & mask;

    total |= (aux1+aux2)&mask;

    return total;

}

