#include "Highlighter.h"
#include <QSyntaxHighlighter>
#include <QDebug>
#include <QStack>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QTextCharFormat format;
    QString pattern;

    pattern = "[0-9]+";
    rule.pattern = QRegExp(pattern);
    format.setForeground(Qt::green);
    rule.format = format;
    highlightingRules.append(rule);

    pattern = "[a-zA-Z]+";
    rule.pattern = QRegExp(pattern);
    format.setForeground(Qt::blue);
    rule.format = format;
    highlightingRules.append(rule);

    pattern = "[a-zA-Z]+\\(\\)";
    rule.pattern = QRegExp(pattern);
    format.setForeground(Qt::magenta);
    rule.format = format;
    highlightingRules.append(rule);

//    pattern = "[()]";
//    rule.pattern = QRegExp(pattern);
//    format.setForeground(Qt::yellow);
//    rule.format = format;
//    highlightingRules.append(rule);

}

void Highlighter::highlightBlock(const QString &text)
{
    QTextCharFormat errorFormat, format;
    errorFormat.setBackground(Qt::red);
    errorFormat.setForeground(Qt::white);
    format.setForeground(Qt::black);

    QStack<int> st1, st2;
    for(int i = 0; i < text.size(); ++i)
    {
        if(text.at(i) == '(')
        {
            st2.push(i);
            st1.push('(');
        }
        else if(text.at(i) == ')')
        {
            if(st1.isEmpty())
                setFormat(i, 1, errorFormat);
            else
            {
                st1.pop();
                st2.pop();
            }
        }
    }
    while(!st2.isEmpty())
        setFormat(st2.pop(), 1, errorFormat);


    foreach(const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while(index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
qDebug() << 123;
    setCurrentBlockState(0);
}


