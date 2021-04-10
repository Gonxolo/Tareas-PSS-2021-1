#include "t1.h"

#include<stdio.h>
#include<stdlib.h>


unsigned long long sumaVecs(unsigned long long vx, unsigned long long vy) {
    
    unsigned long long mask = 0x00ff00ff00ff00ff;
    unsigned long long total = 0, aux1 = 0, aux2 = 0;

    aux1 = vx & mask;
    aux2 = vy & mask;
    
    // printf("Aux1: %llx\n", aux1);
    // printf("Aux2: %llx\n", aux2);
    // printf("Mask: %llx\n", mask);

    total |= (aux1+aux2)&mask;
    // printf("Total: %llx\n", total);
    mask<<=8;

    aux1 = 0;
    aux2 = 0;

    aux1 = vx & mask;
    aux2 = vy & mask;
    // printf("Aux1: %llx\n", aux1);
    // printf("Aux2: %llx\n", aux2);
    // printf("Mask: %llx\n", mask);

    total |= (aux1+aux2)&mask;
    // printf("Total: %llx\n", total);

    return total;

}

// int main(int argc, char *argv[]){

//     unsigned long long vx = 0x6500000000000000;
//     unsigned long long vy = 0x6400000000000000;

//     unsigned long long vz = sumaVecs(vx, vy);

//     printf("vz: %#llx\n", vz);
    

// }

// 0xff016701a1000003;
// 0x010903ff1f0000ff;

