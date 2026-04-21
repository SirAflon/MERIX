#include "lib/header/output.h"
#include <vector>
#include <cstring>

bool endsWith(const char* str, const char* suffix) {
    if (!str || !suffix) return false;
    size_t str_len = std::strlen(str);
    size_t suff_len = std::strlen(suffix);
    if (suff_len > str_len) return false;
    return std::strcmp(str + str_len - suff_len, suffix) == 0;
}


int main(int argc, char* args[]){
    if(argc <= 2){
        output::println(output::red,"ERROR: ",output::reset,"please provide a inputfile file path and the output path.");
        return 1;
    }
    char* outputfile = args[1];
    std::vector<char*> inputfiles;
    int curr = 2;
    while(curr < argc && endsWith(args[curr],".mrx")){
        inputfiles.push_back(args[curr]);
        curr++;
    }
    for(int i=0;i<inputfiles.size();i++){
        output::println(inputfiles[i]);
    }
}