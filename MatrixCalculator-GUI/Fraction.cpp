#include "Fraction.h"

#include <QStringList>

Fraction::Fraction(const Fraction& rhs)
{
    this->type = rhs.type;
    this->u = rhs.u;
    this->d = rhs.d;
    this->data = rhs.data;
}

Fraction::Fraction(const QString& rhs)
{
    QStringList sep = rhs.trimmed().split("/");
    if(sep.size() == 2)
    {
        this->u = sep.at(0).trimmed().toInt();
        this->d = sep.at(1).trimmed().toInt();
        this->type = Frac;
        this->data = double(u) / d;
    }
}

Fraction::Fraction(const double& rhs)
{
    this->type = Deci;
    this->data = rhs;
}

Fraction::Fraction(const int& u, const int& d)
{
    this->type = Frac;
    this->u = u;
    this->d = d;
}

Fraction::Fraction(const char* rhs)
{
    QString t(rhs);
    QStringList sep = t.trimmed().split("/");
    if(sep.size() == 2)
    {
        this->u = sep.at(0).trimmed().toInt();
        this->d = sep.at(1).trimmed().toInt();
        this->type = Frac;
        this->data = double(u) / d;
    }
}

QString Fraction::toString(int precision) const
{
    if(type == Frac)
        return QString::number(this->u) + "/" + QString::number(this->d);
    else if(type == Deci)
        return QString::number(this->data, 'g', precision);
    else
        return QString();
}

void Fraction::toFraction()
{
    this->type = Frac;
    QStringList sep = QString::number(this->data).split(".");
    if(sep.size() == 2)
    {
        this->d = 1;
        for(int i = 0; i < sep.at(1).size(); ++i)
            this->d *= 10;
        this->u = this->data * this->d;
    }
    else
    {
        this->u = this->data;
        this->d = 1;
    }
}

Fraction& Fraction::operator = (const Fraction& rhs)
{
    if(this == &rhs)
        return *this;
    this->type = rhs.type;
    this->u = rhs.u;
    this->d = rhs.d;
    this->data = rhs.data;
    return *this;
}

Fraction& Fraction::operator = (const QString& rhs)
{
    QStringList sep = rhs.trimmed().split("/");
    if(sep.size() == 2)
    {
        this->u = sep.at(0).trimmed().toInt();
        this->d = sep.at(1).trimmed().toInt();
        this->type = Frac;
        this->data = double(u) / d;
    }
    return *this;
}

Fraction& Fraction::operator = (const char* rhs)
{
    QString t(rhs);
    QStringList sep = t.trimmed().split("/");
    if(sep.size() == 2)
    {
        this->u = sep.at(0).trimmed().toInt();
        this->d = sep.at(1).trimmed().toInt();
        this->type = Frac;
        this->data = double(u) / d;
    }
    return *this;
}

Fraction& Fraction::operator = (const double& rhs)
{
    this->type = Deci;
    this->data = rhs;
    return *this;
}


const Fraction operator + (const Fraction& lhs, const Fraction& rhs)
{
    Fraction res;
    if(lhs.type == Frac && rhs.type == Frac)
    {
        res.type == Frac;
        int div = gcd(lhs.d, rhs.d);
        res.d = lhs.d * rhs.d / div;
        res.u = lhs.u * res.d / lhs.d + rhs.u * res.d / rhs.d;
        res.data = double(res.u) / res.d;
    }
    else if(lhs.type == Frac && rhs.type == Deci)
    {
        res.type = Deci;
        res.data = rhs.data * lhs.u / lhs.d;
    }
}

const Fraction operator + (const double lhs, const Fraction& rhs)
{

}

const Fraction operator + (const Fraction& lhs, const double rhs)
{

}

const Fraction operator - (const Fraction& lhs, const Fraction& rhs)
{

}

const Fraction operator - (const double lhs, const Fraction& rhs)
{

}

const Fraction operator - (const Fraction& lhs, const double rhs)
{

}

const Fraction operator * (const Fraction& lhs, const Fraction& rhs)
{

}

const Fraction operator * (const double lhs, const Fraction& rhs)
{

}

const Fraction operator * (const Fraction& lhs, const double rhs)
{

}

const Fraction operator / (const Fraction& lhs, const Fraction& rhs)
{

}

const Fraction operator / (const double lhs, const Fraction& rhs)
{

}

const Fraction operator / (const Fraction& lhs, const double rhs)
{

}
