/**
 *	@file		zm32xml.hpp
 *	@brief		XMLパーサ
 *	@details	ZM32 ペアプログラミング専科での制作物
 *	
 *	@author		Kawahara, Shiba
 *	
 *	@date		2024/07/24
 *	
 *	@version	0.0.1
 */


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


/**
 *	@brief	 XML文書全体を扱うクラス
 *	@details ルートのtagを持つ
 *	
 *	@date    2025/07/24 メンバを実装 (D: kawahara, N: shiba)
 */ 
class document final
{
public:
	document() = default;
	~document() = default;

private:
	tag root;
};


/**
 *  @brief	 XMLのタグを扱うクラス
 *  @details 名前、属性、子タグをメンバに持つ
 *  
 *  @date    2025/07/24 メンバを実装 (D: kawahara, N: shiba)
 */
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


/**
 *	@brief	 XMLの属性を扱うクラス
 *	@details 名前、値をメンバに持つ
 *	
 *	@date    2025/07/24 メンバを実装 (D: kawahara, N: shiba)
 */
class attribute final
{
public:
	attribute() = default;
	~attribute() = default;

private:
	std::string m_name;
	std::string m_value;
};


namespace internal {


/**
 *	@brief	tokenの種類
 *	@date	2025/07/24	作成 (D: shiba, N: kawahara)
 */
enum TokenType
{
	TT_TAG,			//	XMLタグを表す記号（<, >, /）
	TT_EQUAL,		//	=
	TT_LITERAL,		//	ダブルクオーテーションで囲われていない文字列
	TT_VALUE,		//	ダブルクオーテーションで囲われた文字列
	TT_EOF			//	末尾を表すトークン
};


/**
 *	@brief	XMLをパースする際のトークン
 *	@date	2025/07/24	作成 (D: shiba, N: kawahara)
 */
struct token
{
	TokenType type;
	std::string value;
	token* next;
};


/**
 *	@brief		XMLをトークンに分割
 *	@details	名前、値をメンバに持つ
 *	
 *	@param[in]	src		XMLのソース文字列
 *	@param[in]	size	文字列のサイズ
 *	@return		トークンの先頭ポインタ
 *	@date		2025/07/24 プロトタイプ宣言 (D: shiba, N: kawahara)
 */
token* tokenize(const char* src, size_t size);


}


} // namespace zm32xml


#endif // ZM32XML_HPP
