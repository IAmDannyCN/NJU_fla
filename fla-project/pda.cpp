#include "pda.h"

using namespace std;

PDA::PDA(ifstream& infile) {
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
                        if(!Check_Printable(tmp) || tmp.size() != 1) {
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
                } else if (name == "z0") {
                    if(this->z0 != 0) {
                        // Second #z0
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    string tmp = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                    if(tmp.size() != 1) {
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    if(this->S.count(tmp[0])) {
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
                    this->z0 = tmp[0];
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
                            cerr << "illegal input" << endl;
                            exit(1);
                        }
                        this->F.insert(tmp);
                    }
                    if(sv.sv_before({'\n'}, {' ', '\t'}, true).to_string().size() != 0) {
                        // Unrecognized character left
                        cerr << "syntax error" << endl;
                        exit(1);
                    }
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
                string str_in = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_Z0 = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_q1 = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_Z1 = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                if(sv.size() != 0) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                if(!this->Q.count(str_q0) || !this->Q.count(str_q1)) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                if(str_in.size() != 1 || str_Z0.size() != 1) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                if((!this->S.count(str_in[0]) && str_in[0] != '_')) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                if(!this->G.count(str_Z0[0])) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                if(str_Z1.size() == 0) {
                    cerr << "syntax error" << endl;
                    exit(1);
                }
                if(str_Z1 != "_") {
                    for(char &z: str_Z1) {
                        if(!this->G.count(z)) {
                            cerr << "syntax error" << endl;
                            exit(1);
                        }
                    }
                }
                delta.push_back(PDA_Delta(move(str_q0), str_in[0], str_Z0[0], move(str_q1), move(str_Z1)));
            } catch (const SyntaxErrorException& e) {
                cerr << "syntax error" << endl;
                exit(1);
            }
        }
    }
}

bool PDA::run(const string& input) {
    for(const char &c: input) {
        if(!S.count(c)) {
            cerr << "illegal input" << endl;
            exit(1);
        }
    }

    state = q0;
    while(!st.empty()) st.pop();
    st.push(z0);

    StringView sv(input);

    while(true) {
        char cur = sv.size() ? sv[0] : '\x00';

        // cout << ">> state: " << state << ", input: " << cur << ", stack = ";
        // printStack(st);

        vector<reference_wrapper<const PDA_Delta>> match;
        for(auto &rule: delta) {
            if( rule.q0 == state &&
                (cur == rule.in || (rule.in == '_' && cur == '\x00')) &&
                ((!st.empty() && st.top() == rule.Z0) || rule.Z0 == '_')) {
                    match.push_back(rule);
                    // rule._Out();
                }
        }

        if(match.size() == 0) {
            break;
        }
        assert(match.size() == 1);
        
        const PDA_Delta& rule = match[0];
        if(rule.in != '_') {
            sv.cut_head(1);
        }
        if(rule.Z0 != '_') {
            st.pop();
        }
        state = rule.q1;
        for(int i = rule.Z1.size() - 1; i >= 0; i--) {
            if(rule.Z1[i] != '_') {
                st.push(rule.Z1[i]);
            }
        }

        // cout << "== state: " << state << ", stack = ";
        // printStack(st);
    }

    return F.count(state);
}