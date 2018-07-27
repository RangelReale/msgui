#include "msglib/CmdParser.h"

#include <QJsonObject>
#include <QJsonArray>

#include <stack>

namespace msglib {
namespace cmd {

//
// parser
//
parser::parser()
{

}

base::ptr parser::parse(QJsonDocument doc)
{
	if (doc.isObject() && doc.object().contains("type")) 
	{
		QString type_str(cleanString(doc.object()["type"].toString()));

		if (type_str == "error")
		{
			return std::make_shared<error>(cleanString(doc.object()["msg"].toString()));
		}
		else if (type_str == "backtrace")
		{
			auto ret = std::make_shared<backtrace>();
			auto frames = doc.object()["frames"].toArray();
			for (auto frame : frames) {
				ret->frames.push_back(std::make_shared<frame_base>(
					cleanString(frame.toObject()["name"].toString()),
					cleanString(frame.toObject()["source_location"].toString()),
					cleanString(frame.toObject()["kind"].toString()),
					cleanString(frame.toObject()["point_of_event"].toString())
				));
			}
			return ret;
		}
		else if (type_str == "call_graph")
		{
			auto ret = std::make_shared<call_graph>();
			auto nodes = doc.object()["nodes"].toArray();
			for (auto node : nodes) {
				ret->nodes.push_back(std::make_shared<call_graph_node>(
					cleanString(node.toObject()["name"].toString()),
					cleanString(node.toObject()["source_location"].toString()),
					cleanString(node.toObject()["kind"].toString()),
					cleanString(node.toObject()["point_of_event"].toString()),
					node.toObject()["depth"].toInt(),
					node.toObject()["children"].toInt()
					));
			}
			ret->add_node_finished();
			return ret;
		}
		else if (type_str == "code_completion_result")
		{
			auto ret = std::make_shared<code_completion_result>();
			auto completions = doc.object()["completions"].toArray();
			for (auto completion : completions) {
				ret->completions.push_back(cleanString(completion.toString()));
			}
			return ret;
		}
		else if (type_str == "comment")
		{
			auto ret = std::make_shared<comment>();
			auto paragraphs = doc.object()["paragraphs"].toArray();
			for (auto paragraph : paragraphs) {
				ret->paragraphs.push_back(std::make_shared<comment_paragraph>(
					cleanString(paragraph.toObject()["first_line_indentation"].toString()),
					cleanString(paragraph.toObject()["rest_of_lines_indentation"].toString()),
					cleanString(paragraph.toObject()["content"].toString())
					));
			}
			return ret;
		}
		else if (type_str == "cpp_code")
		{
			return std::make_shared<cpp_code>(cleanString(doc.object()["code"].toString()));
		}
		else if (type_str == "filename_list")
		{
			auto ret = std::make_shared<filename_list>();
			auto filenames = doc.object()["filenames"].toArray();
			for (auto filename : filenames) {
				ret->filenames.push_back(cleanString(filename.toString()));
			}
			return ret;

		}
		else if (type_str == "filename_set")
		{
			auto ret = std::make_shared<filename_set>();
			auto filenames = doc.object()["filenames"].toArray();
			for (auto filename : filenames) {
				ret->filenames.push_back(cleanString(filename.toString()));
			}
			return ret;
		}
		else if (type_str == "frame")
		{
			auto ret = std::make_shared<frame>(cleanString(doc.object()["name"].toString()));
			ret->source_location = cleanString(doc.object()["source_location"].toString());
			if (doc.object().contains("kind"))
				ret->kind = cleanString(doc.object()["kind"].toString());
			if (doc.object().contains("point_of_instantiation"))
				ret->point_of_instantiation = cleanString(doc.object()["point_of_instantiation"].toString());
			if (doc.object().contains("time_taken"))
				ret->time_taken = std::make_shared<double>(doc.object()["time_taken"].toDouble());
			if (doc.object().contains("time_taken_ratio"))
				ret->time_taken_ratio = std::make_shared<double>(doc.object()["time_taken_ratio"].toDouble());
			return ret;
		}
		else if (type_str == "prompt")
		{
			return std::make_shared<prompt>(cleanString(doc.object()["prompt"].toString()));
		}
		else if (type_str == "raw_text")
		{
			return std::make_shared<raw_text>(cleanString(doc.object()["value"].toString()));
		}
		else if (type_str == "type")
		{
			return std::make_shared<type_>(cleanString(doc.object()["name"].toString()));
		}
		else
		{
			return std::make_shared<error>("Unknown type: "+type_str);
		}
	}
	return nullptr;
}

QString parser::cleanString(QString str)
{
	//str.replace(QString("\\n"), QString("\n"));
	//str.replace(QString("\\r"), QString("\r"));
	return str.trimmed();
}

//
// list
//
list_::list_() : list()
{

}

} }