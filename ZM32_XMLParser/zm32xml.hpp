/**
 *	@file		zm32xml.hpp
 *	@brief		XMLパーサ
 *	@details	ZM32 ペアプログラミング専科での制作物
 *	
 *	@author		Kawahara, Shiba
 *	
 *	@date		2025/07/24
 *	@date		2025/07/25	tokenizer作成
 *	@date		2025/08/26	parser作成
 *	
 *	@version	0.0.1
 */


// Parsing                         :
// 1. Attribute class members      : D: kawahara M: shiba 2025/07/24 11:45 ~ 12:00
// 2. Tag class members            : D: kawahara M: shiba 2025/07/24 12:30 ~ 13:00
// 3. Document class members       : D: kawahara M: shiba 2025/07/24 12:40 ~ 13:00
// 4. Introduce document.parse()   :
// 4.1. Introduce tokenizer class  : D: shiba M: kawahara 2025/07/25 9:30 ~ 15:00
// 4.2. Introduce parser    class  : D: shiba M: kawahara 2025/07/25 14:50 ~ 16:15
// User interface                  :
// 1. Document interface methods   : D: kawahara M: shiba 2025/08/26 10:50 ~ 11:20
// 2. Tag interface methods        : D: kawahara M: shiba 2025/08/26 11:45 ~ 
// 3. Attribute interface methods  : D: kawahara M: shiba 2025/08/26 11:20 ~ 11:45


//	TODO	2025/07/25
//	
//	parser::parse()を実装
//	parserのデバッグ
//	


#ifndef ZM32XML_HPP
#define ZM32XML_HPP


#include <optional>
#include <regex>
#include <string>
#include <vector>


namespace zm32xml {


namespace internal {

class tokenizer;
class parser;

} // namespace internal

	//class attribute;
	//class element;
	//class document;


/**
 *	@brief	 XMLの属性を扱うクラス
 *	@details 名前、値をメンバに持つ
 *
 *	@date	2025/07/24 メンバを実装 (D: kawahara, N: shiba)
 *  @date	2025/08/26 ゲッターを実装 (D: kawahara, N: shiba)
 */
class attribute final
{
public:
	attribute() = default;
	~attribute() = default;
	inline std::string name() { return m_name; }
	inline std::string value() { return m_value; }

private:
	friend class internal::parser;

	std::string m_name;
	std::string m_value;
};


/**
 *  @brief	 XMLのタグを扱うクラス
 *  @details 名前、属性、子タグをメンバに持つ
 *  
 *  @date	2025/07/24 メンバを実装 (D: kawahara, N: shiba)
 *  @data	2025/08/26 ゲッターを実装 (D: kawahara, N: shiba)
 */
class element final
{
public:
	element() = default;
	~element() = default;
	inline std::string tag_name() { return m_tag_name; }
	inline std::string value() { return m_value; }

private:
	friend class internal::parser;

	std::string m_tag_name;
	std::string m_value;
	std::vector<attribute> m_attributes;
	std::vector<element> m_children;
};


/**
 *	@brief	 XML文書全体を扱うクラス
 *	@details ルートのtagを持つ
 *
 *	@date	2025/07/24	メンバを実装 (D: kawahara, N: shiba)
 *	@data	2025/08/26	parseとchildを実装 (D: kawahara, N: shiba)
 */
class document final
{
public:
	document() = default;
	~document() = default;
	int parse(const char8_t* src, size_t size);
	element child(const std::string& name);

private:
	friend class internal::parser;

	element root;
};


inline int document::parse(const char8_t* xml_data, size_t size)
{
	zm32xml::internal::parser parser_instance;
	auto result = parser_instance.parse(xml_data, size);
	if (!result) {
		return -1;
	}
	root = result.value();
}


inline element document::child(const std::string& name)
{
	if (root.tag_name() != name) {
		return element{};
	}

	return root;
}


namespace internal {


/**
 *	@brief	tokenの種類
 *	@date	2025/07/24	作成 (D: shiba, N: kawahara)
 */
enum TokenType
{
	TT_TAG_START_OPEN,	//	開始タグの先頭を表す記号
	TT_TAG_START_CLOSE,	//	終了タグの先頭を表す記号
	TT_TAG_END,			//	タグの末尾を表す記号
	TT_TAG_END_EMPTY,	//	空タグの末尾を表す記号
	TT_EQUAL,			//	=
	TT_LITERAL,			//	ダブルクオーテーションで囲われていない文字列
	TT_VALUE,			//	ダブルクオーテーションで囲われた文字列
	TT_EOF				//	末尾を表すトークン
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
 *	@brief	トークンを連鎖的に破棄する関数
 *	@date	2025/07/25	作成 (D: shiba, N: kawahara)
 */
void free_token(token* head)
{
	if (head->next) {
		free_token(head->next);
	}
	delete head;
}


/**
 *	@brief		XMLをトークンに分割するクラス
 *
 *	@date		2025/07/25	作成。tokenize関数、 advance関数、delete_token関数、regex_match_char_utf8関数作成 (D: shiba, N: kawahara)
 */
class tokenizer final
{
public:
	/**
	 *	@brief		XMLをトークンに分割
	 *
	 *	@param[in]	src		XMLのソース文字列
	 *	@param[in]	size	文字列のサイズ
	 *	@return		トークンの先頭ポインタ
	 * 
	 *	@date		2025/07/24	プロトタイプ宣言 (D: shiba, N: kawahara)
	 *	@date		2025/07/25	実装 (D: shiba, N: kawahara)
	 */
	token* tokenize(const char8_t* src, size_t size);

private:
	const char8_t* p;
	const char8_t* end;
	token head;
	token* cur;
	bool is_eof;
	bool is_last;

	/**
	 *	@brief		pを1つ読み進める
	 *	@details	既にEOFの場合は読み進めない。読み進めてEOFの場合はis_eofをtrueにする
	 *
	 *	@return		pの指す文字。EOFの場合は0を返す
	 * 
	 *	@date		2025/07/25 作成 (D: shiba, N: kawahara)
	 */
	char advance();

	/**
	 *	@brief		トークンを破棄して初期化
	 *
	 *	@date		2025/07/25 作成 (D: shiba, N: kawahara)
	 */
	void delete_token();

	/**
	 *	@brief		pが指す文字列の先頭文字が正規表現に一致するか判定
	 *
	 *	@param[in]	re	正規表現
	 *	@return		一致したら文字のバイト数を返す。そうでなければ0を返す
	 *
	 *	@date		2025/07/25	作成 (D: shiba, N: kawahara)
	 */
	size_t regex_match_char_utf8(std::regex re);
	//size_t is_match_literal_start();
	//size_t is_match_literal_start();
};


inline token* tokenizer::tokenize(const char8_t* src, size_t size)
{
	p = src;
	end = src + size - 1u;
	cur = &head;
	is_eof = false;
	is_last = false;

	while (!is_eof)
	{
		if (regex_match_char_utf8(std::regex{ "[\x20\x9\xD\xA]" })) {
			advance();
			continue;
		}

		if (*p == u8'<') {
			advance();
			if (*p == u8'!' || *p == u8'?') {
				while (advance() != 0 && *p != u8'>');
				advance();
				continue;
			}
			if (*p == u8'/') {
				advance();
				cur->next = new token{ TT_TAG_START_CLOSE, {}, nullptr };
				cur = cur->next;
				continue;
			}
			cur->next = new token{ TT_TAG_START_OPEN, {}, nullptr };
			cur = cur->next;
			continue;
		}

		if (*p == u8'>') {
			cur->next = new token{ TT_TAG_END, {}, nullptr };
			cur = cur->next;
			advance();
			continue;
		}

		if (*p == u8'/') {
			if (advance() != u8'>') {
				delete_token();
				return nullptr;
			}
			
			cur->next = new token{ TT_TAG_END_EMPTY, {}, nullptr };
			cur = cur->next;
			advance();
			continue;
		}

		if (*p == u8'=') {
			cur->next = new token{ TT_EQUAL, {}, nullptr };
			cur = cur->next;
			advance();
			continue;
		}

		if (*p == u8'\'' || *p == u8'"') {
			char8_t q = *p;
			std::string buf;
			while (advance() != 0 && *p != q)
			{
				buf += *p;
			}
			if (is_eof) {
				delete_token();
				return nullptr;
			}
			
			cur->next = new token{ TT_VALUE, buf, nullptr };
			cur = cur->next;
			advance();
			continue;
		}

		if (regex_match_char_utf8(std::regex{ "[:A-Z_a-z]" })) {
			std::string buf;
			size_t byte;
			while (byte = regex_match_char_utf8(std::regex{ "[-.:0-9A-Z_a-z]" })) {
				for (size_t i = 0; i < byte; ++i) {
					buf += *p;
					advance();
				}
				if (is_eof) {
					break;
				}
			}
			cur->next = new token{ TT_LITERAL, buf, nullptr };
			cur = cur->next;
			continue;
		}

		if (*p == u8'\0') {
			is_eof = true;
			continue;
		}

		delete_token();
		return nullptr;
	}

	cur->next = new token{ TT_EOF, {}, nullptr };

	return head.next;
}

inline char tokenizer::advance()
{
	if (is_last) {
		is_eof = true;
		return 0;
	}

	if (++p == end) {
		is_last = true;
	}

	return *p;
}

inline void tokenizer::delete_token()
{
	if (head.next) {
		free_token(head.next);
	}
	cur = nullptr;
	head.next = nullptr;
}


inline size_t tokenizer::regex_match_char_utf8(std::regex re)
{
	std::string str;
	const char8_t* c = p;

	if ((*c & 0x80) == 0x00) {
		str += *c;
	}
	else if ((*c & 0xE0) == 0xC0) {
		for (int i = 0;;++c) {
			str += *c;

			if (++i >= 2) {
				break;
			}
			if (c == end) {
				return 0;
			}
		}
	}
	else if ((*c & 0xF0) == 0xE0) {
		for (int i = 0;; ++c) {
			str += *c;

			if (++i >= 3) {
				break;
			}
			if (c == end) {
				return 0;
			}
		}
	}
	else if ((*c & 0xF8) == 0xF0) {
		for (int i = 0;; ++c) {
			str += *c;

			if (++i >= 4) {
				break;
			}
			if (c == end) {
				return 0;
			}
		}
	}
	else {
		return 0;
	}

	if (std::regex_match(str, re)) {
		return str.length();
	}
	return 0;
}


/**
 *	@brief		XMLをパースするクラス
 *
 *	@date		2025/07/25	作成 (D: shiba, N: kawahara)
 *	@date		2025/08/26	parseメソッド実装 (D: shiba, N: kawahara)
 */
class parser final
{
public:
	/**
	 *	@brief		XMLの文字列をパースする
	 *
	 *	@param[in]	src	
	 *	@return		一致したら文字のバイト数を返す。そうでなければ0を返す
	 *
	 *	@date		2025/07/25	作成 (D: shiba, N: kawahara)
	 */
	std::optional<element> parse(const char8_t* src, size_t size);								//	<--	TODO

private:
	token* cur;


	/**
	 *	@brief		トークンからelementに再帰的にパースする
	 *
	 *	@return		パースしたelement
	 *
	 *	@date		2025/07/25	作成 (D: shiba, N: kawahara)
	 */
	std::optional<element> make_element();

	token* advance();
};


inline std::optional<element> parser::parse(const char8_t* src, size_t size)
{
	tokenizer tokenizer_instance;
	cur = tokenizer_instance.tokenize(src, size);

	std::optional<element> result = make_element();
	free_token(cur);

	return result;
}


inline std::optional<element> parser::make_element()
{
	if (!cur) {
		return std::nullopt;
	}

	if (cur->type != TT_TAG_START_OPEN) {
		return std::nullopt;
	}

	element elem;

	if (!advance()) {
		return std::nullopt;
	}
	if (cur->type != TT_LITERAL) {
		return std::nullopt;
	}
	elem.m_tag_name = cur->value;
	
	if (!advance()) {
		return std::nullopt;
	}

	//	タグのattributeをパース
	while (cur->type != TT_EOF) {
		if (cur->type == TT_TAG_END_EMPTY) {
			if (!advance()) {
				return std::nullopt;
			}
			return elem;
		}
		if (cur->type == TT_TAG_END) {
			if (!advance()) {
				return std::nullopt;
			}
			break;
		}
		if (cur->type == TT_LITERAL) {
			attribute attr;
			attr.m_name = cur->value;
			if (!advance()) {
				return std::nullopt;
			}
			if (cur->type != TT_EQUAL) {
				return std::nullopt;
			}
			if (!advance()) {
				return std::nullopt;
			}
			if (cur->type != TT_VALUE) {
				return std::nullopt;
			}
			attr.m_value = cur->value;
			if (!advance()) {
				return std::nullopt;
			}
			elem.m_attributes.push_back(attr);
		}
	}

	switch (cur->type) {
	case TT_LITERAL:
		elem.m_value += cur->value;
		if (!advance()) {
			return std::nullopt;
		}
		while (cur->type == TT_LITERAL)
		{
			elem.m_value += '\n' + cur->value;
			if (!advance()) {
				return std::nullopt;
			}
		}

		break;

	case TT_TAG_START_OPEN:
		while (cur->type == TT_TAG_START_OPEN) {
			auto child = make_element();
			if (!child) {
				return std::nullopt;
			}
			elem.m_children.push_back(child.value());
		}

		break;

	default:
		return std::nullopt;
	}

	if (cur->type != TT_TAG_START_CLOSE) {
		return std::nullopt;
	}

	if (!advance()) {
		return std::nullopt;
	}
	if (cur->type != TT_LITERAL) {
		return std::nullopt;
	}
	if (cur->value != elem.m_tag_name) {
		return std::nullopt;
	}

	if (!advance()) {
		return std::nullopt;
	}
	if (cur->type != TT_TAG_END) {
		return std::nullopt;
	}

	if (!advance()) {
		return std::nullopt;
	}

	return elem;
}


inline token* parser::advance()
{
	if (!cur) {
		return nullptr;
	}
	token* prev = cur;
	cur = cur->next;
	delete prev;
	return cur;
}


} // namespace internal


} // namespace zm32xml


#endif // ZM32XML_HPP
