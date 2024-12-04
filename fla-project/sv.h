#pragma once

#include <string>
#include <memory>
#include <stdexcept>

using namespace std;

class SyntaxErrorException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Syntax Error.";
    }
};

class StringView {
private:
    // shared_ptr<string> str_ptr {};
    const string* str_ptr {};
    size_t lpos {}, rpos {};

public:
    StringView(const string &str, size_t lpos=-1, size_t rpos=-1) {
        // str_ptr = make_shared<string>(str);
        str_ptr = &str;
        this->lpos = lpos == -1 ? 0 : lpos;
        this->rpos = rpos == -1 ? str.size() : rpos;
    }
    void set_head(size_t pos) {
        lpos = min(pos, rpos);
    }
    StringView sv_before(set<char> stop, set<char> ignore, bool cut=false) {
        size_t start_pos = lpos;
        while(start_pos < rpos) {
            char ch = (*str_ptr)[start_pos];
            if(stop.count(ch)) {
                break;
            }
            if(ignore.count(ch)) {
                start_pos++;
                continue;
            }
            start_pos++;
        }
        if(start_pos == rpos) {
            throw SyntaxErrorException();
        }

        size_t resl = lpos, resr = start_pos;
        if(cut) {
            set_head(start_pos + 1);
        }
        
        while(resl < resr && ignore.count((*str_ptr)[resl])) {
            resl ++;
        }
        while(resl < resr && ignore.count((*str_ptr)[resr - 1])) {
            resr --;
        }
        // cout << "> " << *str_ptr << "[" << resl << ", " << resr << "] (" << lpos << ", " << rpos << ") -> " << to_string() << endl; 
        return StringView(*str_ptr, resl, resr);
    }
    string to_string() const {
        return (*str_ptr).substr(lpos, rpos - lpos);
    }
    size_t size() const {
        return rpos - lpos;
    }
    char operator [](size_t index) const {
        size_t biased_index = index + lpos;
        if(biased_index < 0 || biased_index >= (*str_ptr).size()) {
            throw std::out_of_range("Index out of range for StringView");
        }
        return (*str_ptr)[biased_index];
    }
    void cut_head(size_t len) {
        set_head(lpos + len);
    }
};