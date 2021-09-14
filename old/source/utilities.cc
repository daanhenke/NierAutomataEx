#include "utilities.hh"

#include <vector>



unsigned int CalculateRGB(double ratio)
{
    int normalized = int(ratio * 256 * 6);

    int x = normalized % 256;

    int red = 0, grn = 0, blu = 0;
    switch(normalized / 256)
    {
    case 0: red = 255;      grn = x;        blu = 0;       break;//red
    case 1: red = 255 - x;  grn = 255;      blu = 0;       break;//yellow
    case 2: red = 0;        grn = 255;      blu = x;       break;//green
    case 3: red = 0;        grn = 255 - x;  blu = 255;     break;//cyan
    case 4: red = x;        grn = 0;        blu = 255;     break;//blue
    case 5: red = 255;      grn = 0;        blu = 255 - x; break;//magenta
    }

    return red + (grn << 8) + (blu << 16);
}