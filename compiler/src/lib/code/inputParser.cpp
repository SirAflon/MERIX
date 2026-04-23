#include "../header/inputParser.h"

bool endsWith(const char* str, const char* suffix) {
    if (!str || !suffix) 
        return false;
    size_t str_len = std::strlen(str);
    size_t suff_len = std::strlen(suffix);
    if (suff_len > str_len) 
        return false;
    return std::strcmp(str + str_len - suff_len, suffix) == 0;
}
bool toInt(const char* s,unsigned int& out){
    std::string_view sv(s);
    auto [ptr,ec] = std::from_chars(sv.data(),sv.data() + sv.size(),out);
    return ec == std::errc() && ptr == sv.data() + sv.size();
}
bool toFloat(const char* s,float& out){
    std::string_view sv(s);
    auto [ptr,ec] = std::from_chars(sv.data(),sv.data() + sv.size(),out);
    return ec == std::errc() && ptr == sv.data() + sv.size();    
}

parsedCommand parseInput(int argc,char* args[]){
    parsedCommand cmd;
    bool projFileUsed = false;
    char* projFilePath;
    int curr = 1;
    if(endsWith(args[curr],".merp")){
        projFileUsed = true;
        projFilePath = args[curr];
        curr++;
    }
    while(curr < argc){
        if(!projFileUsed&&endsWith(args[curr],".mrx")){
            cmd.inputFiles.push_back(args[curr]);
            curr++;
            continue;
        }
        const char* arg = args[curr];
        if(arg[0] != '-'){
            curr++;
            continue;
        }
        if(strcmp(arg,"-o")==0||strcmp(arg,"-output")==0){
            if(curr + 1 >= argc){
                output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"after the '-output'(shortened '-o') needs to come a fileName after.");
                exit(1);
            }
            if(args[curr+1][0] == '-'){
                output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"after the '-output'(shortened '-o') needs to come a fileName after.");
                exit(1); 
            }
            cmd.outputFile = args[++curr];
            continue;
        }
         if(strcmp(arg,"-mT")==0||strcmp(arg,"-maxThreads")==0){
            if(curr + 1 >= argc){
                output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"after the '-maxThreads'(shortened '-mT' needs to come a integer.");
                exit(1);
            }
            if(args[curr+1][0] == '-'){
                output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"after the '-maxThreads'(shortened '-mT') needs to come a integer.");
                exit(1); 
            }
            if(toInt(args[curr+1],cmd.flag.maxThreads)){
                output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"after the '-maxThreads'(shortened '-mT') needs to come a integer.");
                exit(1); 
            }
            curr += 2;
            continue;
        }
        if(strcmp(arg,"-showStatus")==0||strcmp(arg,"-sS")==0){
            cmd.flag.showStatus=true;
            curr++;
            continue;
        }
        if(strcmp(arg,"-ignorWarnings")==0||strcmp(arg,"-iW")==0){
            cmd.flag.ignorWarning=true;
            curr++;
            continue;
        }
        if(!cmd.flag.ignorWarning)
            output::println(output::UseColor(output::FG_YELLOW),"WARNING<MAIN>: ",output::UseColor(output::FG_DEFAULT),"the flag '",args[curr],"' is unknown.");
        curr++;
    }
    if(cmd.outputFile == nullptr){
        output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"please provide a valid output filepath.(using the '-output'('-o' for shorter) flags to declare it)");
        exit(1);
    }
    if(cmd.inputFiles.size()==0){
        output::println(output::UseColor(output::FG_RED),"ERROR<MAIN>: ",output::UseColor(output::FG_DEFAULT),"please provide a valid inputfile filepath. The ending must be a '.mrx' or '.merp'.");
        exit(1);
    }
    return cmd;
}