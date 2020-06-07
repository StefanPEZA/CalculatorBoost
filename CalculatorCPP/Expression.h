#pragma once
class Expression {
public:
	std::string text;
	std::vector<std::string> infixText;
	std::vector<std::string> postfixText;


	boost::multiprecision::cpp_dec_float_50 epsilon;

	Expression() {
		epsilon = std::numeric_limits<boost::multiprecision::cpp_dec_float_50>::epsilon();
		this->unaryOperators.insert({ {"sin", 1},{"cos", 1},{"tg", 1}, {"ctg", 1}, {"asin", 1},{"acos", 1},{"atg", 1},{"actg", 1} });
		this->unaryOperators.insert({ {"log", 1}, {"ln", 1}, {"log2", 1} });
		this->unaryOperators.insert({ {"sqrt", 1}, {"exp", 1}, {"fact", 1} });
	}

	boost::multiprecision::cpp_dec_float_50 Eval(std::string expression);

	boost::multiprecision::cpp_dec_float_50 getValue(std::string val)
	{
		if (val == "pi")
			return boost::math::constants::pi<boost::multiprecision::cpp_dec_float_50>();
		else if (val == "e")
			return boost::math::constants::e<boost::multiprecision::cpp_dec_float_50>();

		try {
			boost::multiprecision::cpp_dec_float_50 value(val);
			return value;
		}
		catch (std::exception&)
		{
			return std::numeric_limits<boost::multiprecision::cpp_dec_float_50>::quiet_NaN();//error
		}

	}

private:
	std::stack<std::string> Result;
	std::stack<std::string> Stack;
	std::unordered_map<std::string, char> unaryOperators;

	void setTokensVectors() {
		std::string temp = "";
		this->infixText.clear();
		this->postfixText.clear();
		for (std::string::iterator it = this->text.begin(); it < this->text.end(); it++)
		{
			temp.push_back(*it);
			if (it + 1 != this->text.end())
			{
				if (strchr("+-/*)(^", *(it + 1)) || strchr("+-/*)(^", *it)) {
					if (it == this->text.begin() && (*it == '-' || *it == '+'))
						this->infixText.push_back("0");

					this->infixText.push_back(temp);
					temp = "";

					if (*it == '(' && (*(it + 1) == '-' || *(it + 1) == '+'))
						this->infixText.push_back("0");
					else if (*it == ')' && *(it + 1) == '(')
						this->infixText.push_back("*");
				}
			}
			else if (it + 1 == this->text.end())
			{
				infixText.push_back(temp);
				temp = "";
			}
		}
		setPostfixVector();
	}


	bool isUnary(std::string symbol)
	{
		if (this->unaryOperators.find(symbol) == this->unaryOperators.end())
			return false;
		else
			return true;
	}

	bool is_operator(std::string symbol)
	{
		if (symbol == "^" || symbol == "-" || symbol == "+" || symbol == "/" || symbol == "*")
		{
			return true;
		}
		else if (symbol == "sin")
		{
			return true;
		}
		else if (isUnary(symbol))
		{
			return true;
		}
		else
		{
			return false;
		}
	}


	int precedence(std::string symbol)
	{
		if (symbol == "^")/* exponent operator, highest precedence*/
		{
			return 3;
		}
		else if (symbol == "*" || symbol == "/")
		{
			return 2;
		}
		else if (symbol == "+" || symbol == "-")/* lowest precedence */
		{
			return 1;
		}
		else if (isUnary(symbol))
		{
			return 4;
		}
		else
		{
			return(0);
		}
	}

	void setPostfixVector(){
		Stack.push("(");
		this->infixText.push_back(")");

		std::vector<std::string>::iterator it;
		std::string item;
		for (it = this->infixText.begin(); it < infixText.end(); ++it)
		{
			if (*it == "(")
			{
				Stack.push("(");
			}
			else if (is_operator(*it))
			{
				while (!Stack.empty() && isPrecedenceOk(Stack.top(), *it))
				{
					item = Stack.top();
					Stack.pop();
					this->postfixText.push_back(item);                 /* so pop all higher precendence operator and */
					                      
				}
				Stack.push(*it);
			}
			else if (*it == ")")         /* if current symbol is ')' then */
			{
				
				/* pop and keep popping until */
				while (!Stack.empty() && Stack.top() != "(")        /* '(' encounterd */
				{
					item = Stack.top();
					Stack.pop();
					this->postfixText.push_back(item);
					
				}
				if (!Stack.empty() && Stack.top() == "(")
					Stack.pop();
			}
			else
			{
				this->postfixText.push_back(*it);
			}
		}
		while (!Stack.empty())
		{
			item = Stack.top();
			Stack.pop();
			if (!std::strchr("()", *item.c_str()))
				this->postfixText.push_back(item);
		}
		this->infixText.erase(this->infixText.end()-1);
	}

	bool isPrecedenceOk(std::string stackTop, std::string it)
	{
		if(precedence(stackTop) >= precedence(it))
			if (it != "^" || (isUnary(stackTop) && it == "^"))
				return true;
		return false;
	}

	boost::multiprecision::cpp_dec_float_50 fact(boost::multiprecision::cpp_dec_float_50 val)
	{
		boost::multiprecision::cpp_dec_float_50 factorial(1.0);
		boost::multiprecision::cpp_dec_float_50 i;
		for (i = boost::multiprecision::cpp_dec_float_50(1.0); i <= val; ++i)
		{
			factorial *= i;
		}
		return factorial;
	}

	std::string evalUnary(std::string opr, std::string val)
	{
		std::string returnValue;
		std::stringstream SS;
		SS.precision(std::numeric_limits<boost::multiprecision::cpp_dec_float_50>::max_digits10);
		boost::multiprecision::cpp_dec_float_50 nr = getValue(val);
		boost::multiprecision::cpp_dec_float_50 result;
		try {
			if (opr == "sin")
				result =  boost::multiprecision::sin(nr);
			else if (opr == "cos")
				result = boost::multiprecision::cos(nr);
			else if (opr == "tg")
				result = boost::multiprecision::sin(nr)/ boost::multiprecision::cos(nr);
			else if (opr == "ctg")
				result = boost::multiprecision::cos(nr)/ boost::multiprecision::sin(nr);
			else if (opr == "asin")
				result = boost::multiprecision::asin(nr);
			else if (opr == "acos")
				result = boost::multiprecision::acos(nr);
			else if (opr == "atg")
				result = boost::multiprecision::atan(nr);
			else if (opr == "actg")
				result = boost::math::constants::pi<boost::multiprecision::cpp_dec_float_50>()/2 - boost::multiprecision::atan(nr);
			else if (opr == "ln")
				result = log(nr);
			else if (opr == "log")
				result = boost::multiprecision::log10(nr);
			else if (opr == "log2")
				result = boost::multiprecision::log(nr) / boost::multiprecision::log(boost::multiprecision::cpp_dec_float_50(2));
			else if (opr == "sqrt")
				result = boost::multiprecision::sqrt(nr);
			else if (opr == "exp")
				result = boost::multiprecision::exp(nr);
			else if (opr == "fact")
				result = fact(nr);
			if (result > -epsilon && result < epsilon)
				SS << 0;
			else 
				SS << result;
			SS >> returnValue;
			return returnValue;
		}
		catch (std::exception&)
		{
			return "";
		}
		
	}

	std::string evalBinary(std::string val1, std::string val2, std::string opr)
	{
		std::string returnValue;
		std::stringstream SS;
		SS.precision(std::numeric_limits<boost::multiprecision::cpp_dec_float_50>::max_digits10);
		boost::multiprecision::cpp_dec_float_50 nr1 = this->getValue(val1);
		boost::multiprecision::cpp_dec_float_50 nr2 = this->getValue(val2);
		boost::multiprecision::cpp_dec_float_50 result;
		try {
			if (opr == "+")
				result = nr1 + nr2;
			else if (opr == "-")
				result = nr1 - nr2;
			else if (opr == "*")
				result = nr1 * nr2;
			else if (opr == "/")
				result = nr1 / nr2;
			else if (opr == "^")
				result = boost::multiprecision::pow(nr1, nr2);
			SS << result;
			SS >> returnValue;
			return returnValue;
		}
		catch (std::exception&)
		{
			return "";
		}

	}
};

boost::multiprecision::cpp_dec_float_50 Expression::Eval(std::string expression) {
	std::string val1;
	std::string val2;
	std::string result;
	this->text = expression;
	this->setTokensVectors();

	for (std::vector<std::string>::iterator it = this->postfixText.begin(); it < this->postfixText.end(); ++it)
	{
		if (this->is_operator(*it))
		{
			if (this->isUnary(*it))
			{
				if (!Result.empty())
				{
					val1 = Result.top();
					Result.pop();
					result = this->evalUnary(*it, val1);
					Result.push(result);
				}
				else
					return std::numeric_limits<boost::multiprecision::cpp_dec_float_50>::quiet_NaN();//error
			}
			else
			{
				if (!Result.empty())
				{
					val2 = Result.top();
					Result.pop();
					if (!Result.empty())
					{
						val1 = Result.top();
						Result.pop();
						result = this->evalBinary(val1, val2, *it);
						Result.push(result);
					}
					else
						return std::numeric_limits<boost::multiprecision::cpp_dec_float_50>::quiet_NaN();//error
				}
				else
					return std::numeric_limits<boost::multiprecision::cpp_dec_float_50>::quiet_NaN();//error
			}
		}
		else
		{
			Result.push(*it);
		}
	}

	if (!Result.empty()) {
		result = Result.top();
		Result.pop();
		boost::multiprecision::cpp_dec_float_50 value = getValue(result);
		return value;
	}
	return 0;
	
}