#pragma once

#include <string>
#include <vector>
#include <set>
#include <deque>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>

#include "sv.h"

using namespace std;

class TM_Delta {
public:
    TM_Delta(string _q0, string _Z0, string _Z1, string _dir, string _q1)
        : q0(_q0), Z0(_Z0), Z1(_Z1), dir(_dir), q1(_q1) {}
    string q0;
    string q1;
    string Z0;
    string Z1;
    string dir;

    void _Out() const {
        cout << q0 << " " << Z0 << " " << Z1 << " " << dir << " " << q1 << endl;
    }
    inline bool is_Z0_all_star() const {
        for(const char &c: Z0) {
            if(c != '*') {
                return false;
            }
        }
        return true;
    }
};

class TM_Verbose {
public:
    TM_Verbose(int _idx, char _content, bool _ishead)
        : idx(_idx), content(_content), ishead(_ishead) {}
    int idx;
    char content;
    bool ishead;
};

class TM {
private:
    // TM parameters
    set<string> Q   {};
    set<char> S     {};
    set<char> G     {};
    string q0       {};
    char B          {};
    set<string> F   {};
    int N           {};
    vector<TM_Delta> delta    {};

    int definition_record   {};
    const int Q_OCCUR = 0x1;
    const int S_OCCUR = 0x2;
    const int G_OCCUR = 0x4;
    const int q0_OCCUR = 0x8;
    const int B_OCCUR = 0x10;
    const int F_OCCUR = 0x20;
    const int N_OCCUR = 0x40;

    // Runtime environment
    string state                        {};
    vector<deque<char>> tape            {};
    vector<deque<char>::iterator> head  {};
    vector<int> lidx                    {}; // index for the left-most gird on each tape
    vector<int> hidx                    {}; // index for the head on each tape
    
    
    bool Check_Nonblank(char c) {
        if(c >= 'a' && c <= 'z') return true;
        if(c >= 'A' && c <= 'Z') return true;
        if(c >= '0' && c <= '9') return true;
        if(c == '_') return true;
        return false;
    }
    bool Check_Nonblank(string s) {
        for(char& c: s) {
            if(!Check_Nonblank(c)) {
                return false;
            }
        }
        return true;
    }
    bool Check_Printable(char c) {
        set<char> rule_out = {' ', ',', ';', '{', '}', '*', '_'};
        if(rule_out.count(c)) return false;
        return (c >= 32 && c <= 126);
    }
    bool Check_Printable(string s) {
        for(char& c: s) {
            if(!Check_Printable(c)) {
                return false;
            }
        }
        return true;
    }
    int Check_Digit(string s) {
        int ans = 0;
        for(char&c : s) {
            if(!isdigit(c)) {
                return -1;
            }
            ans = ans * 10 + c - '0';
        }
        return ans;
    }
    void printDeque(deque<char> dq, bool printBlank=true) {
        while(!dq.empty() && dq.front() == '_') dq.pop_front();
        while(!dq.empty() && dq.back() == '_') dq.pop_back();
        for(const auto& c : dq) {
            if(printBlank || c != '_') {
                cout << c;
            }
        }
        cout << endl;
    }
    int cnt_digits(int n) {
        if(n <= 9) return 1;
        if(n <= 99) return 2;
        if(n <= 999) return 3;
        if(n <= 9999) return 4;
        if(n <= 99999) return 5;
        if(n <= 999999) return 6;
        if(n <= 9999999) return 7;
        if(n <= 99999999) return 8;
        return 9;
    }
    void _Out() {
        cout << "Q: ";
        for(auto &s: Q) {
            cout << s << " ";
        } cout << endl;
        cout << "S: ";
        for(auto &s: S) {
            cout << s << " ";
        } cout << endl;
        cout << "G: ";
        for(auto &s: G) {
            cout << s << " ";
        } cout << endl;
        cout << "q0: " << q0 << endl;
        cout << "B: " << B << endl;
        cout << "F: ";
        for(auto &s: F) {
            cout << s << " ";
        } cout << endl;
        cout << "N: " << N << endl;
        cout << "delta:" << endl;
        for(auto &s: delta) {
            s._Out();
        }
    }


public:
    TM(ifstream& infile, bool verbose=false);
    bool run(const string& input, bool verbose=false);
    void Output(bool verbose=false);
};