struct Variables { //o lista care va memora toate variabilele expresiei
    char* name; //numele variabilei
    double value;  //valoarea variabilei
    Variables* next; //urmatoarea variabila

    Variables(char* name, double value):name(name), value(value), next(NULL) {}
}*vars;

//functie pentru a aduga o variabila noua in lista de variabile
void addVariable(Variables* &vars, char* name, double value) {
    Variables* newVar = new Variables(name, value);

    if (vars == NULL) {
        vars = newVar;
        return;
    }

    Variables* tempVar = vars;
    while(tempVar->next)
        tempVar = tempVar->next;
    tempVar->next = newVar;
}

//sterge toate valorile si variabilele
void resetVariables(Variables* &vars) {
    Variables* temp = vars;
    while(temp != NULL) {
        temp = vars->next;
        delete(vars);
        vars = temp;
    }
}

//deseneaza variabilele pe ecran
void drawVariables(Variables* &vars, int x, int y) {
    Variables* p = vars;
    char text[50];
    int i = 0;
    while(p) {
        sprintf(text, "%s = %.7f", p->name, p->value);
        outtextxy(x, y+i*(textheight(text)+5), text);
        i++;
        p = p->next;
    }
}

//returneaza valoarea variabilei in caz ca o gaseste,iar in caz contrar va pune utilizatorul sa dea o valoare variabilei respective
double getVariableValue(Variables* &vars, char name[]) {
    bool ok;
    double value;

    Variables* tempVar = vars;
    while (tempVar) {
        if (!strcmp(tempVar->name, name))
            return tempVar->value;
        tempVar = tempVar->next;
    }

    do {
        if (!strcmp(name, "PI")) {
            value = PI; //constanta pi
            addVariable(vars, name, value);
            ok = 1;
        } else if (!strcmp(name, "E")) {
            value = E; //constanta euler
            addVariable(vars, name, value);
            ok = 1;
        } else {
            return NAN;//Not a number, reprezinta faptul ca variabila nu a fost initializata inainte
        }
    } while (ok == 0);

    return value;
}

//verifica daca o variabila este initializata
bool isVariableSet(Variables* &vars, char name[]) {
    if(isnan(getVariableValue(vars, name)))
        return false;
    return true;
}

//in caz ca variabila nu a fost initializata
//se va dechide o fereastra noua care va cere o valoare utilizatorului
double getUninitializedVariables(Variables* &vars, char name[]) {
    double value;

    int length = 0;
    char valueString[50] = "";
    char temp[25];
    char key;

    bool ok = false;

    int height = 300;
    int width = 840;
    int thisWindow = initwindow(width,height, "Input variabile...", getmaxwidth()/2-width/2, getmaxheight()/2-height/2);

    setbkcolor(COLOR(0,0,100));


    int counter = 0;
    bool firstPoint = true;
    int page = 0;

    settextstyle(FONT, 0, FONT_SIZE);
    setcurrentwindow(thisWindow);
    while(!ok) {

        //double buffering
        setactivepage(page);
        setvisualpage(1-page);

        if (kbhit()) {
            key = (char)getch();
            if (key != 0) {
                if (length == 0 && (key == '-' || key == '+'))
                {
                    valueString[length]= key;
                    length++;
                    counter++;
                    valueString[length] = '\0';
                }
                if ((key >= '0' && key <= '9') && (length < 10 || counter < 9)) {
                    valueString[length]= key;
                    length++;
                    counter++;
                    valueString[length] = '\0';
                } else if (key == '.') {
                    if (firstPoint) {
                        firstPoint = false;
                        counter=0;
                        valueString[length]= key;
                        length++;
                        valueString[length] = '\0';
                    }
                } else if (key == '\b' && length > 0) {
                    counter--;
                    if (counter < 0)
                        firstPoint = true;
                    valueString[length-1] = '\0';
                    length--;
                }
            } else
                getch();
        }

        if (GetAsyncKeyState(VK_RETURN) && strlen(valueString) > 0) {
            if (isnan(convertToNumber(valueString))) {
                strcpy(valueString, "");
                length = 0;
            } else {
                ok = true;
                value = convertToNumber(valueString);
                addVariable(vars, name, value);
            }
        }

        setbkcolor(COLOR(0,0,100));
        cleardevice();
        sprintf(temp, "%s%s", name, "= ");

        if (!strcmp(language, "Romanian"))
            outtextxy(10,10, (char*)"Introduceti o valoare pentru variabila:");
        else
            outtextxy(10,10, (char*)"Enter a value for the variable:");
        outtextxy(10,40, temp);
        outtextxy(textwidth(temp)+5,40, valueString);

        page = 1 - page;

        delay(15);
    }
    setcurrentwindow(0);
    closegraph(thisWindow);
    return value;

}
