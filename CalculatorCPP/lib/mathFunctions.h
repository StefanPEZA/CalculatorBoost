bool isRadian = true;

void setUsingRadiansTo(bool usingRadians) {
    isRadian = usingRadians;
}

double Sinus(double degrees) {
    double result;
    if (isRadian)
        result = sin(degrees); //cu radiani
    else
        result = sin(degrees*PI/180);//cu grade

    if ((result < EPSILON  && result >= 0) || (result > -EPSILON && result < 0))
        return 0.0;
    return result;
}

double Cosinus(double degrees) {
    double result;
    if (isRadian)
        result = cos(degrees);
    else
        result = cos(degrees*PI/180);

    if ((result < EPSILON  && result >= 0) || (result > -EPSILON && result < 0))
        return 0.0;
    return result;
}

double Tangent(double degrees) {
    double result;
    if (isRadian)
        result = tan(degrees);
    else
        result = tan(degrees*PI/180);

    if (result > INFINIT)
        return INFINITY;
    else if(result < -INFINIT)
        return -INFINITY;

    if ((result < EPSILON && result >= 0) || (result > -EPSILON && result < 0))
        return 0.0;

    return result;
}

double Cotangent(double degrees) {
    double result;
    result = 1/Tangent(degrees);

    return result;
}

double arcSinus(double value) {
    double result = asin(value);
    if (isRadian)
        return result;
    else
        return result*180/PI;
}

double arcCosinus(double value) {
    double result = acos(value);
    if (isRadian)
        return result;
    else
        return result*180/PI;
}

double arcTangent(double value) {
    double result = atan(value);
    if (isRadian)
        return result;
    else
        return result*180/PI;
}

double arcCotangent(double value) {
    double result = PI/2 - atan(value);
    if (isRadian)
        return result;
    else
        return result*180/PI;
}

//radacina patrata a unui numar
double Radical(double Value) {
    if (Value < 0)
        return NAN;
    if (Value > INFINIT)
        return INFINITY;
    return sqrt(Value);
}

//logaritm natural dintr-un numar
double Log(double Value) {
    if (Value < 0)
        return NAN;
    if (Value == 0)
        return -INFINITY;
    if (Value > INFINIT)
        return INFINITY;
    return log(Value);
}

//logaritm din x in baza b;
double LogInBase(double Value, double Base = E) {
    if (Value < 0)
        return NAN;
    if (Value == 0)
        return -INFINITY;
    return Log(Value)/Log(Base);
}

//ridicarea la putere(x la puterea y)
double Power(double Value1, double Value2) {
    if (Value1 == 0 && Value2 == 0)
        return NAN;
    if (Value1 == 0)
        return 0.0;
    if (Value2 == 0)
        return 1.0;

    double result = pow(Value1, Value2);

    if (result > INFINIT)
        return INFINITY;
    else if (result < -INFINIT)
        return -INFINITY;

    return result;
}

//adunarea a doua numere
double Plus(double Value1, double Value2) {
    if (Value1 > INFINIT || Value2 > INFINIT)
        return INFINITY;

    double result = Value1+Value2;
    if (result > INFINIT)
        return INFINITY;
    else if (result < -INFINIT)
        return -INFINITY;
    return result;
}

//scaderea a doua numere
double Minus(double Value1, double Value2) {
    double result = Value1-Value2;
    if (result > INFINIT)
        return INFINITY;
    else if (result < -INFINIT)
        return -INFINITY;
    return result;
}

double UnaryMinus(double Value) {
    if (Value < 0 && Value < -INFINIT)
        return INFINITY;
    else if (Value > 0 && Value > INFINIT)
        return -INFINITY;

    return -Value;
}

double UnaryPlus(double Value) {
    if (Value < 0 && Value < -INFINIT)
        return -INFINITY;
    else if (Value > 0 && Value > INFINIT)
        return INFINITY;

    return Value;
}


//inmultirea a doua numere
double Multiply(double Value1, double Value2) {


    if (Value1 > INFINIT || Value2 > INFINIT)
        return INFINITY;

    double result = Value1*Value2;
    if (result > INFINIT)
        return INFINITY;
    else if (result < -INFINIT)
        return -INFINITY;
    return result;
}

//impartirea a doua numere
double Divide(double Value1, double Value2) {
    if (Value2 == 0 && Value1 != 0)
        return INFINITY;
    if (Value1 == 0 && Value2 == 0)
        return NAN;

    double result = Value1/Value2;
    if (result > INFINIT)
        return INFINITY;
    else if (result < -INFINIT)
        return -INFINITY;
    else if ((result < EPSILON  && result >= 0) || (result > -EPSILON && result < 0))
        return 0.0;
    return result;
}

//restul impartirii a doua numere
double Modulo(double Value1, double Value2) {
    if (Value2 == 0)
        return NAN;
    if (Value1>INFINIT || Value2>INFINIT)
        return INFINITY;
    return (int)Value1 % (int)Value2;
}

//valoare absoluta(modul)
double Absolute(double Value) {
    if (abs(Value) > INFINIT)
        return INFINITY;
    if (Value < 0)
        return -Value;
    return Value;
}


//sunt egale
int Equal(double Value1, double Value2) {
    if (Value1==Value2)
        return 1;
    else
        return 0;
}

//nu sunt egale
int notEqual(double Value1, double Value2) {
    if (Value1!=Value2)
        return 1;
    else
        return 0;
}

//x mai mare decat y
int Greater(double Value1, double Value2) {
    if (Value1 > Value2)
        return 1;
    return 0;
}

//x mai mare sau egal cu y
int greaterOrEqual(double Value1, double Value2) {
    if (Value1 >= Value2)
        return 1;
    return 0;
}

//x mai mic decat y
int Less(double Value1, double Value2) {
    if (Value1 < Value2)
        return 1;
    return 0;
}

//x mai mic sau egal cu y
int lessOrEqual(double Value1, double Value2) {
    if (Value1 <= Value2)
        return 1;
    return 0;
}




