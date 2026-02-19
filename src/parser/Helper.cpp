#include "Helper.h"
#include "DeclarationNode.h"
#include "ExpressionNode.h"
#include "Parser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

class ParameterNode;

// Implementation of helper classes
dataTypeHolder::dataTypeHolder(Parser& p) : parser(p) {
    // Reference initialized in initialization list
}

// Implementation of varNameHolder class    
varNameHolder::varNameHolder(Parser& p) : parser(p) {
    // Reference initialized in initialization list
}

// used for deep copy
dataTypeHolder::dataTypeHolder(const dataTypeHolder& other) : parser(other.parser),
    baseTypeArray(other.baseTypeArray), signModifiersArray(other.signModifiersArray),
    sizeModifiersArray(other.sizeModifiersArray), typeQualifiersArray(other.typeQualifiersArray),
    storageClassArray(other.storageClassArray), starDataArray(other.starDataArray),
    trKeywordArray(other.trKeywordArray), trBaseArray(other.trBaseArray), tdNew(other.tdNew) {}

varNameHolder::varNameHolder(const varNameHolder& other) : parser(other.parser),
    namePropArray(other.namePropArray), isArray(other.isArray), arrayDimensions(other.arrayDimensions) {}

// Assignment operators
dataTypeHolder& dataTypeHolder::operator=(const dataTypeHolder& other) {
    if (this != &other) {
        // Note: parser reference cannot be reassigned, it stays the same
        baseTypeArray = other.baseTypeArray;
        signModifiersArray = other.signModifiersArray;
        sizeModifiersArray = other.sizeModifiersArray;
        typeQualifiersArray = other.typeQualifiersArray;
        storageClassArray = other.storageClassArray;
        starDataArray = other.starDataArray;
        trKeywordArray = other.trKeywordArray;
        trBaseArray = other.trBaseArray;
        tdNew = other.tdNew;
    }
    return *this;
}

varNameHolder& varNameHolder::operator=(const varNameHolder& other) {
    if (this != &other) {
        // Note: parser reference cannot be reassigned, it stays the same
        namePropArray = other.namePropArray;
        isArray = other.isArray;
        arrayDimensions = other.arrayDimensions;
    }
    return *this;
}

void dataTypeHolder::getDataType(){
    bool firstStarFound = false;
    bool isBaseTypeFound = false;

    bool isShortLongSignUnsignFound = false;
    
    TokenType latestType;

    evaluate_again : 

    // get data type properties now
    while(this->parser.isThisTokenDataTypeOrPropToken(this->parser.tokens[this->parser.currentPos]) || this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // parsing till data type prop or star
        if(this->parser.isThisTokenDataBaseTypeToken(this->parser.tokens[this->parser.currentPos])){ // base type
            isBaseTypeFound = true;
            this->baseTypeArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSignModifierToken(this->parser.tokens[this->parser.currentPos])){ // sign
            isShortLongSignUnsignFound = true;
            if(firstStarFound){ // sign modifier token not allowed after first * is found
                cout << "Error: Sign modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }
            this->signModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSizeModifierToken(this->parser.tokens[this->parser.currentPos])){ // size
            isShortLongSignUnsignFound = true;
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
            if(!isBaseTypeFound && !isShortLongSignUnsignFound){ // * found before base type was specified
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

        // here it is struct/enum/union definition
        if(this->parser.tokens[this->parser.currentPos+2].type == LBRACE){
            TokenType helpType = this->parser.tokens[this->parser.currentPos].type;

            if(helpType == KEYWORD_STRUCT){
                this->parser.parseStruct(this);
            }
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
        for(size_t i=0 ; i<this->starDataArray.size() ; i++){
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

DeclarationNode* varNameHolder::getVarName(dataTypeHolder& typeHolder , bool isFuncParam){

    // cout << "[NAME] Current token is : " << this->parser.tokens[this->parser.currentPos].data << "\n";
    
    bool static idFound = false; // flag to check if first id is found
    if(isFuncParam){
        idFound = false;
    }
    short static unsigned bracketStackCount = 0; // breacket stack counter

    short static initBrackCount = -1; // used for evaluating pointors

    short unsigned tempInitBrack = initBrackCount;

    short addStarCount = 0; // pointor numbers
    
    bool static isInit = false;
    static ExpressionNode* initExpr = NULL;


    bool static isFirstVar = true; // flag if the var is first in multiple decl

    bool gotoHelper = false; // helper flag
    bool gotoHelper2 = false; // helper flag2
    bool gotoHelper3 = false;

    Token current = this->parser.getCurrentToken(); // set the current properly
        
    short indexIfExist; // helper flag 

    bool isVariad = false;
    vector<ParameterNode> paramList;

    

    while(current.type != SEMICOLON && current.type != OP_ASSIGN && current.type != COMMA && current.type != RBRACE){ // var name ends when next token is ; or = or ,
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
            for(size_t i=0 ; i<typeHolder.starDataArray.size() ; i++){
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
                // starData tempStarData({addStarCount , baseType}); // generate a starData object 
                // typeHolder.starDataArray.push_back(tempStarData); // add this object to starData array
            } else{ // base type alr exist in starData array
                //typeHolder.starDataArray[indexIfExist].numOfStars += addStarCount; // update the number of stars in the starData
                //addStarCount = typeHolder.starDataArray[indexIfExist].numOfStars;
            }
            
            // updation is done on the data type side
            // now need to update on the name side

            checkAgain: // label used for pointor evaluatin jumps            

            if(current.type == LPAREN){ // ( present after *
                //cout << "DEBUG: Setting initBrackCount=" << bracketStackCount << endl;
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

                        current = this->parser.tokens[++this->parser.currentPos]; // skip ] and go to ) now
                        
                    } else if(this->parser.tokens[this->parser.currentPos+1].type == SEMICOLON || this->parser.tokens[this->parser.currentPos+1].type == OP_ASSIGN || this->parser.tokens[this->parser.currentPos+1].type == COMMA || this->parser.tokens[this->parser.currentPos+1].type != RBRACE){ // token next to ] is ; = ,
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
            else if(current.type == SEMICOLON || current.type == OP_ASSIGN || current.type == COMMA ||  current.type == RBRACE){ // next token is either ; = , }
                            
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


                if(gotoHelper && addStarCount>0){
                    // function parameter ) - add deferred star!
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1;
                }

                // Stars already added when ID was processed (in gotoHelper flow)
                // Don't add them again here - would cause double addition
                
                
                //if(gotoHelper && addStarCount != -1 && addStarCount != 0) {
                    // This case is for function parameters, not regular declarations
                    // For regular declarations with (**name), stars were already added
                    // Only add if this is a deferred star scenario
                //}
            }
            else{
                cout << this->parser.tokens[this->parser.currentPos].data;
                cout << " || Unknown token error in star\n";
                exit(1);
            }

            continue; // using to avoid the advance at the end of while loop

            

        } else if(current.type == LBRACKET){ // index [

            cameAgainFromPointor: // label used to jump from pointor section to here

            myNewHelperAgain:

            // generating data to add in the array
            varNameProp temp;
            temp.type = ARRAY;
            
            current = this->parser.tokens[++this->parser.currentPos]; // advance 1 token , skip [         
            // current = this->parser.tokens[this->parser.currentPos]; // update current   

            temp.arrayExpr = this->parser.parseExpression(0, false , 3); // evaluate the index in the brackets
            this->namePropArray.push_back(temp);

            // if came from pointor section using goto, go back there
            if(gotoHelper2){
                // for O(1) check in future 
                this->isArray = true;
                this->arrayDimensions++;     
                
                goto cameBackFromLbrakSide;
            }            

            
            if(this->parser.tokens[this->parser.currentPos].type != RBRACKET){ // closing token must be present as ]
                cout << "expected ] here\n";
                exit(1);
            }
            current = this->parser.tokens[++this->parser.currentPos]; // advance 1 token , skip ]       
            
            if(gotoHelper3 && initBrackCount != -1 && initBrackCount == bracketStackCount && addStarCount > 0 && current.type != LBRACKET){
                varNameProp temp;
                temp.type = POINTOR;
                temp.numPointor = addStarCount;
                this->namePropArray.push_back(temp);
                addStarCount = -1;
                // initBrackCount = -1;
                initBrackCount = tempInitBrack;
            }

            // for O(1) check in future 
            this->isArray = true;
            this->arrayDimensions++;
            continue;            
            
        } else if(current.type == LPAREN){ // found ( ,  need to call same function recursively

            myNewHelper:

            if(idFound){ // if ID alr found, then it has to be params opening PAREN
                
                if(this->parser.tokens[this->parser.currentPos-1].type == RPAREN && initBrackCount == bracketStackCount && addStarCount>0 && !gotoHelper3){
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1;
                    // initBrackCount = -1;
                    initBrackCount = tempInitBrack;
                }
                
                this->parser.currentPos++; // advance 1 token , skip (

                varNameProp temp;
                temp.type = FUNC;
                temp.funcParams = ParameterNode::evaluateParams(this->parser); // parsing the params
                paramList = temp.funcParams;
                if(this->parser.tokens[this->parser.currentPos-1].type == OP_DOT){ // check if varaidic func
                    isVariad = true;
                    temp.isVariadic = true;
                } else{
                    temp.isVariadic = false;
                }
                this->namePropArray.push_back(temp);
                
                if(initBrackCount == bracketStackCount && addStarCount>0){
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1;
                    // initBrackCount = -1;
                    initBrackCount = tempInitBrack;
                }

                
                
                // current = this->parser.getCurrentToken(); // set current to )
                
                // after parameter evaluation, it will come at ), skip )
                current = this->parser.tokens[++this->parser.currentPos]; 
                continue; 
            } else{ // ID alr not found, ( found before ID, then call the function recursively to handle precedence rules
                this->parser.currentPos++; // advance 1 token , skip (
                bracketStackCount++; // increase current bracket stack count by 1
                this->getVarName(typeHolder , isFuncParam); // recursive call
                bracketStackCount--;
                // used to evaluate stars when recursion ends
                // Only apply if stars were deferred (init Brack was set AND stars exist)
                //cout << "DEBUG after recursion: initBrackCount=" << initBrackCount 
                //     << " bracketStackCount=" << bracketStackCount 
                //     << " addStarCount=" << addStarCount 
                //     << " gotoHelper2=" << gotoHelper2 << endl;                
                if(initBrackCount != -1 && bracketStackCount == initBrackCount && addStarCount > 0 && !gotoHelper2){
                    // goto checkAgain;
                //    cout << "DEBUG: Adding deferred pointer with " << addStarCount << " stars" << endl;
                    // cout << "Here\n";

                    if(this->parser.tokens[this->parser.currentPos+1].type == LPAREN){
                        this->parser.currentPos++; // skip )
                        gotoHelper3 = true;
                        goto myNewHelper;
                    }
                    if(this->parser.tokens[this->parser.currentPos+1].type == LBRACKET){
                        this->parser.currentPos++; // skip )
                        gotoHelper3 = true;
                        goto myNewHelperAgain;
                    }

                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1;
                    // initBrackCount = -1;
                    initBrackCount = tempInitBrack;
                }                

                current = this->parser.tokens[++this->parser.currentPos]; // advance token by 1
                continue;
            }            
            
        } else if(current.type == RPAREN){ // found )            

            if(isFuncParam){ // for function parameters - check THIS FIRST!                 

                return nullptr;
            } else if(bracketStackCount > 0){ // for recursion case
                // bracketStackCount--; // reduce bracket stack count by 1
                // Don't add stars here - they're handled after recursion returns (line 677-682)
                return nullptr;
            } else if(gotoHelper && addStarCount > 0) { // goto * case , for params
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
        } else if(current.type == LBRACE){
            // need proper evlauation again when dealing wiht struct/enum/union and all

            /*
                CAREFUL || NEED FURTHER ATTENTION AND EVALUATION
            */

            break;
        }

        else{
            cout << "Token = " << this->parser.tokens[this->parser.currentPos].data << " | ";
            cout << "Error this one\n";
            exit(1);
        }

        current = this->parser.tokens[++this->parser.currentPos]; // advance token by 1
        
    }


    if(isFirstVar && bracketStackCount == 0){ // if the var is first in multiple decl


        
        
        // get base token type
        TokenType baseType;
        if(typeHolder.baseTypeArray.size() == 1){

            baseType = typeHolder.baseTypeArray.front();
            
            // If baseType is INT (possibly auto-added when short/long/signed/unsigned present without explicit int)
            // Check if INT has 0 stars but short/long/signed/unsigned have stars - transfer them
            if(baseType == KEYWORD_INT){
                // First check if INT already has stars
                bool intHasStars = false;
                for(size_t i=0 ; i<typeHolder.starDataArray.size() ; i++){
                    if(typeHolder.starDataArray[i].typeBeforeStar == KEYWORD_INT && typeHolder.starDataArray[i].numOfStars > 0){
                        intHasStars = true;
                        break;
                    }
                }
                
                // If INT has no stars, check if size/sign modifiers have stars and transfer them
                if(!intHasStars){
                    for(size_t i=0 ; i<typeHolder.starDataArray.size() ; i++){
                        TokenType t = typeHolder.starDataArray[i].typeBeforeStar;
                        if((t == KEYWORD_SHORT || t == KEYWORD_LONG || t == KEYWORD_SIGNED || t == KEYWORD_UNSIGNED) 
                           && typeHolder.starDataArray[i].numOfStars > 0){
                            // Transfer these stars to KEYWORD_INT
                            typeHolder.starDataArray[i].typeBeforeStar = KEYWORD_INT;
                            break;
                        }
                    }
                }
            }
        } else if(typeHolder.trBaseArray.size() == 1){
            baseType = HELPER_TOKEN;
        } else{
            cout << "Unknown error of base type again\n";
            exit(1);
        }

        // check if base type alr has some stars in starData (Valid only for first variable decl)
        short indexIfExist = -1;
        for(size_t i=0 ; i<typeHolder.starDataArray.size() ; i++){
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
    if(!isFuncParam){
        idFound = false;
        bracketStackCount = 0;
        // initBrackCount = -1;
        initBrackCount = tempInitBrack;
    }
    

    if(isFuncParam){ 
        isInit = false; // reset static value for next var
        initExpr = NULL; // reset static value for next var
        isFirstVar = true; // reset static var for the next call
        idFound = true;
        return nullptr;
    }

    comeAfterInit:
    
    

    // now, have to put the algo to decide if this is var decl or func decl or func def

    if(current.type == COMMA){ // if multiple decl

        
        // it is variable multi decl
        
        // validate if varName is syntactical valid for variable
        // if invalid, stop for now
        short check = this->checkValidity();
        if(check == -1){
            exit(1);
        } else if(check == 2){ // func 
            // validate data type to make sure it supports func
            if(typeHolder.isCurrentTypeValid() == 1){
                cout << "Data Type NOT valid for function\n";
                exit(1);
            }

            // proceed with func decl
            FunctionDeclarationNode* temp = new FunctionDeclarationNode(&typeHolder , this , paramList , isVariad);

            resetDataTypeAndNameObjectForNext(typeHolder);

            isFirstVar = false;
            isInit = false;
            initExpr = NULL;

            return temp;

            
        } else if(check == 1){ // var
            // validity check to make sure data type is valid for var
            if(typeHolder.isCurrentTypeValid() == 2){
                cout << "Data Type NOT valid for vaiable\n";
                exit(1);
            }

            // proceed with var decl
            // Create node for this variable
            VariableDeclarationNode* temp = new VariableDeclarationNode(&typeHolder , this , isInit , initExpr , this->isArray , this->arrayDimensions);

            resetDataTypeAndNameObjectForNext(typeHolder);

            isFirstVar = false;
            isInit = false;
            initExpr = NULL;

            return temp;
        }
                        

        // proceed to next one
        
    } else if(current.type == OP_ASSIGN){ // initialized

        // it is variable decl
        // it is initialized

        // validate if varName is syntactical valid for variable
        if(this->checkValidity() == -1){
            exit(1);
        } else if(this->checkValidity() == 2){ // valid for func
            cout << "Name NOT valid for var\n";
            exit(1);
        }        
        // if invalid, stop for now
        // if valid, proceed to evalute rhs
        
        // check validity of data type for var
        if(typeHolder.isCurrentTypeValid() == 2){ // valid only for func
            cout << "Data Type NOT valid for vaiable\n";
            exit(1);
        }

        // set isInit flag to true
        isInit = true;
        
        current = this->parser.tokens[++this->parser.currentPos]; // advance 1 position, skip =

        // cout << "Data before parsing expessions is " << current.data << "\n";

        // initExpr = evaluate
        initExpr = this->parser.parseExpression(2 , true , -1);
        

        current = this->parser.tokens[this->parser.currentPos]; // update current

        // cout << "Data after parsing expessions is " << current.data << "\n";

        // Create node for this variable
        // VariableDeclarationNode* temp = new VariableDeclarationNode(&typeHolder , this , isInit , initExpr , this->isArray , this->arrayDimensions);

        goto comeAfterInit; // check the next token again (can be , or ; or ERROR)
        
        // proceed accordingly

    } else if(current.type == SEMICOLON){ // end

        // cout << "semi found here\n";

        short check = this->checkValidity();

        if(check == -1){
            exit(1);
        } else if(check == 1){ // var
            // validity check to make sure data type is valid for var
            if(typeHolder.isCurrentTypeValid() == 2){
                cout << "Data Type NOT valid for vaiable\n";
                exit(1);
            }

            // proceed with var decl
            // Create node for this variable
            VariableDeclarationNode* temp = new VariableDeclarationNode(&typeHolder , this , isInit , initExpr , this->isArray , this->arrayDimensions);

            resetDataTypeAndNameObjectForNext(typeHolder);

            isFirstVar = true;
            isInit = false;
            initExpr = NULL;

            return temp;
        } else if(check == 2){ // func
            // validate data type to make sure it supports func
            if(typeHolder.isCurrentTypeValid() == 1){
                cout << "Data Type NOT valid for function\n";
                exit(1);
            }

            // proceed with func decl
            FunctionDeclarationNode* temp = new FunctionDeclarationNode(&typeHolder , this , paramList , isVariad);

            resetDataTypeAndNameObjectForNext(typeHolder);

            isFirstVar = true;
            isInit = false;
            initExpr = NULL;

            return temp;
        }

        
        
    } else if(current.type == LBRACE){ // func definition
        
        // validate if varName is syntactical valid for variable
        if(this->checkValidity() == -1){
            exit(1);
        } else if(this->checkValidity() == 1){ // valid for var
            cout << "Name NOT valid for func\n";
            exit(1);
        }

        // check validity of data type for func
        if(typeHolder.isCurrentTypeValid() == 1){ // valid only for var
            cout << "Data Type NOT valid for func\n";
            exit(1);
        }

        // evaluate block inside func for func definitiuon
        // evaluate block here

        BlockExpressionNode* body = parseBlock(this->parser);

        FunctionDefinitionNode* funcDef = new FunctionDefinitionNode(&typeHolder , this , paramList  , isVariad , body);

        return funcDef;

        // generate func defi node 

        // return definition node
    } else{
        
        cout << this->parser.tokens[this->parser.currentPos].data << " || Causing the error\n";
        exit(1);
    }

    return nullptr;

    
}

int varNameHolder::checkValidity(){
    // -1 if invalid
    // 1 if valid for var
    // 2 if valid for func

    if(this->namePropArray.front().type != VAR_NAME){ // VAR_NAME has highest precedence, so it is always the first
        return -1;
    }

    for(size_t i=0 ; i<this->namePropArray.size()-1 ; i++){
        if(this->namePropArray[i].type == FUNC){
            if(this->namePropArray[i+1].type == FUNC){                
                cout << "F + F\n";
                return -1;
            } else if(this->namePropArray[i+1].type == ARRAY){
                cout << "F + A\n";
                return -1;
            }
        } else if(this->namePropArray[i].type == ARRAY){
            if(this->namePropArray[i+1].type == FUNC){
                cout << "A + F\n";
                return -1;
            }
        }
    }

    if(this->namePropArray.size() > 1 && this->namePropArray[1].type == FUNC){
        return 2; // valid only for func
    } else {
        return 1; // valid only for var
    }

}

void varNameHolder::resetDataTypeAndNameObjectForNext(dataTypeHolder& typeHolder){
    // reset the name prop array
    vector<varNameProp>().swap(namePropArray);

    // reset isArray and arrDim also
    this->isArray = false;
    this->arrayDimensions = 0;

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
    for(size_t i=0 ; i<typeHolder.starDataArray.size() ; i++){
        if(typeHolder.starDataArray[i].typeBeforeStar == baseType && typeHolder.starDataArray[i].numOfStars > 0){
            indexIfExist = i;
            break;
        }
    }

    if(indexIfExist != -1){ // if it exists, then reset
        typeHolder.starDataArray[indexIfExist].numOfStars = 0;
    }
}


// Static function to get operator precedence (15 = highest, 1 = lowest)
int getOperatorPrecedence(TokenType op) {
    switch(op) {
        // Precedence 15 (HIGHEST) - Postfix
        case LPAREN:        // () function call
        case LBRACKET:      // [] array subscript
        case OP_DOT:        // . member access
        case OP_ARROW:      // -> pointer member access
            return 15;
        
        // Precedence 13 - Multiplicative
        case OP_STAR:       // * multiplication (also unary dereference at prec 14)
        case OP_SLASH:      // / division
        case OP_PERCENT:    // % modulo
            return 13;
        
        // Precedence 12 - Additive
        case OP_PLUS:       // + addition (also unary at prec 14)
        case OP_MINUS:      // - subtraction (also unary at prec 14)
            return 12;
        
        // Precedence 11 - Shift
        case OP_LSHIFT:     // <<
        case OP_RSHIFT:     // >>
            return 11;
        
        // Precedence 10 - Relational
        case OP_LESS:       // <
        case OP_LESS_EQ:    // <=
        case OP_GREATER:    // >
        case OP_GREATER_EQ: // >=
            return 10;
        
        // Precedence 9 - Equality
        case OP_EQ_EQ:      // ==
        case OP_NOT_EQ:     // !=
            return 9;
        
        // Precedence 8 - Bitwise AND
        case OP_AND:        // & (also unary address-of at prec 14)
            return 8;
        
        // Precedence 7 - Bitwise XOR
        case OP_XOR:        // ^
            return 7;
        
        // Precedence 6 - Bitwise OR
        case OP_OR:         // |
            return 6;
        
        // Precedence 5 - Logical AND
        case OP_AND_AND:    // &&
            return 5;
        
        // Precedence 4 - Logical OR
        case OP_OR_OR:      // ||
            return 4;
        
        // Precedence 3 - Ternary
        case OP_QUESTION:   // ? :
            return 3;
        
        // Precedence 2 - Assignment
        case OP_ASSIGN:         // =
        case OP_PLUS_EQ:        // +=
        case OP_MINUS_EQ:       // -=
        case OP_STAR_EQ:        // *=
        case OP_SLASH_EQ:       // /=
        case OP_PERCENT_EQ:     // %=
        case OP_LSHIFT_EQ:      // <<=
        case OP_RSHIFT_EQ:      // >>=
        case OP_AND_EQ:         // &=
        case OP_XOR_EQ:         // ^=
        case OP_OR_EQ:          // |=
            return 2;
        
        // Precedence 1 (LOWEST) - Comma
        case COMMA:         // ,
            return 1;        
        
        // Not a binary operator or unknown
        default:
            return 0;
    }
}


// ============================================================================
// Expression Parsing Implementation
// ============================================================================

ExpressionNode* Parser::parseExpression(short initPrec , bool stopAtComma , int needManualPushOfRBrackets) {    

    static vector<TokenType> expectedStack;

    if(needManualPushOfRBrackets == 1){
        expectedStack.push_back(RPAREN);
    } else if(needManualPushOfRBrackets == 3){
        expectedStack.push_back(RBRACKET);
    } else if(needManualPushOfRBrackets == 4){
        expectedStack.push_back(OP_COLON);
    }

    // static ExpressionNode* temp = nullptr;

    ExpressionNode* left = nullptr;
    ExpressionNode* right = nullptr;

    Token currToken = tokens[currentPos];

    
    if(currToken.type == INT_LITERAL){ // int
        left = new IntLiteralNode(stoi(currToken.data));        
    } else if(currToken.type == CHAR_LITERAL){ // char
        left = new CharLiteralNode(currToken.data[0]);
    } else if(currToken.type == STRING_LITERAL){ // string
        left = new StringLiteralNode(currToken.data);
    } else if(currToken.type == FLOAT_LITERAL){ // float
        left = new FloatLiteralNode(stof(currToken.data));
    } else if(currToken.type == DOUBLE_LITERAL){ // double
        left = new DoubleLiteralNode(stod(currToken.data));
    } else if(currToken.type == ID){ // ID
        left = new IdentifierNode(currToken.data);
    } else if(isThisTokenUnaryOp(currToken.type)){ // unary operand found 
        if(currToken.type == KEYWORD_SIZEOF){ // sizeof()
            currentPos++; // skip sizeof_keyword
            if(isThisParenForTypeCast()){ // condition is same as type cast, so can use same func again
                currentPos++; // skip (
                dataTypeHolder* type = new dataTypeHolder(*this); // data type to store the type cast data
                type->evaluateTypeCast();

                // validaiton that it is not only void is left, rest done
                type->rejectOnlyVoid();

                if(!type->validateTypeCast()){
                cout << "Invaid sizeof case\n";
                exit(1);
                }
                currToken = tokens[currentPos]; // update current 
                if(currToken.type != RPAREN){
                    cout << "Expected closing ) for the sizeof bracket\n";
                    exit(1);
                }
                currToken = tokens[++currentPos]; // skip )
                left = new SizeofNode(type);

                goto skipAdvance;
                // data type sizeof
            } else{
                currentPos++; // skip (
                left = parseExpression(0 , false , 1);
                currentPos++; // skip )
                left = new SizeofNode(left);
                goto skipAdvance;
            }
        } else if(currToken.type == OP_AND){ // & referencing to address
            if(tokens[currentPos+1].type == OP_AND){ // only 1 allowed
                cout << "ONLY 1 & is allowed\n"; 
                exit(1);
            }
            TokenType op = currToken.type;
            currentPos++;  // skip unary operator
            left = parseExpression(14 , false , -1);
            left = new UnaryOpNode(op , left , true);
            goto skipAdvance;
        } else { // ++ -- ! ~ * + -
            TokenType op = currToken.type;
            currentPos++; // skip unary operator
            left = parseExpression(14 , false , -1);
            left = new UnaryOpNode(op , left , true);
            goto skipAdvance;
            //
        }
    }
    else if(currToken.type == LPAREN){ // found (

        // check if it is type case or recursive call using lookup method
        
        // put check algo here

        if(this->isThisParenForTypeCast()){
            currToken = tokens[++currentPos]; // skip ( 
            dataTypeHolder* type = new dataTypeHolder(*this); // data type to store the type cast data
            type->evaluateTypeCast();
            if(!type->validateTypeCast()){
                cout << "Invaid type casting case\n";
                exit(1);
            }
            currToken = tokens[currentPos]; // update current 
            if(currToken.type != RPAREN){
                cout << "Expected closing ) for the type cast bracket\n";
                exit(1);
            }
            currToken = tokens[++currentPos]; // skip ) 

            left = parseExpression(14 , false , -1);

            left = new CastNode(type, left);

            goto skipAdvance;
            
        } else {
            currToken = tokens[++currentPos]; // skip ( 
            expectedStack.push_back(RPAREN);
            left = parseExpression(0 , false , -1);
            currToken = tokens[currentPos]; // update token 
            if(currToken = tokens[currentPos] , currToken.type != RPAREN){
                cout << "Expected closing ) in this position\n";
                exit(1);
            }
            expectedStack.pop_back();
            // currentPos++;
        }
        

        
    } else if(currToken.type == LBRACE){ // found {
        currToken = tokens[++currentPos]; // skip {
        expectedStack.push_back(RBRACE);
        vector<ASTNode*> myData;
            if(currToken.type != RBRACE){                

                multiData:
                left = parseExpression(0 , true , -1);
                myData.push_back(left);

                if(tokens[currentPos].type == COMMA){
                    currentPos++; // skip comma
                    goto multiData;
                }
            }
        
        currToken = tokens[currentPos]; // update token 

        if(currToken = tokens[currentPos] , currToken.type != RBRACE){
            cout << "Expected closing }\n";
            exit(1);
        }

        expectedStack.pop_back();
        left = new BlockExpressionNode(myData);

        // currentPos++;
    } else if(currToken.type == LBRACKET){ // found [
        currToken = tokens[++currentPos]; // skip [
        expectedStack.push_back(RBRACKET);
        left = parseExpression(0 , false , -1);
        expectedStack.pop_back();
        if(currToken = tokens[currentPos] , currToken.type != RBRACKET){
            cout << "Expected closing ]\n";
            exit(1);
        }
        currToken = tokens[currentPos]; // update token 
        // currentPos++;
    }
    else if(currToken.type == RPAREN || currToken.type == RBRACE || currToken.type == RBRACKET){ // ) or } or ]  
        return left; 
    } 
    else{
        cout << "Error: Unsupported expression token: " << currToken.data << endl;
        exit(1);
        //
    }

    currToken = tokens[++currentPos]; // move to next token and update current
    
    skipAdvance:

    while(1){
        
        currToken = tokens[currentPos];

        if(currToken.type == SEMICOLON){
            return left;
        }
        

        if(currToken.type == COMMA){
            if(stopAtComma){
                return left;
            }
        }

        if(currToken.type == OP_COLON){
            if(!expectedStack.empty() && expectedStack.back() == OP_COLON){
                if(needManualPushOfRBrackets == 4){
                    expectedStack.pop_back();
                }
                return left;
            }

            cout << "Unexpected :\n";
            exit(1);
        }

        if(currToken.type == OP_ARROW){ // -> 

            if(tokens[currentPos+1].type != ID){
                cout << "Expected ID after ->\n";
                exit(1);
            }

            left = new PointerMemberAccessNode(left , tokens[currentPos+1].data);

            currentPos += 2; // skip -> and ID
            continue;
        } 

        if(currToken.type == OP_DOT){ // .

            if(tokens[currentPos+1].type != ID){
                cout << "Expected ID after .\n";
                exit(1);
            }

            left = new MemberAccessNode(left , tokens[currentPos+1].data);

            currentPos += 2; // skip . and ID
            continue;
        }

        if(currToken.type == LPAREN){ // func call
            ExpressionNode* temp = left;
            currToken = tokens[++currentPos]; // skip ( 
            expectedStack.push_back(RPAREN);
            vector<ExpressionNode*> params;
            if(currToken.type != RPAREN){

                multiParams:
                left = parseExpression(0 , true , -1); // function call (stop at ,)
                params.push_back(left);

                if(tokens[currentPos].type == COMMA){ // multi params
                    currToken = tokens[++currentPos]; // skip ,
                    goto multiParams;
                }
            }
            currToken = tokens[currentPos]; // update token 
            
            if(currToken = tokens[currentPos] , currToken.type != RPAREN){
                cout << "Expected closing ) func call\n";
                exit(1);
            }
            expectedStack.pop_back();

            currToken = tokens[++currentPos]; // skip )

            left = new FunctionCallNode(temp , params);
            continue;

            // generate func call node
        }

        if(currToken.type == RPAREN){
            if(!expectedStack.empty() &&  expectedStack.back() == RPAREN){
                if(needManualPushOfRBrackets == 1){
                    expectedStack.pop_back();
                }
                
                return left;
            }
            
            cout << "Unexpected ) Found\n";
            exit(1);
        }

        if(currToken.type == RBRACE){
            if(!expectedStack.empty() &&  expectedStack.back() == RBRACE){
                return left;
            }            
            cout << "Unexpected } Found\n";
            exit(1);
        }

        if(currToken.type == LBRACKET){ // array indexing
            ExpressionNode* temp = left;
            currToken = tokens[++currentPos]; // skip [
            expectedStack.push_back(RBRACKET);
            left = parseExpression(0 , false , -1);
            expectedStack.pop_back();
            if(currToken = tokens[currentPos] , currToken.type != RBRACKET){
                cout << "Expected closing ] array index\n";
                exit(1);
            }
            currToken = tokens[++currentPos]; // update token 

            left = new ArrayAccessNode(temp , left);
            continue;

        }

        if(currToken.type == RBRACKET){            
            if(!expectedStack.empty() &&  expectedStack.back() == RBRACKET){
                if(needManualPushOfRBrackets == 3){
                    expectedStack.pop_back();
                }
                return left;
            }            
            cout << "Unexpected ] Found\n";
            exit(1);
        }


        if(currToken.type == OP_PLUS_PLUS || currToken.type == OP_MINUS_MINUS){ // postfix ++ or --
            TokenType op = currToken.type;

            left = new UnaryOpNode(op , left , false);

            currToken = tokens[++currentPos]; // skip ++ or --
            continue;
        }
        
        

        short nextPred = getOperatorPrecedence(currToken.type);
        
        if(nextPred == 3){ // terneray operator ?
            if(3 <= initPrec){  // Ternary precedence too low for current context
                return left;     // Stop parsing, let outer level handle it
            }
            currentPos++; // skip ?
            expectedStack.push_back(OP_COLON);
            ExpressionNode* ifTrue = parseExpression(0 , false , -1);
            
            if(tokens[currentPos].type != OP_COLON){
                cout << "Expected : here\n";
                exit(1);
            }
            currentPos++; // skip :
            expectedStack.pop_back();
            ExpressionNode* ifFalse = parseExpression(0 , false , -1);
            
            left = new TernaryOpNode(left , ifTrue , ifFalse);
            // 
        } else if((nextPred > initPrec) || (nextPred == initPrec && (nextPred == 2 || nextPred == 3))){ // precedence of next operator is greater than the initPred for this call
        // 2nd if condition is for right associativity operators like + += -= etc            

            TokenType op = currToken.type;


            currentPos++; // skip operator as it is alr stored in op


            right = parseExpression(nextPred , stopAtComma , -1);

            left = new BinaryOpNode(op , left , right);

            
        } else { // precedence of next operator is less than or equal to the initPred for this call
                 // if same, we follow L->R order
            return left;
        }
    }

        
        
        
    

    

    // if precedence of next token is lower, then jsut return left, recursive decent will automatically handle this
    if(getOperatorPrecedence(currToken.type) < initPrec){
        return left;
    } 

    // if same
    

    

    

    // generate current node and advance 1 token

    // save precedence of this operator(op1) in currPrec

    // check next operator(op2), 
        // if next(op2) operator has lower or same precedence as op1, evaluate the op1 and update left to be it's result , advance 1 token , and do it again

        // if next(op2) operator has higher precedence than op1, then we evaluate op2 before op1 using recursive decent approach
    

    // evalauting completed, advance 1 token
    this->currentPos++;
    
    return nullptr; // placeholder
}



bool isThisTokenUnaryOp(TokenType op){
    switch(op){
        case OP_PLUS_PLUS:
        case OP_MINUS_MINUS:
        case OP_NOT:
        case OP_TILDE:
        case KEYWORD_SIZEOF:
        case OP_STAR:
        case OP_AND:
        case OP_MINUS:
        case OP_PLUS:        
            return true;        

        default:
            return false;
    }
}

void dataTypeHolder::evaluateTypeCast(){
    bool firstStarFound = false;
    bool isBaseTypeFound = false;

    bool isShortLongSignUnsignFound = false;
    
    TokenType latestType;
    
    evaluate_again:

    while(this->parser.isThisTokenDataTypeOrPropToken(this->parser.tokens[this->parser.currentPos]) || this->parser.tokens[this->parser.currentPos].type == OP_STAR){
        if(this->parser.isThisTokenStorageClassToken(this->parser.tokens[this->parser.currentPos])){
            cout << "Storage class not allowed inside Type cast\n";
            exit(1);
        } 
        if(this->parser.isThisTokenDataBaseTypeToken(this->parser.tokens[this->parser.currentPos])){ // base type
            isBaseTypeFound = true;
            this->baseTypeArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSignModifierToken(this->parser.tokens[this->parser.currentPos])){ // sign
            isShortLongSignUnsignFound = true;
            if(firstStarFound){ // sign modifier token not allowed after first * is found
                cout << "Error: Sign modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }
            this->signModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSizeModifierToken(this->parser.tokens[this->parser.currentPos])){ // size
            isShortLongSignUnsignFound = true;
            if(firstStarFound){ // size modifier token not allowed after first * is found
                cout << "Error: Size modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }
            this->sizeModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenTypeQualifierToken(this->parser.tokens[this->parser.currentPos])){ // type
            this->typeQualifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // *
            if(!isBaseTypeFound && !isShortLongSignUnsignFound){ // * found before base type was specified
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

    if(this->parser.tokens[this->parser.currentPos].type == RPAREN) return;

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
    if(this->parser.tokens[this->parser.currentPos].type != ID){ // closing must be ) , if not ID or struct/enum/union or data type prop or * , then error
        cout << "Expected closing ) for type casting\n";
        exit(1);
    }
    
    // current token is ID now

    // check if this ID is present in the key side in TD hashmap
    if(this->parser.isThisStringPresentAsKeyInTdMap(this->parser.tokens[this->parser.currentPos].data)){ // found in TD hashmap

        // check if it is actually specifying the data type or it is actualy an ID        
        if(!this->parser.isCurrentIdValidTdAlias()){ // closing must be ) , if not ID or struct/enum/union or data type prop or * , then error , if ID found, then it has to be some match
            cout << "Expected closing ) for type cast\n";
            exit(1);
        } 

        // ID is valid TD entry, push it
        this->tdNew.push_back(this->parser.tokens[this->parser.currentPos].data);
        this->parser.currentPos++;
        latestType = ID;
        goto evaluate_again;
    }        

    return; // validation completed
}

bool dataTypeHolder::validateTypeCast(){
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

    // number of type qualifiers prop can be 0-3 (all unique)
    if(this->typeQualifiersArray.size() > 3){
        cout << "Error: Too many type qualifiers in type cast" << endl;
        exit(1);
    }        
    if(this->typeQualifiersArray.size() > 0){ // checking for uniqueness
        bool hasConst = false, hasVolatile = false, hasRestrict = false;
        for(TokenType qualifier : this->typeQualifiersArray){
            if(qualifier == KEYWORD_CONST){
                if(hasConst){
                    cout << "Error: Duplicate 'const' qualifier in type cast" << endl;
                    exit(1);
                }
                hasConst = true;
            } else if(qualifier == KEYWORD_VOLATILE){
                if(hasVolatile){
                    cout << "Error: Duplicate 'volatile' qualifier in type cast" << endl;
                    exit(1);
                }
                hasVolatile = true;
            } else if(qualifier == KEYWORD_RESTRICT){
                if(hasRestrict){
                    cout << "Error: Duplicate 'restrict' qualifier in type cast" << endl;
                    exit(1);
                }
                hasRestrict = true;
            }
        }
    }

    bool sizeOrSignPresent = false; // used to assume base type as int if absent
    
    // number of sign modifiers prop can be 0-1
    if(this->signModifiersArray.size() > 1){
        cout << "Error: Multiple sign modifiers not allowed in type cast" << endl;
        exit(1);
    } else if(this->signModifiersArray.size() == 1) sizeOrSignPresent = true;
    
    // number of size modifiers prop can be 0-2 (short , long , long long)
    if(this->sizeModifiersArray.size() > 2){
        cout << "Error: Too many size modifiers in type cast" << endl;
        exit(1);
    }
    if(this->sizeModifiersArray.size() == 2){
        if(this->sizeModifiersArray[0] == KEYWORD_SHORT || this->sizeModifiersArray[1] == KEYWORD_SHORT){
            cout << "Error: 'short' cannot be combined with other size modifiers in type cast" << endl;
            exit(1);
        }
        if(this->sizeModifiersArray[0] != KEYWORD_LONG || this->sizeModifiersArray[1] != KEYWORD_LONG){
            cout << "Error: Only 'long long' is valid for two size modifiers in type cast" << endl;
            exit(1);
        }        
    }
    if(!sizeOrSignPresent && this->sizeModifiersArray.size() > 0) sizeOrSignPresent = true;

    // number of struct/enum/union type registry keywords can be 0-1
    if(this->trKeywordArray.size() > 1){
        cout << "Error: Multiple struct/union/enum type registry keywords not allowed in type cast" << endl;
        exit(1);
    }
    // size of trKeywordArray and trBaseArray must be same
    if(this->trKeywordArray.size() != this->trBaseArray.size()){
        cout << "Error: Mismatched struct/union/enum type registry keyword and base type entries in type cast" << endl;
        exit(1);
    }
    
    // base type check: exactly one base type required (either from standard base types or from type registry)
    if(this->baseTypeArray.size() + this->trBaseArray.size() != 1){
        if((this->baseTypeArray.size() + this->trBaseArray.size() == 0) && sizeOrSignPresent) {
            this->baseTypeArray.push_back(KEYWORD_INT); // add base type as int
        } else{
            cout << "Error: Exactly one base type required in type cast" << endl;
            exit(1);
        }
    }  

    // number of data type prop check completed, lets proceed to actual validation of data type prop with the base type

    if(this->baseTypeArray.size() == 1 && this->baseTypeArray.back() == KEYWORD_INT){ // base type is int
        return true; // always valid
    } else if(this->baseTypeArray.size() == 1 && this->baseTypeArray.back() == KEYWORD_CHAR){ // base type is char
        if(this->sizeModifiersArray.size() != 0){
            cout << "Error: Size modifiers not allowed for char in type cast" << endl;
            exit(1);
        }
        return true; // valid
    } else if(this->baseTypeArray.size() == 1 && this->baseTypeArray.back() == KEYWORD_VOID){ // base type is void
        if(this->signModifiersArray.size() > 0 || this->sizeModifiersArray.size() > 0){
            cout << "Error: Sign and size modifiers not allowed for void in type cast" << endl;
            exit(1);
        }
        return true; // valid for type cast (both (void) and (void*) allowed)
    } else{ // base type is either float/double/bool/struct/enum/union        
        if(this->signModifiersArray.size() > 0 || this->sizeModifiersArray.size() > 0){ // sign and size modifiers NOT allowed for these base types
            if(this->baseTypeArray.size() != 1 || this->baseTypeArray.front() != KEYWORD_DOUBLE){
                cout << "Error: Sign/size modifiers not allowed for this base type in type cast" << endl;
                exit(1);
            }

            // base type is double now
            if(this->sizeModifiersArray.size() != 1 || this->sizeModifiersArray.front() != KEYWORD_LONG){
                cout << "Error: Only 'long' size modifier allowed for double in type cast" << endl;
                exit(1);
            }

            if(this->signModifiersArray.size() != 0){
                cout << "Error: Sign modifiers not allowed for double in type cast" << endl;
                exit(1);
            }

            // base type is double + size modifier is long, this is valid
        } 

        return true; // valid
    }
}

bool Parser::isThisParenForTypeCast(){    

    Token evaToken = tokens[currentPos+1];
    
    if(evaToken.type == RPAREN) return false; // false if empty paren , ()

    short unsigned count = 2;

    reEvaluate:

    if(isThisTokenDataTypeOrPropToken(evaToken) || evaToken.type == OP_STAR) return true; // return if next is either data type prop or * , it has to be type cast

    if(evaToken.type == KEYWORD_STRUCT || evaToken.type == KEYWORD_ENUM || evaToken.type == KEYWORD_UNION) return true; // return if struct/enum/union keyword , it has to be type cast, further validation will be done while evaluating in next step

    if(evaToken.type != ID) return false; // next has to be ID , if not return false

    if(!isThisStringPresentAsKeyInTdMap(evaToken.data)) return false; // if next is ID, it has to be present in TD, if not, return false

    

    // not next is ID and also present in TD, 
    evaToken = tokens[currentPos + count++];
    if(evaToken.type == RPAREN) return true;
    goto reEvaluate;

    
}

void dataTypeHolder::rejectOnlyVoid(){
    if(this->baseTypeArray.size() != 0 && this->baseTypeArray.back() == KEYWORD_VOID){
        // void exist

        for(size_t i=0 ; i<this->starDataArray.size() ; i++){
            if(starDataArray[i].typeBeforeStar == KEYWORD_VOID && starDataArray[i].numOfStars>0) return;
        }

        cout << "void is NOT allowed inside sizeof\n";
        exit(1);
    }

    return;
}

BlockExpressionNode* parseBlock(Parser& parser){
    vector<ASTNode*> blockASTs;

    if(parser.tokens[parser.currentPos].type != LBRACE){
        cout << "Expected { to start block\n";
        exit(1);
    }

    parser.currentPos++; // skip {

    while(parser.currentPos < parser.tokens.size() && parser.tokens[parser.currentPos].type != RBRACE){
        blockASTs.push_back(parser.startParsingOfCurrentToken());
    }

    if(parser.currentPos >= parser.tokens.size() || parser.tokens[parser.currentPos].type != RBRACE){
        cout << "Expected } to close block\n";
        exit(1);
    }

    parser.currentPos++; // skip }

    return new BlockExpressionNode(blockASTs);
    
}


