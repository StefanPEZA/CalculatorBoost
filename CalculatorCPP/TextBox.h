#pragma once
class TextBox {
public:
	float left;
	float top;
	float right;
	float bottom;
	int height;
	float center;
	int width;
	int pos;
	bool isSelected;
	bool readOnly;
	std::string text;

	static void clearKeyBoardBuffer();

	TextBox()
	{
		this->left = 0;
		this->top = 0;
		this->right = 0;
		this->bottom = 0;
		this->height = 0;
		this->center = 0;
		this->width = 0;
		this->text = "";
		this->pos = 0;
		this->isSelected = false;
		this->readOnly = false;
	}

	TextBox(float left, float top, int width, int height)
	{
		this->left = left;
		this->top = top;
		this->right = left + width;
		this->bottom = top + height;
		this->height = height;
		this->width = width;
		this->center = height / 2 + top;
		this->text = "";
		this->pos = 0;
		this->isSelected = false;
		this->readOnly = false;
	}

	void MoveDown(float y)
	{
		this->top += y;
		this->bottom += y;
		this->center = height / 2 + top;
	}

	void Draw()
	{
		if (isSelected)
			setcolor(YELLOW);
		else
			setcolor(WHITE);
		outtextxy((int)this->left + 5, (int)this->center - textheight((char*)this->text.c_str()) / 2, (char*)this->text.c_str());
		rectangle((int)this->left, (int)this->top, (int)this->right, (int)this->bottom);
		if (!readOnly)
			line(textwidth((char*)this->text.substr(0, this->pos).c_str())+ (int)this->left+5, (int)this->top+6, textwidth((char*)this->text.substr(0, this->pos).c_str()) + (int)this->left+5, (int)this->bottom-6);
		setcolor(WHITE);
	}

	void setTextInput()
	{
		this->getFocus();
		if (kbhit() && isSelected) {
			char key = getch();
			if (key != 0) {
				if (key == '\b') {
					this->pos--;
					if (this->pos < 0)
						this->pos = 0;
					else
						this->text.erase(this->text.begin() + this->pos);
				}
				else if (textwidth((char*)this->text.c_str()) < this->width - 20 && isKeyOk(key)) {
					this->text.insert(this->text.begin() + this->pos, key);
					this->pos++;
				}
			}
			else {
				key = getch();
				if (key == KEY_LEFT) {
					this->pos--;
					if (this->pos <= 0)
						this->pos = 0;
				}
				else if (key == KEY_RIGHT) {
					this->pos++;
					if (this->pos >= (int)this->text.length())
						this->pos = this->text.length();
				}
				else if (key == KEY_DELETE) {
					if (this->pos < (int)this->text.length())
						this->text.erase(this->text.begin() + this->pos);
				}
			}

		}
	}

	bool isClicked;
	void getFocus()
	{
		if (GetAsyncKeyState(VK_LBUTTON) && !readOnly) {
			if (mousex() > this->left&& mousex() < this->right&& mousey() < this->bottom&& mousey() > this->top) {
				if (!isClicked)
				{
					this->isSelected = !this->isSelected;
				}
			}
			else
				this->isSelected = false;
			isClicked = true;
		}
		else
			isClicked = false;
	}

	bool isKeyOk(char key)
	{
		if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))
			return true;
		if (strchr("+-*/^)(.", key))
		{
			if (this->pos > 0) {
				if (strchr("+-*/^", this->text[pos - 1]) && key != '(')
					return false;
			}
			else if (strchr("*/^)", key))
				return false;
			return true;
		}
		return false;
	}

	bool isReturnPressed()
	{
		if (GetAsyncKeyState(VK_RETURN) && isSelected)
			return true;
		else
			return false;
	}
};

void TextBox::clearKeyBoardBuffer() {
	while (kbhit())
	{
		getch();
	}
}