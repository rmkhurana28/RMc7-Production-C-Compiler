#include "Helper.h"
#include "DeclarationNode.h"
#include "Parser.h"


// Implementation of helper classes
dataTypeHolder::dataTypeHolder(Parser& p) : parser(p) {
    // Reference initialized in initialization list
}

void dataTypeHolder::getDataType(){
    bool firstStarFound = false;
    bool isBaseTypeFound = false;
    
    TokenType latestType;

    // get data type properties now
    while(this->parser.isThisTokenDataTypeOrPropToken(this->parser.tokens[this->parser.currentPos]) || this->parser.tokens[this->parser.currentPos].type == OP_STAR){
        if(this->parser.isThisTokenDataBaseTypeToken(this->parser.tokens[this->parser.currentPos])){ // base type
            isBaseTypeFound = true;
            this->baseTypeArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSignModifierToken(this->parser.tokens[this->parser.currentPos])){ // sign
            if(firstStarFound){ // sign modifier token not allowed after first * is found
                // error message
            }
            this->signModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenSizeModifierToken(this->parser.tokens[this->parser.currentPos])){ // sizze
            if(firstStarFound){ // size modifier token not allowed after first * is found
                // error message
            }
            this->sizeModifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenTypeQualifierToken(this->parser.tokens[this->parser.currentPos])){ // type
            this->typeQualifiersArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.isThisTokenStorageClassToken(this->parser.tokens[this->parser.currentPos])){ // storage
            if(firstStarFound){ // storage class token not allowed after first * is found
                // error message
            }
            this->storageClassArray.push_back(this->parser.tokens[this->parser.currentPos].type);
            latestType = this->parser.tokens[this->parser.currentPos].type;
        } else if(this->parser.tokens[this->parser.currentPos].type == OP_STAR){ // *
            if(!isBaseTypeFound){ // * found before base type was specified
                // error message
            }
            firstStarFound = true;
            if(!isPrevTokenValidForCurrentStar(latestType)){ // * not allowed after prev tokenType
                // error message
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

    return;
}

bool dataTypeHolder::isCurrentTypeValid(){

    if(this->storageClassArray.size() > 1){
        // error message
        return false;        
    }

    // Check for duplicate qualifiers
    if(this->typeQualifiersArray.size() > 3){
        // error message
        return false;        
    }
    
    if(this->typeQualifiersArray.size() > 0){
        bool hasConst = false, hasVolatile = false, hasRestrict = false;
        for(TokenType qualifier : this->typeQualifiersArray){
            if(qualifier == KEYWORD_CONST){
                if(hasConst) return false; // duplicate const
                hasConst = true;
            } else if(qualifier == KEYWORD_VOLATILE){
                if(hasVolatile) return false; // duplicate volatile
                hasVolatile = true;
            } else if(qualifier == KEYWORD_RESTRICT){
                if(hasRestrict) return false; // duplicate restrict
                hasRestrict = true;
            }
        }
    }

    if(this->signModifiersArray.size() > 1){
        // error message
        return false;        
    }    

    if(this->baseTypeArray.size() != 1){
        // error message
        return false;        
    }    

    // check size modifiers, allowed are :
    // short , long , long long
    if(this->sizeModifiersArray.size() > 2){
        // error message
        return false;        
    }

    if(this->sizeModifiersArray.size() == 2){
        if(this->sizeModifiersArray[0] == KEYWORD_SHORT || this->sizeModifiersArray[1] == KEYWORD_SHORT){
            // error message
            return false;
        }
        if(this->sizeModifiersArray[0] != KEYWORD_LONG || this->sizeModifiersArray[1] != KEYWORD_LONG){
            // error message
            return false;
        }
    }

    // call function to validate stars also

    return true;
    
}

bool dataTypeHolder::isPrevTokenValidForCurrentStar(TokenType prevTokenType){
    if(prevTokenType == KEYWORD_CONST || prevTokenType == KEYWORD_VOLATILE || prevTokenType == KEYWORD_RESTRICT || prevTokenType == KEYWORD_INT || prevTokenType == KEYWORD_CHAR || prevTokenType == KEYWORD_BOOL || prevTokenType == KEYWORD_FLOAT || prevTokenType == KEYWORD_DOUBLE || prevTokenType == KEYWORD_VOID || prevTokenType == KEYWORD_SHORT || prevTokenType == KEYWORD_LONG || prevTokenType == KEYWORD_SIGNED || prevTokenType == KEYWORD_UNSIGNED){
        return true;
    }

    return false;
}