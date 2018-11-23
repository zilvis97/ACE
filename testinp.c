#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {

    FILE *fp = fopen("labas.txt", "r");
    //FILE *write = fopen("rasom.txt", "w+");
    char buff[4096][17];
    long long int mem[4096];
    int buff_index = 0, i;

    while(!feof(fp)) {
        long long int inp;
        char temp[17];
        fscanf(fp, "%lli", &inp);
        mem[buff_index] = inp;
        buff_index++;
    }

    for(i = 0; i < buff_index; i++) {
        printf("%d --- %lli\n", i, mem[i]);
    }

    printf("%c", (char) 1);

    fclose(fp);
}
