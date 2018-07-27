#ifndef CPP_HIGHLIGHTER_H
#define CPP_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

namespace msgwidget {
namespace highlighter {

class HL_CPP : public QSyntaxHighlighter
{
    Q_OBJECT

public:
	HL_CPP(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
	QTextCharFormat punctuationFormat;
};

} }

#endif // CPP_HIGHLIGHTER_H