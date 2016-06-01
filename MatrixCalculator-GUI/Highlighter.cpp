#include "Highlighter.h"
#include <QSyntaxHighlighter>
#include <QDebug>

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
    int  startIndex = text.indexOf("(");
    while (startIndex >= 0)
    {
        int endIndex = text.indexOf(")", startIndex);
        if (endIndex == -1)
        {
            setFormat(startIndex, 1, errorFormat);
            break;
        }
        else
        {
            setFormat(startIndex, 1, format);
            setFormat(endIndex, 1, format);
            startIndex = text.indexOf("(", startIndex + 1);
        }
    }

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

    setCurrentBlockState(0);
}


