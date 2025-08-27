#include <iostream>

#include "zm32xml.hpp"


void display_xml(zm32xml::element parent, int indent)
{
	for (int i = 0;i < indent;++i) {
		std::cout << " ";
	}
	//std::cout << "<" << parent.tag_name() << "> ";
	std::cout << "<" << parent.tag_name();

	int i = 0;
	for (auto attr : parent.attributes()) {
		//if (i == 0) {
		//	std::cout << "[";
		//}
		//else {
		//	std::cout << " ";
		//}
		//std::cout << attr.name() << "=" << attr.value();
		//if (i == parent.attributes().size() - 1) {
		//	std::cout << "] ";
		//}
		std::cout << " " << attr.name() << "=" << attr.value();
		++i;
	}

	std::cout << "> " << parent.value();
	std::cout << std::endl;

	for (auto child : parent.children()) {
		display_xml(child, indent + 2);
	}
}


int main()
{
	zm32xml::document doc;
	int result = doc.parse_file("testdata/test07.xml");
	if (result) {
		return -1;
	}

	display_xml(doc.root(), 0);
}