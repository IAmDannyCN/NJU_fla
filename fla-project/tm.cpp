#include "tm.h"

using namespace std;

static void CerrSyntaxError(bool verbose, int lineno, string message) {
    if(!verbose) {
        cerr << "syntax error" << endl;
    } else {
        cerr << "syntax error on line " << lineno << ": " << message << endl;
    }
}

TM::TM(ifstream& infile, bool verbose) {
    string line;
    int lineno = 0;
    while(getline(infile, line)) {
        lineno ++;
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
                    definition_record |= Q_OCCUR;
                    if(!this->Q.empty()) {
                        // Second #Q
                        CerrSyntaxError(verbose, lineno, "Redefinition of #Q.");
                        exit(1);
                    }
                    sv.sv_before({'{'}, {' ', '\t'}, true);
                    bool first = true;
                    while(sv[-1] == '{' || sv[-1] == ',') {
                        string tmp = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string();
                        if(this->Q.count(tmp)) {
                            // Redefine of Q
                            CerrSyntaxError(verbose, lineno, "Redefinition of status `"+tmp+"`.");
                            exit(1);
                        }
                        if(sv[-1] == '}' && first && tmp.size() == 0) {
                            continue;
                        }
                        if(!Check_Nonblank(tmp) || tmp.size() == 0) {
                            // Contains blank char or is blank itthis
                            CerrSyntaxError(verbose, lineno, "Status `"+tmp+"` is empty or contains illegal characters.");
                            exit(1);
                        }
                        this->Q.insert(tmp);
                        first = false;
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        CerrSyntaxError(verbose, lineno, "Illegal characters left after `}`.");
                        exit(1);
                    }
                } else if (name == "S") {
                    definition_record |= S_OCCUR;
                    if(!this->S.empty()) {
                        // Second #S
                        CerrSyntaxError(verbose, lineno, "Redefinition of #S.");
                        exit(1);
                    }
                    sv.sv_before({'{'}, {' ', '\t'}, true);
                    bool first = true;
                    while(sv[-1] == '{' || sv[-1] == ',') {
                        string tmp = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string();
                        if(tmp.size() > 0 && this->S.count(tmp[0])) {
                            // Redefine of S
                            CerrSyntaxError(verbose, lineno, "Redefinition of input symbol `"+tmp+"`.");
                            exit(1);
                        }
                        if(sv[-1] == '}' && first && tmp.size() == 0) {
                            continue;
                        }
                        if(!Check_Printable(tmp)) {
                            // Contains blank char or is blank itthis
                            CerrSyntaxError(verbose, lineno, "Redefinition of input symbol `"+tmp+"`.");
                            exit(1);
                        }
                        if(tmp.size() != 1) {
                            CerrSyntaxError(verbose, lineno, "Input symbol `"+tmp+"` has length not equal to 1.");
                            exit(1);
                        }
                        this->S.insert(tmp[0]);
                        first = false;
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        CerrSyntaxError(verbose, lineno, "Illegal characters left after `}`.");
                        exit(1);
                    }
                } else if (name == "G") {
                    definition_record |= G_OCCUR;
                    if(!this->G.empty()) {
                        // Second #G
                        CerrSyntaxError(verbose, lineno, "Redefinition of #G.");
                        exit(1);
                    }
                    sv.sv_before({'{'}, {' ', '\t'}, true);
                    bool first = true;
                    while(sv[-1] == '{' || sv[-1] == ',') {
                        string tmp = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string();
                        if(tmp.size() > 0 && this->G.count(tmp[0])) {
                            // Redefine of G
                            CerrSyntaxError(verbose, lineno, "Redefinition of tape symbol `"+tmp+"`.");
                            exit(1);
                        }
                        if(sv[-1] == '}' && first && tmp.size() == 0) {
                            continue;
                        }
                        if((!Check_Printable(tmp) && tmp != "_") || tmp.size() != 1) {
                            // Contains blank char or is blank itthis
                            CerrSyntaxError(verbose, lineno, "Tape symbol `"+tmp+"` contains illegal characters or has length inequal to 1.");
                            exit(1);
                        }
                        this->G.insert(tmp[0]);
                        first = false;
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        CerrSyntaxError(verbose, lineno, "Illegal characters left after `}`.");
                        exit(1);
                    }
                } else if (name == "q0") {
                    definition_record |= q0_OCCUR;
                    if(this->q0 != "") {
                        // Second #q0
                        CerrSyntaxError(verbose, lineno, "Redefinition of #q0.");
                        exit(1);
                    }
                    string tmp = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                    if(!this->Q.count(tmp)) {
                        CerrSyntaxError(verbose, lineno, "Undefined status `"+tmp+"`.");
                        exit(1);
                    }
                    this->q0 = tmp;
                } else if (name == "B") {
                    definition_record |= B_OCCUR;
                    if(this->B != 0) {
                        // Second #B
                        CerrSyntaxError(verbose, lineno, "Redefinition of #B.");
                        exit(1);
                    }
                    string tmp = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                    if(tmp.size() == 0 || this->G.count(tmp[0]) == 0) {
                        CerrSyntaxError(verbose, lineno, "Undefined symbol `"+tmp+"` as blank symbol.");
                        exit(1);
                    }
                    if(tmp != "_") {
                        CerrSyntaxError(verbose, lineno, "Blank symbol must be `_`.");
                        exit(1);
                    }
                    this->B = '_';
                } else if (name == "F") {
                    definition_record |= F_OCCUR;
                    if(!this->F.empty()) {
                        // Second #F
                        CerrSyntaxError(verbose, lineno, "Redefinition of #F.");
                        exit(1);
                    }
                    sv.sv_before({'{'}, {' ', '\t'}, true);
                    bool first = true;
                    while(sv[-1] == '{' || sv[-1] == ',') {
                        string tmp = sv.sv_before({',', '}'}, {' ', '\t'}, true).to_string();
                        if(this->F.count(tmp)) {
                            // Redefine of F
                            CerrSyntaxError(verbose, lineno, "Redefinition of final status `"+tmp+"`.");
                            exit(1);
                        }
                        if(sv[-1] == '}' && first && tmp.size() == 0) {
                            continue;
                        }
                        if(!this->Q.count(tmp)) {
                            CerrSyntaxError(verbose, lineno, "Undefined status `"+tmp+"`.");
                            exit(1);
                        }
                        this->F.insert(tmp);
                        first = false;
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        CerrSyntaxError(verbose, lineno, "Illegal characters left after `}`.");
                        exit(1);
                    }
                } else if (name == "N") {
                    definition_record |= N_OCCUR;
                    if(this->N != 0) {
                        // Second #N
                        CerrSyntaxError(verbose, lineno, "Redefinition of #N.");
                        exit(1);
                    }
                    string tmp = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                    int i_tmp = Check_Digit(tmp);
                    if(i_tmp == -1 || i_tmp == 0) {
                        CerrSyntaxError(verbose, lineno, "`"+tmp+"` is not a valid tape number.");
                        exit(1);
                    }
                    this->N = i_tmp;
                } else {
                    CerrSyntaxError(verbose, lineno, "Unrecognized identifier `"+name+"`.");
                    exit(1);
                }
            } catch (const SyntaxErrorException& e) {
                CerrSyntaxError(verbose, lineno, "Parser error. Probably missing critical characters.");
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
                    CerrSyntaxError(verbose, lineno, "Illegal characters left.");
                    exit(1);
                }
                if(!this->Q.count(str_q0)) {
                    CerrSyntaxError(verbose, lineno, "Undefined status `"+str_q0+"`.");
                    exit(1);
                }
                if(!this->Q.count(str_q1)) {
                    CerrSyntaxError(verbose, lineno, "Undefined status `"+str_q1+"`.");
                    exit(1);
                }
                if(str_Z0.size() != this->N) {
                    CerrSyntaxError(verbose, lineno, "`"+str_Z0+"` should contains #N symbols.");
                    exit(1);
                }
                if(str_Z1.size() != this->N) {
                    CerrSyntaxError(verbose, lineno, "`"+str_Z1+"` should contains #N symbols.");
                    exit(1);
                }
                for(char& c: str_Z0) {
                    if(c != '*' && c != this->B && !this->G.count(c)) {
                        CerrSyntaxError(verbose, lineno, "Undefined tape symbol `"+string(1, c)+"`.");
                        exit(1);
                    }
                }
                for(char& c: str_Z1) {
                    if(c != '*' && c != this->B && !this->G.count(c)) {
                        CerrSyntaxError(verbose, lineno, "Undefined tape symbol `"+string(1, c)+"`.");
                        exit(1);
                    }
                }
                for(char& c: str_dir) {
                    if(c != '*' && c != 'l' && c != 'r') {
                        CerrSyntaxError(verbose, lineno, "Direction symbol should be `l`, `r` or `*`.");
                        exit(1);
                    }
                }
                delta.push_back(TM_Delta(move(str_q0), move(str_Z0), move(str_Z1), move(str_dir), move(str_q1)));
            } catch (const SyntaxErrorException& e) {
                CerrSyntaxError(verbose, lineno, "Parser error. Probably missing critical characters.");
                exit(1);
            }
        }
    }
    if(definition_record == 0x7f) {
        return ;
    }
    if(!(definition_record & Q_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #Q.");
    }
    if(!(definition_record & S_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #S.");
    }
    if(!(definition_record & G_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #G.");
    }
    if(!(definition_record & q0_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #q0.");
    }
    if(!(definition_record & B_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #B.");
    }
    if(!(definition_record & F_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #F.");
    }
    if(!(definition_record & N_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #N.");
    }
    exit(1);
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
        // if(match.size() > 1) {
        //     cerr << "Multiple delta matching!" << endl;
        //     if(verbose) {
        //         for(auto &d: match) {
        //             d.get()._Out();
        //         }
        //     }
        //     exit(1);
        // }
        if(match.size() > 1) {
            auto match_backup(match);
            for(auto it = match.begin(); it != match.end(); ) {
                if(it->get().is_Z0_all_star()) {
                    it = match.erase(it);
                } else {
                    it ++;
                }
            }
            if(match.size() != 1) {
                cerr << "Multiple delta matching! (conflicting)" << endl;
                if(verbose) {
                    for(auto &d: match_backup) {
                        d.get()._Out();
                    }
                }
                exit(1);
            }
        }

        const TM_Delta& rule = match[0];
        state = rule.q1;
        for(int i = 0; i < N; i++) {
            if(rule.Z1[i] != '*') {
                *head[i] = rule.Z1[i];
            }
            if(rule.dir[i] == 'l') {
                if(head[i] == tape[i].begin()) {
                    tape[i].push_front(this->B);
                    head[i] = tape[i].begin();
                } else {
                    head[i] --;
                }
                hidx[i] --;
                lidx[i] = min(lidx[i], hidx[i]);
            } else if (rule.dir[i] == 'r') {
                if(head[i] == prev(tape[i].end())) {
                    tape[i].push_back(this->B);
                    head[i] = prev(tape[i].end());
                } else {
                    head[i] ++;
                }
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