#include "atp/atp.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QTranslator>

#include "internal.hpp"

static constexpr bool external_research =
	ADBPICKER_INTERNAL_EXTERNAL_TRANSLATIONS;

namespace ATP
{
bool initialize_translator(const QString & locale_name)
{
	static QTranslator translator;

	if constexpr (external_research)
	{
		QDir translation_dir(ADBPICKER_INTERNAL_TRANSLATIONS_DIR);
		if (translator.load(translation_dir.filePath(locale_name)))
		{
			QCoreApplication::installTranslator(&translator);
			return true;
		}
		return false;
	}
	else
	{
		if (translator.load(":/i18n/" + locale_name))
		{
			QCoreApplication::installTranslator(&translator);
			return true;
		}
		return false;
	}
}
} // namespace ATP
