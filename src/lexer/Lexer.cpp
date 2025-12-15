#include <iostream>
#include <cctype>

#include "Lexer.h"

using namespace std;

Lexer::Lexer(const string& sourceCode){
    this->source = sourceCode;
    this->currentPos = 0;
    this->currentLine = 1;
    this->currentColumn = 1;
}

vector<Token> Lexer::startTokenization(){
    vector<Token> tokenList;

    string temp;
    
    // Check if file is empty
    if(this->source.empty()) {
        return tokenList;
    }
    
    char c = this->source[0]; // initialize to first character of the file

    // start the loop over the given code
    while(this->currentPos < this->source.length()){
        c = this->getCurrentChar(); // Update c to current character

        // check if comments
        if(c == '/'){ 
            if(this->getNextChar() == '/'){ // single line comment
                this->handleSingleComment();
                this->currentLine++;
                this->currentColumn = 1;
                continue;
            } else if(this->getNextChar() == '*'){ // multi line comments starting point found
                this->handleMultiComment();
                continue;
            }
        }
        
        if(c == ' ') this->skipWhiteSpaces(); // skip all white spaces
        else if(isalpha(c) || c == '_') tokenList.push_back(this->evaluateAlphabetOrUnderScore());// a-z or A-Z or _
        else if(isdigit(c)){ // 0-9
            tokenList.push_back(this->evaluateNumber());
        } else if(c == '\'') tokenList.push_back(this->evaluateSingleQuote()); // store 'W' in the token
        else if(c == '\"') tokenList.push_back(this->evaluateDoubleQuote()); // store "WORD" in the token
        else if(isSingleCharToken(c)){ // # . , EOF ? ~ : ; ( ) { } [ ] 
            string temp;
            temp += c;
            
            Token tok;
            tok.data = temp;
            tok.type = this->getTokenTypeOf(temp);
            tok.line = this->currentLine;
            tok.column = this->currentColumn++;

            tokenList.push_back(tok);

            this->advanceOne();
        } else if(c == '+') tokenList.push_back(this->evaluatePlus()); // starting is + , can lead to + or += or ++ 
        else if(c == '-') tokenList.push_back(this->evaluateMinus()); // starting is - , can lead to - or -= or -- or ->
        else if(c == '*') tokenList.push_back(this->evaluateStar()); // starting is * , can lead to * or *=
        else if(c == '/') tokenList.push_back(this->evaluateDivide()); // starting is / , can lead to / or /=
        else if(c == '%') tokenList.push_back(this->evaluateMod()); // starting is % , can lead to % or %= 
        else if(c == '<') tokenList.push_back(this->evaluateLesser()); // starting is < , can lead to < or <= or <<=
        else if(c == '>') tokenList.push_back(this->evaluateGreater()); // starting is > , can lead to > or <= or >>=
        else if(c == '=') tokenList.push_back(this->evaluateEqual()); // starting is = , can lead to = or ==
        else if(c == '&') tokenList.push_back(this->evaluateAnd()); // starting is & , can lead to & or &= or &&
        else if(c == '|') tokenList.push_back(this->evaluateOr()); // starting is | , can lead to | or |= or ||
        else if(c == '!') tokenList.push_back(this->evaluateNot()); // starting is ! , can lead to ! or !=
        else if(c == '^') tokenList.push_back(this->evaluatePowExp()); // starting is ^ , can lead to ^ or ^=
        else if(c == '\n'){ // end-line used to advance line number and reset column number
            this->advanceOne();
            this->currentLine++;
            this->currentColumn = 1;
        } else {
            // unrecognized character, skip it for now
            this->advanceOne();
        }
    }

    return tokenList;
}

char Lexer::getCurrentChar(){ // return character at current position
    return this->source[this->currentPos];
}

char Lexer::getNextChar(){ // return character at just next position
    if(this->currentPos + 1 >= this->source.length()) {
        return '\0';
    }
    return this->source[this->currentPos+1];
}

void Lexer::advanceOne(){ // increment the current position pointor/index by 1
    this->currentPos++;
    return;
}

void Lexer::advanceBy(int n){ // increment the current position pointor/index by n
    this->currentPos = this->currentPos + n;
    return;
}

char Lexer::getCurrentCharAndAdvanceOne(){
    if(this->currentPos >= this->source.length()) {
        return '\0';
    }    
    return this->source[this->currentPos++];
}

char Lexer::getNextCharAndAdvanceOne(){
    this->currentPos++;
    if(this->currentPos >= this->source.length()) {
        return '\0';
    }
    return this->source[this->currentPos];
}

void Lexer::skipWhiteSpaces(){
    while(this->currentPos < this->source.length() && this->source[this->currentPos] == ' '){
        this->currentPos++;
    }

    return;
}

Token Lexer::evaluateAlphabetOrUnderScore(){
    string temp;

    char c = this->source[this->currentPos]; // starting is either a-z or A-Z or _

    while(isalpha(c) || isdigit(c) || c == '_'){ // variable name or keyword can include a-z or A-Z or _ or 0-9
        temp += c;
        c = this->getNextCharAndAdvanceOne();
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;
    
    return tok;
}

Token Lexer::evaluateEqual(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is =

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is ==

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateMod(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is %

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is %=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateNot(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is !

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is !=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluatePowExp(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is ^

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is ^=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluatePlus(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is +

    if((this->getCurrentChar() == '=') || (this->getCurrentChar() == '+')) temp += this->getCurrentCharAndAdvanceOne(); // add = or + if it is += or ++

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateMinus(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is -

    if((this->getCurrentChar() == '=') || (this->getCurrentChar() == '-') || (this->getCurrentChar() == '>')) temp += this->getCurrentCharAndAdvanceOne(); // add = or - or > if it is -= or -- or ->

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateStar(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is *

    if((this->getCurrentChar() == '=')) temp += this->getCurrentCharAndAdvanceOne(); // add = if it is *=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateDivide(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is /

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is /=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateAnd(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is &

    if((this->getCurrentChar() == '=') || (this->getCurrentChar() == '&')) temp += this->getCurrentCharAndAdvanceOne(); // add = or & if it is &= or &&

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateOr(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is |

    if((this->getCurrentChar() == '=') || (this->getCurrentChar() == '|')) temp += this->getCurrentCharAndAdvanceOne(); // add = or | if it is |= or ||

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateLesser(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is <    

    if(this->getCurrentChar() == '='){
        temp += this->getCurrentCharAndAdvanceOne(); // add = if <=
    } else if(this->getCurrentChar() == '<'){
        temp += this->getCurrentCharAndAdvanceOne(); // add < if <<
        if(this->getCurrentChar() == '='){
            temp += this->getCurrentCharAndAdvanceOne(); // add = if <<=
        }
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateGreater(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is >

    if(this->getCurrentChar() == '='){
        temp += this->getCurrentCharAndAdvanceOne(); // add = if >=
    } else if(this->getCurrentChar() == '>'){
        temp += this->getCurrentCharAndAdvanceOne(); // add > if >>
        if(this->getCurrentChar() == '='){
            temp += this->getCurrentCharAndAdvanceOne(); // add = if >>=
        }
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateSingleQuote(){
    string temp;

    temp += '\''; // '

    this->currentPos++;

    // find the closing '
    while(this->source[this->currentPos] != '\'' && this->currentPos < this->source.length()){
        if(this->source[this->currentPos] == '\\' && this->currentPos < this->source.length()){ // found escape sequence starting point
            temp += this->getCurrentCharAndAdvanceOne(); /* add \ */
            
            temp += this->getCurrentCharAndAdvanceOne(); // add next character, either \ or n or 0 or ' or "
        } else{            
            temp += this->source[this->currentPos++];
        }
    }

    if(this->currentPos < this->source.length()){
        temp += this->source[this->currentPos++]; // add closing ' and advance by one
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

Token Lexer::evaluateDoubleQuote(){
    string temp;

    temp += '\"'; // "

    this->currentPos++;

    // find the closing "
    while(this->source[this->currentPos] != '\"' && this->currentPos < this->source.length()){
        if(this->source[this->currentPos] == '\\' && this->currentPos < this->source.length()){ // found escape sequence starting point
            temp += this->getCurrentCharAndAdvanceOne(); /* add \ */     
            
            temp += this->getCurrentCharAndAdvanceOne(); // add next character, either \ or n or 0 or ' or "
        } else{            
            temp += this->source[this->currentPos++];
        }
    }

    if(this->currentPos < this->source.length()){
        temp += this->source[this->currentPos++]; // add closing " and advance by one
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

void Lexer::handleSingleComment(){
    while(this->source[this->currentPos] != '\n' && this->currentPos < this->source.length()){
        this->currentPos++;
    }

    if(this->source[this->currentPos] == '\n') this->currentPos++;

    return;
}

void Lexer::handleMultiComment(){
    // skip /*
    this->currentPos = this->currentPos + 2;

    find_star : 
    while(this->getCurrentChar() != '*' && this->currentPos < this->source.length()){
        if(this->getCurrentChar() == '\n'){
            this->currentLine++;
            this->currentColumn = 1;
        }
        this->advanceOne();        
    }

    if(this->getCurrentChar() == '*'){ // * found , find closing /
        if(this->getCurrentChar() == '\n'){
            this->currentLine++;
            this->currentColumn = 1;
        }
        this->advanceOne(); // skip *

        if(this->currentPos < this->source.length()){
            if(this->getCurrentChar() == '/'){ // / also found, multi-comment closed
                this->advanceOne(); // skip /
                return;
            } else{
                goto find_star;
            }
        }
    }

    return;
    
}

Token Lexer::evaluateNumber(){
    string temp;    

    bool isDecimal = false;
    
    evaluate_number : 
    while(isdigit(this->getCurrentChar()) && this->currentPos < this->source.length()){
        temp += this->getCurrentCharAndAdvanceOne();
    }

    if(this->getCurrentChar() == '.' && this->currentPos < this->source.length()){
        isDecimal = true;
        temp += this->getCurrentCharAndAdvanceOne(); // Add the decimal point to temp
        goto evaluate_number;
    }

    if(isDecimal){
        if((this->getCurrentChar() == 'f' || this->getCurrentChar() == 'F') && this->currentPos < this->source.length()){
            temp += this->getCurrentCharAndAdvanceOne();
        }
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->currentLine;
    tok.column = this->currentColumn++;

    return tok;
}

bool Lexer::isSingleCharToken(char c){
    if((c == '.') || (c == ',') || (c == '?') || (c == '~') || (c == ':') || (c == ';')|| (c == '(') || (c == ')') || (c == '{') || (c == '}') || (c == '[') || (c == ']') || (c == '#') || (c == EOF)) return true;

    return false;
}

TokenType Lexer::getTokenTypeOf(string data){
    // Character Literal - starts and ends with single quote
    if(!data.empty() && data[0] == '\'' && data[data.length()-1] == '\'') {
        return CHAR_LITERAL;
    }
    
    // String Literal - starts and ends with double quote
    if(!data.empty() && data[0] == '\"' && data[data.length()-1] == '\"') {
        return STRING_LITERAL;
    }
    
    // Numeric Literals - check if first character is a digit or decimal point with digits
    if(!data.empty() && (isdigit(data[0]) || (data[0] == '.' && data.length() > 1))) {
        bool hasDecimal = false;
        bool endsWithF = false;
        
        // Check for decimal point
        for(size_t i = 0; i < data.length(); i++) {
            if(data[i] == '.') {
                hasDecimal = true;
            }
        }
        
        // Check if last character is 'f' or 'F'
        if(data[data.length()-1] == 'f' || data[data.length()-1] == 'F') {
            endsWithF = true;
        }
        
        // Determine numeric type
        if(!hasDecimal) {
            return INT_LITERAL;  // No decimal point -> integer
        } else if(endsWithF) {
            return FLOAT_LITERAL;  // Has decimal + ends with 'f'/'F' -> float
        } else {
            return DOUBLE_LITERAL;  // Has decimal, no 'f' at end -> double
        }
    }
    
    // Keywords - Data Types
    if(data == "int") return KEYWORD_INT;
    if(data == "double") return KEYWORD_DOUBLE;
    if(data == "float") return KEYWORD_FLOAT;
    if(data == "bool") return KEYWORD_BOOL;
    if(data == "char") return KEYWORD_CHAR;
    if(data == "void") return KEYWORD_VOID;
    if(data == "short") return KEYWORD_SHORT;
    if(data == "long") return KEYWORD_LONG;
    if(data == "unsigned") return KEYWORD_UNSIGNED;
    if(data == "signed") return KEYWORD_SIGNED;
    
    // Keywords - Control Flow
    if(data == "if") return KEYWORD_IF;
    if(data == "else") return KEYWORD_ELSE;
    if(data == "while") return KEYWORD_WHILE;
    if(data == "do") return KEYWORD_DO;
    if(data == "for") return KEYWORD_FOR;
    if(data == "switch") return KEYWORD_SWITCH;
    if(data == "case") return KEYWORD_CASE;
    if(data == "default") return KEYWORD_DEFAULT;
    if(data == "break") return KEYWORD_BREAK;
    if(data == "continue") return KEYWORD_CONTINUE;
    if(data == "return") return KEYWORD_RETURN;
    if(data == "goto") return KEYWORD_GOTO;
    
    // Keywords - Storage & Type
    if(data == "struct") return KEYWORD_STRUCT;
    if(data == "union") return KEYWORD_UNION;
    if(data == "enum") return KEYWORD_ENUM;
    if(data == "typedef") return KEYWORD_TYPEDEF;
    if(data == "sizeof") return KEYWORD_SIZEOF;
    if(data == "const") return KEYWORD_CONST;
    if(data == "static") return KEYWORD_STATIC;
    if(data == "extern") return KEYWORD_EXTERN;
    if(data == "auto") return KEYWORD_AUTO;
    if(data == "register") return KEYWORD_REGISTER;
    if(data == "volatile") return KEYWORD_VOLATILE;
    
    // Preprocessor Directives (without #)
    if(data == "include") return PREP_INCLUDE;
    if(data == "define") return PREP_DEFINE;
    if(data == "ifdef") return PREP_IFDEF;
    if(data == "ifndef") return PREP_IFNDEF;
    if(data == "elif") return PREP_ELIF;
    if(data == "endif") return PREP_ENDIF;
    if(data == "undef") return PREP_UNDEF;
    if(data == "pragma") return PREP_PRAGMA;
    
    // Operators
    if(data == "=") return OP_ASSIGN;
    if(data == "==") return OP_EQ_EQ;
    if(data == "+") return OP_PLUS;
    if(data == "++") return OP_PLUS_PLUS;
    if(data == "+=") return OP_PLUS_EQ;
    if(data == "-") return OP_MINUS;
    if(data == "--") return OP_MINUS_MINUS;
    if(data == "-=") return OP_MINUS_EQ;
    if(data == "->") return OP_ARROW;
    if(data == "*") return OP_STAR;
    if(data == "*=") return OP_STAR_EQ;
    if(data == "/") return OP_SLASH;
    if(data == "/=") return OP_SLASH_EQ;
    if(data == "%") return OP_PERCENT;
    if(data == "%=") return OP_PERCENT_EQ;
    if(data == "<") return OP_LESS;
    if(data == "<=") return OP_LESS_EQ;
    if(data == "<<") return OP_LSHIFT;
    if(data == "<<=") return OP_LSHIFT_EQ;
    if(data == ">") return OP_GREATER;
    if(data == ">=") return OP_GREATER_EQ;
    if(data == ">>") return OP_RSHIFT;
    if(data == ">>=") return OP_RSHIFT_EQ;
    if(data == "!") return OP_NOT;
    if(data == "!=") return OP_NOT_EQ;
    if(data == "&") return OP_AND;
    if(data == "&&") return OP_AND_AND;
    if(data == "&=") return OP_AND_EQ;
    if(data == "|") return OP_OR;
    if(data == "||") return OP_OR_OR;
    if(data == "|=") return OP_OR_EQ;
    if(data == "^") return OP_XOR;
    if(data == "^=") return OP_XOR_EQ;
    if(data == "~") return OP_TILDE;
    if(data == ".") return OP_DOT;
    if(data == ",") return COMMA;
    if(data == ":") return OP_COLON;
    if(data == ";") return SEMICOLON;
    if(data == "?") return OP_QUESTION;
    if(data == "#") return OP_HASH;
    if(data == "(") return LPAREN;
    if(data == ")") return RPAREN;
    if(data == "{") return LBRACE;
    if(data == "}") return RBRACE;
    if(data == "[") return LBRACKET;
    if(data == "]") return RBRACKET;
    
    // Not a keyword or operator - it's an identifier
    return ID;
}