#include "msglib/Cmd.h"

#include <QJsonObject>
#include <QJsonArray>

#include <stack>

namespace msglib {
namespace cmd {

//
// base
//
base::base()
{
}

//
// backtrace_frame
//
frame_base::frame_base(const QString &name) :
	name(name)
{

}

frame_base::frame_base(const QString &name, const QString &source_location, const QString &kind, const QString &point_of_instantiation) :
	name(name), source_location(source_location), kind(kind), point_of_instantiation(point_of_instantiation)
{

}

//
// backtrace
//
backtrace::backtrace() : base()
{

}

//
// call_graph_node
//
call_graph_node::call_graph_node(const QString &name, const QString &source_location, const QString &kind, const QString &point_of_event,
	int depth, int children) :
	name(name), source_location(source_location), kind(kind), point_of_event(point_of_event), depth(depth), children(children)
{

}

//
// call_graph_tree_node
//
call_graph_tree_node::call_graph_tree_node(call_graph_node::ptr node, int depth) :
	node(node), depth(depth), nodes()
{

}

//
// call_graph
//
call_graph::call_graph() : base()
{

}

void call_graph::add_node_finished()
{
	// build tree
	nodes_root.reset(new call_graph_tree_node(nullptr, -1));

	std::stack<call_graph_tree_node::ptr> stack;
	stack.push(nodes_root);

	for (auto tnode : nodes) {
		auto newitem = std::make_shared<call_graph_tree_node>(tnode, tnode->depth);

		while (tnode->depth <= stack.top()->depth)
		{
			stack.pop();
		}
		int last_depth = stack.top()->depth;
		stack.top()->nodes.push_back(newitem);
		if (tnode->depth > last_depth)
		{
			stack.push(newitem);
		} 
	}
}

//
// code_completion_result
//
code_completion_result::code_completion_result() : base()
{

}

//
// comment_paragraph
//
comment_paragraph::comment_paragraph(QString first_line_of_identation,
	QString rest_of_lines_of_identation, QString content) : 
	first_line_of_identation(first_line_of_identation),
	rest_of_lines_of_identation(rest_of_lines_of_identation), content(content)
{

}

//
// comment
//
comment::comment() : base()
{

}

//
// cpp_code
//
cpp_code::cpp_code(const QString &code) : base(), code(code)
{

}

//
// error
//
error::error(const QString &message) : base(), message(message)
{

}

//
// filename_list
//
filename_list::filename_list() : filename_list_base()
{

}

//
// filename_set
//
filename_set::filename_set() : filename_list_base()
{

}

//
// frame
//
frame::frame(const QString &name) : base(), frame_base(name)
{

}

//
// prompt
//
prompt::prompt(const QString &value) : base(), value(value)
{

}

//
// raw_text
//
raw_text::raw_text(const QString &text) : base(), text(text)
{

}

//
// type_
//
type_::type_(const QString &type_name) : base(), type_name(type_name)
{

}

} }