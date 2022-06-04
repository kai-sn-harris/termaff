#include "parser.hpp"

Token curTok;
int tokPos = -1;
std::vector<Token> tokens;

void interpret_function(std::string input) {
    for(int i = 0; i < input.length(); i++) {
        if(std::isspace(input[i])) continue;
        if(std::isdigit(input[i])) {
            Token tok;
            tok.type = t_NUM;
            tok.value = input[i];
            tokens.push_back(tok);
        }
        if(std::isalpha(input[i])) {
            Token tok;
            tok.type = t_VAR;
            tok.value = input[i];
            tokens.push_back(tok);
        }
        if(input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/') {
            Token tok;
            tok.value = input[i];
            if(input[i]=='+') tok.type = t_ADD;
            if(input[i]=='-') tok.type = t_SUB;
            if(input[i]=='*') tok.type = t_MUL;
            if(input[i]=='/') tok.type = t_DIV;
            tokens.push_back(tok);
        }
        if(input[i] == '^') {
            Token tok;
            tok.type = t_CARROT;
            tok.value = '^';
            tokens.push_back(tok);
            
        }
        if(input[i] == '(') {
            Token tok;
            tok.type = t_LPAREN;
            tok.value = '(';
            tokens.push_back(tok);
        }
        if(input[i] == ')') {
            Token tok;
            tok.type = t_RPAREN;
            tok.value = ')';
            tokens.push_back(tok);
        }
    }
    // debug
    for(auto tok : tokens)
        std::cout << "Token(" << tok.type << ", '" << tok.value << "')" << std::endl;
}

Token getNextToken() {
    tokPos++;
    curTok = tokens[tokPos];
    return curTok;
}

void eat(Tok_Type type) {
    if(curTok.type == type)
        curTok = getNextToken();
    else {
        std::cout << "Expected type " << type << " but got type " << curTok.type << std::endl;
        exit(EXIT_FAILURE);
    }
}

// prototype so it can be used before definition
Node AddSubTerm(int var);

Node factor(int var) {
    if(curTok.type == t_VAR) {
        Node num;
        num.type = n_NUM;
        num.node.num.value = var;
        curTok = getNextToken();
        return num;
    } else if(curTok.type == t_NUM) {
        Node num;
        num.type = n_NUM;
        num.node.num.value = (int)(curTok.value)-48;
        curTok = getNextToken();
        return num;
    } else if(curTok.type == t_LPAREN) {
        eat(t_LPAREN);
        Node node = AddSubTerm(var);
        eat(t_RPAREN);          
        return node;
    } else {
        printf("expected num or var but u did some dumbass shit lmao %d\n", curTok.type);
        exit(EXIT_FAILURE);
    }
}

Node MulDivTerm(int var) {
    Node num = factor(var);

    while(curTok.type == t_MUL || curTok.type == t_DIV) {
        Node binop;
        binop.type = n_BINOP;
        binop.node.binop.op = curTok.type == t_MUL ? '*' : '/';
        curTok = getNextToken();
        
        Node *left = NULL;
        left = (Node*)malloc(sizeof(Node));
        *left = num;
        binop.node.binop.left = left;

        Node raw = factor(var);
        Node *right = NULL;
        right = (Node*)malloc(sizeof(Node));
        *right = raw;
        binop.node.binop.right = right;

        num = binop;
    }
    return num;
}

Node AddSubTerm(int var) {
    Node mulDiv = MulDivTerm(var);

    while(curTok.type == t_ADD || curTok.type == t_SUB) {
        Node binop;
        binop.type = n_BINOP;
        binop.node.binop.op = curTok.type == t_ADD ? '+' : '-';
        curTok = getNextToken();

        Node *left = NULL;
        left = (Node*)malloc(sizeof(Node));
        *left = mulDiv;
        binop.node.binop.left = left;

        Node raw = MulDivTerm(var);
        Node *right = NULL;
        right = (Node*)malloc(sizeof(Node));
        *right = raw;
        binop.node.binop.right = right;

        mulDiv = binop;
    }
    return mulDiv;
}

Node parse_function(std::string input, int var) {
    // this sets the tokens value
    interpret_function(input);
    curTok = getNextToken();
    return AddSubTerm(var);
}

float visit(Node node) {
    if(node.type == n_NUM)
        return (float)node.node.num.value;
    if(node.node.binop.op == '+')
        return visit(*node.node.binop.left) + visit(*node.node.binop.right);
    if(node.node.binop.op == '-')
        return visit(*node.node.binop.left) - visit(*node.node.binop.right);
    if(node.node.binop.op == '*')
        return visit(*node.node.binop.left) * visit(*node.node.binop.right);
    if(node.node.binop.op == '/')
        return visit(*node.node.binop.left) / visit(*node.node.binop.right);
    return -1.0f;
}

float calc(std::string input, int var) {
    float value = visit(parse_function(input, var));
    // clean up
    tokens.clear();
    tokPos = -1;
    return value;
}