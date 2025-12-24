#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "Token.h"

using namespace std;

class Lexer {
private:
    string source; // entire code text
    size_t currentPos; // current posistion of the pointor/index
    size_t currentLine; // current line number
    size_t currentColumn; // current column number in a line (resets everytime when a new line is countered)

    
    char getCurrentChar(); // return character at current position    
    char getNextChar(); // return character at just next position
    
    void advanceOne(); // increment the current position pointor/index by 1    
    void advanceBy(int n); // increment the current position pointor/index by n

    char getCurrentCharAndAdvanceOne(); // increment the current position pointor/index by 1 and return the prev character
    char getNextCharAndAdvanceOne(); // increment the current position pointor/index by 1 and return the new character

    TokenType getTokenTypeOf(string data);

    void handleSingleComment();
    void handleMultiComment();

    bool isSingleCharToken(char c);

    Token evaluateAlphabetOrUnderScore(); // evalute token starting with alphabet (a-z) or (A-Z) or (_)
    Token evaluateNumber(); // evaluate token starting with number (0-9)

    Token evaluateSingleQuote(); // evalute '
    Token evaluateDoubleQuote(); // evalute "

    // # . , EOF ? ~ : ; ' " ( ) { } [ ] 
    Token evaluateSingleChar(); // evaluate tokens starting with single char token 

    Token evaluatePlus(); // evaluate token starting with PLUS (+)
    Token evaluateMinus(); // evaluate token starting with MINUS (-)
    Token evaluateStar(); // evaluate token starting with STAR (*)
    Token evaluateDivide(); // evaluate token starting with DIVIDE (/)
    Token evaluateMod(); // evaluate token starting with MOD (%)
    Token evaluateLesser(); // evaluate token starting with LESSER (<)
    Token evaluateGreater(); // evaluate token starting with GREATER (>)
    Token evaluateEqual(); // evaluate token starting with EQUAL (=)
    Token evaluateAnd(); // evaluate token starting with AND (&)
    Token evaluateOr(); // evaluate token starting with OR (|)
    Token evaluateBackSlash(); // evaluate token starting with BACKSLASH (\)
    Token evaluateNot(); // evaluate token starting with NOT (!)
    Token evaluatePowExp(); // evaluate token starting with EXP (^)
    
    

    void skipWhiteSpaces();    

    
public:
    Lexer(const string& sourceCode);

    vector<Token> startTokenization();
};  // Utility function - accessible to other components (static)
    static TokenType getTokenTypeOf(string data);
};

#endif