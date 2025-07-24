///
/// @file		zm32xml.hpp
/// @brief		XMLパーサ
/// @details	ZM32 ペアプログラミング専科での制作物
/// 
/// @author		Kawahara, Shiba
/// 
/// @date		2024/07/24
/// 
/// @version	0.0.1
///


// Parsing                         :
// 1. Attribute class members      : D: kawahara M: shiba 2025/07/24 11:45 ~ 12:00
// 2. Tag class members            : D: kawahara M: shiba 2025/07/24 12:30 ~ 13:00
// 3. Document class members       : D: kawahara M: shiba 2025/07/24 12:40 ~ 13:00
// 4. Introduce document.parse()   :
// 4.1. Introduce tokenizer class  :
// 4.2. Introduce parser    class  :
// User interface                  :
// 1. Document interface methods   :
// 2. Tag interface methods        :
// 3. Attribute interface methods  :


#ifndef ZM32XML_HPP
#define ZM32XML_HPP


#include <string>
#include <vector>


namespace zm32xml {

class document;
class tag;
class attribute;


/// @brief	 XML文書全体を扱うクラス
/// @details ルートのtagを持つ
/// 
/// @date    2025/07/24 メンバを実装 (D: kawahara, N: shiba)
class document final
{
public:
	document() = default;
	~document() = default;

private:
	tag root;
};


class tag final
{
public:
	tag() = default;
	~tag() = default;

private:
	std::string m_name;
	std::vector<attribute> m_attributes;
	std::vector<tag> m_children;
};


class attribute final
{
public:
	attribute() = default;
	~attribute() = default;

private:
	std::string m_name;
	std::string m_value;
};


} // namespace zm32xml


#endif // ZM32XML_HPP
