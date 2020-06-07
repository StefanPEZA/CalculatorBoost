#include <iostream>
#include <string>
#include <unordered_map>
#include <string.h>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/constants/constants.hpp>
#include "winbgim.h"
#include "TextBox.h"
#include "Expression.h"
using namespace boost::multiprecision;

TextBox* inputText = nullptr;
Expression* inputExpression = nullptr;

std::string resultString;
void manageInputExpression();

int __stdcall WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initwindow(1100, 600, "Calculator");
	std::ostringstream SS;

	inputText = new TextBox(10, 15, getmaxx()-200, 35);
	inputExpression = new Expression();

	settextstyle(3, 0, 2);
	while (1)
	{
		inputText->setTextInput();
		TextBox::clearKeyBoardBuffer();
		manageInputExpression();
		cleardevice();
		inputText->Draw();
		SS << "= " << resultString << "\n\n";
		
		outstreamxy(8, 60, SS);
		delay(10);
		swapbuffers();
	}
	getch();
}

bool updateText = true;
void manageInputExpression()
{
	if (inputText->isReturnPressed())
	{
		if (updateText) {
			cpp_dec_float_50 result;
			std::stringstream str;
			str.precision(50);
			result = inputExpression->Eval(inputText->text);
			if (boost::math::isnan(result))
			{
				resultString = "Eroare";
			}
			else
			{
				str << result;
				str >> resultString;
			}
		}
		updateText = false;
	}
	else
		updateText = true;
}
