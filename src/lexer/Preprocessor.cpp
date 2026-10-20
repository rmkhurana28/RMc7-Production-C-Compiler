#include "Lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

// hashmap to store values for the #define
unordered_map<string,vector<Token>> defMap;
unordered_map<string, FuncMacroConfig> funcDefMap;

// vector<string> includeSearchPaths = {
//     "/usr/include",
//     "/usr/local/include",
//     "/opt/include",
//     "/opt/local/include",

//     "/usr/include/x86_64-linux-gnu",
//     "/usr/include/aarch64-linux-gnu",
//     "/usr/include/arm-linux-gnueabihf",
//     "/usr/include/i386-linux-gnu",

//     "/opt/homebrew/include",

//     "/Library/Developer/CommandLineTools/usr/include",
//     "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include",

//     "C:/msys64/mingw64/include",
//     "C:/msys64/mingw32/include",
//     "C:/msys64/usr/include",
//     "C:/mingw64/include",
//     "C:/mingw/include"
// };

vector<string> includeSearchPaths = {
    // ==========================================
    // 1. Linux Standard System Include Paths
    // ==========================================
    "/usr/include",
    "/usr/local/include",
    "/opt/include",
    "/opt/local/include",

    // Linux Multiarch Paths
    "/usr/include/x86_64-linux-gnu",
    "/usr/include/aarch64-linux-gnu",
    "/usr/include/arm-linux-gnueabihf",
    "/usr/include/i386-linux-gnu",

    // ==========================================
    // 2. Linux GCC Internal Headers (stddef.h, stdarg.h, stdbool.h, limits.h)
    // ==========================================
    "/usr/lib/gcc/x86_64-linux-gnu/14/include",
    "/usr/lib/gcc/x86_64-linux-gnu/13/include",
    "/usr/lib/gcc/x86_64-linux-gnu/12/include",
    "/usr/lib/gcc/x86_64-linux-gnu/11/include",
    "/usr/lib/gcc/x86_64-linux-gnu/10/include",
    "/usr/lib/gcc/x86_64-linux-gnu/14/include-fixed",
    "/usr/lib/gcc/x86_64-linux-gnu/13/include-fixed",
    "/usr/lib/gcc/x86_64-linux-gnu/12/include-fixed",

    // ARM Linux GCC Internal Headers (Raspberry Pi / ARM Servers)
    "/usr/lib/gcc/aarch64-linux-gnu/14/include",
    "/usr/lib/gcc/aarch64-linux-gnu/13/include",
    "/usr/lib/gcc/aarch64-linux-gnu/12/include",
    "/usr/lib/gcc/aarch64-linux-gnu/11/include",

    // Linux Clang/LLVM Builtin Headers
    "/usr/lib/clang/18/include",
    "/usr/lib/clang/17/include",
    "/usr/lib/clang/16/include",
    "/usr/lib/clang/15/include",

    // ==========================================
    // 3. macOS Include Paths
    // ==========================================
    "/opt/homebrew/include", // Apple Silicon Homebrew
    "/Library/Developer/CommandLineTools/usr/include",
    "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include",
    "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include", // Full Xcode

    // ==========================================
    // 4. Windows (MSYS2 & MinGW) Include Paths
    // ==========================================
    "C:/msys64/ucrt64/include",  // Modern MSYS2 UCRT64 (Recommended default)
    "C:/msys64/mingw64/include", // MSYS2 MinGW64
    "C:/msys64/clang64/include", // MSYS2 Clang64
    "C:/msys64/mingw32/include", // MSYS2 MinGW32
    "C:/msys64/usr/include",     // MSYS2 POSIX
    "C:/mingw64/include",        // Standalone MinGW64
    "C:/mingw/include"           // Standalone MinGW32
};

vector<Token> Lexer::startPreprocessing(vector<Token>& tokens){
    // this func is called when # is encountered in a file, it is a preprocessor 

    

    
    // this vector will contain the final tokens after preprocessing
    vector<Token> processedTokens;
    
    size_t tokenCount = 0;
    size_t evaluated = 0;
    
    canContinueAgain:

    // copy the tokens blindly untill # or ID is found (# for obv preprocessors, and ID for pre-defined macros, they are currently stored as ID in rawTokenStream)
    /*
        addition: since the name of the #define can be anything, including some reserved keywords also, so we need to check if the token can be a valid #define name or not
        isTokenValidDefineName(tokens[evaluated]) == "" means the token can NOT be a #define name
    */
    while(evaluated < tokens.size() && tokens[evaluated].type != OP_HASH && tokens[evaluated].type != ID && isTokenValidDefineName(tokens[evaluated]) == ""){
        processedTokens.push_back(tokens[evaluated]);
        tokenCount++;
        evaluated++;
    }

    // now we found # or ID
    
    if(evaluated == tokens.size()) return processedTokens;
    
    if(tokens[evaluated].type == OP_HASH){

        // #include case
        if(evaluated+1 < tokens.size() && tokens[evaluated+1].type == PREP_INCLUDE){ 

            // #include <
            if(evaluated+2 < tokens.size() && tokens[evaluated+2].type == OP_LESS){
                // #include <fileName.h>                

                // check if alr included, skip if yes
                /*
                    currnetly it is bugged since it is hard-coded to skip 5 tokens, which can be wrong in some(many) cases
                */
                // for(size_t i=0 ; i<=this->lastAddedInLexerStackIndex ; i++){
                //     if(this->lexerStack[i].currentFile == tokens[evaluated+2].data){
                //         evaluated += 5;
                //         goto canContinueAgain;
                //     }
                // }

                {
                    // 1. First, extract the full filename between < and >
                    string filename = "";
                    size_t tempIdx = evaluated + 3;
                    while (tempIdx < tokens.size() && tokens[tempIdx].type != OP_GREATER) {
                        filename += tokens[tempIdx++].data;
                    }

                    // 2. Check if this actual filename is already in the stack
                    for (size_t i = 0; i <= this->lastAddedInLexerStackIndex; i++) {
                        if (this->lexerStack[i].currentFile == filename) {
                            evaluated = tempIdx + 1; // skip past the closing '>'
                            goto canContinueAgain;
                        }
                    }

                    // 3. Store the actual filename in the new config (not tokens[evaluated+2].data)
                    LexerConfig* file = new LexerConfig{0, 1, 1, filename, "", ""};
                    lexerStack.push_back(*file);
                    this->lastAddedInLexerStackIndex++;

                }

                // make a new lexerConfig objet to hold deatails of the new file
                // LexerConfig* file = new LexerConfig{0,1,1,tokens[evaluated+2].data,"",""};                

                // push to lexer stack
                // lexerStack.push_back(*file);

                // increment lexer stack index
                // this->lastAddedInLexerStackIndex++;

                // var potentially to hold the new source file raw (if found)
                string newSource;

                // var to store filename that is inside < and >
                string filename = "";

                // move tempIdx to token just after < 
                size_t tempIdx = evaluated + 3;

                // keep updating file name it reaches > 
                while(tempIdx < tokens.size() && tokens[tempIdx].type != OP_GREATER) {
                    filename += tokens[tempIdx++].data;
                    // tempIdx++;
                }
                
                // look for the file from the hardcoded possible locations, and update found booleon
                bool found = false;
                for (const string& path : includeSearchPaths) {
                    string fullPath = path + "/" + filename;
                    ifstream searchFile(fullPath);
                    if (searchFile.is_open()) {
                        stringstream buffer;
                        buffer << searchFile.rdbuf();
                        newSource = buffer.str();
                        searchFile.close();
                        // cout << "CASE <> | Data copied to newSource from " << fullPath << "\n";
                        found = true;
                        break;
                    }
                }

                // file not found, error
                if (!found) {
                    // cout << "Error < : Could not open " << filename << "\n";
                    // cout << "Skipping " << filename << endl;

                    // keep updating file name it reaches > 
                    while(evaluated < tokens.size() && tokens[evaluated].type != OP_GREATER) {
                        evaluated++;
                        // tempIdx++;
                    }
                    evaluated++;

                    goto canContinueAgain;
                    
                    // exit(1);a
                }

                
                // this->source = newSource;
                this->lexerStack[lastAddedInLexerStackIndex].currentSource = newSource;

                // run the tokenization of the filename that was in #include
                vector<Token> newTokenList = this->startTokenization();

                // testing it here
                newTokenList = this->startPreprocessing(newTokenList);

                // copy the tokens in the final tokenList
                for(size_t i=0 ; i<newTokenList.size() ; i++){                                        
                    // cout << "Copied\n";
                    processedTokens.push_back(newTokenList[i]);
                    tokenCount++;
                }

                // evaluated += 5;                
                // evaluated += tempIdx+1; // update evaluated for the current token list
                evaluated = tempIdx+1; // update evaluated for the current token list

                goto canContinueAgain;
                
            } else if(evaluated+2 < tokens.size() && tokens[evaluated+2].type == STRING_LITERAL){
                // #include "fileName.h"                

                // check if this file is alr included in the stack to avoid double include                           
                for(size_t i=0 ; i<=this->lastAddedInLexerStackIndex ; i++){
                    if(this->lexerStack[i].currentFile == tokens[evaluated+2].data){
                        evaluated += 3;
                        // cout << "skipping in \" case\n";
                        goto canContinueAgain;
                    }
                }
                
                // make a new lexerConfig objet to hold deatails of the new file
                LexerConfig* file = new LexerConfig{0,1,1,tokens[evaluated+2].data,"",""};
                
                // push to lexer stack
                lexerStack.push_back(*file);

                // increment lexerStack index
                this->lastAddedInLexerStackIndex++;

                // generate new source file copying the content of new header file into the sring and update it to the source vector 

                // var potentially to hold the new source file raw (if found)
                string newSource;

                // get the filename
                string filename = tokens[evaluated+2].data; 

                // remove " from filename if needed
                if (filename.length() >= 2 && filename.front() == '"' && filename.back() == '"') {
                    filename = filename.substr(1, filename.length() - 2);
                }
                
                // try to open the file directly
                ifstream inFile(filename);

                // if file is opened directly, copy it to newSource in raw form
                if (inFile.is_open()) {
                    stringstream buffer;
                    buffer << inFile.rdbuf();
                    newSource = buffer.str();
                    inFile.close();

                    // cout << "CASE \"\" | Direct file open | Data copied to newSource\n";
                } else {
                    
                    // here try to find the file from the direct include locations specified in vector includeSearchPaths
                    bool found = false;
                    for (const string& path : includeSearchPaths) {
                        string fullPath = path + "/" + filename;
                        ifstream searchFile(fullPath);
                        if (searchFile.is_open()) {
                            stringstream buffer;
                            buffer << searchFile.rdbuf();
                            newSource = buffer.str();
                            searchFile.close();
                            // cout << "CASE \"\" | Data copied to newSource from " << fullPath << "\n";
                            // cout << "Data = " << newSource << endl;
                            found = true;
                            break;
                        }
                    }

                    // file not found, error
                if (!found) {
                    // cout << "Error \" : Could not open " << filename << "\n";
                    // cout << "Skipping " << filename << endl;

                    // keep updating file name it reaches > 
                    while(evaluated < tokens.size() && tokens[evaluated].type != STRING_LITERAL) {
                        evaluated++;
                        // tempIdx++;
                    }
                    evaluated++;

                    goto canContinueAgain;
                    
                    // exit(1)
                }
                }

                // update this->srouce since tokenization reads this->source to generate tokens
                // this->source = newSource;
                this->lexerStack[lastAddedInLexerStackIndex].currentSource = newSource;
                // cout << "Data = " << this->lexerStack[lastAddedInLexerStackIndex].currentSource << endl;

                // perform tokenization on the new source
                vector<Token> newTokenList = this->startTokenization();
                
                // testing it here
                newTokenList = this->startPreprocessing(newTokenList);
                // cout << "length of newTokenList = " << newTokenList.size() << endl;

                // copy the tokens in the final tokenList
                for(size_t i=0 ; i<newTokenList.size() ; i++){                                        
                    // cout << "Copied\n";
                    processedTokens.push_back(newTokenList[i]);
                    tokenCount++;
                }

                // update evaluated for the current token list
                evaluated += 3;

                goto canContinueAgain;

                
            } else if(evaluated+2 < tokens.size() && tokens[evaluated+2].type == ID){
                //
            } else{
                cout << "Invalid token in preprocessing\n";
                exit(1);
            }
        }
        else if(evaluated+1 < tokens.size() && tokens[evaluated+1].type == PREP_DEFINE){

            // cout << "Reached here atleast " << tokens[evaluated+2].data << endl;

            // used to check the func define name validity 
            string macroKey = tokens[evaluated+2].fileName + ":" + to_string(tokens[evaluated+2].line);


            // #define func(args) value
            if(evaluated+2 < tokens.size() && funcLikeMacroSet.count(macroKey) > 0){
                // cout << "Reached inside also\n";

                // it is a func like macro, deal it accordingly

                // hold the funcName
                string funcNameValue = isTokenValidDefineName(tokens[evaluated+2]);

                // name is invalid
                if(funcNameValue == ""){
                    cout << "Preprocessing #define funcName error\n";
                    cout << "Preprocessing funcName is invalid\n";
                    exit(1);
                } 

                FuncMacroConfig config;                
                
                int idx = 1;

                // check for empty parameters
                if(evaluated+3+idx < tokens.size() && tokens[evaluated+4].type == RPAREN){
                    idx++;
                    goto skip_args;
                } else if(evaluated+3+idx < tokens.size() && tokens[evaluated+3+idx].type == OP_DOT){ // checking for variadic func as first param
                    // check for 3 conseqetive dots, if not, print error

                    // next tokens after first . strictly MUST be ..)
                    if(evaluated+3+idx+3 >= tokens.size() || tokens[evaluated+3+idx+1].type != OP_DOT || tokens[evaluated+3+idx+2].type != OP_DOT || tokens[evaluated+3+idx+3].type != RPAREN){
                        cout << "Variadic macro func error\n";
                        exit(1);
                    }

                    config.isVariadic = true;
                    idx += 4;
                    goto skip_args;
                }

                while(evaluated+3+idx < tokens.size() && tokens[evaluated+3+idx].line == tokens[evaluated+2].line){

                    string helperStr = isTokenValidDefineName(tokens[evaluated+3+idx]);

                    // if (config.args.count(helperStr) > 0) {
                    //     cout << "Duplicate macro parameter: " << helperStr << "\n";
                    //     exit(1);
                    // }

                    for (const string& existingArg : config.args) {
                        if (existingArg == helperStr) {
                            cout << "Duplicate macro parameter: " << helperStr << "\n";
                            exit(1);
                        }
                    }

                    if(helperStr == ""){
                        cout << "Func Arg Error in macro\n";
                        exit(1);
                    }

                    config.args.push_back(helperStr);

                    if(evaluated+3+idx+1 < tokens.size() && tokens[evaluated+3+idx+1].type == COMMA) {

                        // Check if the token right after comma is '.'
                        if (evaluated+3+idx+2 < tokens.size() && tokens[evaluated+3+idx+2].type == OP_DOT) {
                            // Must be followed by ..)
                            if (evaluated+3+idx+5 >= tokens.size() || 
                                tokens[evaluated+3+idx+3].type != OP_DOT || 
                                tokens[evaluated+3+idx+4].type != OP_DOT || 
                                tokens[evaluated+3+idx+5].type != RPAREN) {
                                cout << "Variadic macro func error\n";
                                exit(1);
                            }
                            config.isVariadic = true;
                            idx += 6; // skips arg, comma, . . . and )
                            break;
                        }


                        // skip current token and ,
                        idx += 2;
                        continue;
                    } else if(evaluated+3+idx+1 < tokens.size() && tokens[evaluated+3+idx+1].type == RPAREN){
                        // skip current token and )
                        idx += 2;
                        break;
                    } 
                    // else if(evaluated+3+idx+1 < tokens.size() && tokens[evaluated+3+idx+1].type == OP_DOT){
                    //     //

                    //     // next tokens after first . strictly MUST be ..)
                    //     if(evaluated+3+idx+3+1 >= tokens.size() || tokens[evaluated+3+idx+1+1].type != OP_DOT || tokens[evaluated+3+idx+2+1].type != OP_DOT || tokens[evaluated+3+idx+3+1].type != RPAREN){
                    //         cout << "Variadic macro func error\n";
                    //         exit(1);
                    //     }

                    //     config.isVariadic = true;

                    //     idx += 4;
                    //     break;
                    // } 
                    else{
                        // syntax error, neither , nor )
                        cout << "Func define parameter error\n";
                        exit(1);
                    }
                }

                skip_args:

                // insert tokens 
                while(evaluated+3+idx < tokens.size() && tokens[evaluated+3+idx].line == tokens[evaluated+2].line){
                    if(!config.isVariadic && tokens[evaluated+3+idx].data == "__VA_ARGS__"){
                        cout << "__VA_ARGS__ not allowed for non-variadic macro func decl\n";
                        exit(1);
                    }
                    config.tokList.push_back(tokens[evaluated+3+idx++]);
                }

                // check for dublicates now
                if(funcDefMap.count(funcNameValue) > 0){
                    if(config != funcDefMap[funcNameValue]){
                        cout << "Redefinition of fnc macro\n";
                        exit(1);
                    } 
                    else {
                        // it alr exists, just skip it
                        evaluated += 3 + idx;
                    }
                } else{
                    funcDefMap[funcNameValue] = config;
                    evaluated += 3 + idx;
                }

                goto canContinueAgain;
                
            }

            // here, it is obj-like macor, deal it accordingly

            // #define name value

            // this string will contain define value
            string defValue = "";

            // check if the name is valid to be used a define
            // if yes, this func will return the name as string
            // if no, this func will return ""
            string nameValue = isTokenValidDefineName(tokens[evaluated+2]);

            // name is invalid
            if(nameValue == ""){
                cout << "Preprocessing #define name error\n";
                cout << "Preprocessing name is invalid\n";
                exit(1);
            } 

            // name is valid now, proceed
            
            // check for empty value
            if(evaluated+3 < tokens.size() && tokens[evaluated+3].line != tokens[evaluated+2].line){
                // it is #define without value
                // #define name

                // check if it alr exists with something in value, if yes, show error, if not, add it
                if(defMap.count(nameValue) > 0){
                    if(!defMap[nameValue].empty()){
                        cout << "Redefinition (different) of " << nameValue << endl;
                        exit(1);
                    } 
                }

                // insert the defMap in the map, but keep the value vector as empty
                defMap[nameValue] = {};
                
                // skip # and define and name
                evaluated += 3;
                goto canContinueAgain;
            }

            // now it is #define name value

            int nxtIdx = 1;
            vector<Token> helperVector;
            while(evaluated+2+nxtIdx < tokens.size() && tokens[evaluated+2+nxtIdx].line == tokens[evaluated+2].line){

                /*
                    here, need to check if the token that we r adding to the value part is valid or not, we can do it either for each token as we add it or we can do it direclty by calling a checker func after all the tokens are added, 
                    checking after adding all will be btr since it will save the func-call stack calls as func will be called only once
                */

                // push token to the vector
                helperVector.push_back(tokens[evaluated+2+nxtIdx++]);
            }

            // check if the tokens added in the value part is valid or not
            if(!isValuePartValid(helperVector)){
                cout << "Value part is NOT valid\n";
                exit(1);
            }

            // check if the defName alr exists or not
            // if exists, it must have same value, if not, show the error
            if(defMap.count(nameValue) > 0){
                if(defMap[nameValue] != helperVector) {
                // if(1) {
                    cout << "Redefinition (different) of " << nameValue << endl;
                    exit(1);
                } else{
                    evaluated += 2 + nxtIdx;
                    goto canContinueAgain;
                }
            } 

            // now, nameValue doesnt alr exists in defMap, add it
            defMap[nameValue] = helperVector;
            evaluated += 2 + nxtIdx;
            goto canContinueAgain;

            // push to token array for now
            // processedTokens.push_back(tokens[evaluated]);
            // tokenCount++;
            // evaluated++;
            // goto canContinueAgain;
        }
        else {
            // push to token array for now
            processedTokens.push_back(tokens[evaluated]);
            tokenCount++;
            evaluated++;
            goto canContinueAgain;
        }
    } else if(isTokenValidDefineName(tokens[evaluated]) != ""){
        /* 
            now it can be a #define name, so we need to check if it is or not, 
            if it is not, add it to the processed tokens vector and move on
            if it is , then expand it accoridngly
        */

        handleDefine(tokens, processedTokens , tokens[evaluated] , tokenCount , evaluated);
        
        goto canContinueAgain;

        
    }     
    else{        
        // push to token array for now
        processedTokens.push_back(tokens[evaluated]);
        tokenCount++;
        evaluated++;
        goto canContinueAgain;
    }

    return processedTokens;
}

void Lexer::handleDefine(vector<Token>& tokens , vector<Token>& processedTokens , Token& currToken , size_t& tokenCount , size_t& evaluated){
    // here, currToken = tokens[evaluated]

    static unordered_set<string> nested_set;

    if(nested_set.count(currToken.data) > 0) {
        processedTokens.push_back(currToken);
        tokenCount++;
        // if(nested_set.size() == 1) evaluated++;
        
        return;
    }
    
    nested_set.insert(currToken.data);

    if((defMap.count(currToken.data) > 0 && funcDefMap.count(currToken.data) == 0)
        || (defMap.count(currToken.data) > 0 && funcDefMap.count(currToken.data) > 0 && (evaluated+1 < tokens.size() || tokens[evaluated+1].type != LPAREN))){
        // it is in defMap and not in func def map, hence it is obj expansion          
        // it is in defMap and also in func def map, but doesnt have ( just after name, hence it is obj expansion       

        for(auto repToken : defMap[currToken.data]){
            if(isTokenValidDefineName(repToken) != ""){
                handleDefine(tokens,processedTokens,repToken, tokenCount , evaluated);
            } else{
                processedTokens.push_back(repToken);
                tokenCount++;                                            
            }
        }

        if(nested_set.size() == 1) evaluated++;
        
        
    } else if(defMap.count(currToken.data) == 0 && funcDefMap.count(currToken.data) > 0 && evaluated+1 < tokens.size() && tokens[evaluated+1].type == LPAREN) {
        // it is not in defMap, it is in func def map, and also has ( right after name (next token), hence it is func expansion

        

        // push to token array for now
        processedTokens.push_back(currToken);
        tokenCount++;
        if(nested_set.size() == 1) evaluated++;
        return;

    } else{
        // it is just a simple token, ignore it and add to processed token , thats it
        // push to token array for now
        processedTokens.push_back(currToken);
        tokenCount++;
        if(nested_set.size() == 1) evaluated++;

        nested_set.erase(currToken.data);

        return;
    }

    nested_set.erase(currToken.data);
    return;
}


bool Lexer::isValuePartValid(vector<Token>& list){
    // single # not allowed
    // double ## not allowed at extremes
    // consequtive double hashes #### not allowed without tokens bw them
    // __VA_ARGS__ and __VA_OPT__ not allowed
    
    for(size_t i=0 ; i<list.size() ; i++){
        
        if((i == 0 && list[i].type == OP_HASH )|| // prevents # as first token, hence avoids ## at left-extreme
            (i == list.size()-1 && list[i].type == OP_HASH)){// prevents # as last token, hence avoides ## at right-extreme         
                return false;
        }

        if(list[i].type == OP_HASH){
            // must be a double hash
            if(list[i-1].type != OP_HASH && list[i+1].type != OP_HASH) return false; // no need to do boundary check since both extreme cases are alr covered

            // must not be a 3 or more consequtive hashes
            if(list[i-1].type == OP_HASH && list[i+1].type == OP_HASH) return false; // reject if left and right both are hashes

        }

        if(list[i].data == "__VA_ARGS__" || list[i].data == "__VA_OPT__") return false;
    }

    return true;
}