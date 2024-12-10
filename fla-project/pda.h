#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <stack>
#include <set>
#include <vector>
#include <cassert>

#include "sv.h"

using namespace std;

class PDA_Delta {
public:
    PDA_Delta(string _q0, char _in, char _Z0, string _q1, string _Z1) :
        q0(_q0), in(_in), Z0(_Z0), q1(_q1), Z1(_Z1) {}

    string q0   {};
    string q1   {};
    char in     {};
    char Z0     {};
    string Z1   {};

    void _Out() {
        cout << q0 << " " << in << " " << Z0 << " " << q1 << " " << Z1 << endl;
    }
};

class PDA {
private:
    // PDA parameters
    set<string> Q   {};
    set<char> S   {};
    set<char> G     {};
    string q0       {};
    char z0         {};
    set<string> F   {};
    vector<PDA_Delta> delta    {};

    // Runtime environment
    string state    {};
    stack<char> st  {};
    
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
    void printStack(stack<char>& s) {
        stack<char> tempStack = s;
        while (!tempStack.empty()) {
            cout << tempStack.top();
            tempStack.pop();
        }
        cout << endl;
    }

public:
    PDA(ifstream& infile);
    bool run(const string& input);
};