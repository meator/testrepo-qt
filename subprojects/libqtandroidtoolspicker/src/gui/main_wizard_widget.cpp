#include "atp/atp.hpp"

#include "ui_main_wizard_widget.h"

using requested_tools_type = std::underlying_type_t<ATP::RequestedTools>;

namespace ATP
{
class ATPWizard::Impl
{
public:
	Ui::MainWizard * ui;
	requested_tools_type selected_tools;

	Impl(const Impl &) = delete;
	Impl & operator=(const Impl &) = delete;

	Impl(requested_tools_type selected_tools)
		: ui(new Ui::MainWizard), selected_tools(selected_tools)
	{
	}
	~Impl() { delete this->ui; }
};

ATPWizard::ATPWizard(requested_tools_type tools, QWidget * parent)
	: QWizard(parent), data(spimpl::make_impl<Impl>(tools))
{
	if (tools == 0)
		throw InvalidArgument(
			"At least one tool must be specified for checking!");

	this->data->ui->setupUi(this);
}
} // namespace ATP
