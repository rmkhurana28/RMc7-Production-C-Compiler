#include <iostream>
#include <cctype>

#include "Lexer.h"

using namespace std;

unordered_set<string> funcLikeMacroSet;

Lexer::Lexer(const string& sourceCode , const string& fileName){

    LexerConfig* first = new LexerConfig{0,1,1,fileName,"",sourceCode};
    
    lexerStack.push_back(*first);
    this->lastAddedInLexerStackIndex = 0;

    // this->currentFile = fileName;

    // this->lexerStack[this->lastAddedInLexerStackIndex].currentSource = sourceCode;
    // this->currentPos = 0;
    // this->currentLine = 1;
    // this->currentColumn = 1;
}

vector<Token> Lexer::startTokenization(){
    vector<Token> tokenList;

    string temp;
    
    // Check if file is empty
    if(this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.empty()) {
        return tokenList;
    }

    // remove the line splicing chaaracters if needed
    // considering \n and \r\n 
    {
        string& src = this->lexerStack[this->lastAddedInLexerStackIndex].currentSource;
        string cleanSource = "";
        cleanSource.reserve(src.length());

        size_t i = 0;
        while (i < src.length()) {
            if (src[i] == '\\') {
                // Check for Windows CRLF: \ + \r + \n
                if (i + 2 < src.length() && src[i + 1] == '\r' && src[i + 2] == '\n') {
                    i += 3; // skip '\', '\r', '\n'
                    continue;
                }
                // Check for Unix LF: \ + \n
                else if (i + 1 < src.length() && src[i + 1] == '\n') {
                    i += 2; // skip '\', '\n'
                    continue;
                }
            }
            cleanSource += src[i];
            i++;
        }

        src = cleanSource;

        // If file became empty after splicing
        if (src.empty()) {
            return tokenList;
        }
    }
    
    char c = this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[0]; // initialize to first character of the file

    // start the loop over the given code
    while(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){

        // if(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos > 0 && tokenList.back().type == PREP_INCLUDE){
        //     // cout << "Yes\n";
        // }

        c = this->getCurrentChar(); // Update c to current character

        // check if comments
        if(c == '/'){ 
            if(this->getNextChar() == '/'){ // single line comment
                this->handleSingleComment();

                this->lexerStack[this->lastAddedInLexerStackIndex].currentLine++;
                this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn = 1;
                
                continue;
            } else if(this->getNextChar() == '*'){ // multi line comments starting point found
                this->handleMultiComment();
                continue;
            }
        }
        
        if(c == ' ') this->skipWhiteSpaces(); // skip all white spaces
        else if(isalpha(c) || c == '_') tokenList.push_back(this->evaluateAlphabetOrUnderScore());// a-z or A-Z or _
        else if(isdigit(c) || (c == '.' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos + 1 < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length() && isdigit(this->getNextChar()))){ // 0-9 or .5
            tokenList.push_back(this->evaluateNumber());
        } else if(c == '\'') tokenList.push_back(this->evaluateSingleQuote()); // store 'W' in the token
        else if(c == '\"') tokenList.push_back(this->evaluateDoubleQuote()); // store "WORD" in the token
        else if(isSingleCharToken(c)){ // # . , EOF ? ~ : ; ( ) { } [ ]             
            string temp;
            temp += c;
            
            Token tok;
            tok.data = temp;
            tok.type = this->getTokenTypeOf(temp);
            tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
            tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

            tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
            tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;



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
            this->lexerStack[this->lastAddedInLexerStackIndex].currentLine++;
            this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn = 1;
        } else {
            // unrecognized character, skip it for now
            this->advanceOne();
        }
    }

    // this->lexerStack.pop_back();
    // this->lastAddedInLexerStackIndex--;

    return tokenList;
}

string Lexer::isTokenValidDefineName(const Token& token){

    string retValue = "";

    // 1. Regular Identifiers
    if (token.type == ID) {
        string test = token.data;

        // Rule 2 & Rule 3: Reject forbidden & built-in macros
        if (test == "defined" ||
            test == "__LINE__" || 
            test == "__FILE__" ||
            test == "__DATE__" || 
            test == "__TIME__" ||
            test == "__STDC__" ||
            test == "__STDC_VERSION__") {
            return retValue; // Invalid name
        }

        return test; // Valid identifier name
    }
    // 2. C Keywords (allowed to be redefined in preprocessor)
    else if (token.type >= KEYWORD_INT && token.type <= KEYWORD_RESTRICT) {
        return token.data; // Valid keyword name (e.g., int, return, goto, bool)
    }

    // 3. Operators, numbers, literals, delimiters are rejected (returns "")
    return retValue;
}

char Lexer::getCurrentChar(){ // return character at current position
    return this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos];
}

char Lexer::getNextChar(){ // return character at just next position
    if(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos + 1 >= this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
        return '\0';
    }
    return this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos+1];
}

void Lexer::advanceOne(){ // increment the current position pointor/index by 1
    this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++;
    return;
}

void Lexer::advanceBy(int n){ // increment the current position pointor/index by n
    this->lexerStack[this->lastAddedInLexerStackIndex].currentPos = this->lexerStack[this->lastAddedInLexerStackIndex].currentPos + n;
    return;
}

char Lexer::getCurrentCharAndAdvanceOne(){
    if(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos >= this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
        return '\0';
    }    
    return this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++];
}

char Lexer::getNextCharAndAdvanceOne(){

    this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++;
    
    if(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos >= this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
        return '\0';
    }
    return this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos];
}

void Lexer::skipWhiteSpaces(){
    while(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length() && this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos] == ' '){

        this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++;
    }

    return;
}

Token Lexer::evaluateAlphabetOrUnderScore(){
    string temp;

    static bool wasPrevTokenDefine = false;

    char c = this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos]; // starting is either a-z or A-Z or _

    while(isalpha(c) || isdigit(c) || c == '_'){ // variable name or keyword can include a-z or A-Z or _ or 0-9
        temp += c;
        c = this->getNextCharAndAdvanceOne();
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);

    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    {
        // check if tok is of define or not

        if(wasPrevTokenDefine){
            // check if current character is space or not
            // if it is space,then it is obj-like macro, do NOTHING
            // if it NOT space, and it is ( , then it is func-like macro, add in the set            

            c = this->getCurrentChar();
            if(c == '('){
                // it is func-like macro, add in the set
                funcLikeMacroSet.insert(tok.fileName + ":" + to_string(tok.line));
                cout << "Added " << tok.data << endl;
            } 

            wasPrevTokenDefine = false;
        }
        
        if(tok.type == PREP_DEFINE){
            // now check the next token and then again check if there is space after the token
            wasPrevTokenDefine = true;
        } 
    }

    
    
    return tok;
}

Token Lexer::evaluateEqual(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is =

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is ==

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateMod(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is %

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is %=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateNot(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is !

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is !=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluatePowExp(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is ^

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is ^=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluatePlus(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is +

    if((this->getCurrentChar() == '=') || (this->getCurrentChar() == '+')) temp += this->getCurrentCharAndAdvanceOne(); // add = or + if it is += or ++

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateMinus(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is -

    if((this->getCurrentChar() == '=') || (this->getCurrentChar() == '-') || (this->getCurrentChar() == '>')) temp += this->getCurrentCharAndAdvanceOne(); // add = or - or > if it is -= or -- or ->

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateStar(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is *

    if((this->getCurrentChar() == '=')) temp += this->getCurrentCharAndAdvanceOne(); // add = if it is *=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateDivide(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is /

    if(this->getCurrentChar() == '=') temp += this->getCurrentCharAndAdvanceOne(); // add = if it is /=

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateAnd(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is &

    if((this->getCurrentChar() == '=') || (this->getCurrentChar() == '&')) temp += this->getCurrentCharAndAdvanceOne(); // add = or & if it is &= or &&

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateOr(){
    string temp;

    temp += this->getCurrentCharAndAdvanceOne(); // starting is |

    if((this->getCurrentChar() == '=') || (this->getCurrentChar() == '|')) temp += this->getCurrentCharAndAdvanceOne(); // add = or | if it is |= or ||

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

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
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

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
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateSingleQuote(){
    string temp;

    temp += '\''; // '

    this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++;

    // find the closing '
    while(this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos] != '\'' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){
        if(this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos] == '\\' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){ // found escape sequence starting point
            temp += this->getCurrentCharAndAdvanceOne(); /* add \ */
            
            temp += this->getCurrentCharAndAdvanceOne(); // add next character, either \ or n or 0 or ' or "
        } else{            
            temp += this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++];
        }
    }

    if(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){
        temp += this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++]; // add closing ' and advance by one
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

Token Lexer::evaluateDoubleQuote(){
    string temp;

    temp += '\"'; // "

    this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++;

    // find the closing "
    while(this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos] != '\"' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){
        if(this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos] == '\\' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){ // found escape sequence starting point
            temp += this->getCurrentCharAndAdvanceOne(); /* add \ */     
            
            temp += this->getCurrentCharAndAdvanceOne(); // add next character, either \ or n or 0 or ' or "
        } else{            
            temp += this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++];
        }
    }

    if(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){
        temp += this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++]; // add closing " and advance by one
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

    return tok;
}

void Lexer::handleSingleComment(){
    while(this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos] != '\n' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){
        this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++;
    }

    if(this->lexerStack[this->lastAddedInLexerStackIndex].currentSource[this->lexerStack[this->lastAddedInLexerStackIndex].currentPos] == '\n') this->lexerStack[this->lastAddedInLexerStackIndex].currentPos++;

    return;
}

void Lexer::handleMultiComment(){
    // skip /*
    this->lexerStack[this->lastAddedInLexerStackIndex].currentPos = this->lexerStack[this->lastAddedInLexerStackIndex].currentPos + 2;

    find_star : 
    while(this->getCurrentChar() != '*' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){
        if(this->getCurrentChar() == '\n'){

            this->lexerStack[this->lastAddedInLexerStackIndex].currentLine++;
            this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn = 1;
            
        }
        this->advanceOne();        
    }

    if(this->getCurrentChar() == '*'){ // * found , find closing /
        if(this->getCurrentChar() == '\n'){

            this->lexerStack[this->lastAddedInLexerStackIndex].currentLine++;
            this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn = 1;
        
        }
        this->advanceOne(); // skip *

        if(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()){
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

    // STEP 1: Detect format and collect number part
    char first = this->getCurrentChar();

    // LEADING DECIMAL: .5, .5e2, .5f, etc.
    if(first == '.') {
        temp += this->getCurrentCharAndAdvanceOne(); // consume '.'

        // Collect digits after decimal
        while(isdigit(this->getCurrentChar()) && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne();
        }

        // Check for scientific notation (e or E)
        if((this->getCurrentChar() == 'e' || this->getCurrentChar() == 'E') &&
           this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne(); // 'e' or 'E'

            // Optional +/-
            if((this->getCurrentChar() == '+' || this->getCurrentChar() == '-') &&
               this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
                temp += this->getCurrentCharAndAdvanceOne();
            }

            // Collect exponent digits
            while(isdigit(this->getCurrentChar()) && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
                temp += this->getCurrentCharAndAdvanceOne();
            }
        }

        // Collect suffixes (f, F, l, L, u, U in any order)
        while(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            char c = this->getCurrentChar();
            if(c == 'f' || c == 'F' || c == 'l' || c == 'L' || c == 'u' || c == 'U') {
                temp += this->getCurrentCharAndAdvanceOne();
            } else {
                break;
            }
        }

        Token tok;
        tok.data = temp;
        tok.type = this->getTokenTypeOf(temp);
        tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
        tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

        tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
        tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

        return tok;
    }

    // HEX mode: 0x or 0X
    if(first == '0' && (this->getNextChar() == 'x' || this->getNextChar() == 'X')) {
        temp += this->getCurrentCharAndAdvanceOne(); // '0'
        temp += this->getCurrentCharAndAdvanceOne(); // 'x' or 'X'

        // Collect hex digits (0-9, a-f, A-F)
        while((isdigit(this->getCurrentChar()) ||
               (tolower(this->getCurrentChar()) >= 'a' && tolower(this->getCurrentChar()) <= 'f')) &&
              this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne();
        }

        // Check for hex float (decimal point)
        if(this->getCurrentChar() == '.' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne(); // '.'

            // Collect hex digits after decimal
            while((isdigit(this->getCurrentChar()) ||
                   (tolower(this->getCurrentChar()) >= 'a' && tolower(this->getCurrentChar()) <= 'f')) &&
                  this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
                temp += this->getCurrentCharAndAdvanceOne();
            }
        }

        // Check for hex float exponent (p or P)
        if((this->getCurrentChar() == 'p' || this->getCurrentChar() == 'P') &&
           this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne(); // 'p' or 'P'

            if((this->getCurrentChar() == '+' || this->getCurrentChar() == '-') &&
               this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
                temp += this->getCurrentCharAndAdvanceOne();
            }

            // Collect exponent digits
            while(isdigit(this->getCurrentChar()) && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
                temp += this->getCurrentCharAndAdvanceOne();
            }
        }
    }
    // OCTAL mode: 0 followed by digits 0-7
    else if(first == '0' && isdigit(this->getNextChar()) && this->getNextChar() <= '7') {
        // Collect octal digits (0-7)
        while(isdigit(this->getCurrentChar()) && this->getCurrentChar() <= '7' &&
              this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne();
        }
    }
    // DECIMAL mode (including decimal floats)
    else {
        // Collect decimal digits
        while(isdigit(this->getCurrentChar()) && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne();
        }

        // Check for decimal point
        if(this->getCurrentChar() == '.' && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne(); // '.'

            // Collect digits after decimal (optional for cases like 5. or 5.e2)
            while(isdigit(this->getCurrentChar()) && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
                temp += this->getCurrentCharAndAdvanceOne();
            }
        }
    }

    // STEP 2: Check for scientific notation (e or E)
    if((this->getCurrentChar() == 'e' || this->getCurrentChar() == 'E') &&
       this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
        temp += this->getCurrentCharAndAdvanceOne(); // 'e' or 'E'

        // Optional +/-
        if((this->getCurrentChar() == '+' || this->getCurrentChar() == '-') &&
           this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne();
        }

        // Collect exponent digits
        while(isdigit(this->getCurrentChar()) && this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
            temp += this->getCurrentCharAndAdvanceOne();
        }
    }

    // STEP 3: Collect suffixes (f, F, l, L, u, U in any order)
    while(this->lexerStack[this->lastAddedInLexerStackIndex].currentPos < this->lexerStack[this->lastAddedInLexerStackIndex].currentSource.length()) {
        char c = this->getCurrentChar();
        if(c == 'f' || c == 'F' || c == 'l' || c == 'L' || c == 'u' || c == 'U') {
            temp += this->getCurrentCharAndAdvanceOne();
        } else {
            break;
        }
    }

    Token tok;
    tok.data = temp;
    tok.type = this->getTokenTypeOf(temp);
    tok.line = this->lexerStack[this->lastAddedInLexerStackIndex].currentLine;
    tok.column = this->lexerStack[this->lastAddedInLexerStackIndex].currentColumn++;

    tok.fileName = this->lexerStack[this->lastAddedInLexerStackIndex].currentFile;
    tok.filePath = this->lexerStack[this->lastAddedInLexerStackIndex].currentFilePath;

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

    // Numeric Literals - check if first character is a digit or decimal point
    if(!data.empty() && (isdigit(data[0]) || (data[0] == '.' && data.length() > 1))) {
        bool hasDecimal = false;      // decimal point (.)
        bool hasExponent = false;      // e or E (decimal) or p or P (hex)
        bool hasHexPrefix = false;     // 0x or 0X
        bool endsWithF = false;        // f or F suffix

        // Check for hex prefix (0x or 0X)
        if(data.length() > 1 && data[0] == '0' && (data[1] == 'x' || data[1] == 'X')) {
            hasHexPrefix = true;
        }

        // Check for decimal point or exponent
        for(size_t i = 0; i < data.length(); i++) {
            if(data[i] == '.') {
                hasDecimal = true;
            } else if(!hasHexPrefix && (data[i] == 'e' || data[i] == 'E')) {
                // For non-hex numbers: e/E indicates scientific notation
                hasExponent = true;
            } else if(data[i] == 'p' || data[i] == 'P') {
                // For hex floats: p/P indicates binary exponent
                hasExponent = true;
            }
        }

        // Check if ends with 'f' or 'F' (suffix)
        // For pure hex integers (0x... with no decimal/exponent), F is a hex digit, not a suffix
        bool isPureHexInteger = (hasHexPrefix && !hasDecimal && !hasExponent);
        if(!data.empty() && !isPureHexInteger && (data[data.length()-1] == 'f' || data[data.length()-1] == 'F')) {
            endsWithF = true;
        }

        // Determine numeric type
        // Integer: hex integer (0x...) or decimal/octal integer without decimal/exponent/suffix
        if(hasHexPrefix && !hasDecimal && !hasExponent) {
            return INT_LITERAL;
        }
        // Float: has 'f' or 'F' suffix (works for both decimal and hex floats)
        else if(endsWithF) {
            return FLOAT_LITERAL;
        }
        // Double: has decimal point or scientific notation (e/E or p/P)
        else if(hasDecimal || hasExponent) {
            return DOUBLE_LITERAL;
        }
        // Integer: otherwise (decimal/octal integer with or without L/U/LL suffixes)
        else {
            return INT_LITERAL;
        }
    }

    // Keywords - Data Types
    if(data == "int") return KEYWORD_INT;
    if(data == "double") return KEYWORD_DOUBLE;
    if(data == "float") return KEYWORD_FLOAT;
    if(data == "bool") return KEYWORD_BOOL;
    if(data == "_Bool") return KEYWORD_BOOL;
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
    if(data == "restrict") return KEYWORD_RESTRICT;
    
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

