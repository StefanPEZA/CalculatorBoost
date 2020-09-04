struct treeNode {
    char* info;
    treeNode* left;
    treeNode* right;
};

//construieste un nou nod
treeNode* newTreeNode(char info[]) {
    treeNode* newTree = new treeNode;
    newTree->info = info;
    newTree->left = NULL;
    newTree->right = NULL;
    return newTree;
}

bool isOperand(char*);
bool isUnaryOperator(char*);
bool isBinaryOperator(char*);
double convertToNumber(char*);
int nrNodes = 0;
//construieste arborele de expresie care va fi folosit pentru evaluarea functiei
treeNode* getExpressionTree(char postfixVec[][30]){
    StackTree *treeStack = NULL;
    treeNode *tree, *tree1, *tree2;
    nrNodes = 0;
    // parcurge fiecare cuvant din expresia postfixata
    for (int i=0; postfixVec[i][0] != 0; i++) {
        nrNodes++;
        // Daca e operand, aduga in stiva
        if (isOperand(postfixVec[i])) {
            tree = newTreeNode(postfixVec[i]);
            pushTree(treeStack, tree);
        } else if (isUnaryOperator(postfixVec[i])) {
            //construieste un nod cu operatorul unar care va avea doar un fiu
            tree = newTreeNode(postfixVec[i]);

            tree1 = (treeNode*)topTree(treeStack);
            popTree(treeStack);

            tree->right = tree1;
            tree->left = NULL;

            pushTree(treeStack, tree);
        } else if (isBinaryOperator(postfixVec[i])) { // operator
            //construieste un nou nod cu operatorul binar care va avea doi fii
            tree = newTreeNode(postfixVec[i]);

            //salveaza ultimi 2 subarbori din varful stivei
            tree1 = (treeNode*)topTree(treeStack);
            popTree(treeStack);
            tree2 = (treeNode*)topTree(treeStack);
            popTree(treeStack);

            //cei 2 subarbori din stiva vor devenii fii subarborelui nou creat
            tree->right = tree1;
            tree->left = tree2;

            //Adauga subarborele obtinut in stiva
            pushTree(treeStack, tree);
        }
    }

    tree = (treeNode*)topTree(treeStack);
    popTree(treeStack);

    return tree;
}

//parcurge arborele in ordine infixata
void checkInfix(treeNode* tree) {
    if (tree == NULL)
        return;

    if (tree->left && tree->right) {
        std::cout << "(";
        checkInfix(tree->left);
        std::cout << tree->info;
        checkInfix(tree->right);
        std::cout << ")";
    } else if (tree->right) {
        std::cout << tree->info << "(";
        checkInfix(tree->right);
        std::cout << ")";
    } else
        std::cout << tree->info;

}

//parcurge arborele in ordine posfixata
void checkPostfix(treeNode* tree) {
    if (tree == NULL)
        return;

    if (tree->left && tree->right) {
        checkPostfix(tree->left);
        checkPostfix(tree->right);
        std::cout << tree->info << " ";
    } else if (tree->right) {
        checkPostfix(tree->right);
        std::cout << tree->info << " ";
    } else
        std::cout << tree->info << " ";
}


//parcurge arborele in ordine prefixata
void checkPrefix(treeNode* tree) {
    if (tree == NULL)
        return;

    if (tree->left && tree->right) {
        std::cout << tree->info << " ";
        checkPrefix(tree->left);
        checkPrefix(tree->right);
    } else if (tree->right) {
        std::cout << tree->info << " ";
        checkPrefix(tree->right);
    } else
        std::cout << tree->info << " ";
}
