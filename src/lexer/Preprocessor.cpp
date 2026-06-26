#include "Lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

vector<string> includeSearchPaths = {
    "/usr/include",
    "/usr/local/include",
    "/opt/include",
    "/opt/local/include",

    "/usr/include/x86_64-linux-gnu",
    "/usr/include/aarch64-linux-gnu",
    "/usr/include/arm-linux-gnueabihf",
    "/usr/include/i386-linux-gnu",

    "/opt/homebrew/include",

    "/Library/Developer/CommandLineTools/usr/include",
    "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include",

    "C:/msys64/mingw64/include",
    "C:/msys64/mingw32/include",
    "C:/msys64/usr/include",
    "C:/mingw64/include",
    "C:/mingw/include"
};

vector<Token> Lexer::startPreprocessing(vector<Token>& tokens){
    // this func is called when # is encountered in a file, it is a preprocessor 

    
    
    vector<Token> processedTokens;
    
    size_t tokenCount = 0;
    size_t evaluated = 0;
    
    canContinueAgain:

    // copy the tokens blindly untill # or ID is found (# for obv preprocessors, and ID for pre-defined macros, they are currently stored as ID in rawTokenStream)
    while(evaluated < tokens.size() && tokens[evaluated].type != OP_HASH && tokens[evaluated].type != ID){
        processedTokens.push_back(tokens[evaluated]);
        tokenCount++;
        evaluated++;
    }

    // now we found # or ID
    
    if(evaluated == tokens.size()) return processedTokens;
    
    if(tokens[evaluated].type == ID){
        //

        // push to token array for now
        processedTokens.push_back(tokens[evaluated]);
        tokenCount++;
        evaluated++;
        goto canContinueAgain;
    } else if(tokens[evaluated].type == OP_HASH){
        if(evaluated+1 < tokens.size() && tokens[evaluated+1].type == PREP_INCLUDE){
            if(evaluated+2 < tokens.size() && tokens[evaluated+2].type == OP_LESS){
                //
            } else if(evaluated+2 < tokens.size() && tokens[evaluated+2].type == STRING_LITERAL){
                // #include "fileName.h"

                // verify if file exists
                // assuming it exists, lets proceed rn


                // check if this file is alr included in the stack                
            
                for(size_t i=0 ; i<=this->lastAddedInLexerStackIndex ; i++){
                    if(this->lexerStack[i].currentFile == tokens[evaluated+2].data){
                        evaluated += 3;
                        goto canContinueAgain;
                    }
                }

                
                LexerConfig* file = new LexerConfig{0,1,1,tokens[evaluated+2].data,""};
                
                lexerStack.push_back(*file);
                this->lastAddedInLexerStackIndex++;

                // generate new source file copying the content of new header file into the sring and update it to the source vector 

                string newSource;
                string filename = tokens[evaluated+2].data;
                if (filename.length() >= 2 && filename.front() == '"' && filename.back() == '"') {
                    filename = filename.substr(1, filename.length() - 2);
                }
                
                ifstream inFile(filename);
                if (inFile.is_open()) {
                    stringstream buffer;
                    buffer << inFile.rdbuf();
                    newSource = buffer.str();
                    inFile.close();

                    cout << "Data copied to newSource\n";
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
                            cout << "Data copied to newSource from " << fullPath << "\n";
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        cout << "Error: Could not open " << filename << "\n";
                        exit(1);
                    }
                }

                this->source = newSource;

                vector<Token> newTokenList = this->startTokenization();

                for(size_t i=0 ; i<newTokenList.size() ; i++){                    
                    
                    // cout << "Copied\n";
                    processedTokens.push_back(newTokenList[i]);
                    tokenCount++;
                }

                evaluated += 3;

                goto canContinueAgain;

                
            } else if(evaluated+2 < tokens.size() && tokens[evaluated+2].type == ID){
                //
            } else{
                cout << "Invalid token in preprocessing\n";
                exit(1);
            }
        }
    } else{
        // do nothing
    }

    return processedTokens;
}
