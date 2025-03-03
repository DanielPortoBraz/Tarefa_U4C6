#include "pico/stdlib.h"

#define NUM_PIXELS 25

// Matriz com os padrões de 0 a 9
// Os desenhos seguem a ordem de "zigue-zague" da matriz de LEDs 
bool nums [10][NUM_PIXELS] = {{
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, // 0
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
},
{
    0, 0, 1, 0, 0, 
    0, 0, 1, 1, 0, 
    0, 0, 1, 0, 0, // 1
    0, 0, 1, 0, 0, 
    0, 1, 1, 1, 0
},
{
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 1, 0, 0, //2
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
},
{
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 1, 1, 0, //3
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
},
{
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, //4
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0
},
{
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, //5
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
},
{
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, //6
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
},
{
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, //7
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0
},
{
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, //8
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
},
{
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, //9
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
}
};