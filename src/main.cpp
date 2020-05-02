#include <cstdio>

#include "main.h"
#include "interpreter.h"
#include "parser.h"

int main(){
    printf("%s\n",getStringInt().c_str());
    printf("%s\n", getStringParse().c_str());
    return 0;
}