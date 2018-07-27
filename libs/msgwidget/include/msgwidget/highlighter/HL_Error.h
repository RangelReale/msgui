#ifndef COMPILER_ERROR_HIGHLIGHTER_H
#define COMPILER_ERROR_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;

namespace msgwidget {
namespace highlighter {

class HL_Error : public QSyntaxHighlighter
{
    Q_OBJECT

public:
	HL_Error(QTextDocument *parent = 0);

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
};

} }

#endif // COMPILER_ERROR_HIGHLIGHTER_H