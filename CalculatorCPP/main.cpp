/****************************************************************************
------------------------Evaluator matematic----------------------------------

*Proiect realizat de: -Pezamosca Stefanel
                      -Nitica Victor
*****************************************************************************/
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <thread>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <graphics.h>

#define FONT 8
#define FONT_SIZE 3

#include "lib/mainLib.h"//librarie principala de functii

using namespace std;

Expression inputExpression;//salveaza expresia
double result = 0;
char resultText[100];
bool Key_pressed = false;


struct TextBox {
    int left;
    int top;
    int height;
    int width;
    int maxTextLength;
    int selectedPosition;
    bool isSelected;
    char* text;
} inputTextBox;

void newTextBox(TextBox& textBox, int left, int top, int height, int width, int maxLength) {
    textBox.left = left;
    textBox.top = top;
    textBox.height = height;
    textBox.width = width;
    textBox.maxTextLength = maxLength;
    textBox.isSelected = true;
    textBox.selectedPosition = 0;
    textBox.text = (char*)calloc(105, sizeof(char));
}

void drawTextBox(TextBox& textBox) {
    int left = textBox.left;
    int top = textBox.top;
    int bottom = textBox.top + textBox.height;
    int right = textBox.left + textBox.width;

    int selectedIndex = textBox.selectedPosition;

    char* tempText = (char*)malloc(105*sizeof(char));
    *tempText = '\0';
    if (strlen(textBox.text) > 0)
        strncpy(tempText, textBox.text, selectedIndex);
    tempText[selectedIndex] = '\0';

    setcolor(YELLOW);
    outtextxy(left + 5, top+(bottom-top)/2 - textheight(textBox.text)/2, textBox.text);

    setlinestyle(0, 0, 2);
    rectangle(left, top, right, bottom);

    setlinestyle(0, 0, 1);
    line (left + textwidth(tempText)+5, top + 5, left + textwidth(tempText)+5, bottom - 5);
    free(tempText);
}

bool isKeyAccepted(char key) {
    char temp[2] = "";
    sprintf(temp, "%c", key);
    if (isDigitOrLetter(key) || isBinaryOperator(temp))
        return true;
    if (key == '.' || key == '(' || key == ')' || key == ' ')
        return true;

    return false;
}

void manageTextInput(TextBox&);
void getTextInput(TextBox& textBox) {

    int Index;
    char key;

    Index = textBox.selectedPosition;

    if (kbhit() && !Key_pressed) {
        key = (char)getch();
        if (key != 0) {
            if (isKeyAccepted(key) && (int)strlen(textBox.text) < textBox.maxTextLength && textwidth(textBox.text) < (textBox.width + textBox.left)-50) {
                for (int i = (int)strlen(textBox.text)+1 ; i >= Index; i--)
                    textBox.text[i] = textBox.text[i-1];
                textBox.text[Index] = key;

                Index ++;
                Key_pressed = true;
            } else if (key == '\b' && strlen(textBox.text)>0 && Index > 0) {
                for (int i = Index-1 ; i < (int)strlen(textBox.text); i++)
                    textBox.text[i] = textBox.text[i+1];
                Index--;
                Key_pressed = true;
            }
        } else {
            key = (char)getch();
            if (key == 75) {
                if (Index > 0 && !Key_pressed)
                    Index--;
                Key_pressed = true;
            } else if (key == 77) {
                if (Index < (int)strlen(textBox.text) && !Key_pressed)
                    Index++;
                Key_pressed = true;
            }
        }
    }

    if (GetAsyncKeyState(VK_RETURN)) {
        if (textBox.isSelected && !Key_pressed) {
            manageTextInput(textBox);
            Index = strlen(textBox.text);
        }
        Key_pressed = true;
    } else
        Key_pressed = false;

    textBox.selectedPosition = Index;
}

bool playSound;
void manageTextInput(TextBox& textBox) {
    removeSpaces(textBox.text);
    setExpression(inputExpression, textBox.text);
    bool isValid = isValidExpression(inputExpression);
    if (!isValid)
        playSound = true;
    else if (isValid) {
        result = evaluateExpression(inputExpression);
        strcpy(resultText, "");
        if (isnan(result)) {
            if (!strcmp(language, "Romanian"))
                strcpy(resultText, "= Nu este numar (real)");
            else
                strcpy(resultText, "= It's not a number (real number)");
        } else if (result == INFINITY) {
            if (!strcmp(language, "Romanian"))
                strcpy(resultText, "= Infinit/Un numar prea mare pentru a putea fi reprezentat");
            else
                strcpy(resultText, "= Infinity/A number too large to be represented");
        } else if (result == -INFINITY) {
            if (!strcmp(language, "Romanian"))
                strcpy(resultText, "= -Infinit/Un numar prea mic pentru a putea fi reprezentat");
            else
                strcpy(resultText, "= -Infinity/A number too small to be represented");
        } else
            sprintf(resultText, "= %.7f", result);
    }

}

struct Button {
    int left;
    int top;
    int height;
    int width;
    int state;
    bool pressed;
    char text[14];
};

void newButton(Button& button, int left, int top, int width, int height) {
    button.pressed = false;
    button.state = 1;
    button.top = top;
    button.left = left;
    button.height = height;
    button.width = width;
    if (!strcmp(language, "Romanian"))
        strcpy(button.text, "Radiani");
    else
        strcpy(button.text, "Radians");
}

void drawButton(Button &button) {
    int bkcolor = GREEN;
    int txtcolor = WHITE;
    int left = button.left;
    int top = button.top;
    int bottom = button.top + button.height;
    int right = button.left + button.width;

    if (!GetAsyncKeyState(VK_LBUTTON))
        button.pressed = false;

    if (mousex()>left && mousex() <right && mousey() > top && mousey()<bottom ) {
        if (GetAsyncKeyState((VK_LBUTTON)) && !button.pressed) {
            PlaySound(TEXT("Data/Select.wav"), NULL, SND_FILENAME|SND_ASYNC);
            if (button.state == 1) {
                setUsingRadiansTo(true);
                button.state = 0;
            } else {
                setUsingRadiansTo(false);
                button.state = 1;
            }
            button.pressed = true;
            delay(200);

        }

        bkcolor = LIGHTGREEN;
        txtcolor = GREEN;
    } else {
        bkcolor = GREEN;
        txtcolor = WHITE;
    }

    if (GetAsyncKeyState(VK_LBUTTON))
        button.pressed = true;

    if (button.state == 1) {
        if (!strcmp(language, "Romanian"))
            strcpy(button.text, "Radiani");
        else
            strcpy(button.text, "Radians");
        setUsingRadiansTo(true);
    } else {
        if (!strcmp(language, "Romanian"))
            strcpy(button.text, "Grade");
        else
            strcpy(button.text, "Degrees");
        setUsingRadiansTo(false);
    }

    setfillstyle(SOLID_FILL, bkcolor);
    bar(left, top, right, bottom);

    setbkcolor(bkcolor);
    setcolor(txtcolor);
    outtextxy(left + (right-left)/2 - textwidth(button.text)/2, top + (bottom-top)/2-textheight(button.text)/2, button.text);

    setbkcolor(COLOR(0,0,60));

    setcolor(WHITE);
    rectangle(left, top, right, bottom);
}

void drawLanguageButton(int, int, int, int);
void showOutput(TextBox&, bool&);
bool update = true;

int main () {
    strcpy(language, "Romanian");
    setExpression(inputExpression, NULL);
    int page = 0;
    int height = 605;
    int width = 1200;

    initwindow(width, height, "Evaluator matematic", getmaxwidth()/2-width/2, getmaxheight()/2-height/2);
    newTextBox(inputTextBox, 10, 40, 40, getmaxx()-20, 100);

    Button RadiansOrDegrees;
    newButton(RadiansOrDegrees, getmaxx()-150, 5, 140, 30);

    setbkcolor(COLOR(0,0,60));
    settextstyle(FONT, 0, FONT_SIZE);
    while(true) {
        //double buffering
        setactivepage(page);
        setvisualpage(1-page);

        getTextInput(inputTextBox);

        cleardevice();

        if (!strcmp(language, "Romanian"))
            outtextxy(50,8, (char*)"Scrieti expresia algebrica aici:");
        else
            outtextxy(50,8, (char*)"Write the algebraic expression here:");

        drawTextBox(inputTextBox);
        drawButton(RadiansOrDegrees);
        drawLanguageButton(8,10,46,30);
        showOutput(inputTextBox, inputExpression.isOK);

        page = 1-page;

        delay(10);

    }

    getch();
    closegraph();
    return 0;
}

void showOutput(TextBox& inputTextBox, bool &isValid) {
    if (isValid) {
        outtextxy(10,inputTextBox.height+inputTextBox.top + textheight(resultText)/2, resultText);
        if (vars != NULL) {
            int x = 10;
            int y = inputTextBox.height+inputTextBox.top + 50;
            if (!strcmp(language, "Romanian")) {
                outtextxy(x, y, (char*)"Variabile: ");
                drawVariables(vars, x+textwidth((char*)"Variabile: "), y);
            } else {
                outtextxy(x, y, (char*)"Variables: ");
                drawVariables(vars, x+textwidth((char*)"Variables: "), y);
            }
        }
    } else {
        if (playSound == true) {
            PlaySound(TEXT("Data/Error.wav"), NULL, SND_FILENAME|SND_ASYNC);
            playSound = false;
            delay(200);
        }
        int x = 8;
        int y = inputTextBox.height+inputTextBox.top + textheight((char*)"Expresie gresita!")/2;

        if (!strcmp(language, "Romanian"))
            outtextxy(x,y,(char*)"Eroare de sintaxa!");
        else
            outtextxy(x,y,(char*)"Syntax error!");
        int nrError = 0;
        for (int i = 0; i < 120; i++)
            if (error[i] != NULL) {
                Errors* tempErr = error[i];
                while (tempErr) {
                    nrError++;
                    outtextxy(x, y + (nrError)*(textheight(tempErr->errorMessage)) + 5, tempErr->errorMessage);
                    tempErr = tempErr->next;
                }
            }
    }
}

bool languagePressed = false;
void drawLanguageButton(int left, int top, int right, int bottom) {
    if (!GetAsyncKeyState(VK_LBUTTON))
        languagePressed = false;

    if (mousex()>left && mousex() <right && mousey() > top && mousey()<bottom ) {
        if (GetAsyncKeyState((VK_LBUTTON)) && !languagePressed) {
            PlaySound(TEXT("Data/Select.wav"), NULL, SND_FILENAME|SND_ASYNC);
            if (!strcmp(language, "Romanian"))
                strcpy(language, "English");
            else if (!strcmp(language, "English"))
                strcpy(language, "Romanian");
            languagePressed = true;
            deleteErrors();
            isValidExpression(inputExpression);
            delay(200);
        }
    }

    if (GetAsyncKeyState(VK_LBUTTON))
        languagePressed = true;

    if (!strcmp(language,"Romanian"))
        readimagefile("Data/romanian.bmp", left, top, right, bottom);
    else
        readimagefile("Data/english.bmp", left, top, right, bottom);
}
