#include <iostream>

#include "zm32xml.hpp"


int main()
{
	char8_t xml_data[] = u8"<AA value1=\"hello\" value2=\"world\">\n\t<BB>\n<Lang value=\"C\"/>\n\t\t<Lang value=\"C++\"/>\n\t\t<Lang value=\"Python\"/>\n\t</BB>\n</AA>";

	//zm32xml::internal::tokenizer tokenizer;
	//auto token = tokenizer.tokenize(xml_data, sizeof(xml_data) / sizeof(char8_t));
	//auto cur = token;
	//while (cur->type != zm32xml::internal::TT_EOF) {
	//	std::cout << "type:\t" << cur->type << std::endl;
	//	std::cout << "value:\t" << cur->value << std::endl;
	//	cur = cur->next;
	//}

	//zm32xml::internal::free_token(token);

	zm32xml::internal::parser parser;
	auto root = parser.parse(xml_data, sizeof(xml_data) / sizeof(char8_t));
}