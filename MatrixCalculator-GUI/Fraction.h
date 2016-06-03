#ifndef FRACTION_H
#define FRACTION_H

#include <QString>

enum {Frac, Deci};

class Fraction
{

private:
    double data = -1.0;
    int u = -1, d = -1;
    int type = -1;

    void toFraction();

public:
    Fraction() = default;
    ~Fraction() = default;
    Fraction(const Fraction& rhs);
    Fraction(const QString& rhs);
    Fraction(const double& rhs);
    Fraction(const int& u, const int& d);
    Fraction(const char* rhs);

    QString toString(int precision = 6) const;

    Fraction& operator = (const Fraction& rhs);
    Fraction& operator = (const QString& rhs);
    Fraction& operator = (const char* rhs);
    Fraction& operator = (const double& rhs);
    friend const Fraction operator + (const Fraction& lhs, const Fraction& rhs);
    friend const Fraction operator + (const double lhs, const Fraction& rhs);
    friend const Fraction operator + (const Fraction& lhs, const double rhs);
    friend const Fraction operator - (const Fraction& lhs, const Fraction& rhs);
    friend const Fraction operator - (const double lhs, const Fraction& rhs);
    friend const Fraction operator - (const Fraction& lhs, const double rhs);
    friend const Fraction operator * (const Fraction& lhs, const Fraction& rhs);
    friend const Fraction operator * (const double lhs, const Fraction& rhs);
    friend const Fraction operator * (const Fraction& lhs, const double rhs);
    friend const Fraction operator / (const Fraction& lhs, const Fraction& rhs);
    friend const Fraction operator / (const double lhs, const Fraction& rhs);
    friend const Fraction operator / (const Fraction& lhs, const double rhs);
};

int gcd(int a, int b)
{
    if(b == 0)
        return a;
    else
        return gcd(b, a % b);
}

#endif // FRACTION_H
