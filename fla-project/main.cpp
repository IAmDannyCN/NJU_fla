#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cassert>

#include "pda.h"
#include "tm.h"

using namespace std;

#define TYPE_NONE 0x0
#define TYPE_PDA  0x1
#define TYPE_TM   0x2

int main(int argc, char* argv[]){
    
    char* file_name;
    char* input;
    bool verbose = false;
    bool help = false;

    int input_arg_cnt = 0;
    for(int i = 1; i < argc; i++) {
        if(strlen(argv[i]) >= 1 && argv[i][0] == '-') {
            if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h")) {
                help = true;
            } else if (!strcmp(argv[i], "--verbose") || !strcmp(argv[i], "-v")) {
                verbose = true;
            } else {
                cerr << "Invalid argument `" << argv[i] << "`." << endl;
                return 1;
            }
        } else {
            if(input_arg_cnt == 0) {
                file_name = argv[i];
            } else if (input_arg_cnt == 1) {
                input = argv[i];
            }
            input_arg_cnt += 1;
        }
    }

    if(help == true) {
        cerr << "usage:  fla [-h|--help] <pda> <input>" << endl;
        cerr << "        fla [-v|--verbose] [-h|--help] <tm> <input>" << endl;
        return 0;
    }

    if(input_arg_cnt == 0) {
        cerr << "Missing critical parameter: <pda> or <tm>, <input>" << endl;
        return 1;
    } else if (input_arg_cnt == 1) {
        cerr << "Missing critical parameter: <input>" << endl;
        return 1;
    } else if (input_arg_cnt >= 3) {
        cerr << "Too many arguments." << endl;
        return 1;
    }

    int type = TYPE_NONE;
    int file_len = strlen(file_name);
    if(file_len >= 4 && !strcmp(file_name + file_len - 4, ".pda")) {
        type = TYPE_PDA;
    } else if (file_len >= 3 && !strcmp(file_name + file_len - 3, ".tm")) {
        type = TYPE_TM;
    }

    if(type == TYPE_NONE) {
        cerr << "Unrecognized suffix, only .pda and .tm are allowed." << endl;
        return 1;
    }
    
    ifstream infile(file_name);
    if(!infile) {
        cerr << "Cannot open file `" << file_name << "`." << endl;
        return 1;
    }

    if(type == TYPE_PDA) {
        cout << (PDA(infile).run(string(input), verbose) ? "true" : "false") << endl;
        if(verbose) {
            cout << "==================== END ====================" << endl;
        }
    } else if (type == TYPE_TM) {
        TM tm(infile);
        tm.run(string(input), verbose);
        tm.Output(verbose);
        // bool accept = tm.run(string(input), verbose);
        // if(accept) {
        //     tm.Output(verbose);
        // } else {
        //     cout << "illegal input" << endl;
        // }
    } else {
        assert(0 && "Should not reach here.");
    }

    return 0;
}