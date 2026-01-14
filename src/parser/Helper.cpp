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

// Implementation of varNameHolder class    
varNameHolder::varNameHolder(Parser& p) : parser(p) {
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
            starData tempData;
            if(latestType == ID){ // base type found was TR/TD
                tempData = {0,HELPER_TOKEN};            
            } else{
                tempData = {0,latestType};            
            }            
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
            latestType = ID;
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
        latestType = ID;
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

void varNameHolder::getVarName(dataTypeHolder& typeHolder){
    bool static idFound = false; // flag to check if first id is found
    short static unsigned bracketStackCount = 0; // breacket stack counter
    short static initBrackCount = -1; // used for evaluating pointors
    short addStarCount = 0; // pointor numbers

    bool static isFirstVar = true; // flag if the var is first in multiple decl

    bool gotoHelper = false; // helper flag
    bool gotoHelper2 = false; // helper flag2

    Token current = this->parser.getCurrentToken(); // set the current properly
        
    short indexIfExist; // helper flag 
    

    while(current.type != SEMICOLON && current.type != OP_ASSIGN && current.type != COMMA){ // var name ends when next token is ; or = or ,
        if(current.type == ID){ // actaul var name

            cameFromPointorSide: // label used when jumping from pointor to ID

            // generate the name prop struct and push to array
            varNameProp temp;
            temp.type = VAR_NAME;
            temp.varName = current.data;
            this->namePropArray.push_back(temp);

            if(gotoHelper){ // if came here usng goto jump, use it again to go back to pointor section
                idFound = true; // flag to know the id is found
                goto cameBackFromIdSide;
            }

            idFound = true; // flag to know the id is found
        } else if(current.type == OP_STAR){ // pointor 
            if(idFound){ // stars NOT allowed after var name is found
                cout << "* not allowed after var name is alr defined\n";
                exit(1);
            }
            // update the var decl node

            // get the base type of var
            TokenType baseType;
            if(typeHolder.baseTypeArray.size() == 1){
                baseType = typeHolder.baseTypeArray.front();
            } else if(typeHolder.trBaseArray.size() == 1){
                baseType = HELPER_TOKEN;
            } else{
                cout << "Unknown error of base type\n";
                exit(1);
            }

            // check if base type alr has some stars in starData (Valid only for first variable decl)
            indexIfExist = -1;
            for(int i=0 ; i<typeHolder.starDataArray.size() ; i++){
                if(typeHolder.starDataArray[i].typeBeforeStar == baseType && typeHolder.starDataArray[i].numOfStars > 0){
                    indexIfExist = i;
                    break;
                }
            }

            // count the number of continous stars present
            addStarCount = 0;
            while(current.type == OP_STAR){
                addStarCount++;
                current = this->parser.tokens[++this->parser.currentPos];
            }
            
            if(indexIfExist == -1){ // base type doesnt alr exist in starData array
                starData tempStarData({addStarCount , baseType}); // generate a starData object 
                typeHolder.starDataArray.push_back(tempStarData); // add this object to starData array
            } else{ // base type alr exist in starData array
                typeHolder.starDataArray[indexIfExist].numOfStars += addStarCount; // update the number of stars in the starData
                addStarCount = typeHolder.starDataArray[indexIfExist].numOfStars;
            }            
            
            // updation is done on the data type side
            // now need to update on the name side

            checkAgain: // label used for pointor evaluatin jumps

            if(current.type == LPAREN){ // ( present after *
                initBrackCount = bracketStackCount; // store the number of nested brackets 
            } else if(current.type == ID){ // var name after *

                if(gotoHelper){ // 2 ID found continously after *
                    cout << "New error\n";
                    exit(1);
                }

                // generate for ID first, then check the next one
                
                gotoHelper = true; // helper flag to indicate we found id after *

                goto cameFromPointorSide; // go to evaluate ID before pointor
                cameBackFromIdSide: // label to come back from ID section

                current = this->parser.tokens[++this->parser.currentPos]; // advance 1 token , skip ID
                
                goto checkAgain; // check the next token after ID
                
            } else if(current.type == LBRACKET){ // [ present after *
                gotoHelper2 = true; // helper flag to indicate [ found after *
                goto cameAgainFromPointor; // go to evaluate [ token first
                cameBackFromLbrakSide: // label to come back from [ section

                current = this->parser.tokens[this->parser.currentPos]; // update current, now is equal to ]

                goto checkAgain; // check the next token 
            } else if(current.type == RBRACKET){ // ] present after *
                if(gotoHelper2){ // came from the [ evaluation section jump , to evaluate pointor
                    // check next token

                    if(this->parser.tokens[this->parser.currentPos+1].type == LBRACKET){ // token next to ] is [
                        current = this->parser.tokens[++this->parser.currentPos]; // skip ] and proceed
                        goto cameAgainFromPointor; // go to evaluate [ token first
                    } else if(this->parser.tokens[this->parser.currentPos+1].type == RPAREN){ // token next to ] is )
                        // next token is closing braket, we can safely evaluate pointor at this point
                        varNameProp temp;
                        temp.type = POINTOR;
                        temp.numPointor = addStarCount;
                        this->namePropArray.push_back(temp);
                        addStarCount = -1; // reset the addStarCounter 
                        
                    } else if(this->parser.tokens[this->parser.currentPos+1].type == SEMICOLON || this->parser.tokens[this->parser.currentPos+1].type == OP_ASSIGN || this->parser.tokens[this->parser.currentPos+1].type == COMMA){ // token next to ] is ; = ,
                        // we can safely evaluate pointor at this point, but dont advance the current 2 times (required to end the parent while loop)
                        varNameProp temp;
                        temp.type = POINTOR;
                        temp.numPointor = addStarCount;
                        this->namePropArray.push_back(temp);
                        addStarCount = -1; // reset the addStarCounter
                        
                        current = this->parser.tokens[++this->parser.currentPos]; // skipped ] and kept current as ; = ,
                    } else if(this->parser.tokens[this->parser.currentPos+1].type == LPAREN){ // token after ] is (
                        // new bracket started after ] , will be evaluated first, and then pointor will be added after that is closed
                        initBrackCount = bracketStackCount; // store the number of nested brackets at this point (used when closng PAREN is found, to evaluate stars)
                    } 
                    else{
                        cout << "Nested new error\n";
                        exit(1);
                    }
                } else{
                    cout << "New error haha\n";
                    exit(1);
                }
            } 
            else if(current.type == SEMICOLON || current.type == OP_ASSIGN || current.type == COMMA){ // next token is either ; = ,
                            
                if(gotoHelper2 && !gotoHelper){ // error coz we found [ , but NOT varName so far
                    // error
                } else if(gotoHelper){ // we reached end of this var name, can safely evaluate stars now (ex: int num1 , **num2; || reached here on num2)
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1; // reset addStarCount
                } 
                else{
                    continue; // NOT advancing so that the parent while loop can end with current token
                }
            } else if(current.type == RPAREN) { // next token is )
                // will put the stars but not advance , so tha it doesnt brek the next section of the code
                if(gotoHelper){ // if ID is alr found and we jumped in order to evalute stars
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1; // reset addStarCount
                }
            }
            else{
                cout << "Unknown token error in star\n";
                exit(1);
            }

            continue; // using to avoid the advance at the end of while loop

            

        } else if(current.type == LBRACKET){ // index [

            cameAgainFromPointor: // label used to jump from pointor section to here

            // generating data to add in the array
            varNameProp temp;
            temp.type = ARRAY;
            
            current = this->parser.tokens[++this->parser.currentPos]; // advance 1 token , skip [         

            /* temp.astData = expr; */ // evaluate the index in the brackets
            this->namePropArray.push_back(temp);

            // if came from pointor section using goto, go back there
            if(gotoHelper2){
                // for O(1) check in future 
                this->isArray = true;
                this->arrayDimensions++;     
                
                goto cameBackFromLbrakSide;
            }

            
            if(this->parser.tokens[this->parser.currentPos+1].type != RBRACKET){ // closing token must be present as ]
                cout << "expected ]\n";
                exit(1);
            }
            current = this->parser.tokens[++this->parser.currentPos]; // advance 1 token , skip ]            

            // for O(1) check in future 
            this->isArray = true;
            this->arrayDimensions++;            
            
        } else if(current.type == LPAREN){ // found ( ,  need to call same function recursively
            if(idFound){ // if ID alr found, then it has to be params opening PAREN                
                this->parser.currentPos++; // advance 1 token , skip (
                
                // evaluate params 
                // add parameter evaluation code
                
                current = this->parser.getCurrentToken(); // set current to )
                continue; 
            } else{ // ID alr not found, ( found before ID, then call the function recursively to handle precedence rules
                this->parser.currentPos++; // advance 1 token , skip ()                
                bracketStackCount++; // increase current bracket stack count by 1
                this->getVarName(typeHolder); // recursive call
                
                // used to evaluate stars when recursion ends
                if(bracketStackCount == initBrackCount && addStarCount != -1){
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1;
                    initBrackCount = -1;
                }                
            }            
            
        } else if(current.type == RPAREN){ // found )
            if(bracketStackCount > 0){ // for recursion case
                bracketStackCount--; // reduce bracket stack count by 1
                if(bracketStackCount == initBrackCount && addStarCount != -1){ // evaluate stars 
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);

                    addStarCount = -1; // to reset it
                }
                return;
            } else if(gotoHelper && addStarCount != -1 && addStarCount != 0) { // goto * case , for params
                // function parameter ) - add deferred star!
                varNameProp temp;
                temp.type = POINTOR;
                temp.numPointor = addStarCount;
                this->namePropArray.push_back(temp);
                addStarCount = -1;
                // don't return, continue processing
            } else {
                cout << "Unknown ) found\n";
                exit(1);
            }
        } else{
            cout << "Error\n";
            exit(1);
        }

        current = this->parser.tokens[++this->parser.currentPos]; // advance token by 1
        
    }

    if(isFirstVar && bracketStackCount == 0){ // if the var is first in multiple decl

        // get base token type
        TokenType baseType;
        if(typeHolder.baseTypeArray.size() == 1){
            baseType = typeHolder.baseTypeArray.front();
        } else if(typeHolder.trBaseArray.size() == 1){
            baseType = HELPER_TOKEN;
        } else{
            cout << "Unknown error of base type again\n";
            exit(1);
        }

        // check if base type alr has some stars in starData (Valid only for first variable decl)
        short indexIfExist = -1;
        for(int i=0 ; i<typeHolder.starDataArray.size() ; i++){
            if(typeHolder.starDataArray[i].typeBeforeStar == baseType && typeHolder.starDataArray[i].numOfStars > 0){
                indexIfExist = i;
                break;
            }
        }

        if(indexIfExist == -1){ // base type doesnt alr exist in starData array
            // do nothing
        } else{ // base type alr exist in starData array            
            
            // evalute those stars and add at the end
            varNameProp temp;
            temp.type = POINTOR;
            temp.numPointor = typeHolder.starDataArray[indexIfExist].numOfStars;
            this->namePropArray.push_back(temp);
        }
    }

    // reseting static vars
    idFound = false;
    bracketStackCount = 0;
    initBrackCount = -1;

    if(current.type == COMMA){ // if multiple decl

        // deep copy this object for the decl node properly and reset the data 

        // reset the name prop array
        vector<varNameProp>().swap(namePropArray);

        // reset data type star data
        TokenType baseType;
        if(typeHolder.baseTypeArray.size() == 1){
            baseType = typeHolder.baseTypeArray.front();
        } else if(typeHolder.trBaseArray.size() == 1){
            baseType = HELPER_TOKEN;
        } else{
            cout << "Unknown error of base type\n";
            exit(1);
        }

        // check if base type alr has some stars in starData (Valid only for first variable decl)
        short indexIfExist = -1;
        for(int i=0 ; i<typeHolder.starDataArray.size() ; i++){
            if(typeHolder.starDataArray[i].typeBeforeStar == baseType && typeHolder.starDataArray[i].numOfStars > 0){
                indexIfExist = i;
                break;
            }
        }

        if(indexIfExist != -1){ // if it exists, then reset
            typeHolder.starDataArray[indexIfExist].numOfStars = 0;
        }

        isFirstVar = false; // false since we found ,
        
    } else if(current.type == OP_ASSIGN){ // initialized
        // proceed accordingly

    } else if(current.type == SEMICOLON){ // end
        // end this decl

        isFirstVar = true; // reset static var for the next call
    } else{
        // some error
    }

    
}

