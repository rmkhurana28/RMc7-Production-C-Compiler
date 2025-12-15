#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

enum TokenType {
    // Keywords - Data Types
    KEYWORD_INT,           // int
    KEYWORD_DOUBLE,        // double
    KEYWORD_FLOAT,         // float
    KEYWORD_BOOL,          // bool
    KEYWORD_CHAR,          // char
    KEYWORD_VOID,          // void
    KEYWORD_SHORT,         // short
    KEYWORD_LONG,          // long
    KEYWORD_UNSIGNED,      // unsigned
    KEYWORD_SIGNED,        // signed
    
    // Keywords - Control Flow
    KEYWORD_IF,            // if
    KEYWORD_ELSE,          // else
    KEYWORD_WHILE,         // while
    KEYWORD_DO,            // do
    KEYWORD_FOR,           // for
    KEYWORD_SWITCH,        // switch
    KEYWORD_CASE,          // case
    KEYWORD_DEFAULT,       // default
    KEYWORD_BREAK,         // break
    KEYWORD_CONTINUE,      // continue
    KEYWORD_RETURN,        // return
    KEYWORD_GOTO,          // goto
    
    // Keywords - Storage & Type
    KEYWORD_STRUCT,        // struct
    KEYWORD_UNION,         // union
    KEYWORD_ENUM,          // enum
    KEYWORD_TYPEDEF,       // typedef
    KEYWORD_SIZEOF,        // sizeof
    KEYWORD_CONST,         // const
    KEYWORD_STATIC,        // static
    KEYWORD_EXTERN,        // extern
    KEYWORD_AUTO,          // auto
    KEYWORD_REGISTER,      // register
    KEYWORD_VOLATILE,      // volatile
    
    // Operators - Arithmetic
    OP_PLUS,               // +
    OP_MINUS,              // -
    OP_STAR,               // *
    OP_SLASH,              // /
    OP_PERCENT,            // %
    
    // Operators - Relational
    OP_LESS,               // <
    OP_LESS_EQ,            // <=
    OP_GREATER,            // >
    OP_GREATER_EQ,         // >=
    OP_EQ_EQ,              // ==
    OP_NOT_EQ,             // !=
    
    // Operators - Logical
    OP_AND_AND,            // &&
    OP_OR_OR,              // ||
    OP_NOT,                // !
    
    // Operators - Bitwise
    OP_AND,                // &
    OP_OR,                 // |
    OP_XOR,                // ^
    OP_TILDE,              // ~
    OP_LSHIFT,             // <<
    OP_RSHIFT,             // >>
    
    // Operators - Assignment
    OP_ASSIGN,             // =
    OP_PLUS_EQ,            // +=
    OP_MINUS_EQ,           // -=
    OP_STAR_EQ,            // *=
    OP_SLASH_EQ,           // /=
    OP_PERCENT_EQ,         // %=
    OP_AND_EQ,             // &=
    OP_OR_EQ,              // |=
    OP_XOR_EQ,             // ^=
    OP_LSHIFT_EQ,          // <<=
    OP_RSHIFT_EQ,          // >>=
    
    // Operators - Increment/Decrement
    OP_PLUS_PLUS,          // ++
    OP_MINUS_MINUS,        // --
    
    // Operators - Member Access
    OP_DOT,                // .
    OP_ARROW,              // ->
    
    // Operators - Other
    OP_QUESTION,           // ?
    OP_COLON,              // :
    OP_HASH,               // #
    
    // Delimiters
    LPAREN,                // (
    RPAREN,                // )
    LBRACE,                // {
    RBRACE,                // }
    LBRACKET,              // [
    RBRACKET,              // ]
    SEMICOLON,             // ;
    COMMA,                 // ,
    
    // Preprocessor Directives
    PREP_INCLUDE,          // #include
    PREP_DEFINE,           // #define
    PREP_IFDEF,            // #ifdef
    PREP_IFNDEF,           // #ifndef
    PREP_IF,               // #if
    PREP_ELIF,             // #elif
    PREP_ELSE,             // #else
    PREP_ENDIF,            // #endif
    PREP_UNDEF,            // #undef
    PREP_PRAGMA,           // #pragma
    
    // Literals
    INT_LITERAL,           // 42, 0x2A, 052
    FLOAT_LITERAL,         // 3.14, 2.5f
    DOUBLE_LITERAL,        // 3.14159
    CHAR_LITERAL,          // 'a', '\n'
    STRING_LITERAL,        // "hello world"
    
    // Identifiers
    ID,            // variable names, function names
    
    // Special
    END_OF_FILE,           // EOF marker
    ERROR                  // Error token
};

struct Token {
    TokenType type;
    string data;
    int line;
    int column;
};

// Helper function to convert TokenType to string
inline string tokenTypeToString(TokenType type) {
    switch(type) {
        // Keywords - Data Types
        case KEYWORD_INT: return "KEYWORD_INT";
        case KEYWORD_DOUBLE: return "KEYWORD_DOUBLE";
        case KEYWORD_FLOAT: return "KEYWORD_FLOAT";
        case KEYWORD_BOOL: return "KEYWORD_BOOL";
        case KEYWORD_CHAR: return "KEYWORD_CHAR";
        case KEYWORD_VOID: return "KEYWORD_VOID";
        case KEYWORD_SHORT: return "KEYWORD_SHORT";
        case KEYWORD_LONG: return "KEYWORD_LONG";
        case KEYWORD_UNSIGNED: return "KEYWORD_UNSIGNED";
        case KEYWORD_SIGNED: return "KEYWORD_SIGNED";
        
        // Keywords - Control Flow
        case KEYWORD_IF: return "KEYWORD_IF";
        case KEYWORD_ELSE: return "KEYWORD_ELSE";
        case KEYWORD_WHILE: return "KEYWORD_WHILE";
        case KEYWORD_DO: return "KEYWORD_DO";
        case KEYWORD_FOR: return "KEYWORD_FOR";
        case KEYWORD_SWITCH: return "KEYWORD_SWITCH";
        case KEYWORD_CASE: return "KEYWORD_CASE";
        case KEYWORD_DEFAULT: return "KEYWORD_DEFAULT";
        case KEYWORD_BREAK: return "KEYWORD_BREAK";
        case KEYWORD_CONTINUE: return "KEYWORD_CONTINUE";
        case KEYWORD_RETURN: return "KEYWORD_RETURN";
        case KEYWORD_GOTO: return "KEYWORD_GOTO";
        
        // Keywords - Storage & Type
        case KEYWORD_STRUCT: return "KEYWORD_STRUCT";
        case KEYWORD_UNION: return "KEYWORD_UNION";
        case KEYWORD_ENUM: return "KEYWORD_ENUM";
        case KEYWORD_TYPEDEF: return "KEYWORD_TYPEDEF";
        case KEYWORD_SIZEOF: return "KEYWORD_SIZEOF";
        case KEYWORD_CONST: return "KEYWORD_CONST";
        case KEYWORD_STATIC: return "KEYWORD_STATIC";
        case KEYWORD_EXTERN: return "KEYWORD_EXTERN";
        case KEYWORD_AUTO: return "KEYWORD_AUTO";
        case KEYWORD_REGISTER: return "KEYWORD_REGISTER";
        case KEYWORD_VOLATILE: return "KEYWORD_VOLATILE";
        
        // Operators - Arithmetic
        case OP_PLUS: return "OP_PLUS";
        case OP_MINUS: return "OP_MINUS";
        case OP_STAR: return "OP_STAR";
        case OP_SLASH: return "OP_SLASH";
        case OP_PERCENT: return "OP_PERCENT";
        
        // Operators - Relational
        case OP_LESS: return "OP_LESS";
        case OP_LESS_EQ: return "OP_LESS_EQ";
        case OP_GREATER: return "OP_GREATER";
        case OP_GREATER_EQ: return "OP_GREATER_EQ";
        case OP_EQ_EQ: return "OP_EQ_EQ";
        case OP_NOT_EQ: return "OP_NOT_EQ";
        
        // Operators - Logical
        case OP_AND_AND: return "OP_AND_AND";
        case OP_OR_OR: return "OP_OR_OR";
        case OP_NOT: return "OP_NOT";
        
        // Operators - Bitwise
        case OP_AND: return "OP_AND";
        case OP_OR: return "OP_OR";
        case OP_XOR: return "OP_XOR";
        case OP_TILDE: return "OP_TILDE";
        case OP_LSHIFT: return "OP_LSHIFT";
        case OP_RSHIFT: return "OP_RSHIFT";
        
        // Operators - Assignment
        case OP_ASSIGN: return "OP_ASSIGN";
        case OP_PLUS_EQ: return "OP_PLUS_EQ";
        case OP_MINUS_EQ: return "OP_MINUS_EQ";
        case OP_STAR_EQ: return "OP_STAR_EQ";
        case OP_SLASH_EQ: return "OP_SLASH_EQ";
        case OP_PERCENT_EQ: return "OP_PERCENT_EQ";
        case OP_AND_EQ: return "OP_AND_EQ";
        case OP_OR_EQ: return "OP_OR_EQ";
        case OP_XOR_EQ: return "OP_XOR_EQ";
        case OP_LSHIFT_EQ: return "OP_LSHIFT_EQ";
        case OP_RSHIFT_EQ: return "OP_RSHIFT_EQ";
        
        // Operators - Increment/Decrement
        case OP_PLUS_PLUS: return "OP_PLUS_PLUS";
        case OP_MINUS_MINUS: return "OP_MINUS_MINUS";
        
        // Operators - Member Access
        case OP_DOT: return "OP_DOT";
        case OP_ARROW: return "OP_ARROW";
        
        // Operators - Other
        case OP_QUESTION: return "OP_QUESTION";
        case OP_COLON: return "OP_COLON";
        case OP_HASH: return "OP_HASH";
        
        // Delimiters
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case LBRACKET: return "LBRACKET";
        case RBRACKET: return "RBRACKET";
        case SEMICOLON: return "SEMICOLON";
        case COMMA: return "COMMA";
        
        // Preprocessor Directives
        case PREP_INCLUDE: return "PREP_INCLUDE";
        case PREP_DEFINE: return "PREP_DEFINE";
        case PREP_IFDEF: return "PREP_IFDEF";
        case PREP_IFNDEF: return "PREP_IFNDEF";
        case PREP_IF: return "PREP_IF";
        case PREP_ELIF: return "PREP_ELIF";
        case PREP_ELSE: return "PREP_ELSE";
        case PREP_ENDIF: return "PREP_ENDIF";
        case PREP_UNDEF: return "PREP_UNDEF";
        case PREP_PRAGMA: return "PREP_PRAGMA";
        
        // Literals
        case INT_LITERAL: return "INT_LITERAL";
        case FLOAT_LITERAL: return "FLOAT_LITERAL";
        case DOUBLE_LITERAL: return "DOUBLE_LITERAL";
        case CHAR_LITERAL: return "CHAR_LITERAL";
        case STRING_LITERAL: return "STRING_LITERAL";
        
        // Identifiers
        case ID: return "ID";
        
        // Special
        case END_OF_FILE: return "END_OF_FILE";
        case ERROR: return "ERROR";
        
        default: return "UNKNOWN";
    }
}

#endif
