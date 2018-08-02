#include "msgui/Util.h"

#include <QRegularExpression>

namespace msgui {
namespace Util {

QString identCPPType(const QString &type)
{
	QRegularExpression re("[<>]");
	QString ident("    ");

	QString ret;
	int j = 0;
	int newj = 0;
	int level = 0;
	while ((newj = type.indexOf(re, j)) != -1) {
		QStringRef sep(&type, newj, 1);
		QStringRef str(QStringRef(&type, j, newj-j).trimmed());
		QStringRef comma;
		if (newj < type.length()) {
			comma = QStringRef(&type, newj + 1, 1);
		}
		if (sep == "<") {
			ret.append(ident.repeated(level));
			ret.append(str);
			ret.append(sep);
			ret.append("\n");

			level++;
		}
		else if (sep == ">") {
			if (!str.isEmpty()) {
				ret.append(ident.repeated(level));
				ret.append(str);
				ret.append("\n");
			}
			// put > alone in next line
			level--;
			ret.append(ident.repeated(level));
			ret.append(sep);
			// let comma on the same line as the end symbol
			if (comma == ",") {
				ret.append(comma);
				newj++;
			}
			ret.append("\n");
		}

		j = newj+1;
	}

	if (j < type.length()) {
		ret.append(QStringRef(&type, j, type.length()-j));
	}

	//return QString("%1\n\n%2").arg(type).arg(ret);
	return ret.trimmed();
}

} }