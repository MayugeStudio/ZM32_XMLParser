#include <iostream>

#include "zm32xml.hpp"


int main()
{
	char8_t xml_data[] = u8"<AA value1=\"hello\" value2=\"world\">\n\t<BB>\n<Lang value=\"C\"/>\n\t\t<Lang value=\"C++\"/>\n\t\t<Lang value=\"Python\"/>\n\t</BB>\n</AA>";

	zm32xml::document doc;
	int result = doc.parse(xml_data, sizeof(xml_data) / sizeof(char8_t));
	if (!result) {
		return -1;
	}

	for (auto&& lang : doc.child("AA").child("BB").children("Lang")) {
		std::cout << lang.attribute("value").value() << std::endl;
	}
}