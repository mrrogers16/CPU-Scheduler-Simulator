#include <stdio.h>

int main(void)  
{
    int iMax;
    int iX, iY;

    iX = 10;
    iY = iX/0.5;

    iMax = iX > iY ? iX : iY;

    printf("%d", iMax);
    return 0;
}