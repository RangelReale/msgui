#pragma once

#include <QJsonDocument>

#include <QStringList>

#include <memory>
#include <vector>

namespace msglib {
namespace cmd {

//
// base
//
class base : public std::enable_shared_from_this<base>
{
public:
	using ptr = std::shared_ptr<base>;

	base();

	virtual QString cmdtype() = 0;

	virtual ~base() {}
};

//
// backtrace_frame
//
class frame_base
{
public:
	using ptr = std::shared_ptr<frame_base>;

	frame_base(const QString &name);
	frame_base(const QString &name, const QString &source_location, const QString &kind, const QString &point_of_instantiation);

	QString name;
	QString source_location;
	QString kind;
	QString point_of_instantiation;
};

//
// backtrace
//
class backtrace : public base
{
public:
	using ptr = std::shared_ptr<backtrace>;

	backtrace();
	QString cmdtype() override { return "backtrace"; }

	std::vector<frame_base::ptr> frames;
};

//
// call_graph_node
//
class call_graph_node
{
public:
	using ptr = std::shared_ptr<call_graph_node>;

	call_graph_node(const QString &name, const QString &source_location, const QString &kind, const QString &point_of_event,
		int depth, int children);

	QString name;
	QString source_location;
	QString kind;
	QString point_of_event;
	int depth;
	int children;
};

//
// call_graph_tree_node
//
class call_graph_tree_node
{
public:
	using ptr = std::shared_ptr<call_graph_tree_node>;

	call_graph_tree_node(call_graph_node::ptr node, int depth);

	call_graph_node::ptr node;
	int depth;
	std::vector<call_graph_tree_node::ptr> nodes;
};

//
// call_graph
//
class call_graph : public base
{
public:
	using ptr = std::shared_ptr<call_graph>;

	call_graph();
	QString cmdtype() override { return "call_graph"; }

	std::vector<call_graph_node::ptr> nodes;

	call_graph_tree_node::ptr nodes_root;
protected:
	friend class parser;

	void add_node_finished();
};

//
// code_completion_result
//
class code_completion_result : public base
{
public:
	using ptr = std::shared_ptr<code_completion_result>;

	code_completion_result();
	QString cmdtype() override { return "code_completion_result"; }

	QStringList completions;
};

//
// comment_paragraph
//
class comment_paragraph
{
public:
	using ptr = std::shared_ptr<comment_paragraph>;

	comment_paragraph(QString first_line_of_identation,
		QString rest_of_lines_of_identation,
		QString content);

	QString first_line_of_identation;
	QString rest_of_lines_of_identation;
	QString content;
};


//
// comment
//
class comment : public base
{
public:
	using ptr = std::shared_ptr<comment>;

	comment();
	QString cmdtype() override { return "comment"; }

	std::vector<comment_paragraph::ptr> paragraphs;
};

//
// cpp_code
//
class cpp_code : public base
{
public:
	using ptr = std::shared_ptr<cpp_code>;

	cpp_code(const QString &code);
	QString cmdtype() override { return "cpp_code"; }

	QString code;
};

//
// error
//
class error : public base
{
public:
	using ptr = std::shared_ptr<error>;

	error(const QString &message);
	QString cmdtype() override { return "error"; }

	QString message;
};

//
// filename_list_base
//
class filename_list_base : public base
{
public:
	using ptr = std::shared_ptr<filename_list_base>;

	QStringList filenames;
};

//
// filename_list
//
class filename_list : public filename_list_base
{
public:
	using ptr = std::shared_ptr<filename_list>;

	filename_list();
	QString cmdtype() override { return "filename_list"; }
};

//
// filename_set
//
class filename_set : public filename_list_base
{
public:
	using ptr = std::shared_ptr<filename_set>;

	filename_set();
	QString cmdtype() override { return "filename_set"; }
};

//
// frame
//
class frame : public base, public frame_base
{
public:
	using ptr = std::shared_ptr<frame>;

	frame(const QString &name);
	QString cmdtype() override { return "frame"; }

	std::shared_ptr<double> time_taken;
	std::shared_ptr<double> time_taken_ratio;
};

//
// prompt
//
class prompt : public base
{
public:
	using ptr = std::shared_ptr<prompt>;

	prompt(const QString &value);
	QString cmdtype() override { return "prompt"; }

	QString value;
};

//
// raw_text
//
class raw_text : public base
{
public:
	using ptr = std::shared_ptr<raw_text>;

	raw_text(const QString &text);
	QString cmdtype() override { return "raw_text"; }

	QString text;
};

//
// type_
//
class type_ : public base
{
public:
	using ptr = std::shared_ptr<type_>;

	type_(const QString &type_name);
	QString cmdtype() override { return "type"; }

	QString type_name;
};

} }