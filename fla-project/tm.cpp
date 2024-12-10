#include "tm.h"

using namespace std;

TM::TM(ifstream& infile) {
    string line;
    while(getline(infile, line)) {
        // Get rid of comments
        size_t comma_pos = line.find(';');
        if(comma_pos != string::npos) {
            line.erase(comma_pos);
        }
        // empty
        int lpos = 0, rpos = line.size();
        while(lpos < rpos && (line[lpos] == ' ' || line[lpos] == '\t')) {
            lpos ++;
        }
        while(rpos > lpos && (line[rpos - 1] == ' ' || line[rpos - 1] == '\t')) {
            rpos --;
        }
        if(rpos == lpos) {
            continue;
        }
        line.resize(rpos);
        // is_delta
        line += '\n';
        rpos ++;
        if(line[lpos] == '#') {
            StringView sv(line, lpos + 1, rpos);
            try {
                string name = sv.sv_before({'='}, {' ', '\t'}, true).to_string();
                if(name == "Q") {
                    if(!this->Q.empty()) {
                        // Second #Q
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    sv.sv_before({'{'}, {' ', '\t'}, true);
                    while(sv[-1] == '{' || sv[-1] == ',') {
                        string tmp = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string();
                        if(this->Q.count(tmp)) {
                            // Redefine of Q
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                        if(!Check_Nonblank(tmp) || tmp.size() == 0) {
                            // Contains blank char or is blank itthis
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                        this->Q.insert(tmp);
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                } else if (name == "S") {
                    if(!this->S.empty()) {
                        // Second #S
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    sv.sv_before({'{'}, {' ', '\t'}, true);
                    while(sv[-1] == '{' || sv[-1] == ',') {
                        string tmp = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string();
                        if(this->S.count(tmp[0])) {
                            // Redefine of S
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                        if(!Check_Printable(tmp) || tmp.size() != 1) {
                            // Contains blank char or is blank itthis
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                        this->S.insert(tmp[0]);
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                } else if (name == "G") {
                    if(!this->G.empty()) {
                        // Second #G
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    sv.sv_before({'{'}, {' ', '\t'}, true);
                    while(sv[-1] == '{' || sv[-1] == ',') {
                        string tmp = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string();
                        if(this->G.count(tmp[0])) {
                            // Redefine of G
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                        if((!Check_Printable(tmp) && tmp != "_") || tmp.size() != 1) {
                            // Contains blank char or is blank itthis
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                        this->G.insert(tmp[0]);
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                } else if (name == "q0") {
                    if(this->q0 != "") {
                        // Second #q0
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    string tmp = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                    if(!this->Q.count(tmp)) {
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    this->q0 = tmp;
                } else if (name == "B") {
                    if(this->B != 0) {
                        // Second #B
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    string tmp = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                    if(tmp != "_") {
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    this->B = '_';
                } else if (name == "F") {
                    if(!this->F.empty()) {
                        // Second #F
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    sv.sv_before({'{'}, {' ', '\t'}, true);
                    while(sv[-1] == '{' || sv[-1] == ',') {
                        string tmp = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string();
                        if(this->F.count(tmp)) {
                            // Redefine of F
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                        if(!this->Q.count(tmp)) {
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                        this->F.insert(tmp);
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                } else if (name == "N") {
                    if(this->N != 0) {
                        // Second #N
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    string tmp = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                    int i_tmp = Check_Digit(tmp);
                    if(i_tmp == -1 || i_tmp == 0) {
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    this->N = i_tmp;
                } else {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
            } catch (const SyntaxErrorException& e) {
                cerr << "syntax error" << endl;
                exit(1);
            }
        } else {
            // delta
            StringView sv(line, lpos, rpos);
            try {
                string str_q0 = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_Z0 = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_Z1 = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_dir = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_q1 = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                if(sv.size() != 0) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                if(!this->Q.count(str_q0) || !this->Q.count(str_q1)) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                if(str_Z0.size() != this->N || str_Z1.size() != this->N) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                for(char& c: str_Z0) {
                    if(c != '*' && c != this->B && !this->G.count(c)) {
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                }
                for(char& c: str_Z1) {
                    if(c != '*' && c != this->B && !this->G.count(c)) {
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                }
                for(char& c: str_dir) {
                    if(c != '*' && c != 'l' && c != 'r') {
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                }
                delta.push_back(TM_Delta(move(str_q0), move(str_Z0), move(str_Z1), move(str_dir), move(str_q1)));
            } catch (const SyntaxErrorException& e) {
                cerr << "syntax error" << endl;
                exit(1);
            }
        }
    }
}

bool TM::run(const string& input, bool verbose) {
    for(int i = 0; i < (int)input.size(); i++) {
        char c = input[i];
        if(!S.count(c)) {
            if(!verbose) {
                cerr << "illegal input" << endl;
            } else {
                cerr << "Input: " << input << endl;
                cerr << "==================== ERR ====================" << endl;
                cerr << "error: '" << c << "' was not declared in the set of input symbols" << endl;
                cerr << "Input: " << input << endl;
                for(int j = 1; j <= i + 7; j++) {
                    cerr << " ";
                }
                cerr << "^" << endl;
                cerr << "==================== END ====================" << endl;
            }
            
            exit(1);
        }
    }
    if(verbose) {
        cout << "Input: " << input << endl;
        cout << "==================== RUN ====================" << endl;
    }

    state = q0;

    tape.clear();
    head.clear();
    lidx.clear();
    hidx.clear();
    lidx.resize(N, 0);
    hidx.resize(N, 0);

    deque<char> first_tape, other_tape;
    for(char c: input) {
        first_tape.push_back(c);
    }
    other_tape.push_back(this->B);
    if(input.size() == 0) {
        first_tape.push_back(this->B);
    }
    tape.push_back(first_tape);
    head.push_back(tape[0].begin());
    for(int i = 1; i <= (this->N - 1); i++) {
        tape.push_back(other_tape);
        head.push_back(tape[i].begin());
    }

    bool accept = false;

    int step_cnt = 0;
    while(true) {
        if(verbose) {
            cout << "Step   : " << step_cnt << endl;
            cout << "State  : " << state << endl;
            for(int i = 0; i < N; i++) {
                // Construct vinfo
                vector<TM_Verbose> vinfo;
                bool hav_start = false;
                int cur_idx = lidx[i];
                for(char &c: tape[i]) {
                    if(cur_idx == hidx[i] || c != '_') {
                        hav_start = true;
                    }
                    if(!hav_start) {
                        cur_idx ++;
                        continue;
                    }
                    TM_Verbose cur_info(abs(cur_idx), c, cur_idx == hidx[i]);
                    // cout << ">> PUSHED: " << cur_info.idx << " " << cur_info.content << " " << cur_info.ishead << endl;
                    vinfo.push_back(cur_info);
                    cur_idx ++;
                }
                for(int j = vinfo.size() - 1; j >= 0; j--) {
                    if(vinfo[j].ishead || vinfo[j].content != '_') {
                        break;
                    }
                    vinfo.pop_back();
                }

                // Print vinfo
                cout << "Index" << i << " : ";
                for(int j = 0; j < (int)vinfo.size(); j++) {
                    cout << vinfo[j].idx;
                    if(j == vinfo.size() - 1) {
                        cout << endl;
                    } else {
                        cout << " ";
                    }
                }
                cout << "Tape" << i << "  : ";
                for(int j = 0; j < (int)vinfo.size(); j++) {
                    cout << vinfo[j].content;
                    if(j == vinfo.size() - 1) {
                        cout << endl;
                    } else {
                        for(int k = 1; k <= cnt_digits(vinfo[j].idx); k++) {
                            cout << " ";
                        }
                    }
                }
                cout << "Head" << i << "  : ";
                for(int j = 0; j < (int)vinfo.size(); j++) {
                    if(vinfo[j].ishead) {
                        cout << "^";
                    } else {
                        cout << " ";
                    }
                    if(j == vinfo.size() - 1) {
                        cout << endl;
                    } else {
                        for(int k = 1; k <= cnt_digits(vinfo[j].idx); k++) {
                            cout << " ";
                        }
                    }
                }
            }
            cout << "---------------------------------------------" << endl;
        }

        step_cnt ++;
        if(F.count(state)) {
            accept = true;
        }

        vector<reference_wrapper<const TM_Delta>> match;
        for(auto &rule: delta) {
            if(rule.q0 != state) {
                goto _FAIL;
            }
            for(int i = 0; i < N; i++) {
                if(rule.Z0[i] == '*') {
                    if(*head[i] == '_') {
                        goto _FAIL;
                    }
                } else if (rule.Z0[i] != *head[i]) {
                    goto _FAIL;
                }
            }
            match.push_back(rule);
        _FAIL:
            continue;
        }

        if(match.size() == 0) {
            break;
        }
        assert(match.size() == 1);

        const TM_Delta& rule = match[0];
        state = rule.q1;
        for(int i = 0; i < N; i++) {
            if(rule.Z1[i] != '*') {
                *head[i] = rule.Z1[i];
            }
            if(rule.dir[i] == 'l') {
                if(head[i] == tape[i].begin()) {
                    tape[i].push_front(this->B);
                }
                head[i] --;
                hidx[i] --;
                lidx[i] = min(lidx[i], hidx[i]);
            } else if (rule.dir[i] == 'r') {
                if(head[i] == prev(tape[i].end())) {
                    tape[i].push_back(this->B);
                }
                head[i] ++;
                hidx[i] ++;
            }
        }
    }

    return accept;
}

void TM::Output(bool verbose) {
    if(!verbose) {
        printDeque(tape[0]);
    } else {
        cout << "Result: ";
        printDeque(tape[0]);
        cout << "==================== END ====================" << endl;
    }
}