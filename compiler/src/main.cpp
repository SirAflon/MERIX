#include "header/all.h"
#include <iostream>




int main(int argc, char* args[]){
    for(int i=0;i<argc;i++){
        std::cout << "arg";
        std::cout << i;
        std::cout << " = ";
        std::cout << args[i]<< std::endl;
    }
    std::cout << argc << std::endl;
}