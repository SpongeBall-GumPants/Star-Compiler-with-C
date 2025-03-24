#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IDENTIFIER_SIZE 10
#define MAX_INT_CONST_SIZE 8
#define MAX_STRING_CONST_SIZE 256

typedef enum {
    IDENTIFIER,
    INT_CONST,
    OPERATOR,
    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,
    STRING_CONST,
    KEYWORD,
    END_OF_LINE,
    COMMA
} TokenType;
#define MAX_SIZE 100

// Anahtar-değer çiftini temsil eden yapı
struct KeyValue {
    char key[50];
    char value[100];

};

// Sözlük yapısı
struct Dictionary {
    struct KeyValue elements[MAX_SIZE];
    int size;
};

FILE *inputFile;
FILE *outputFile;
int intOperation(char operation ,int a,int b);
const char* txtOperation(char operation,const char* a,const char* b);
void scanFile(const char *filename);
void addToken(TokenType type, const char *value);
void processCode();
void declaration(char *codeline,const char *delimiters,char* saveptr,struct Dictionary *Dict,int boolie);
void processBlock(char* lineDelim,const char* delimiters,struct Dictionary *intDic,struct Dictionary *txtDic,int enabler,char* looper);
int main() {
    // Perform lexical analysis
    inputFile = fopen("code.sta.txt", "r");
    outputFile = fopen("code.lex.txt", "w");
    scanFile("code.sta");
    fclose(inputFile);
    fclose(outputFile);
    printf("Lexical analysis completed. Output written to code.lex\n");

    // Perform semantic analysis
    inputFile = fopen("code.lex.txt", "r");
    processCode();
    fclose(inputFile);
    return 0;
}

void scanFile(const char *filename) {
    // Your existing lexical analysis code here
    char buffer[MAX_STRING_CONST_SIZE];
    char ch;


    while ((ch = fgetc(inputFile)) != EOF) {
        if (isspace(ch)) {
            continue;
        } else if (ch == '/') {
            char nextCh = fgetc(inputFile);
            if (nextCh == '*') {
                do {

                    ch= fgetc(inputFile);
                    nextCh= getc(inputFile);
                    fseek(inputFile,-1,SEEK_CUR);
                    if (nextCh<0) {
                        fprintf(stderr, "Error: Unterminated comment\n");
                        exit(1);}

                } while (ch !='*' && nextCh!='/');
                fgetc(inputFile);
                continue;


            } else {
                ungetc(nextCh, inputFile);
            }
        }

        switch (ch) {

            case '{':
                addToken(LEFT_CURLY_BRACKET, "");
                break;
            case '}':
                addToken(RIGHT_CURLY_BRACKET, "");
                break;

            case '"': {
                char buffer[MAX_STRING_CONST_SIZE];
                int index = 0;
                char nextCh;
                while ((nextCh = fgetc(inputFile)) != EOF && nextCh != '"') {
                    if (index < MAX_STRING_CONST_SIZE - 1) {
                        buffer[index++] = nextCh;
                    } else {
                        fprintf(stderr, "Error: Check String constant\n");
                        exit(1);
                    }
                }
                if (nextCh == '"') {
                    buffer[index] = '\0';
                    addToken(STRING_CONST, buffer);
                } else {
                    fprintf(stderr, "Error: Unterminated string constant\n");
                    exit(1);
                }
                break;
            }

            case '+':
            case '-':
            case '*':
            case '/':

                addToken(OPERATOR, (const char *) ch);
                break;
            case '.':
                addToken(END_OF_LINE, "");
                break;
            case ',':
                addToken(COMMA, "");
                break;
            default:
                if (isalpha(ch) ) {

                    int i = 0;
                    buffer[i++] = ch;
                    while ((ch = fgetc(inputFile)) != EOF && (isalnum(ch) || ch == '_')) {
                        buffer[i++] = ch;
                        if (i >= MAX_IDENTIFIER_SIZE) {
                            fprintf(stderr, "Error: Identifier too long\n");
                            exit(1);
                        }
                    }
                    buffer[i] = '\0';
                    ungetc(ch, inputFile);
                    if (strcmp(buffer, "int") == 0 || strcmp(buffer, "text") == 0 ||
                        strcmp(buffer, "is") == 0 || strcmp(buffer, "loop") == 0 ||
                        strcmp(buffer, "times") == 0 || strcmp(buffer, "read") == 0 ||
                        strcmp(buffer, "write") == 0 || strcmp(buffer, "newLine") == 0) {
                        addToken(KEYWORD, buffer);
                    } else {
                        addToken(IDENTIFIER, buffer);
                    }
                } else if (isdigit(ch)) {

                    int i = 0;
                    buffer[i++] = ch;
                    while ((ch = fgetc(inputFile)) != EOF && isdigit(ch)) {
                        buffer[i++] = ch;
                        if (i >= MAX_INT_CONST_SIZE) {
                            fprintf(stderr, "Error: Integer constant too long\n");
                            exit(1);
                        }
                    }
                    buffer[i] = '\0';
                    ungetc(ch, inputFile);
                    addToken(INT_CONST, buffer);
                } else {
                    fprintf(stderr, "Error: Invalid character '%c'\n", ch);
                    exit(1);
                }
                break;
        }
    }
}

void addToken(TokenType type, const char *value) {
    // Your existing token adding code here
    switch (type) {
        case OPERATOR:
            fprintf(outputFile,"Operator(%c)\n",value);
            break;
        case IDENTIFIER:
            fprintf(outputFile, "Identifier(%s)\n", value);
            break;
        case INT_CONST:
            fprintf(outputFile, "IntConst (%s)\n", value);
            break;
        case LEFT_CURLY_BRACKET:
            fprintf(outputFile, "LeftCurlyBracket\n");
            break;
        case RIGHT_CURLY_BRACKET:
            fprintf(outputFile, "RightCurlyBracket\n");
            break;
        case STRING_CONST:
            fprintf(outputFile, "String(%s)\n", value);
            break;
        case END_OF_LINE:
            fprintf(outputFile, "EndOfLine\n");
            break;
        case COMMA:
            fprintf(outputFile, "Comma\n");
            break;
        case KEYWORD:
            fprintf(outputFile,"Keyword(%s)\n",value);
            break;

        default:
            break;
    }
}

//////////////////////////////////Proje 2 Kısmı/////////////////////////////////////
// Sözlük oluşturma fonksiyonu
void initializeDictionary(struct Dictionary *dict) {
    dict->size = 0;
}

// Anahtar-değer çifti eklemek için fonksiyon
void addKeyValuePair(struct Dictionary *dict, char *key, char *value) {
    if (dict->size < MAX_SIZE) {
        strcpy(dict->elements[dict->size].key, key);
        strcpy(dict->elements[dict->size].value, value);
        dict->size++;
    } else {
        printf("Sözlük dolu, yeni çift eklenemedi.\n");
    }
}
void updateValue(struct Dictionary *dict, char *key, char *newValue) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->elements[i].key, key) == 0) {
            strcpy(dict->elements[i].value , newValue);
            return;
        }
    }
    printf("Anahtar bulunamadı.\n");
}
// Anahtarın değerini döndüren fonksiyon
char* getValue(const struct Dictionary *dict, char *key) {
    for (int i = 0; i < dict->size; i++) {
        if (strcmp(dict->elements[i].key, key) == 0) {
            return dict->elements[i].value;
        }
    }
    return NULL; // Eğer anahtar bulunamazsa NULL döndür
}
int CheckStringNumber(char * src) {
    volatile char c;
    if (src==NULL)
        return 0;
    if (*src==0)
        return 0;
    while ((c=*(src++))!=0) {
        if (c<'0' || c>'9')
            return 0;
    }
    return 1;
}
void processCode() {
    struct Dictionary intDic;
    struct Dictionary txtDic;
    initializeDictionary(&intDic);
    initializeDictionary(&txtDic);
    int e=0;
    char g=0;
    char lineDelim[] = "EndOfLine\n";
    const char delimiters[] = "() \t\n";
    char looper[]="";
    g = fgetc(inputFile); // Read the first character

// Loop until the end of the file is reached
    while (g != EOF) {
        fseek(inputFile, -1, SEEK_CUR);
        processBlock(lineDelim, delimiters, &intDic, &txtDic, e, looper);
        g = fgetc(inputFile);
        if (g == EOF) {
            break; // Exit the loop gracefully when reaching EOF
        }
    }


}
void processBlock(char* lineDelim,const char* delimiters,struct Dictionary *intDic,struct Dictionary *txtDic,int enabler,char *looper){
    int a = 0,c;
    char line[256];
    char codeline[2048];
    char *token;
    char temp[100];
    char temp2[100];
    if(enabler==0){
        fgets(line, sizeof(line), inputFile);
        while (strcmp(line, lineDelim) != 0) {
            strcat(codeline, line);
            memset(line, 0, sizeof line);
            fgets(line, sizeof(line), inputFile);

        }} else if (enabler==1){
        memcpy(codeline,looper,sizeof codeline);
    }

    char *saveptr = codeline;
    char *tempointer = (char *) malloc(strlen(saveptr) + 1);
    while ((token = strtok_r(saveptr, delimiters, &saveptr))) {
        if (strcmp(token, "Keyword") == 0) {
            token = strtok_r(saveptr, delimiters, &saveptr);
            /////////////////////////////////////////////DECLARATION/////////////////////////////////////////
            if (strcmp(token, "int") == 0) {
                declaration(codeline, delimiters, saveptr, intDic, 1);
            } else if (strcmp(token, "text") == 0) {
                declaration(codeline, delimiters, saveptr, txtDic, 0);
                /////////////////////////////////////////////LOOP/////////////////////////////////////////
            } else if (strcmp(token, "loop") == 0) {

                token = strtok_r(saveptr, delimiters, &saveptr);
                if(strcmp(token,"IntConst")!=0){
                    printf("please only constant integers for the loop...");
                    exit(1);

                }
                token = strtok_r(saveptr, delimiters, &saveptr);
                c= atoi(token);
                token = strtok_r(saveptr, delimiters, &saveptr);
                if(strcmp(token,"Keyword")!=0){
                    printf("where is times , syntax error...");
                }
                token = strtok_r(saveptr, delimiters, &saveptr);
                char loopString[2048];
                memcpy(loopString,saveptr,sizeof loopString);
                if(strcmp(token,"times")!=0){
                    printf("wrong keyword , syntax error...");
                }
                token = strtok_r(saveptr, delimiters, &saveptr);
                if(strcmp(token,"LeftCurlyBracket")==0){
                    memcpy(line,saveptr,sizeof line);
                    memset(loopString, 0, sizeof loopString);
                    while (strcmp(line, "RightCurlyBracket\n") != 0) {

                        strcat(loopString, line);
                        memset(line, 0, sizeof line);
                        fgets(line, sizeof(line), inputFile);

                    }

                }
                for(int i = 0; i < c; i++){
                    processBlock(lineDelim,delimiters,intDic,txtDic,1,loopString);

                }
                memset(codeline, 0, sizeof codeline);
                saveptr=NULL;
                //code block var ise
                //processBlock("RightCurlyBracket",delimiters,intDic,txtDic);
                /////////////////////////////////////////////INPUT/////////////////////////////////////////
            } else if (strcmp(token, "read") == 0) {
                token = strtok_r(saveptr, delimiters, &saveptr);

                if (strcmp(token, "String") == 0) {
                    token = strtok_r(saveptr, "()", &saveptr);
                    printf("%s", token);
                    token = strtok_r(saveptr, delimiters, &saveptr);

                    if (strcmp(token, "Comma") != 0) {
                        printf("There is not a comma");
                        exit(1);
                    }
                    token = strtok_r(saveptr, delimiters, &saveptr);                }
                if (strcmp(token, "Identifier") != 0) {
                    printf("This is not know ");
                    exit(1);
                }
                token = strtok_r(saveptr, delimiters, &saveptr);
                memcpy(temp, token, sizeof temp);
                scanf("%s", temp2);
                if (getValue(intDic, temp) != NULL) {
                    if (!CheckStringNumber(temp2)) {
                        printf("please input integer , continuing...");
                        updateValue(intDic, temp, "0");
                    } else {
                        updateValue(intDic, temp, temp2);
                    }
                } else {
                    updateValue(txtDic, temp, temp2);
                }


            }/////////////////////////////////////////////OUTPUT/////////////////////////////////////////
            else if (strcmp(token, "write") == 0) {
                int iscomma ;
                do {
                    token = strtok_r(saveptr, delimiters, &saveptr);

                    if (strcmp(token, "String") == 0) {
                        token = strtok_r(saveptr, "()", &saveptr);
                        printf("%s \t", token);
                    } else if (strcmp(token, "Identifier") == 0) {
                        token = strtok_r(saveptr, delimiters, &saveptr);
                        if (getValue(intDic, token) != NULL) {
                            printf("%s \t", getValue(intDic, token));
                        } else if (getValue(txtDic, token) != NULL) {
                            printf("%s \t", getValue(txtDic, token));
                        } else {
                            printf("There is no value like that ");
                        }

                    }
                    strcpy(tempointer,saveptr);
                    token = strtok_r(saveptr, delimiters, &saveptr);
                    if (token != NULL && strcmp(token, "Comma") == 0) {


                        iscomma = 1;
                    } else{
                        if(saveptr!=NULL){
                        strcpy(saveptr,tempointer);}
                        iscomma = 0;}
                } while (iscomma == 1);

            }

        }
            /////////////////////////////////////////////ASSIGNMENT/////////////////////////////////////////
        else if (strcmp(token, "Identifier") == 0) {
            token = strtok_r(saveptr, delimiters, &saveptr);
            memcpy(temp, token, sizeof temp);
            strtok_r(saveptr, delimiters, &saveptr);
            if (getValue(intDic, temp) != NULL) {
                a = 1;
            }
            token = strtok_r(saveptr, delimiters, &saveptr);
            if (strcmp(token, "is") != 0) {
                printf("where is the assignment");
                exit(1);
            }
            token = strtok_r(saveptr, delimiters, &saveptr);
            //////////////////////////////integer assignment//////////////////////////////
            if (a == 1) { // integer ise
                if (strcmp(token, "Identifier") == 0) {
                    token = strtok_r(saveptr, delimiters, &saveptr);
                    token = getValue(intDic, token);
                    if (token == NULL) {
                        printf("this is a wrong assignment");
                        exit(1);
                    }
                    memcpy(temp2, token, sizeof temp2);


                } else if (strcmp(token, "IntConst") == 0) {
                    token = strtok_r(saveptr, delimiters, &saveptr);
                    memcpy(temp2, token, sizeof temp2);
                } else {
                    printf("Error: assign proper");
                }
                strcpy(tempointer, saveptr);
                token = strtok_r(saveptr, delimiters, &tempointer);
                if (token != NULL && strcmp(token, "Operator") == 0) {
                    strtok_r(saveptr, delimiters, &saveptr);
                    token = strtok_r(saveptr, delimiters, &tempointer);
                    char operation = token[0];
                    token = strtok_r(saveptr, delimiters, &tempointer);
                    if (strcmp(token, "Identifier") == 0) {
                        token = strtok_r(saveptr, delimiters, &tempointer);
                        token = getValue(intDic, token);

                    } else if (strcmp(token, "IntConst") == 0) {
                        token = strtok_r(saveptr, delimiters, &tempointer);
                    }


                    c = intOperation(operation, atoi(token), atoi(temp2));
                    memset(temp2, '\0', sizeof temp2);
                    sprintf(temp2, "%d", c);
                    updateValue(intDic, temp, temp2);

                } else {
                    updateValue(intDic, temp, temp2);

                }

                token=strtok_r(saveptr, delimiters, &tempointer);

///     ///////////////////////////////////////////////////////text assignment///////////////////////////////////////////
            } else {
                if (strcmp(token, "Identifier") == 0) {
                    token = strtok_r(saveptr, delimiters, &saveptr);
                    token = getValue(txtDic, token);
                    if (token == NULL) {

                        printf("this is a wrong assignment");

                        exit(1);

                    }
                    memcpy(temp2, token, sizeof temp2);


                } else if (strcmp(token, "String") == 0) {
                    token = strtok_r(saveptr, "\"", &saveptr);
                    memcpy(temp2, token, sizeof temp2);
                } else {
                    printf("Error: assign proper");
                }
                strcpy(tempointer, saveptr);
                token = strtok_r(saveptr, delimiters, &tempointer);
                if (token != NULL && strcmp(token, "Operator") == 0) {
                    strtok_r(saveptr, delimiters, &saveptr);
                    token = strtok_r(saveptr, delimiters, &tempointer);
                    char operation = token[0];
                    token = strtok_r(saveptr, delimiters, &tempointer);
                    if (strcmp(token, "Identifier") == 0) {
                        token = strtok_r(saveptr, delimiters, &tempointer);
                        token = getValue(txtDic, token);

                    } else if (strcmp(token, "String") == 0) {
                        token = strtok_r(saveptr, ")", &tempointer);
                    }
                    memcpy(temp2, txtOperation(operation, token, temp2), sizeof temp2);
                    updateValue(txtDic, temp, temp2);
                    puts(getValue(txtDic, temp));
                } else {
                    updateValue(txtDic, temp, temp2);

                }


            }
        }else if(strcmp(token, "NewLine") == 0){
            printf("\n");
        }

        if(token!=NULL&& strcmp(token,"EndOfLine")==0&&enabler==1){
           token=strtok_r(saveptr, delimiters, &saveptr);
           if (token!=NULL){
            processBlock(lineDelim,delimiters,intDic,txtDic,1,saveptr);}

        }
        else if( token==NULL || strcmp(token,"EndOfLine")==0|| strcmp(token,"int")==0|| strcmp(token,"text")==0||
                                                                                        strcmp(token,"RightCurlyBracket")==0){

        memset(codeline, 0, sizeof codeline);}



    }

}
int intOperation(char operation ,int b,int a){

    switch (operation) {
        case '+':{
            a=a+b;
            break;
        }
        case '-':{
            a= a-b;
            if (a<0){a=0;}
            break;
        }
        case '*':{
            a=a*b;
            break;
        }
        case '/':{
            a= a/b;
            break;
        }
        default:return 0;

    }


    return a;
}

const char* txtOperation(char operation,const char* b,const char* a){
    switch (operation) {
        case '+':return strcat(a,b);
        case '-':
        {
            const char* found = strstr(a, b); // Find occurrence of b in a
            if (found != NULL) {
                size_t len_b = strlen(b); // Length of b
                size_t len_a = strlen(a); // Length of a
                if(len_b>len_a){
                    printf("no - string ,how does that work tho");
                    exit(1);
                }
                size_t len_remaining = len_a - len_b; // Length of remaining string

                // Allocate memory for remaining string
                char* result = (char*)malloc((len_remaining + 1) * sizeof(char));

                // Copy before the occurrence
                strncpy(result, a, found - a);
                // Copy after the occurrence
                strcpy(result + (found - a), found + len_b);

                return result;
            } else {
                // If b is not found in a, just return a
                return a;
            }
        }
        default:{
            printf("Can't do that with strings");
            exit(1);
        }
    }
}






void declaration(char *codeline, const char *delimiters, char* saveptr, struct Dictionary *Dict, int boolie) {
    char variablename[125];
    char *tempptr = NULL;
    char *tempchar = strtok_r(saveptr, delimiters, &tempptr);
    while (tempchar != NULL) {
        if (strcmp(tempchar, "Identifier") == 0) {
            tempchar = strtok_r(NULL, delimiters, &tempptr); // İlerlet
            if (tempchar == NULL) {
                fprintf(stderr, "Error: Identifier expected\n");
                exit(1);
            }
            if (getValue(Dict, tempchar) == NULL) {
                strcpy(variablename,tempchar);
                if (boolie == 1) {

                    addKeyValuePair(Dict, tempchar, "0"); // int ise

                } else {
                    addKeyValuePair(Dict, tempchar, ""); // txt ise
                }
            } else {
                fprintf(stderr, "Error: Variable already exists\n");
                exit(1);
            }

            // Daha fazla token almak için ilerlet
            tempchar = strtok_r(NULL, delimiters, &tempptr);
            if(tempchar != NULL && strcmp(tempchar, "Keyword") == 0) {
                tempchar = strtok_r(NULL, delimiters, &tempptr);
                if (tempchar != NULL && strcmp(tempchar, "is") == 0) {
                    tempchar = strtok_r(NULL, delimiters, &tempptr); // İlerlet
                    if (tempchar == NULL) {
                        fprintf(stderr, "Error: Value expected after 'is'\n");
                        exit(1);
                    }
                    // Değerin türüne bağlı olarak işlemler yapılabilir
                    if (boolie == 1) {
                        if(strcmp(tempchar, "IntConst") == 0) {
                            tempchar = strtok_r(NULL, delimiters, &tempptr);
                            if (CheckStringNumber(tempchar)) {
                                updateValue(Dict, variablename, tempchar); // İnteger değer
                            } else {
                                fprintf(stderr, "Error: Invalid value for integer\n");
                                exit(1);
                            }
                        }else{
                            printf("integer atayınız");
                            exit(1);
                        }
                    } else {
                        if(strcmp(tempchar, "String") == 0) {
                            tempchar = strtok_r(NULL, ")", &tempptr);
                            updateValue(Dict, variablename, tempchar); //TEXT değer
                        }

                    }
                    // Daha fazla token almak için ilerlet
                    tempchar = strtok_r(NULL, delimiters, &tempptr);
                    if (tempchar!=NULL&&strcmp(tempchar,"Comma")==0){
                        tempchar = strtok_r(NULL, delimiters, &tempptr);
                    }else{ break;}
                }
            }
        } else if (strcmp(tempchar, "Comma") == 0) {
            // Virgül varsa bir sonraki tanıma geç

            tempchar = strtok_r(NULL, delimiters, &tempptr);
        } else {
            fprintf(stderr, "Error: Unexpected token '%s'\n", tempchar);
            exit(1);
        }
    }
}
