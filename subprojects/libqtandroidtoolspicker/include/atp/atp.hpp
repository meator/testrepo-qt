#pragma once

#include <QWizard>

#include <atp/dll.hpp>
#include <atp/spimpl.hpp>

/**
 * @brief Primary namespace for libqtandroidtoolspicker public API.
 *
 * @note
 * ATP = libAndroidToolsPicker (macros sometimes use `ATPICKER` shorthand).
 */
namespace ATP
{
/**
 * @brief Tools from the android-tools suite needed by the user.
 *
 * These can be ORed (`|`) together to select multiple tools in @ref
 * ATPWizard::ATPWizard().
 */
enum RequestedTools : std::uint16_t
{
	//! `adb`
	ADB = 0x1,
	//! `fastboot`
	FASTBOOT = 0x2,
	//! `append2simg`
	APPEND2SIMG = 0x4,
	//! `img2simg`
	IMG2SIMG = 0x8,
	//! `simg2img`
	SIMG2IMG = 0x10,
};

/**
 * @brief Invalid argument specified.
 */
class ATPICKER_API InvalidArgument : public std::runtime_error
{
	using std::runtime_error::runtime_error;
};

/**
 * @brief The primary QWidget class provided by this library.
 */
class ATPICKER_API ATPWizard : public QWizard
{
	Q_OBJECT

public:
	/**
	 * @brief Construct ATPWizard.
	 *
	 * @param tools All requested tools. See @ref RequestedTools.
	 *
	 * @throws InvalidArgument If `tools` is 0 (no tools selected).
	 */
	ATPWizard(std::underlying_type_t<RequestedTools> tools, QWidget * parent = nullptr);

	/**
	 * @brief Retrieve user's answer.
	 *
	 * `tool` must contain only a single tool, this function cannot return
	 * multiple results at once.
	 *
	 * @throws InvalidArgument If `tool` is 0 (no tool selected).
	 * @throws InvalidArgument If `tool` contains more than one tool.
	 * @throws InvalidArgument
	 * If `tool` was not specified in `tools` in @ref ATPWizard(). You cannot
	 * request the path for a tool which wasn't prompted for.
	 *
	 * @returns
	 * Path to the requested tool or an empty
	 */
	QString get_tool(RequestedTools tool) const;

private:
	class Impl;
	spimpl::impl_ptr<Impl> data;
};

/**
 * @brief Initialize library's translator.
 *
 * This function should be called before using @ref ATPWizard. It should
 * preferably be called early in the library user's `main()` function. If the
 * user of the library initiates translations of its own, it is best to call
 * this function near it.
 *
 * Example usage:
 * ```cpp
 * for (const QString &locale : QLocale::system().uiLanguages()) {
 *     if (ATP::initialize_translator(locale))
 *         break;
 * }
 * ```
 *
 * @param locale_name A locale name to try to load.
 *
 * @returns
 * If this library provides translations for the requested locale, `true` is
 * returned. If that's the case, no further calls to this function should be
 * necessary. Otherwise return `false`;
 */
bool ATPICKER_API initialize_translator(const QString & locale_name);
}; // namespace ATP
