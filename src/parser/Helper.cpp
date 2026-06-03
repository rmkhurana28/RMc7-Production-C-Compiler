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
    trKeywordArray(other.trKeywordArray), trBaseArray(other.trBaseArray), tdNew(other.tdNew), tdExpanded(other.tdExpanded) {}

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
        tdExpanded = other.tdExpanded;
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

// func to parse the data type and it's prop 
int dataTypeHolder::getDataType(){

    // helper booleon
    bool firstStarFound = false;

    // helper booleon
    bool isBaseTypeFound = false;

    // helper booleon
    bool isShortLongSignUnsignFound = false;

    // helper booleon
    bool wasPrevTokenOfStructEnumUnion = false;

    // helper booleon
    bool wasPrevTokenTypedefName = false;
    
    // helper TokenType
    TokenType latestType;

    // label to come to when needed to keep parsing
    evaluate_again : 

    // main loop
    while(this->parser.isThisTokenDataTypeOrPropToken(this->parser.tokens[this->parser.currentPos]) || this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // parsing till data type prop or star

        // base data type is found
        if(this->parser.isThisTokenDataBaseTypeToken(this->parser.tokens[this->parser.currentPos])){ 

            // set helper booleons accordingly
            wasPrevTokenOfStructEnumUnion = false;
            wasPrevTokenTypedefName = false;
            isBaseTypeFound = true;

            // push to baseTypeArray
            this->baseTypeArray.push_back(this->parser.tokens[this->parser.currentPos].type);

            // update latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSignModifierToken(this->parser.tokens[this->parser.currentPos])){ // sign modifier is found (signed/unsigned)
            
            // sign modifier token not allowed after first * is found
            if(firstStarFound){ 
                cout << "Error: Sign modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }

            // set helper booleon accordingly
            isShortLongSignUnsignFound = true;
            wasPrevTokenOfStructEnumUnion = false;
            wasPrevTokenTypedefName = false;

            // push to signModifiersArray
            this->signModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            
            // update latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSizeModifierToken(this->parser.tokens[this->parser.currentPos])){ // size modifer is found (short/long)
            
            // size modifier token not allowed after first * is found
            if(firstStarFound){ 
                cout << "Error: Size modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }

            // set helper booleon accordingly
            isShortLongSignUnsignFound = true;
            wasPrevTokenOfStructEnumUnion = false;
            wasPrevTokenTypedefName = false;
            
            // push to sizeModifierArray
            this->sizeModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);

            // update the latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenTypeQualifierToken(this->parser.tokens[this->parser.currentPos])){ // type qualifier found (const/volatile/restrict)

            // push to type qualifier array
            this->typeQualifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);

            // update the latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenStorageClassToken(this->parser.tokens[this->parser.currentPos])){ // storage class is found (static/extern/auto/register)

            // storage class token not allowed after first * is found
            if(firstStarFound){ 
                cout << "Error: Storage class not allowed after pointer (*) declaration" << endl;
                exit(1);
            }

            // set helper booleon accordingly
            wasPrevTokenOfStructEnumUnion = false;
            wasPrevTokenTypedefName = false;

            // push to storage classArray
            this->storageClassArray.push_back(this->parser.tokens[this->parser.currentPos].type);

            // update the latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // * is found

            // * found before base type is NOT allowed
            if(!isBaseTypeFound && !isShortLongSignUnsignFound){ 
                cout << "Error: Pointer (*) found before base type declaration" << endl;
                exit(1);
            }
            
            // mark that first star is found now
            firstStarFound = true;
            
            // * not allowed after some specific tokenTypes
            if(!isPrevTokenValidForCurrentStar(latestType) && !wasPrevTokenOfStructEnumUnion && !wasPrevTokenTypedefName){ 
                cout << "Error: Invalid token before pointer (*) declaration" << endl;
                exit(1);
            }

            // create starData object
            starData tempData;
            if(latestType == ID){ // base type found was TR/TD
                tempData = {0,HELPER_TOKEN}; // helper token acting as helper
            } else{
                tempData = {0,latestType}; // base type is there
            }            

            // count the number of stars
            while(this->parser.tokens[this->parser.currentPos].type == OP_STAR){
                tempData.numOfStars++;
                this->parser.currentPos++;
            }
            // push to starDataArray
            this->starDataArray.push_back(tempData);
            continue;
            
            
        }

        this->parser.currentPos++; // advance 1 token
    }

    // now we found some other token except standard data decl prop or *

    // check if current token is struct/enum/union keyword token
    if(this->parser.isThisTokenStructUnionEnumToken(this->parser.tokens[this->parser.currentPos])){ // struct/enum/union

        // check if tagName is present
        if(this->parser.tokens[this->parser.currentPos+1].type != ID){ 

            // since no tagname, check if next token is {, if yes, it can be anonymous struct definition, jump there
            if(this->parser.tokens[this->parser.currentPos+1].type == LBRACE){
                // might be anonymous struct definition
                goto jumpToStructDef; // jump to struct definition section
            }

            cout << "Error: Expected ID after struct/enum/union new one\n" << endl;
            exit(1);
        }

        // here it is struct/enum/union definition
        if(this->parser.tokens[this->parser.currentPos+2].type == LBRACE){
            jumpToStructDef: // came from possibility of anonymous struct/union/enum difinition

            // set helper type
            TokenType helpType = this->parser.tokens[this->parser.currentPos].type;             

            // struct found
            if(helpType == KEYWORD_STRUCT){

                // parse struct
                ASTNode** tempStorage = this->parser.parseStruct(this);

                short pushIndex = 0;
                // push all ast from parseStruct to tempASTStorage (global variable), it can contain struct dec/def or struct var decl also
                while(tempStorage != nullptr && tempStorage[pushIndex] != nullptr){ 
                    tempASTStorage.push_back(tempStorage[pushIndex]);
                    pushIndex++;
                }
                
                // return with proper return code
                return 2;
            } else if(helpType == KEYWORD_UNION){ // union found

                // parse union
                ASTNode** tempStorage = this->parser.parseUnion(this);

                short pushIndex = 0;
                // push all ast from parseUnion to tempASTStorage (global variable), it can contain union dec/def or union var decl also
                while(tempStorage != nullptr && tempStorage[pushIndex] != nullptr){
                    tempASTStorage.push_back(tempStorage[pushIndex]);
                    pushIndex++;
                }

                // return with proper return code
                return 2;
            } else if(helpType == KEYWORD_ENUM){ // enum found

                // parse enum
                ASTNode** tempStorage = this->parser.parseEnum(this);

                short pushIndex = 0;
                // push all ast from parseEnum to tempASTStorage (global variable), it can contain enum dec/def or enum var decl also
                while(tempStorage != nullptr && tempStorage[pushIndex] != nullptr){
                    tempASTStorage.push_back(tempStorage[pushIndex]);
                    pushIndex++;
                }

                // return with proper return code
                return 2;
            }
        }
        
        // ID is found after struct/union.enum token, it has to be struct var declaration, not definition, since that is covered alr in prev if condition
        this->trKeywordArray.push_back(this->parser.tokens[this->parser.currentPos].type); // add struct/enum/union keyword to the tr keyword array
        this->parser.currentPos++; // advance 1 token                

        // now, we check if the ID is there anywhere in TR hashmap, if not present, error, if present, add to TR base array
        if(this->parser.isThisStringPresentAsKeyInTrHm(this->parser.tokens[this->parser.currentPos].data)){ // found in TR hashmap

            // check if it has the correct keyword in the TR hashmap accordign to the given keyword among struct/enum/union
            // give error if mis-match
            if((this->trKeywordArray.back() == KEYWORD_STRUCT && this->parser.typeRegisry[this->parser.tokens[this->parser.currentPos].data] != "struct") ||
               (this->trKeywordArray.back() == KEYWORD_UNION && this->parser.typeRegisry[this->parser.tokens[this->parser.currentPos].data] != "union") ||
               (this->trKeywordArray.back() == KEYWORD_ENUM && this->parser.typeRegisry[this->parser.tokens[this->parser.currentPos].data] != "enum")){
                cout << "Error: Mismatched type registry entry for " << this->parser.tokens[this->parser.currentPos].data << endl;
                exit(1);
            }
            
            // validation completed, ID is present properly, add to TR array
            this->trBaseArray.push_back(this->parser.tokens[this->parser.currentPos].data);
            this->parser.currentPos++; // skip ID

            // update latest type
            latestType = ID; 

            // update helper booleon accordingly
            isBaseTypeFound = true;
            wasPrevTokenOfStructEnumUnion = true;
            wasPrevTokenTypedefName = false;

            // keep parsing the data type prop
            goto evaluate_again;
        } else{ // ID must be in TR to be valid
            cout << "Error: Expected correct ID after struct/enum/union\n" << endl;
            exit(1);
        }
    }


    // if the current token is NOT struct/enum/union (alr done in prev if-cond) AND check if current token is ID or not
    if(this->parser.tokens[this->parser.currentPos].type != ID) return 1; // return if not ID , match completed 
    
    // current token is ID now

    // check if this ID is present in the key side in TD hashmap
    if(this->parser.isThisStringPresentAsKeyInTdMap(this->parser.tokens[this->parser.currentPos].data)){ // found in TD hashmap

        // check if it is actually specifying the data type or it is actualy an ID, done using lookup algo       
        if(!this->parser.isCurrentIdValidTdAlias()) return 1; // return if ID is just a var/func name (found using lookup algo) , match completed

        // ID is valid TD entry, push it
        this->tdNew.push_back(this->parser.tokens[this->parser.currentPos].data);
        this->tdExpanded.push_back(0); // initially mark this TD entry as not expanded
        this->parser.currentPos++; // skip ID

        // update latest type
        latestType = ID;        

        // update helper booleon accordingly
        wasPrevTokenTypedefName = true;
        isBaseTypeFound = true;
        
        // keep parsing the data type prop
        goto evaluate_again;
    }        

    return 1; // data type/prop parsing completed succesfully
}

int dataTypeHolder::isCurrentTypeValid(){

    /*
        -1 means NOT valid
        0 means valid for both var and func
        1 means valid ONLY for var
        2 means valid ONLY for func
    */
    

    // open up TD entry if exists and update the order arrays (do NOT reset them, keep them for future use and validation, keep the TD entry also)
    if(this->tdNew.size() > 0){
        // expand all typedefs and populate the arrays
        for(size_t tdIndex = 0; tdIndex < this->tdNew.size(); tdIndex++){

            // check if already expanded
            if(tdIndex >= this->tdExpanded.size()) this->tdExpanded.push_back(0);
            if(this->tdExpanded[tdIndex] == -1) continue;

            const string& typedefName = this->tdNew[tdIndex];

            // get the vector of strings from tdMap
            vector<string> expandedTokens = tdMap[typedefName][0].declProp;
            
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
                        vector<string> nestedTokens = tdMap[tokenStr][0].declProp;
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

            // mark as expanded
            this->tdExpanded[tdIndex] = -1;

            // tempVarNameHolder = tdMap[tdIndex]->nameProp;
            tempVarNameHolder = tdMap[typedefName][0].nameProp;

        }

        // after expansion, patch starDataArray entries that were tagged HELPER_TOKEN with actual base type
        if(this->baseTypeArray.size() == 1){
            for(size_t i = 0; i < this->starDataArray.size(); i++){
                if(this->starDataArray[i].typeBeforeStar == HELPER_TOKEN){
                    this->starDataArray[i].typeBeforeStar = this->baseTypeArray.front();
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

    } else if(this->baseTypeArray.size() == 1 && this->baseTypeArray.back() == KEYWORD_BOOL){ // base type is bool
        if(this->sizeModifiersArray.size() != 0) return -1; // size modifiers NOT allowed for bool
        if(this->signModifiersArray.size() != 0) return -1; // sign modifiers NOT allowed for bool

        if(this->storageClassArray.size() > 0 && (this->storageClassArray.back() == KEYWORD_AUTO || this->storageClassArray.back() == KEYWORD_REGISTER)) return 1; // auto/register NOT allowed for functions

        return 0; // always valid for bool
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

DeclarationNode* varNameHolder::getVarName(dataTypeHolder& typeHolder , bool isFuncParam, bool forceResetStatics){


    // booleon helper (static)
    bool static finalHelper = false;

    // flag to check if first id is found
    bool static idFound = false; 
    if(isFuncParam){
        idFound = false;
    }

    // breacket stack counter
    short static unsigned bracketStackCount = 0; 

    // used for evaluating pointors
    short static initBrackCount = -1; 

    // helper for evalating pointers and brackets properly
    short unsigned tempInitBrack = initBrackCount;

     // helper to track pointer coutns
    short addStarCount = 0;

    // booelon helper
    bool static isInit = false;
    static ExpressionNode* initExpr = NULL;

    // helper to evaluate stars properly
    static int addAtTheEnd = -1;

    // booleon helper
    bool static isFirstVar = true; // flag if the var is first in multiple decl

    // When called from parseTypedef() for multiple declarators, force reset all statics
    if(forceResetStatics){
        finalHelper = false;
        idFound = false;
        bracketStackCount = 0;
        initBrackCount = -1;
        isInit = false;
        initExpr = NULL;
        isFirstVar = true;
    }

    bool gotoHelper = false; // helper flag
    bool gotoHelper2 = false; // helper flag2
    bool gotoHelper3 = false; // helper flag3

    // set the current properly
    Token current = this->parser.getCurrentToken(); 
        
    // helper flag '
    short indexIfExist; 
    indexIfExist = -1;

    // booelon helper
    bool isVariad = false;
    vector<ParameterNode> paramList;

    // helper tokenType
    TokenType baseType;    

    // parse till ; or = or , or }
    while(current.type != SEMICOLON && current.type != OP_ASSIGN && current.type != COMMA && current.type != RBRACE){ // var name ends when next token is ; or = or ,
        if(current.type == ID){ // actaul var name

            cameFromPointorSide: // label used when jumping from pointor to ID

            // generate the name prop struct and push to array
            varNameProp temp;
            temp.type = VAR_NAME;
            temp.varName = current.data;
            this->namePropArray.push_back(temp);

            if(gotoHelper){ // if came here using goto jump, use it again to go back to pointor section
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
            
            if(typeHolder.baseTypeArray.size() == 1){ // if base type is present normally
                baseType = typeHolder.baseTypeArray.front();
            } else if(typeHolder.trBaseArray.size() == 1){ // if tr is used to get base type
                baseType = HELPER_TOKEN;
            } else{ // base type error
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
            
            
            { // what is this block doing exactly???
                /*
                    what is final helper helping with exactly? 
                    find out proeprly
                */
                if(!finalHelper){ 
                    if(indexIfExist == -1){ // base type doesnt alr exist in starData array
                        starData tempStarData({addStarCount , baseType}); // generate a starData object
                        typeHolder.starDataArray.push_back(tempStarData); // add this object to starData array
                    } else{ // base type alr exist in starData array
                        addAtTheEnd = typeHolder.starDataArray[indexIfExist].numOfStars;                        

                        // generate starData properly and push to typeHolder (came from fnc call itself)
                        {
                            starData tempStarData({addStarCount , baseType}); // generate a starData object
                            typeHolder.starDataArray.push_back(tempStarData); // add this object to starData array
                        }                        
                    }
                }

                finalHelper = true;

            }
            
            
            // updation is done on the data type side
            // now need to update on the name side

            checkAgain: // label used for pointor evaluatin jumps            

            if(current.type == LPAREN){ // ( present after *                
                initBrackCount = bracketStackCount; // store the number of nested brackets , stars will be added when it's corresponding ) is detected
            } else if(current.type == ID){ // var name after *

                // if gotoHelper is true, it means ID is alr found, and hence this is 2nd ID, which is error
                if(gotoHelper){ 
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


                    if(this->parser.tokens[this->parser.currentPos+1].type == LBRACKET){ // token next to ] is [, dont evaluate pointers here
                        current = this->parser.tokens[++this->parser.currentPos]; // skip ] and proceed
                        goto cameAgainFromPointor; // go to evaluate [ token first
                    } else if(this->parser.tokens[this->parser.currentPos+1].type == RPAREN){ // token next to ] is )
                        // next token is closing braket, we can safely evaluate pointor at this point

                        // evalaute pointers and push to array properly
                        varNameProp temp;
                        temp.type = POINTOR;
                        temp.numPointor = addStarCount;
                        this->namePropArray.push_back(temp);

                        // reset the addStarCounter 
                        addStarCount = -1; 

                        current = this->parser.tokens[++this->parser.currentPos]; // skip ] and go to ) now
                        
                    } else if(this->parser.tokens[this->parser.currentPos+1].type == SEMICOLON || this->parser.tokens[this->parser.currentPos+1].type == OP_ASSIGN || this->parser.tokens[this->parser.currentPos+1].type == COMMA || this->parser.tokens[this->parser.currentPos+1].type != RBRACE){ // token next to ] is ; = , }
                        // we can safely evaluate pointor at this point, but dont advance the current 2 times (required to end the parent while loop)

                        // evalaute pointers and push to array properly
                        varNameProp temp;
                        temp.type = POINTOR;
                        temp.numPointor = addStarCount;
                        this->namePropArray.push_back(temp);

                        // reset the addStarCounter
                        addStarCount = -1; 
                        
                        current = this->parser.tokens[++this->parser.currentPos]; // skipped ] and kept current as ; = ,

                    } else if(this->parser.tokens[this->parser.currentPos+1].type == LPAREN){ // token after ] is (
                        // new bracket started after ] , ( will be evaluated first, and then pointor will be added after that is closed
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
                    cout << "New Error added, nothing was here before, care\n";
                    exit(2);
                } else if(gotoHelper){ // we reached end of this var name, can safely evaluate stars now (ex: int num1 , **num2; || reached here on num2)

                    // evalaute pointers and push to array properly
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);

                    // reset addStarCount
                    addStarCount = -1; 
                } 
                else{
                    continue; // NOT advancing so that the parent while loop can end with current token
                }
            } else if(current.type == RPAREN) { // next token is )


                if(gotoHelper && addStarCount>0){            
                    // evalaute pointers and push to array properly
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);

                    // reset addStarCount
                    addStarCount = -1;
                }
                                
            }
            else{
                cout << this->parser.tokens[this->parser.currentPos].data;
                cout << " || Unknown token error in star\n";
                exit(1);
            }

            continue; // using to avoid the advance at the end of while loop

            

        } else if(current.type == LBRACKET){ // index [

            // label used to jump from pointor section to here
            cameAgainFromPointor: 

            // label helping to jump in case [ comes just after ) , but inside () only
            /*  example:
            
                int (*(*array)[0]); used here
                int *(*array)[0]; not used here
            */ 
            myNewHelperAgain:

            // generating data to add in the array
            varNameProp temp;
            temp.type = ARRAY;
            
            current = this->parser.tokens[++this->parser.currentPos]; // advance 1 token , skip [                     

            temp.arrayExpr = this->parser.parseExpression(0, false , 3); // evaluate the index in the brackets, sending 3 , as need to stop when ] is seen
            this->namePropArray.push_back(temp);

            // if came from pointor section using goto, go back there
            if(gotoHelper2){                
                this->isArray = true;
                this->arrayDimensions++;     
                
                goto cameBackFromLbrakSide;
            }            

            // closing token must be present as ]
            if(this->parser.tokens[this->parser.currentPos].type != RBRACKET){ 
                cout << "expected ] here\n";
                exit(1);
            }
            current = this->parser.tokens[++this->parser.currentPos]; // advance 1 token , skip ]       
            
            // evalute stars if conditons required
            if(gotoHelper3 && initBrackCount != -1 && initBrackCount == bracketStackCount && addStarCount > 0 && current.type != LBRACKET){
                varNameProp temp;
                temp.type = POINTOR;
                temp.numPointor = addStarCount;
                this->namePropArray.push_back(temp);
                addStarCount = -1;
                // initBrackCount = -1;
                initBrackCount = tempInitBrack;
            }

            this->isArray = true;
            this->arrayDimensions++;
            continue;            
            
        } else if(current.type == LPAREN){ // found ( ,  need to call same function recursively

            // label to jump here
            myNewHelper:

            if(idFound){ // if ID alr found, then it has to be params opening PAREN
                
                // evaluate pointors since this brakcet is param bracket, but if conditons meet
                if(this->parser.tokens[this->parser.currentPos-1].type == RPAREN && initBrackCount == bracketStackCount && addStarCount>0 && !gotoHelper3){
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1;
                    initBrackCount = tempInitBrack;
                }
                
                this->parser.currentPos++; // advance 1 token , skip (

                varNameProp temp;
                temp.type = FUNC;

                // evaluate params
                temp.funcParams = ParameterNode::evaluateParams(this->parser); 

                paramList = temp.funcParams;

                // check if varaidic func
                if(this->parser.tokens[this->parser.currentPos-1].type == OP_DOT){ 
                    isVariad = true;
                    temp.isVariadic = true;
                } else{
                    temp.isVariadic = false;
                }
                this->namePropArray.push_back(temp);
                
                // evaluate pointors if conditions meet
                if(initBrackCount == bracketStackCount && addStarCount>0){
                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1;
                    initBrackCount = tempInitBrack;
                }                            
                
                // after parameter evaluation, it will come at ), skip )
                current = this->parser.tokens[++this->parser.currentPos]; 
                continue; 
            } else{ // ID alr not found, ( found before ID, then call the function recursively to handle precedence rules
                this->parser.currentPos++; // advance 1 token , skip (

                bracketStackCount++; // increase current bracket stack count by 1

                // this is the recursive call
                this->getVarName(typeHolder , isFuncParam);
                bracketStackCount--;
                            
                if(initBrackCount != -1 && bracketStackCount == initBrackCount && addStarCount > 0 && !gotoHelper2){
                    
                    // jump if next token is (
                    if(this->parser.tokens[this->parser.currentPos+1].type == LPAREN){
                        this->parser.currentPos++; // skip )
                        gotoHelper3 = true;
                        goto myNewHelper;
                    }

                    // jump if next token is [
                    if(this->parser.tokens[this->parser.currentPos+1].type == LBRACKET){
                        this->parser.currentPos++; // skip )
                        gotoHelper3 = true;
                        cout << "here\n";
                        goto myNewHelperAgain;
                    }

                    varNameProp temp;
                    temp.type = POINTOR;
                    temp.numPointor = addStarCount;
                    this->namePropArray.push_back(temp);
                    addStarCount = -1;
                    initBrackCount = tempInitBrack;
                }                

                current = this->parser.tokens[++this->parser.currentPos]; // advance token by 1
                continue;
            }            
            
        } else if(current.type == RPAREN){ // found )            

            // for function parameters, checking this first
            if(isFuncParam){              
                return nullptr;
            } else if(bracketStackCount > 0){ // for recursion case                
                // Don't add stars here - they're handled after recursion returns
                return nullptr;
            } else if(gotoHelper && addStarCount > 0) { // goto * case , for params
                varNameProp temp;
                temp.type = POINTOR;
                temp.numPointor = addStarCount;
                this->namePropArray.push_back(temp);
                addStarCount = -1;                
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



    if(isFirstVar && bracketStackCount == 0 && !finalHelper){ // if the var is first in multiple decl

        // get base token type
        TokenType baseType;
        if(typeHolder.baseTypeArray.size() == 1){ // normal base type present

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

        } else if(typeHolder.trBaseArray.size() == 1){ // bse type is in TR
            baseType = HELPER_TOKEN;
        } else{
            cout << "Unknown error of base type again\n";
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

    if(isFirstVar && bracketStackCount == 0 && addAtTheEnd != -1 && finalHelper){

        starData tempStarData({addAtTheEnd , baseType}); // generate a starData object
        typeHolder.starDataArray.push_back(tempStarData); // add this object to starData array

        varNameProp temp;
        temp.type = POINTOR;
        temp.numPointor = addAtTheEnd;

        this->namePropArray.push_back(temp);

        addAtTheEnd = -1; // reset addAtTheEnd
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
            exit(2);
        } else if(check == 2){ // func 
            // validate data type to make sure it supports func
            if(typeHolder.isCurrentTypeValid() == 1){
                cout << "Data Type NOT valid for function\n";
                exit(1);
            }

            // proceed with func decl
            FunctionDeclarationNode* temp = new FunctionDeclarationNode(&typeHolder , this , paramList , isVariad);

            // reset dataType for further declarations
            resetDataTypeAndNameObjectForNext(typeHolder);

            // resetting some static variables for firther declaraions
            isFirstVar = false;
            isInit = false;
            initExpr = NULL;

            // return
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

            // reset dataType for further declarations
            resetDataTypeAndNameObjectForNext(typeHolder);

            // resetting some static variables for firther declaraions
            isFirstVar = false;
            isInit = false;
            initExpr = NULL;

            // return
            return temp;
        }                                
        
    } else if(current.type == OP_ASSIGN){ // initialized

        // it is variable decl
        // it is initialized

        // validate if varName is syntactical valid for variable
        if(this->checkValidity() == -1){
            exit(2);
        } else if(this->checkValidity() == 2){ // valid for func
            cout << "Name NOT valid for var\n";
            exit(2);
        }        

        // if invalid, stop for now
        // if valid, proceed to evalute rhs
        
        // check validity of data type for var
        if(typeHolder.isCurrentTypeValid() == 2){ // valid only for func
            cout << "Data Type NOT valid for vaiable\n";
            exit(2);
        }

        // set isInit flag to true
        isInit = true;
        
        current = this->parser.tokens[++this->parser.currentPos]; // advance 1 position, skip =

        // initExpr = evaluate
        initExpr = this->parser.parseExpression(2 , true , -1);
        
        current = this->parser.tokens[this->parser.currentPos]; // update current        

        goto comeAfterInit; // check the next token again (can be , or ; or ERROR)
        
        // proceed accordingly

    } else if(current.type == SEMICOLON){ // end

        short check = this->checkValidity();

        if(check == -1){
            exit(2);
        } else if(check == 1){ // var

            // validity check to make sure data type is valid for var
            if(typeHolder.isCurrentTypeValid() == 2){
                cout << "Data Type NOT valid for vaiable\n";
                exit(2);
            }


            // proceed with var decl
            // Create node for this variable
            VariableDeclarationNode* temp = new VariableDeclarationNode(&typeHolder , this , isInit , initExpr , this->isArray , this->arrayDimensions);

            // reset dataType for further declarations
            resetDataTypeAndNameObjectForNext(typeHolder); 

            // resetting some static variables for firther declaraions
            isFirstVar = true;
            isInit = false;
            initExpr = NULL;

            finalHelper = false;

            // return
            return temp;
        } else if(check == 2){ // func
            // validate data type to make sure it supports func
            if(typeHolder.isCurrentTypeValid() == 1){
                cout << "Data Type NOT valid for function\n";
                exit(1);
            }

            // proceed with func decl
            FunctionDeclarationNode* temp = new FunctionDeclarationNode(&typeHolder , this , paramList , isVariad);

            // reset dataType for further declarations
            resetDataTypeAndNameObjectForNext(typeHolder);

            // resetting some static variables for firther declaraions
            isFirstVar = true;
            isInit = false;
            initExpr = NULL;

            // return
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

        // return definition node
        return funcDef;        

        
    } else{        
        cout << this->parser.tokens[this->parser.currentPos].data << " || Causing the error\n";
        exit(1);
    }

    return nullptr;    
}

int varNameHolder::checkValidity(){        
    /*
        this func checks if the varName parsed is valid or not
        -1 if invalid
        1 if valid for var
        2 if valid for func
    */
    

    // VAR_NAME has highest precedence, so it is always the first
    if(this->namePropArray.front().type != VAR_NAME){ 
        return -1;
    }

    for(size_t i=0 ; i<this->namePropArray.size()-1 ; i++){
        if(this->namePropArray[i].type == FUNC){
            if(this->namePropArray[i+1].type == FUNC){ // func + func consecutive is invalid 
                cout << "F + F\n";
                return -1;
            } else if(this->namePropArray[i+1].type == ARRAY){ // func + array consecutive is invalid
                cout << "F + A\n";
                return -1;
            }
        } else if(this->namePropArray[i].type == ARRAY){
            if(this->namePropArray[i+1].type == FUNC){ // array + func consecutive is invalid
                cout << "A + F\n";
                return -1;
            }
        }
    }

    if(this->namePropArray.size() > 1 && this->namePropArray[1].type == FUNC){ // if func is just after varName, it is valid for func, else valid for variable
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
        cout << "Unknown error of base type here also\n";
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

ExpressionNode* Parser::parseExpression(short initPrec , bool stopAtComma , int needManualPushOfRBrackets) {    

    // helper stack to stop at some specific encounters whenever required
    static vector<TokenType> expectedStack;

    if(needManualPushOfRBrackets == 1){ // indicating to stop when ) is encountered
        expectedStack.push_back(RPAREN);
    } else if(needManualPushOfRBrackets == 2){ // indicating to stop when } is encountered
        expectedStack.push_back(RBRACE);
    } else if(needManualPushOfRBrackets == 3){ // indicating to stop when ] is encountered
        expectedStack.push_back(RBRACKET);
    } else if(needManualPushOfRBrackets == 4){ // indicating to stop when : is encountered
        expectedStack.push_back(OP_COLON);
    }

    ExpressionNode* left = nullptr;
    ExpressionNode* right = nullptr;

    Token currToken = tokens[currentPos];

    
    if(currToken.type == INT_LITERAL){ // int
        left = new IntLiteralNode(currToken.data);
    } else if(currToken.type == CHAR_LITERAL){ // char
        left = new CharLiteralNode(currToken.data.substr(1, currToken.data.length() - 2));
    } else if(currToken.type == STRING_LITERAL){ // string
        left = new StringLiteralNode(currToken.data);
    } else if(currToken.type == FLOAT_LITERAL){ // float
        left = new FloatLiteralNode(currToken.data);
    } else if(currToken.type == DOUBLE_LITERAL){ // double
        left = new DoubleLiteralNode(currToken.data);
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

        if(this->isThisParenForTypeCast()){ // this ( is for type cast
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

            // parsing the expr
            left = parseExpression(14 , false , -1);

            // generate new cast node
            left = new CastNode(type, left);

            goto skipAdvance;
            
        } else { // this is recursive call case
            currToken = tokens[++currentPos]; // skip ( 

            expectedStack.push_back(RPAREN); // indicating to stop when ) is encountered

            // parse expr after (
            left = parseExpression(0 , false , -1);

            currToken = tokens[currentPos]; // update token 

            // expected )
            if(currToken = tokens[currentPos] , currToken.type != RPAREN){
                cout << "Expected closing ) in this position\n";
                exit(1);
            }

            expectedStack.pop_back();             
        }        
        
    } else if(currToken.type == LBRACE){ // found {

        currToken = tokens[++currentPos]; // skip {
        
        expectedStack.push_back(RBRACE); // indicating to stop when } is encountered

        vector<ASTNode*> myData;

        if(currToken.type != RBRACE){ // somehting except } is present after {
            multiData:
            
            // parse expr after { or after , if multi-data
            left = parseExpression(0 , true , -1);

            // push ast
            myData.push_back(left);
            if(tokens[currentPos].type == COMMA){ // multi-data inside {
                currentPos++; // skip comma
                goto multiData;
            }
        }
    
        currToken = tokens[currentPos]; // update token 

        // expected }
        if(currToken = tokens[currentPos] , currToken.type != RBRACE){
            cout << "Expected closing }\n";
            exit(1);
        }

        expectedStack.pop_back();

        // generate block expr node
        left = new BlockExpressionNode(myData);
        
    } else if(currToken.type == LBRACKET){ // found [

        currToken = tokens[++currentPos]; // skip [

        expectedStack.push_back(RBRACKET); // indicating to stop when ] is encountered

        // parse expr after [
        left = parseExpression(0 , false , -1);

        expectedStack.pop_back();

        // expected ]
        if(currToken = tokens[currentPos] , currToken.type != RBRACKET){
            cout << "Expected closing ]\n";
            exit(1);
        }

        currToken = tokens[currentPos]; // update token         
    }
    else if(currToken.type == RPAREN || currToken.type == RBRACE || currToken.type == RBRACKET){ // ) or } or ]  
        return left; 
    } 
    else{
        cout << "Prev token is " << tokens[currentPos-1].data << "\n";
        cout << "Error: Unsupported expression token: " << currToken.data << endl;
        cout << "Next token is " << tokens[currentPos+1].data << "\n";
        exit(1);
        //
    }

    currToken = tokens[++currentPos]; // move to next token and update current
    
    skipAdvance:

    while(1){
        
        currToken = tokens[currentPos]; // update current token

        // if ; just return the func
        if(currToken.type == SEMICOLON){
            return left;
        }
        
        // if , found and expected to stop at comma, return
        if(currToken.type == COMMA){
            if(stopAtComma){
                return left;
            }
        }

        // if : and expected to stop at colon, return
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

        // -> found
        if(currToken.type == OP_ARROW){ 

            // ID is expected after ->
            if(tokens[currentPos+1].type != ID){
                cout << "Expected ID after ->\n";
                exit(1);
            }

            // generate pointer member access node
            left = new PointerMemberAccessNode(left , tokens[currentPos+1].data);

            currentPos += 2; // skip -> and ID
            continue;
        } 

        // . found
        if(currToken.type == OP_DOT){ 

            // ID is expected after .
            if(tokens[currentPos+1].type != ID){
                cout << "Expected ID after .\n";
                exit(1);
            }

            // generate member access node
            left = new MemberAccessNode(left , tokens[currentPos+1].data);

            currentPos += 2; // skip . and ID
            continue;
        }

        // this is func call case
        if(currToken.type == LPAREN){ 

            ExpressionNode* temp = left;

            currToken = tokens[++currentPos]; // skip ( 

            expectedStack.push_back(RPAREN); // indicating to stop when ) is encountered
            
            vector<ExpressionNode*> params;
            if(currToken.type != RPAREN){ // if something except ) is there after ( ,it means it has some parameters

                multiParams:
                left = parseExpression(0 , true , -1); // function call (stop at ,)
                params.push_back(left);

                // if , present it is multi-params
                if(tokens[currentPos].type == COMMA){
                    currToken = tokens[++currentPos]; // skip ,
                    goto multiParams;
                }
            }

            currToken = tokens[currentPos]; // update token 
            
            // expected )
            if(currToken = tokens[currentPos] , currToken.type != RPAREN){
                cout << "Expected closing ) func call\n";
                exit(1);
            }
            expectedStack.pop_back();

            currToken = tokens[++currentPos]; // skip )

            // generate func call node
            left = new FunctionCallNode(temp , params);
            continue;            
        }

        // if ) and expected to stop at ) ,return        
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

        // if } and expected to stop at } ,return
        if(currToken.type == RBRACE){
            if(!expectedStack.empty() &&  expectedStack.back() == RBRACE){
                if(needManualPushOfRBrackets == 2){
                    expectedStack.pop_back();
                }
                return left;
            }
            cout << "Unexpected } Found\n";
            exit(1);
        }

        // it is array indexing case
        if(currToken.type == LBRACKET){ 

            ExpressionNode* temp = left;

            currToken = tokens[++currentPos]; // skip [

            expectedStack.push_back(RBRACKET); // indicating to stop when ] is encountered

            left = parseExpression(0 , false , -1); 
            expectedStack.pop_back();

            // expected ]
            if(currToken = tokens[currentPos] , currToken.type != RBRACKET){
                cout << "Expected closing ] array index\n";
                exit(1);
            }

            currToken = tokens[++currentPos]; // update token 

            // generate array access node
            left = new ArrayAccessNode(temp , left);
            continue;

        }

        // if ] and expected to stop at ] ,return
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


        // it is postfix ++ or --
        if(currToken.type == OP_PLUS_PLUS || currToken.type == OP_MINUS_MINUS){ 
            TokenType op = currToken.type;

            // generate unary node
            left = new UnaryOpNode(op , left , false);

            currToken = tokens[++currentPos]; // skip ++ or --
            continue;
        }                

        // helper to store precedence of node
        short nextPred = getOperatorPrecedence(currToken.type);
        
        if(nextPred == 3){ // terneray operator ?
            if(3 <= initPrec){  // Ternary precedence too low for current context
                return left;     // Stop parsing, let outer level handle it
            }

            currentPos++; // skip ?
            
            expectedStack.push_back(OP_COLON); // indicating to stop when : is encountered
            
            // parse expr
            ExpressionNode* ifTrue = parseExpression(0 , false , -1);
            
            // expected :
            if(tokens[currentPos].type != OP_COLON){
                cout << "Expected : here\n";
                exit(1);
            }

            currentPos++; // skip :

            expectedStack.pop_back();

            ExpressionNode* ifFalse = parseExpression(0 , false , -1);
            
            // generate proper ternary node
            left = new TernaryOpNode(left , ifTrue , ifFalse);
             
        } else if((nextPred > initPrec) || (nextPred == initPrec && (nextPred == 2 || nextPred == 3))){ // precedence of next operator is greater than the initPred for this call
        // 2nd if condition is for right associativity operators like + += -= etc            

            TokenType op = currToken.type;

            currentPos++; // skip operator as it is alr stored in op

            // parse expr
            right = parseExpression(nextPred , stopAtComma , -1);

            // generate binary node
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

    // evalauting completed, advance 1 token
    this->currentPos++;
    
    return nullptr; // placeholder
}

// check if current token is a unary operand token or not
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

// parsing typecast case
void dataTypeHolder::evaluateTypeCast(){

    // helper booleon
    bool firstStarFound = false;
    bool isBaseTypeFound = false;

    // helper booleon
    bool isShortLongSignUnsignFound = false;
    
    // helper TokenType
    TokenType latestType;
    
    // label to come to when needed to keep parsing
    evaluate_again:

    // main loop
    while(this->parser.isThisTokenDataTypeOrPropToken(this->parser.tokens[this->parser.currentPos]) || this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // parsing till data type prop or star

        // storage class tokens are not allowed 
        if(this->parser.isThisTokenStorageClassToken(this->parser.tokens[this->parser.currentPos])){
            cout << "Storage class not allowed inside Type cast\n";
            exit(1);
        } 

        // base data type is found
        if(this->parser.isThisTokenDataBaseTypeToken(this->parser.tokens[this->parser.currentPos])){ 

            // mark base type as found
            isBaseTypeFound = true;

            // push to bsaeType array
            this->baseTypeArray.push_back(this->parser.tokens[this->parser.currentPos].type);

            // update latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSignModifierToken(this->parser.tokens[this->parser.currentPos])){ // sign modifier is found
            
            // update helper booleons accordingly
            isShortLongSignUnsignFound = true;

            // sign modifier token not allowed after first * is found
            if(firstStarFound){ 
                cout << "Error: Sign modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }

            // push to signModifier array 
            this->signModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);

            // update latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSizeModifierToken(this->parser.tokens[this->parser.currentPos])){ // size modifier is found

            // update helper booleons accordingly
            isShortLongSignUnsignFound = true;

            // size modifier token not allowed after first * is found
            if(firstStarFound){ 
                cout << "Error: Size modifier not allowed after pointer (*) declaration" << endl;
                exit(1);
            }

            // push to sizeModifierArray
            this->sizeModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);

            // update latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenTypeQualifierToken(this->parser.tokens[this->parser.currentPos])){ // type qualifier is found
            
            // push to type qualifier array
            this->typeQualifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);

            // update latest type
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // *

            // * found before base type was specified
            if(!isBaseTypeFound && !isShortLongSignUnsignFound){ 
                cout << "Error: Pointer (*) found before base type declaration" << endl;
                exit(1);
            }

            // mark first star as found
            firstStarFound = true;

            // * not allowed after specific tokens, check that
            if(!isPrevTokenValidForCurrentStar(latestType)){ 
                cout << "Error: Invalid token before pointer (*) declaration" << endl;
                exit(1);
            }

            starData tempData;
            if(latestType == ID){ // base type found was TR/TD
                tempData = {0,HELPER_TOKEN};            
            } else{ //  base type is normal data type
                tempData = {0,latestType};            
            }            

            // count the number of stars
            while(this->parser.tokens[this->parser.currentPos].type == OP_STAR){
                tempData.numOfStars++;
                this->parser.currentPos++;
            }
            this->starDataArray.push_back(tempData);
            continue;                        
        }

        this->parser.currentPos++; // advance 1 token
    }

    // return if ) is found
    if(this->parser.tokens[this->parser.currentPos].type == RPAREN) return;

    // now we found some other token except standard data decl prop or *

    // check if current token is struct/enum/union keyword token
    if(this->parser.isThisTokenStructUnionEnumToken(this->parser.tokens[this->parser.currentPos])){ // struct/enum/union

        // ID always expected after struct/enum/union keyword
        if(this->parser.tokens[this->parser.currentPos+1].type != ID){ 
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
            cout << "Error: Expected correct ID after struct/enum/union inside typecast also\n" << endl;
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
        this->tdExpanded.push_back(0); // initially mark this TD entry as not expanded
        this->parser.currentPos++;
        latestType = ID;
        goto evaluate_again;
    }        

    return; // validation completed
}

// validating type cast here
bool dataTypeHolder::validateTypeCast(){
    // open up TD entry if exists and update the order arrays (do NOT reset them, keep them for future use and validation, keep the TD entry also)
    if(this->tdNew.size() > 0){
        // expand all typedefs and populate the arrays
        for(size_t tdIndex = 0; tdIndex < this->tdNew.size(); tdIndex++){

            // check if already expanded
            if(tdIndex >= this->tdExpanded.size()) this->tdExpanded.push_back(0);
            if(this->tdExpanded[tdIndex] == -1) continue;

            const string& typedefName = this->tdNew[tdIndex];
            // get the vector of strings from tdMap
            vector<string> expandedTokens = tdMap[typedefName][0].declProp;
            
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
                        vector<string> nestedTokens = tdMap[tokenStr][0].declProp;
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

            // mark as expanded
            this->tdExpanded[tdIndex] = -1;
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

// function to check if ( that is found is for type cast or not
bool Parser::isThisParenForTypeCast(){    

    // store next token
    Token evaToken = tokens[currentPos+1];
    
    // false if empty paren , ()
    if(evaToken.type == RPAREN) return false; 

    short unsigned count = 2;

    reEvaluate:

    if(isThisTokenDataTypeOrPropToken(evaToken) || evaToken.type == OP_STAR) return true; // return true if next is either data type prop or * , it has to be type cast

    if(evaToken.type == KEYWORD_STRUCT || evaToken.type == KEYWORD_ENUM || evaToken.type == KEYWORD_UNION) return true; // return if struct/enum/union keyword , it has to be type cast, further validation will be done while evaluating in next step

    if(evaToken.type != ID) return false; // next has to be ID , if not return false

    if(!isThisStringPresentAsKeyInTdMap(evaToken.data)) return false; // if next is ID, it has to be present in TD, if not, return false    

    // now next is ID and also present in TD, 
    evaToken = tokens[currentPos + count++];

    // return if ) if found
    if(evaToken.type == RPAREN) return true;

    // evaluate further 
    goto reEvaluate;
    
}

// function that rejects only "void" as data type
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

// function to parse block 
BlockExpressionNode* parseBlock(Parser& parser){
    
    // storage to store all ASTs inside the block
    vector<ASTNode*> blockASTs;

    // { expected for the start of block
    if(parser.tokens[parser.currentPos].type != LBRACE){
        cout << "Expected { to start block\n";
        exit(1);
    }

    parser.currentPos++; // skip {

    // parse untill } is found
    while(parser.currentPos < parser.tokens.size() && parser.tokens[parser.currentPos].type != RBRACE){

        // parse the statement from current token and store in myHelper
        ASTNode** myHelper = parser.startParsingOfCurrentToken();

        short i = 0;

        while(myHelper[i] != nullptr){ // add all the ast nodes returned fromhere,

            /*
                using while instead of hardcoding it to 0 , coz some functions can return more than 1 ast at a time, ex struct with var decl
            */

            // push to blockASTs properly
            blockASTs.push_back(myHelper[i]);
            i++;
        }                
    }

    // expected } to close the block
    if(parser.currentPos >= parser.tokens.size() || parser.tokens[parser.currentPos].type != RBRACE){
        cout << "Expected } to close block\n";
        exit(1);
    }

    parser.currentPos++; // skip }

    // generate and return block ast
    return new BlockExpressionNode(blockASTs);
}

// function to parse block of Enum
EnumBlockExpressionNode* parseEnumBlock(Parser& parser){

    // vector to store all enum value objects
    vector<enumComponentHolder> enumerators;

    // { is expected in the starting of enum block 
    if(parser.tokens[parser.currentPos].type != LBRACE){
        cout << "Expected { to start enum block\n";
        exit(1);
    }

    parser.currentPos++; // skip {

    // parse till } is found
    while(parser.currentPos < parser.tokens.size() && parser.tokens[parser.currentPos].type != RBRACE){

        // ID is always expected in left side of the enum line (ex, A or A = 10 or A = expr1 + expr2)
        if(parser.tokens[parser.currentPos].type != ID){
            cout << "Expected identifier in enum block\n";
            exit(1);
        }

        // set ID as enum name
        string enumName = parser.tokens[parser.currentPos].data;
        parser.currentPos++; // skip identifier

        // node to store enum value, might be const or expr
        ExpressionNode* enumValue = nullptr;

        // Check if next token is = or ,
        if(parser.currentPos < parser.tokens.size() && parser.tokens[parser.currentPos].type == OP_ASSIGN){
            parser.currentPos++; // skip =

            // Parse expression, stop at comma ,only if valid expression follows
            if(parser.currentPos < parser.tokens.size() && parser.tokens[parser.currentPos].type != RBRACE && parser.tokens[parser.currentPos].type != COMMA){

                // peek ahead to check if this is the last enumerator
                int peekPos = parser.currentPos;
                int nestLevel = 0;
                bool isLast = true;
                while(peekPos < (int)parser.tokens.size()){
                    TokenType t = parser.tokens[peekPos].type;
                    if(t == LPAREN || t == LBRACKET) nestLevel++;
                    else if(t == RPAREN || t == RBRACKET) nestLevel--;
                    else if(nestLevel == 0 && t == COMMA){ isLast = false; break; }
                    else if(nestLevel == 0 && t == RBRACE){ break; }
                    peekPos++;
                }

                enumValue = parser.parseExpression(0, true, isLast ? 2 : -1); // stop at } if last, hence sneding 2, if not, sending -1 as default
            }
        }

        // Create enumComponentHolder and add to vector
        enumComponentHolder component(enumName, enumValue);
        enumerators.push_back(std::move(component));

        // Check for comma or close brace
        if(parser.currentPos < parser.tokens.size() && parser.tokens[parser.currentPos].type == COMMA){ // , might be present after last line, skip it if present
            parser.currentPos++; // skip ,
        } else if(parser.currentPos < parser.tokens.size() && parser.tokens[parser.currentPos].type != RBRACE){ // expected } for the closing of enum block
            cout << "Expected , or } in enum block at pos " << parser.currentPos << "\n";
            exit(1);
        } else if(parser.currentPos >= parser.tokens.size()){ // overflow
            cout << "Unexpected end of tokens in enum block\n";
            exit(1);
        }
    }

    // expeced }
    if(parser.currentPos >= parser.tokens.size() || parser.tokens[parser.currentPos].type != RBRACE){
        cout << "Expected } to close enum block\n";
        exit(1);
    }

    parser.currentPos++; // skip }

    // generate and return enum block ast
    return new EnumBlockExpressionNode(std::move(enumerators));
}



