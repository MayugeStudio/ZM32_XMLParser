#ifndef ZM32XML_HPP
#define ZM32XML_HPP

#include <string>

// Parsing                         :
// 1. Attribute class members      : D: kawahara M: siba 2025/07/24 11:45 ~ 12:00
// 2. Tag class members            : D: kawahara M: siba 2025/07/24 12:30 ~ 13:00
// 3. Document class members       :
// 4. Introduce document.parse()   :
// 4.1. Introduce tokenizer class  :
// 4.2. Introduce parser    class  :
// User interface                  :
// 1. Document interface methods   :
// 2. Tag interface methods        :
// 3. Attribute interface methods  :

namespace zm32xml {
	class attribute {
	public:
	private:
		std::string m_name;
		std::string m_value;
	};
}

#endif // ZM32XML_HPP
