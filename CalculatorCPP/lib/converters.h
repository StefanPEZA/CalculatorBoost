//converteste un sir de caractere intr-un numar real
double convertToNumber(char* ch) {
    bool isFraction = false;
    int j = 0;
    if (ch == NULL)
        return 0;
    double n = 0, p = 1;
    if (ch[0] == '-' || ch[0] == '+')
        j++;
    for (int i = j; ch[i]; i++) {
        if (!isFraction && (ch[i] != '.' && ch[i] != ',')) {
            if (ch[i] == '0')
                n = n*10 + 0;
            else if (ch[i] == '1')
                n = n*10 + 1;
            else if (ch[i] == '2')
                n = n*10 + 2;
            else if (ch[i] == '3')
                n = n*10 + 3;
            else if (ch[i] == '4')
                n = n*10 + 4;
            else if (ch[i] == '5')
                n = n*10 + 5;
            else if (ch[i] == '6')
                n = n*10 + 6;
            else if (ch[i] == '7')
                n = n*10 + 7;
            else if (ch[i] == '8')
                n = n*10 + 8;
            else if (ch[i] == '9')
                n = n*10 + 9;
            else
                return NAN;
        } else if (!isFraction && (ch[i] == '.' || ch[i] == ',')) {
            p *= 10;
            isFraction = true;
        } else if (isFraction && (ch[i] != '.' && ch[i] != ',')) {
            if (ch[i] == '0')
                n = n + 0;
            else if (ch[i] == '1')
                n = n + 1/p;
            else if (ch[i] == '2')
                n = n + 2/p;
            else if (ch[i] == '3')
                n = n+ 3/p;
            else if (ch[i] == '4')
                n = n + 4/p;
            else if (ch[i] == '5')
                n = n + 5/p;
            else if (ch[i] == '6')
                n = n+ 6/p;
            else if (ch[i] == '7')
                n = n + 7/p;
            else if (ch[i] == '8')
                n = n + 8/p;
            else if (ch[i] == '9')
                n = n + 9/p;
            else
                return NAN;
            p = p * 10;
        }
    }
    if (ch[0] == '-')
        n = -n;
    return n;
}

char* strToLower(char *str) { //converteste literele mari dintr-un sir de caractere in litere mici
    char* scopy= (char*)malloc(strlen(str)*sizeof(char));
    strcpy(scopy, str);
    int slength = strlen(scopy);
    for (int i = 0; i < slength; i++)
    {
        if (scopy[slength] >= 'A' && scopy[slength] <= 'Z')
            scopy[slength] = scopy[slength]+32;
    }
    scopy[slength] = 0;
    return scopy;
}


