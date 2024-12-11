#include "pda.h"

using namespace std;

static void CerrSyntaxError(bool verbose, int lineno, string message) {
    if(!verbose) {
        cerr << "syntax error" << endl;
    } else {
        cerr << "syntax error on line " << lineno << ": " << message << endl;
    }
}

PDA::PDA(ifstream& infile, bool verbose) {
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
                        if(this->S.count(tmp[0])) {
                            // Redefine of S
                            CerrSyntaxError(verbose, lineno, "Redefinition of input symbol `"+tmp+"`.");
                            exit(1);
                        }
                        if(sv[-1] == '}' && first && tmp.size() == 0) {
                            continue;
                        }
                        if(!Check_Printable(tmp) || tmp.size() != 1) {
                            // Contains blank char or is blank itthis
                            CerrSyntaxError(verbose, lineno, "Input symbol `"+tmp+"` contains illegal characters or has length inequal to 1.");
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
                        if(this->G.count(tmp[0])) {
                            // Redefine of G
                            CerrSyntaxError(verbose, lineno, "Redefinition of stack symbol `"+tmp+"`.");
                            exit(1);
                        }
                        if(sv[-1] == '}' && first && tmp.size() == 0) {
                            continue;
                        }
                        if(!Check_Printable(tmp) || tmp.size() != 1) {
                            // Contains blank char or is blank itthis
                            CerrSyntaxError(verbose, lineno, "Stack symbol `"+tmp+"` contains illegal characters or has length inequal to 1.");
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
                } else if (name == "z0") {
                    definition_record |= z0_OCCUR;
                    if(this->z0 != 0) {
                        // Second #z0
                        CerrSyntaxError(verbose, lineno, "Redefinition of #z0.");
                        exit(1);
                    }
                    string tmp = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
                    if(tmp.size() != 1) {
                        CerrSyntaxError(verbose, lineno, "Stack symbol cannot contain more than one character.");
                        exit(1);
                    }
                    if(this->S.count(tmp[0])) {
                        CerrSyntaxError(verbose, lineno, "Undefined stack symbol `"+tmp+"`.");
                        exit(1);
                    }
                    this->z0 = tmp[0];
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
                string str_in = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_Z0 = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_q1 = sv.sv_before({' ', '\t'}, {}, true).to_string();
                string str_Z1 = sv.sv_before({'\n'}, {' ', '\t'}, true).to_string();
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
                if(str_in.size() != 1) {
                    CerrSyntaxError(verbose, lineno, "Input lengths must be 1.");
                    exit(1);
                }
                if(str_Z0.size() != 1) {
                    CerrSyntaxError(verbose, lineno, "Can only read one stack symbol.");
                    exit(1);
                }
                if((!this->S.count(str_in[0]) && str_in[0] != '_')) {
                    CerrSyntaxError(verbose, lineno, "Undefined input symbol `"+str_in+"`.");
                    exit(1);
                }
                if(!this->G.count(str_Z0[0])) {
                    CerrSyntaxError(verbose, lineno, "Undefined stack symbol `"+str_Z0+"`.");
                    exit(1);
                }
                if(str_Z1.size() == 0) {
                    CerrSyntaxError(verbose, lineno, "Use `_` to represent empty stack symbol serial.");
                    exit(1);
                }
                if(str_Z1 != "_") {
                    for(char &z: str_Z1) {
                        if(!this->G.count(z)) {
                            CerrSyntaxError(verbose, lineno, "Undefined stack symbol `"+string(1, z)+"`.");
                            exit(1);
                        }
                    }
                }
                delta.push_back(PDA_Delta(move(str_q0), str_in[0], str_Z0[0], move(str_q1), move(str_Z1)));
            } catch (const SyntaxErrorException& e) {
                CerrSyntaxError(verbose, lineno, "syntax error");
                exit(1);
            }
        }
    }
    if(definition_record == 0x3f) {
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
    if(!(definition_record & z0_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #z0.");
    }
    if(!(definition_record & F_OCCUR)) {
        CerrSyntaxError(verbose, lineno, "Missing the definition for #F.");
    }
    exit(1);
}

bool PDA::run(const string& input, bool verbose) {
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
    while(!st.empty()) st.pop();
    st.push(z0);

    StringView sv(input);

    while(true) {
        char cur = sv.size() ? sv[0] : '\x00';

        if(verbose) {
            cout << "State: " << state << endl;
            cout << "Input: " << input.substr(0, input.size() - sv.size()) << '[' << cur << ']' << (sv.size() >= 2 ? sv.to_string().substr(1) : " ") << endl;
            cout << "Stack: " ; printStack(st);
            cout << "---------------------------------------------" << endl;
        }

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