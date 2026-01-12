#include "Helper.h"
#include "DeclarationNode.h"
#include "Parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// Implementation of helper classes
dataTypeHolder::dataTypeHolder(Parser& p) : parser(p) {
    // Reference initialized in initialization list
}

void dataTypeHolder::getDataType(){
    bool firstStarFound = false;
    bool isBaseTypeFound = false;
    
    TokenType latestType;

    evaluate_again : 

    // get data type properties now
    while(this->parser.isThisTokenDataTypeOrPropToken(this->parser.tokens[this->parser.currentPos]) || this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // parsing till data type prop or star
        if(this->parser.isThisTokenDataBaseTypeToken(this->parser.tokens[this->parser.currentPos])){ // base type
            isBaseTypeFound = true;
            this->baseTypeArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSignModifierToken(this->parser.tokens[this->parser.currentPos])){ // sign
            if(firstStarFound){ // sign modifier token not allowed after first * is found
                cout << "Error: Sign modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }
            this->signModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSizeModifierToken(this->parser.tokens[this->parser.currentPos])){ // size
            if(firstStarFound){ // size modifier token not allowed after first * is found
                cout << "Error: Size modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }
            this->sizeModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenTypeQualifierToken(this->parser.tokens[this->parser.currentPos])){ // type
            this->typeQualifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenStorageClassToken(this->parser.tokens[this->parser.currentPos])){ // storage
            if(firstStarFound){ // storage class token not allowed after first * is found
                cout << "Error: Storage class not allowed after pointer (*) declaration" << endl;
                exit(1);
            }
            this->storageClassArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // *
            if(!isBaseTypeFound){ // * found before base type was specified
                cout << "Error: Pointer (*) found before base type declaration" << endl;
                exit(1);
            }
            firstStarFound = true;
            if(!isPrevTokenValidForCurrentStar(latestType)){ // * not allowed after prev tokenType
                cout << "Error: Invalid token before pointer (*) declaration" << endl;
                exit(1);
            }
            starData tempData({0,latestType});            
            while(this->parser.tokens[this->parser.currentPos].type == OP_STAR){
                tempData.numOfStars++;
                this->parser.currentPos++;
            }
            this->starDataArray.push_back(tempData);
            continue;
            
            
        }

        this->parser.currentPos++;
    }

    // now we found some other token except standard data decl prop or *

    // check if current token is struct/enum/union keyword token
    if(this->parser.isThisTokenStructUnionEnumToken(this->parser.tokens[this->parser.currentPos])){ // struct/enum/union

        if(this->parser.tokens[this->parser.currentPos+1].type != ID){ // ID always expected after struct/enum/union keyword
            cout << "Error: Expected ID after struct/enum/union\n" << endl;
            exit(1);
        }
        
        // ID is found now
        this->trKeywordArray.push_back(this->parser.tokens[this->parser.currentPos].type); // add struct/enum/union keyword to the tr keyword array
        this->parser.currentPos++; // advance 1 token                

        // now, we check if the ID is there anywhere in TR hashmap, if not present, error, if present, add to TR base array
        if(this->parser.isThisStringPresentAsKeyInTrHm(this->parser.tokens[this->parser.currentPos].data)){ // found in TR hashmap

            // check if it has the correct keyword in the TR hashmap accordign to the given keyword among struct/enum/union
            if((this->trKeywordArray.back() == KEYWORD_STRUCT && this->parser.typeRegisry[this->parser.tokens[this->parser.currentPos].data] != "struct") ||
               (this->trKeywordArray.back() == KEYWORD_UNION && this->parser.typeRegisry[this->parser.tokens[this->parser.currentPos].data] != "union") ||
               (this->trKeywordArray.back() == KEYWORD_ENUM && this->parser.typeRegisry[this->parser.tokens[this->parser.currentPos].data] != "enum")){
                cout << "Error: Mismatched type registry entry for " << this->parser.tokens[this->parser.currentPos].data << endl;
                exit(1);
            }
            
            // validation completed, add to TR array
            this->trBaseArray.push_back(this->parser.tokens[this->parser.currentPos].data);
            this->parser.currentPos++;
            goto evaluate_again;
        } else{ // ID must be in TR to be valid
            cout << "Error: Expected correct ID after struct/enum/union\n" << endl;
            exit(1);
        }
    }


    // if the current token is NOT struct/enum/union (alr done in prev if-cond) AND check if current token is ID or not
    if(this->parser.tokens[this->parser.currentPos].type != ID) return; // return if not ID , match completed    
    
    // current token is ID now

    // check if this ID is present in the key side in TD hashmap
    if(this->parser.isThisStringPresentAsKeyInTdMap(this->parser.tokens[this->parser.currentPos].data)){ // found in TD hashmap

        // check if it is actually specifying the data type or it is actualy an ID        
        if(!this->parser.isCurrentIdValidTdAlias()) return; // return if ID is just a var/func name (found using lookup algo) , match completed

        // ID is valid TD entry, push it
        this->tdNew.push_back(this->parser.tokens[this->parser.currentPos].data);
        this->parser.currentPos++;
        goto evaluate_again;
    }        

    return; // validation completed
}

int dataTypeHolder::isCurrentTypeValid(){

    // -1 means NOT valid 
    // 0 means valid for both var and func
    // 1 means valid ONLY for var
    // 2 means valid ONLY for func

    // open up TD entry if exists and update the order arrays (do NOT reset them, keep them for future use and validation, keep the TD entry also)
    if(this->tdNew.size() > 0){
        // expand all typedefs and populate the arrays
        for(const string& typedefName : this->tdNew){
            // get the vector of strings from tdMap
            vector<string> expandedTokens = this->parser.tdMap[typedefName];
            
            // process each token string and add to appropriate arrays
            // use index-based loop to handle consecutive stars properly
            for(size_t i = 0; i < expandedTokens.size(); i++){
                const string& tokenStr = expandedTokens[i];
                
                // check if it's a base type
                if(tokenStr == "int" || tokenStr == "char" || tokenStr == "void" || 
                   tokenStr == "float" || tokenStr == "double" || tokenStr == "bool"){
                    if(tokenStr == "int") this->baseTypeArray.push_back(KEYWORD_INT);
                    else if(tokenStr == "char") this->baseTypeArray.push_back(KEYWORD_CHAR);
                    else if(tokenStr == "void") this->baseTypeArray.push_back(KEYWORD_VOID);
                    else if(tokenStr == "float") this->baseTypeArray.push_back(KEYWORD_FLOAT);
                    else if(tokenStr == "double") this->baseTypeArray.push_back(KEYWORD_DOUBLE);
                    else if(tokenStr == "bool") this->baseTypeArray.push_back(KEYWORD_BOOL);
                }
                // check if it's a sign modifier
                else if(tokenStr == "signed" || tokenStr == "unsigned"){
                    if(tokenStr == "signed") this->signModifiersArray.push_back(KEYWORD_SIGNED);
                    else if(tokenStr == "unsigned") this->signModifiersArray.push_back(KEYWORD_UNSIGNED);
                }
                // check if it's a size modifier
                else if(tokenStr == "short" || tokenStr == "long"){
                    if(tokenStr == "short") this->sizeModifiersArray.push_back(KEYWORD_SHORT);
                    else if(tokenStr == "long") this->sizeModifiersArray.push_back(KEYWORD_LONG);
                }
                // check if it's a type qualifier
                else if(tokenStr == "const" || tokenStr == "volatile" || tokenStr == "restrict"){
                    if(tokenStr == "const") this->typeQualifiersArray.push_back(KEYWORD_CONST);
                    else if(tokenStr == "volatile") this->typeQualifiersArray.push_back(KEYWORD_VOLATILE);
                    else if(tokenStr == "restrict") this->typeQualifiersArray.push_back(KEYWORD_RESTRICT);
                }
                // check if it's a storage class
                else if(tokenStr == "static" || tokenStr == "extern" || tokenStr == "auto" || 
                        tokenStr == "register" || tokenStr == "typedef"){
                    if(tokenStr == "static") this->storageClassArray.push_back(KEYWORD_STATIC);
                    else if(tokenStr == "extern") this->storageClassArray.push_back(KEYWORD_EXTERN);
                    else if(tokenStr == "auto") this->storageClassArray.push_back(KEYWORD_AUTO);
                    else if(tokenStr == "register") this->storageClassArray.push_back(KEYWORD_REGISTER);
                    else if(tokenStr == "typedef") this->storageClassArray.push_back(KEYWORD_TYPEDEF);
                }
                // check if it's a star (pointer) - handle both "*" and "**", "***" etc
                else if(tokenStr.length() > 0 && tokenStr[0] == '*'){
                    // could be "*" or "**" or "***" stored as single string
                    int starCount = 0;
                    TokenType typeBeforeStar = KEYWORD_INT; // default placeholder
                    
                    // determine what came before this star group
                    if(i > 0){
                        const string& prevToken = expandedTokens[i-1];
                        if(prevToken == "int") typeBeforeStar = KEYWORD_INT;
                        else if(prevToken == "char") typeBeforeStar = KEYWORD_CHAR;
                        else if(prevToken == "void") typeBeforeStar = KEYWORD_VOID;
                        else if(prevToken == "float") typeBeforeStar = KEYWORD_FLOAT;
                        else if(prevToken == "double") typeBeforeStar = KEYWORD_DOUBLE;
                        else if(prevToken == "bool") typeBeforeStar = KEYWORD_BOOL;
                        else if(prevToken == "const") typeBeforeStar = KEYWORD_CONST;
                        else if(prevToken == "volatile") typeBeforeStar = KEYWORD_VOLATILE;
                        else if(prevToken == "restrict") typeBeforeStar = KEYWORD_RESTRICT;
                        else if(prevToken == "signed") typeBeforeStar = KEYWORD_SIGNED;
                        else if(prevToken == "unsigned") typeBeforeStar = KEYWORD_UNSIGNED;
                        else if(prevToken == "short") typeBeforeStar = KEYWORD_SHORT;
                        else if(prevToken == "long") typeBeforeStar = KEYWORD_LONG;
                        else if(prevToken == "struct") typeBeforeStar = KEYWORD_STRUCT;
                        else if(prevToken == "union") typeBeforeStar = KEYWORD_UNION;
                        else if(prevToken == "enum") typeBeforeStar = KEYWORD_ENUM;
                        // if prevToken is a struct/union/enum tag name, keep KEYWORD_INT as placeholder
                    }
                    
                    // if current string is multi-star like "**", count all stars in it
                    for(char c : tokenStr){
                        if(c == '*') starCount++;
                    }
                    
                    // count consecutive single "*" strings that immediately follow (no qualifiers between)
                    size_t j = i + 1;
                    while(j < expandedTokens.size() && expandedTokens[j] == "*"){
                        starCount++;
                        j++;
                    }
                    i = j - 1; // adjust index to skip all counted stars
                    
                    // create ONE starData entry with correct count for this consecutive group
                    starData tempData({starCount, typeBeforeStar});
                    this->starDataArray.push_back(tempData);
                }
                // check if it's struct/union/enum keyword
                else if(tokenStr == "struct" || tokenStr == "union" || tokenStr == "enum"){
                    if(tokenStr == "struct") this->trKeywordArray.push_back(KEYWORD_STRUCT);
                    else if(tokenStr == "union") this->trKeywordArray.push_back(KEYWORD_UNION);
                    else if(tokenStr == "enum") this->trKeywordArray.push_back(KEYWORD_ENUM);
                }
                // otherwise it might be a struct/union/enum tag name or nested typedef
                else {
                    // check if this is a known type registry entry (struct/union/enum tag)
                    if(this->parser.isThisStringPresentAsKeyInTrHm(tokenStr)){
                        this->trBaseArray.push_back(tokenStr);
                    }
                    // check if it's another typedef that needs recursive expansion
                    else if(this->parser.isThisStringPresentAsKeyInTdMap(tokenStr)){
                        // recursively expand this nested typedef
                        vector<string> nestedTokens = this->parser.tdMap[tokenStr];
                        // replace current typedef name with its expansion
                        expandedTokens.erase(expandedTokens.begin() + i); // remove typedef name
                        // insert expanded tokens at current position
                        for(size_t k = 0; k < nestedTokens.size(); k++){
                            expandedTokens.insert(expandedTokens.begin() + i + k, nestedTokens[k]);
                        }
                        // reprocess this position with the first expanded token
                        i--;
                    }
                    // if not recognized at all, might be error - silently ignore for now
                    // semantic analyzer will catch if it's genuinely invalid
                }
            }
        }
    }

    // number of storage class prop can be 0-1
    if(this->storageClassArray.size() > 1){
        cout << "Error: Multiple storage class specifiers not allowed" << endl;
        return -1;        
    }

    // number of type qualifiers prop can be 0-3 (all unique)
    if(this->typeQualifiersArray.size() > 3){
        cout << "Error: Too many type qualifiers" << endl;
        return -1;        
    }        
    if(this->typeQualifiersArray.size() > 0){ // checking for uniqueness
        bool hasConst = false, hasVolatile = false, hasRestrict = false;
        for(TokenType qualifier : this->typeQualifiersArray){
            if(qualifier == KEYWORD_CONST){
                if(hasConst) return -1; // duplicate const
                hasConst = true;
            } else if(qualifier == KEYWORD_VOLATILE){
                if(hasVolatile) return -1; // duplicate volatile
                hasVolatile = true;
            } else if(qualifier == KEYWORD_RESTRICT){
                if(hasRestrict) return -1; // duplicate restrict
                hasRestrict = true;
            }
        }
    }

    bool sizeOrSignPresent = false; // used to assume base type as int if absent
    
    // number of sign modifiers prop can be 0-1
    if(this->signModifiersArray.size() > 1){
        cout << "Error: Multiple sign modifiers not allowed" << endl;
        return -1;        
    } else if(this->signModifiersArray.size() == 1) sizeOrSignPresent = true;
    
    // number of size modifiers prop can be 0-2 (short , long , long long)
    if(this->sizeModifiersArray.size() > 2){
        cout << "Error: Too many size modifiers" << endl;
        return -1;        
    }
    if(this->sizeModifiersArray.size() == 2){
        if(this->sizeModifiersArray[0] == KEYWORD_SHORT || this->sizeModifiersArray[1] == KEYWORD_SHORT){
            cout << "Error: 'short' cannot be combined with other size modifiers" << endl;
            return -1;
        }
        if(this->sizeModifiersArray[0] != KEYWORD_LONG || this->sizeModifiersArray[1] != KEYWORD_LONG){
            cout << "Error: Only 'long long' is valid for two size modifiers" << endl;
            return -1;
        }        
    }
    if(!sizeOrSignPresent && this->sizeModifiersArray.size() > 0) sizeOrSignPresent = true;

    // stars validation alr done while collecting starData

    // number of struct/enum/union type registry keywords can be 0-1
    if(this->trKeywordArray.size() > 1){
        cout << "Error: Multiple struct/enum/union type registry keywords not allowed" << endl;
        return -1;        
    }
    // size of trKeywordArray and trBaseArray must be same
    if(this->trKeywordArray.size() != this->trBaseArray.size()){
        cout << "Error: Mismatched struct/enum/union type registry keyword and base type entries" << endl;
        return -1;        
    }
    

    // base type check: exactly one base type required (either from standard base types or from type registry)
    if(this->baseTypeArray.size() + this->trBaseArray.size() != 1){
        if((this->baseTypeArray.size() + this->trBaseArray.size() == 0) && sizeOrSignPresent) {
            this->baseTypeArray.push_back(KEYWORD_INT); // add base type as int
        } else{
            cout << "Error: Exactly one base type required" << endl;
            return -1;        
        }
        
    }  

    // number of data type prop check completed, lets proceed to actual validation of data type prop with the base type

    if(this->baseTypeArray.size() == 1 && this->baseTypeArray.back() == KEYWORD_INT){ // base type is int
        if(this->storageClassArray.size() == 0) return 0; // if no prop of storage class , valid for both var and func
        else{
            if(this->storageClassArray.back() == KEYWORD_AUTO || this->storageClassArray.back() == KEYWORD_REGISTER) return 1; // auto/register NOT allowed for functions
            
            return 0; // always valid
        }
    } else if(this->baseTypeArray.size() == 1 && this->baseTypeArray.back() == KEYWORD_CHAR){ // base type is char
        if(this->sizeModifiersArray.size() != 0) return -1; // size modifiers NOT allowed for char
        
        if(this->storageClassArray.size() > 0 && (this->storageClassArray.back() == KEYWORD_AUTO || this->storageClassArray.back() == KEYWORD_REGISTER)) return 1; // auto/register NOT allowed for functions
            
        return 0; // always valid
    } else if(this->baseTypeArray.size() == 1 && this->baseTypeArray.back() == KEYWORD_VOID){ // base type is void
        if(this->signModifiersArray.size() > 0 || this->sizeModifiersArray.size() > 0) return -1; // sign and size modifiers NOT allowed for void base data type

        bool withPointor = false;
        for(int i=0 ; i<this->starDataArray.size() ; i++){
            if(this->starDataArray[i].typeBeforeStar == KEYWORD_VOID){
                withPointor = true;
                break;
            }
        }

        if(this->typeQualifiersArray.size() == 0){ // type qualifiers NOT present 
        
            if(this->storageClassArray.size() > 0 && (this->storageClassArray.back() == KEYWORD_AUTO || this->storageClassArray.back() == KEYWORD_REGISTER)){ // auto/register NOT allowed for func
                if(!withPointor) return -1; // * required for var

                return 1; // valid only for var
            }

            if(!withPointor){ // * required for var
                return 2; // valid only for func
            }

            return 0; // valid for both
        } else{
            if(!withPointor){ // * required if type qualifiers used
                return -1;
            }

            return 1; // valid only for var (rule)
        }
        
    } else{ // base type is either float/double/struct/enum/union        
        if(this->signModifiersArray.size() > 0 || this->sizeModifiersArray.size() > 0){ // sign and size modifiers NOT allowed for these base types
            if(this->baseTypeArray.size() != 1 || this->baseTypeArray.front() != KEYWORD_DOUBLE) return -1; // base type is NOT double, so return -1 , invalid

            // base type is double now
            if(this->sizeModifiersArray.size() != 1 || this->sizeModifiersArray.front() != KEYWORD_LONG) return -1; // invalid, return -1

            if(this->signModifiersArray.size() != 0) return -1;

            // base type is double + size modifier is long, this is valid , we can proceed
        } 

        if(this->storageClassArray.size() > 0 && (this->storageClassArray.back() == KEYWORD_AUTO || this->storageClassArray.back() == KEYWORD_REGISTER)){ // auto register NOT allowed for func
            return 1; // valid only for var
        }

        return 0; // valid for both

        
    }

    
}

bool dataTypeHolder::isPrevTokenValidForCurrentStar(TokenType prevTokenType){
    if(prevTokenType == KEYWORD_CONST || prevTokenType == KEYWORD_VOLATILE || prevTokenType == KEYWORD_RESTRICT || prevTokenType == KEYWORD_INT || prevTokenType == KEYWORD_CHAR || prevTokenType == KEYWORD_BOOL || prevTokenType == KEYWORD_FLOAT || prevTokenType == KEYWORD_DOUBLE || prevTokenType == KEYWORD_VOID || prevTokenType == KEYWORD_SHORT || prevTokenType == KEYWORD_LONG || prevTokenType == KEYWORD_SIGNED || prevTokenType == KEYWORD_UNSIGNED){
        return true;
    }

    return false;
}

