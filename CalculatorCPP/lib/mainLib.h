/**********************************************************************************
---------------------- Libraria principala-----------------------------------------
Aici se includ celelalte librarii, plus cateva functii ajutatoare
***********************************************************************************/
//constante folosite
const double PI = 3.141592653589793115997963468544185161590576171875; //Pi constant
const double E = 2.718281828459045090795598298427648842334747314453125; //Euler constant
const double EPSILON = 0.0000001; //Epsilon - reprezentare numar foarte mic (>0)
const double INFINIT = 99999999999999.0;//Infinit - reprezentare numar foarte mare
char language[20];
#include "stack.h"
#include "binaryTree.h"
#include "mathFunctions.h"
#include "variable.h"
#include "converters.h"

struct Errors {
    char* errorMessage;
    Errors* next;
};

struct Errors *error[120];
//un vector care va memora erorile

Errors* newError(char* errorMessage) {
    Errors* newErr = (Errors*)malloc(sizeof(Errors));
    newErr->errorMessage = errorMessage;
    newErr->next = NULL;
    return newErr;
}//functie pentru construirea unei erori

//functie de adaugare a unei erori
void addError(char* errorText, int position) {
    char* tempErr = (char*)malloc((100)*sizeof(char));
    if (!strcmp(language, "Romanian"))
        sprintf(tempErr, "Pozitia %d->%s", position, errorText);
    else
        sprintf(tempErr, "Position %d->%s", position, errorText);
    if (error[position] == NULL) {
        error[position] = newError(tempErr);
        return;
    }
    Errors* newErr = newError(tempErr);
    newErr->next = error[position];
    error[position] = newErr;
}

//stergerea/resetarea erorilor
void deleteErrors() {
    for (int i = 0; i < 120; i++) {
        if (error[i] != NULL) {
            while (error[i] != NULL) {
                Errors* tempErr = error[i];
                error[i] = error[i]->next;
                free(tempErr->errorMessage);
                free(tempErr);
                tempErr = NULL;
            }
        }
    }
}

//calculeaza numarul de puncte decimale/virgule dintr-un cuvant
int numberOfDecimalPoints(char word[]) {
    int length = strlen(word);
    int nrPoints = 0;
    for (int i = 0; i < length; ++i)
        if (word[i] == '.')
            nrPoints++;

    return nrPoints;
}

//verifica daca caracterul dat este o cifra sau o litera, se accepta si virgula/punctul
bool isDigitOrLetter(char ch) { //verifica daca e litera sau cifra
    if ((ch >= '0' && ch <= '9')||(ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch=='.') || (ch == ','))
        return true;
    return false;
}

char* strToLower(char*);
//verifica daca este operator unar
bool isUnaryOperator(char* opr) {
    switch(opr[0]) {
        case 'a':
            if (!strcmp(opr, "abs") || !strcmp(opr, "asin") || !strcmp(opr, "acos") || !strcmp(opr, "atan") || !strcmp(opr, "actg"))
                return true;
            else
                return false;
        case 'c':
            if (!strcmp(opr, "ctg") || !strcmp(opr, "cos"))
                return true;
            else
                return false;
        case 'l':
            if (!strncmp(opr, "ln", 2) || !strncmp(opr, "log2", 4) || !strncmp(opr, "log", 3))
                return true;
            else
                return false;
        case 's':
            if (!strcmp(opr, "sin") || !strcmp(opr, "sqrt"))
                return true;
            else
                return false;
        case 't':
            if (!strcmp(opr, "tan"))
                return true;
            else
                return false;
        case '-':
            if (!strcmp(opr, "-u")) //minus unar
                return true;
            else
                return false;
        case '+':
            if (!strcmp(opr, "+u")) //minus unar
                return true;
            else
                return false;
        default:
            return false;
    }
}

//verfica daca este operator binar
bool isBinaryOperator(char* opr) { //verifica daca e un operator binar
    switch(opr[0]) {
        case '+':
        case '-':
            if (isUnaryOperator(opr))
                return false;
            else
                return true;
            break;
        case '*':
        case '/':
        case '%':
        case '^':
        case '#':
            return true;
        case '<':
            if (!strcmp(opr, "<") || !strcmp(opr, "<=") || !strcmp(opr, "<>"))
                return true;
            else
                return false;
        case '>':
            if (!strcmp(opr, ">") || !strcmp(opr, ">="))
                return true;
            else
                return false;
        case '=':
            return true;
        default:
            return false;
    }
}

//verifica daca este operand valid sau nu
bool isOperand(char* operand) {

    if (isUnaryOperator(operand) || isBinaryOperator(operand))
        return false;

    bool isVariable;
    int length = strlen(operand);


    if (operand[0] == '(' || operand[0] == ')')
        return false;

    if ((operand[0] >= '0' && operand[0] <= '9') || operand[0] == '.')
        isVariable = false;//ar trebui sa fie un numar
    else
        isVariable = true;//ar trebui sa fie o variabila


    for (int i=1; i < length; i++) {
        if (isVariable && operand[i] == '.')
            return false;
        else if (!isVariable)
            if ((operand[i] >= 'A' && operand[i] <= 'Z') || (operand[i] >= 'a' && operand[i] <= 'z'))
                return false;
        if (!isDigitOrLetter(operand[i]))
            return false;
    }
    return true;
}

//verifica prioritatea operatorilor
int operatorPriority(char* opr) {
    switch(opr[0]) {
        case '-':
            if (!strcmp(opr, "-u"))
                return 3;
            else
                return 1;
            break;
        case '+':
            if (!strcmp(opr, "+u"))
                return 3;
            else
                return 1;
            break;
        case '*':
        case '/':
        case '%':
            return 2;
        case '^':
            return 3;
        case '=':
        case '>':       //>, >=
        case '<':       //<, <=, <>
        case '#':
            return 3;
        case 'a':       //abs
        case 'c':       //ctg
        case 'l':       //ln, log, log2
        case 's':       //sin, sqrt
        case 't':       //tan
            return 4;
        default:
            return -1;
    }
}

//structura pentru a memora date despre expresie
struct Expression {
    char *expression;//expresia implicita
    char infixVec[105][30];//vectorul de cuvinte
    char postfixVec[105][30];//vectorul de cuvinte in ordine postfixata
    int infixLength;//lungimea vecorului de cuvinte in ordine infixata
    bool isOK;//este sau nu corecta din punct de vdere sintactic
};


char* removeSpaces(char* exp);
void createVector(struct Expression&);
void setExpression(struct Expression &newE, char* expression) {
    newE.isOK = true;
    newE.infixLength = 0;
    if (expression == NULL)
        return;
    else {
        newE.expression = expression;
        removeSpaces(newE.expression);
        createVector(newE);
    }
}

//functie pentru stergerea spatiilor din expresie
char* removeSpaces(char* str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ')
            str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
    return str;
}

//functie ce va adauga spatii intre cuvinte
char* addSpacesIn(struct Expression& input) {

    int newIndex = -1;
    int i = 0;

    char* expr = (char*)malloc(105*sizeof(char));
    strcpy(expr, input.expression);

    int len = strlen(expr);

    char* temp = (char*)malloc(200*sizeof(char));
    *temp = 0;

    //e posibil sa crape aici....
    while (i < len) {
        char tempc[2] = "";
        sprintf(tempc, "%c",expr[i]);
        if (isDigitOrLetter(expr[i]) || expr[i] == '.' || expr[i] == ',') {
            temp[++newIndex] = expr[i];
            i++;
        } else if (isBinaryOperator(tempc)) {
            if ((i > 0 && expr[i-1] != '<' && expr[i-1] != '>') || (expr[i] != '=' && expr[i] != '>'))
                temp[++newIndex] = ' ';

            temp[++newIndex] = expr[i];

            //daca este minus sau plus ca operator unar
            if ((i == 0 || expr[i-1] == '(')  && (expr[i] == '-' || expr[i] == '+') )
                temp[++newIndex] = 'u';
            if(i < len-1 && isDigitOrLetter(expr[i+1]))
                temp[++newIndex] = ' ';
            i++;
        } else if (expr[i] == '(' || expr[i] == ')') {
            temp[++newIndex] = ' ';
            temp[++newIndex] = expr[i];
            temp[++newIndex] = ' ';
            i++;
        } else {
            temp[++newIndex] = expr[i];
            temp[++newIndex] = ' ';
            i++;
        }
    }
    temp[++newIndex] = '\0';
    free(expr);
    return temp;
}

//functie ce va crea vectorul de cuvinte
void createVector(struct Expression& input) {
    char* word;

    char* tempWords = addSpacesIn(input);
    int length = 0;

    word = strtok(tempWords, " ");
    while (word != NULL) {
        strcpy(input.infixVec[length++], word);
        word = strtok(NULL, " ");
    }

    input.infixVec[length][0] = 0;
    input.infixLength = length;

    free(tempWords);
}

//functie ce verfica daca expresia este sau nu este corecta din punct de vedere sintactic
bool isValidExpression(Expression& input) {
    if (input.expression == NULL)
        return true;
    input.isOK = true;
    char error[120] = "";
    char aux[50] = "";
    char* tempW;

    int paranthesis = 0;
    int i;
    int position = 0;

    deleteErrors();

    for(i=0; i<input.infixLength; ++i) {
        position = position + strlen(input.infixVec[i]);
        if (!strcmp(input.infixVec[i], "-u") || !strcmp(input.infixVec[i], "+u"))
            position--;

        strcpy(aux, input.infixVec[i]);
        tempW = (char*)strtok(aux, ".");
        if (strlen(tempW) > 12) {
            if (!strcmp(language, "Romanian"))
                strcpy(error, "Eroare: Numerele/Variabilele pot avea maxim 12 cifre/caractere!");
            else
                strcpy(error, "Numbers/Variables can have a maximum of 12 digits/characters!");
            addError(error,position-strlen(input.infixVec[i])+1);
            input.isOK = false;
        }//lungimea maxima a unei variabile/numar este de 12 cifre/caractere

        if (!isOperand(input.infixVec[i]) && !isBinaryOperator(input.infixVec[i])&& !isUnaryOperator(input.infixVec[i])) {
            if (strcmp(input.infixVec[i], ")") && strcmp(input.infixVec[i], "(")) {
                if (!strcmp(language, "Romanian"))
                    strcpy(error, "Eroare: Numele variabilei sau numarul nu este valid!");
                else
                    strcpy(error, "Error: Invalid variable name or number!");
                addError(error,position-strlen(input.infixVec[i])+1);
                input.isOK = false;
            }
        }//cuvantul nu este valid
        if (isOperand(input.infixVec[i]) && numberOfDecimalPoints(input.infixVec[i]) > 1) {
            if (!strcmp(language, "Romanian"))
                strcpy(error, "Eroare: Numarul are prea multe virgule!");
            else
                strcpy(error, "Error: The number has too many decimal points");
            addError(error,position-strlen(input.infixVec[i])+1);
            input.isOK = false;
        }//daca numarul are mai mult de o virgula
        if(input.infixVec[i][0]=='(') {
            if (i == input.infixLength-1) {
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Dupa \"(\" expresia se termina brusc!");
                else
                    strcpy (error, "Error: After \"(\" the expression ends abruptly!");
                addError(error,position);
                input.isOK = false;
            }
            if (i > 0 && isOperand(input.infixVec[i-1])) {
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Inainte de \"(\" este un numar/variabila");
                else
                    strcpy (error, "Error: Before \"(\" is a number or a variable");
                addError(error,position);
                input.isOK = false;
            }
            if (i < input.infixLength - 1 && isBinaryOperator(input.infixVec[i+1])) {
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Dupa \"(\" este un operator binar");
                else
                    strcpy (error, "Error: After \"(\" is a binary operator!");
                addError(error,position);
                input.isOK = false;
            }
            paranthesis++;
        }//erori cu paranteza deschisa
        if(input.infixVec[i][0]==')') {
            if (i == 0) {
                if (!strcmp(language, "Romanian"))
                    strcpy(error, "Eroare: Expresia incepe cu o paranteza inchisa!");
                else
                    strcpy(error, "Error: The expression starts with a closed bracket!");
                addError(error,position);
                input.isOK = false;
            }
            if (i < input.infixLength - 1 && !strcmp(input.infixVec[i+1], "(")) {
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Intre \")\" si \"(\" nu este nici un operator  binar!");
                else
                    strcpy (error, "Error: Between \")\" and \"(\" is not any binary operator!");
                addError(error,position);
                input.isOK = false;
            }
            if (i < input.infixLength - 1 && isOperand(input.infixVec[i+1])) {
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Dupa \")\" este un numar sau o variabila");
                else
                    strcpy (error, "Error: After \")\" is a number or a variable");
                addError(error,position);
                input.isOK = false;
            }
            if (i > 0 && isBinaryOperator(input.infixVec[i-1])) {
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Inainte de \")\" este un operator binar");
                else
                    strcpy (error, "Error: Before \")\" is a binary operator");
                addError(error,position);
                input.isOK = false;
            }
            if (i > 0 && isUnaryOperator(input.infixVec[i-1])) {
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Inainte de \")\" este un operator unar");
                else
                    strcpy (error, "Error: Before \")\" is a unary operator");
                addError(error,position);
                input.isOK = false;
            }
            if (i < input.infixLength - 1 && isUnaryOperator(input.infixVec[i+1])) {
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Dupa \")\" este un operator unar");
                else
                    strcpy (error, "Error: After \")\" is a unary operator");
                addError(error,position);
                input.isOK = false;
            }
            paranthesis--;
            if (paranthesis < 0) {
                paranthesis++;
                if (!strcmp(language, "Romanian"))
                    strcpy (error, "Eroare: Avem o paranteza inchisa care nu se deschide");
                else
                    strcpy (error, "Error: There is a closed bracket that does not open");
                addError(error,position);
                input.isOK = false;
            }
        }//erori cu paranteeza inchisa

        if (i == input.infixLength-1 && isBinaryOperator(input.infixVec[i])) {
            if (!strcmp(language, "Romanian"))
                strcpy(error, "Eroare: Operator la sfarsitul expresiei!");
            else
                strcpy(error, "Error: Operator at the end of the expression!");
            addError(error,position);
            input.isOK = false;
        } else if (i == input.infixLength-1 && isUnaryOperator(input.infixVec[i])) {
            if (!strcmp(language, "Romanian"))
                strcpy (error, "Eroare: Operator la sfarsitul expresiei!");
            else
                strcpy (error, "Error: Operator at the end of the expression!");
            addError(error,position-strlen(input.infixVec[i])+1);
            input.isOK = false;
        }

        if (i == 0 && isBinaryOperator(input.infixVec[i])) {
            if (!strcmp(language, "Romanian"))
                strcpy(error, "Eroare: Operator binar la inceputul expresiei!");
            else
                strcpy(error, "Error: Binary operator at the beginning of the expression!");
            addError(error,position);
            input.isOK = false;
        }

        if(i < input.infixLength - 1) {
            if(input.infixVec[i][0]=='(' && input.infixVec[i+1][0]==')') {
                if (!strcmp(language, "Romanian"))
                    strcpy(error, "Eroare: Se asteapta o valoare sau o expresie intre paranteze!");
                else
                    strcpy(error, "Error: Expected value or expression between brackets!");
                addError(error,position);
                input.isOK = false;
            }
            if (isBinaryOperator(input.infixVec[i]) && isBinaryOperator(input.infixVec[i+1])) {
                if (!strcmp(language, "Romanian"))
                    strcpy(error, "Eroare: Nu pot exista doi operatori pe pozitii consecutive!");
                else
                    strcpy(error, "Error: There cannot be two operators in consecutive positions!");
                addError(error,position);
                input.isOK = false;
            }
            if (isUnaryOperator(input.infixVec[i]) && isBinaryOperator(input.infixVec[i+1])) {
                if (!strcmp(language, "Romanian"))
                    strcpy(error, "Eroare: Nu pot exista doi operatori pe pozitii consecutive!");
                else
                    strcpy(error, "Error: There cannot be two operators in consecutive positions!");
                addError(error,position);
                input.isOK = false;
            }
        }

        if(isUnaryOperator(input.infixVec[i]) && strcmp(input.infixVec[i+1], "(")) {
            if (strcmp(input.infixVec[i], "-u") && strcmp(input.infixVec[i], "+u")) {
                if(!strcmp(language, "Romanian"))
                    strcpy(error, "Eroare: Functia ");
                else
                    strcpy(error, "Error: The function ");
                strcat(error, input.infixVec[i]);
                if(!strcmp(language, "Romanian"))
                    strcat(error, " nu are niciun argument!");
                else
                    strcat(error, " has no argument!");
                addError(error,position-strlen(input.infixVec[i])+1);
                input.isOK = false;
            }
        }
        if(i < input.infixLength - 1 && isUnaryOperator(input.infixVec[i]) && input.infixVec[i+1][0]=='(')
            if (input.infixVec[i+2][0]==')'){
                if (strcmp(input.infixVec[i], "-u") && strcmp(input.infixVec[i], "+u")) {
                    if(!strcmp(language, "Romanian"))
                        strcpy(error, "Eroare: Functia ");
                    else
                        strcpy(error, "Error: The function ");
                    strcat(error, input.infixVec[i]);
                    if(!strcmp(language, "Romanian"))
                        strcat(error, " nu are niciun argument!");
                    else
                        strcat(error, " has no argument!");
                    addError(error,position-strlen(input.infixVec[i])+1);
                    input.isOK = false;
                }
            }
    }

    if(paranthesis != 0) {
        if (paranthesis > 0) {
            if(!strcmp(language, "Romanian"))
                strcpy(error, "Eroare: Nu toate parantezele deschise se inchid!");
            else
                strcpy(error, "Error: Not all open brackets are closed!");
            addError(error, 0);
        } else {
            if (!strcmp(language, "Romanian"))
                strcpy(error, "Eroare:Exista paranteze inchise care nu au deschidere!");
            else
                strcpy(error, "Error: There are closed brackets that have no opening!");
            addError(error, 0);
        }
        input.isOK = false;
    }//erori parantezare

    return input.isOK;
}//sfarsit isValidExpression


bool checkOperatorPrecedence(char*, Stack*);
//transforma expresia din ordine infixata in ordine postfixata
void infixToPostfix(Expression& input) {
    Stack* tempStack = NULL;
    int i = 0, k = -1;
    while (i < input.infixLength) {
        //daca cuvantul scanat este un operand atunci il aduga direct in expresia postfixata
        if (isOperand(input.infixVec[i]))
            strcpy(input.postfixVec[++k], input.infixVec[i]);
        //daca cuvantul scanat este o paranteza deschisa o adauga in stiva
        else if (*input.infixVec[i] == '(')
            push(tempStack, input.infixVec[i]);
        //daca cuvantul scanat este o paranteza inchisa,elimina si pune din stiva elementele in expresia postfixata
        //pana la intalnirea unei paranteze deschise
        else if (*input.infixVec[i] == ')') {
            while (!isEmpty(tempStack) && *(char*)top(tempStack) != '(')
                strcpy(input.postfixVec[++k], (char*)pop(tempStack));
            pop(tempStack);
        }

        //daca cuvantul scanat este un operator
        else {
            while (!isEmpty(tempStack) && checkOperatorPrecedence(input.infixVec[i], tempStack))
                strcpy(input.postfixVec[++k],(char*)pop(tempStack));
            push(tempStack, input.infixVec[i]);
        }

        i++;
    }
    //elimina toti operatorii din stiva
    while (!isEmpty(tempStack))
        strcpy(input.postfixVec[++k], (char*)pop(tempStack));

    input.postfixVec[++k][0] = 0;

    return;
}

//asigura ordinea corecta a efectuarii operatiilor
bool checkOperatorPrecedence(char* infixVec, Stack* tempStack) {
    if(operatorPriority(infixVec) <= operatorPriority((char*)top(tempStack)) && *infixVec != '^')
        return true;

    else if (operatorPriority(infixVec) <= operatorPriority((char*)top(tempStack)) && *infixVec == '^' && isUnaryOperator((char*)top(tempStack)))
        return true;
    else
        return false;
}


double _evaluateExpression(treeNode*);
double evaluateExpression(Expression& input){
    infixToPostfix(input);
    treeNode* expressionTree = getExpressionTree(input.postfixVec);
    if (vars != NULL)
        resetVariables(vars);
    return _evaluateExpression(expressionTree);
}

//evalueaza expresia
double _evaluateExpression(treeNode* expressionTree) {
    if (expressionTree == NULL)
        return 0;

    if (expressionTree->left && expressionTree->right) {
        double leftValue = _evaluateExpression(expressionTree->left);
        double rightValue = _evaluateExpression(expressionTree->right);

        char* Operator = expressionTree->info;
        switch(*Operator) {
            case '+':
                if (strcmp(Operator, "+u"))
                    return Plus(leftValue, rightValue);
            case '-':
                if (strcmp(Operator, "-u"))
                    return Minus(leftValue, rightValue);
            case '*':
                return Multiply(leftValue, rightValue);
            case '/':
                return Divide(leftValue, rightValue);
            case '%':
                return Modulo(leftValue, rightValue);
            case '^':
                return Power(leftValue, rightValue);
            case '=':
                return Equal(leftValue, rightValue);
            case '#':
                return notEqual(leftValue, rightValue);
            case '<':
                if (!strcmp(Operator, "<="))
                    return lessOrEqual(leftValue, rightValue);
                else if (!strcmp(Operator, "<>"))
                    return notEqual(leftValue, rightValue);
                else if (!strcmp(Operator, "<"))
                    return Less(leftValue, rightValue);
                break;
            case '>':
                if (!strcmp(Operator, ">="))
                    return greaterOrEqual(leftValue, rightValue);
                else if (!strcmp(Operator, ">"))
                    return Greater(leftValue, rightValue);
                break;
            default:
                return 0;

        }
    } else if(expressionTree->left == NULL && expressionTree->right == NULL) {
        double value;
        value = convertToNumber(expressionTree->info);
        if (isnan(value)) {
            char* variableName = expressionTree->info;

            if (isVariableSet(vars, variableName))
                value = getVariableValue(vars, variableName);
            else
                value = getUninitializedVariables(vars, variableName);
        }
        if (value > INFINIT)
            return INFINITY;
        return value;
    } else if (expressionTree->right) {
        double value = _evaluateExpression(expressionTree->right);

        char* Operator = strToLower(expressionTree->info);
        switch(*Operator) {
            case 'a':
                if (!strcmp(Operator, "abs"))
                    return Absolute(value);
                else if (!strcmp(Operator, "asin"))
                    return arcSinus(value);
                else if (!strcmp(Operator, "acos"))
                    return arcCosinus(value);
                else if (!strcmp(Operator, "atan"))
                    return arcTangent(value);
                else if (!strcmp(Operator, "actg"))
                    return arcCotangent(value);
                break;
            case 'c':
                if (!strcmp(Operator, "ctg"))
                    return Cotangent(value);
                if (!strcmp(Operator, "cos"))
                    return Cosinus(value);
                break;
            case 'l':
                if (!strcmp(Operator, "ln"))
                    return Log(value);
                else if (!strcmp(Operator, "log2"))
                    return LogInBase(value, 2);
                else if (!strcmp(Operator, "log"))
                    return LogInBase(value, 10);
                break;
            case 's':
                if (!strcmp(Operator, "sin"))
                    return Sinus(value);
                else if (!strcmp(Operator, "sqrt"))
                    return Radical(value);
                break;
            case 't':
                if (!strcmp(Operator, "tan"))
                    return Tangent(value);
                break;
            case '-':
                if (!strcmp(Operator, "-u"))
                    return UnaryMinus(value);
                break;
            case '+':
                if (!strcmp(Operator, "+u"))
                    return UnaryPlus(value);
                break;
            default:
                return 0;
        }
    }
    return 0;
}







