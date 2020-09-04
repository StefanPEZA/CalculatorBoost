struct Stack {
    char* info;  //stiva are valoarea de tip void pentru a putea stoca mai multe tipuri de informatii
    Stack* next; //urmatorul element din stiva
};

//functie ce verifica daca stiva este goala
bool isEmpty(Stack* S) {
    if (S==NULL)
        return true;
    return false;
}

//functie ce adauga un nou element de tip 'Type' in stiva
void push(Stack* &S, char* info) {
    if (S == NULL) {
        S = (Stack*)malloc(sizeof(Stack));
        S->info = info;
        S->next = NULL;
        return;
    }


    Stack* nodNou = (Stack*)malloc(sizeof(Stack));;
    nodNou->info = info;
    nodNou->next = S;
    S = nodNou;

}

//functie ce va sterge un element din stiva si-l va returna
char* pop(Stack*& S) {
    if (S == NULL)
        return NULL;

    char* info = S->info;
    Stack* temp = S;
    S = S->next;
    free(temp);

    return info;
}

//functie ce returneaza varful stivei
char* top(Stack* S) {
    if (S == NULL)
        return NULL;

    return S->info;
}

struct treeNode;
struct StackTree {
    treeNode* info;  //stiva are valoarea de tip void pentru a putea stoca mai multe tipuri de informatii
    StackTree* next; //urmatorul element din stiva
};

//functie ce verifica daca stiva este goala
bool isEmptyTree(StackTree* S) {
    if (S==NULL)
        return true;
    return false;
}

//functie ce adauga un nou element de tip 'Type' in stiva
void pushTree(StackTree* &S, treeNode* info) {
    if (S == NULL) {
        S = (StackTree*)malloc(sizeof(StackTree));
        S->info = info;
        S->next = NULL;
        return;
    }


    StackTree* nodNou = (StackTree*)malloc(sizeof(StackTree));;
    nodNou->info = info;
    nodNou->next = S;
    S = nodNou;

}

//functie ce va sterge un element din stiva si-l va returna
treeNode* popTree(StackTree*& S) {
    if (S == NULL)
        return NULL;

    treeNode* info = S->info;
    StackTree* temp = S;
    S = S->next;
    free(temp);

    return info;
}

//functie ce returneaza varful stivei
treeNode* topTree(StackTree* S) {
    if (S == NULL)
        return NULL;

    return S->info;
}



