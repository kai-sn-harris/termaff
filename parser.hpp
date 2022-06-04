#pragma once

// interpret input
#include <vector>
#include <string>
#include <iostream>

enum Tok_Type {
    t_VAR, t_NUM, t_ADD, t_SUB, t_MUL, t_DIV, t_CARROT, t_LPAREN, t_RPAREN
};

enum Node_Type {
    n_BINOP, n_NUM
};

typedef struct Token {
    Tok_Type type;
    char value;
} Token;

// AST data structure
struct Node {
    Node_Type type;
    union {
        struct {
            int value;
        } num;
        struct {
            Node *left, *right;
            char op;
        } binop;
    } node;
};

// generates stream of tokens from input std::string and sets the output to the tokens variable in the implementation file
void interpret_function(std::string input);
// parses tokens generated from interpret_function(std::string input);
Node parse_function(std::string input, int var);

// recursively visits nodes in the AST and does maths to get an answer
float visit(Node node);
// calls visit function and does some cleaning up afterwards
float calc(std::string input, int var);