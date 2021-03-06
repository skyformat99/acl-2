#pragma once
#include "acl_cpp/acl_cpp_define.hpp"

namespace acl {

class string;
class ostream;
class socket_stream;
class http_header;
class HttpCookie;
class HttpServlet;

/**
 * 与 HTTP 客户端响应相关的类，该类不应被继承，用户也不需要
 * 定义或创建该类对象
 */
class ACL_CPP_API HttpServletResponse
{
public:
	/**
	 * 构造函数
	 * @param stream {socket_stream&} 数据流，内部不会自动关闭流
	 */
	HttpServletResponse(socket_stream& stream);
	~HttpServletResponse(void);

	/**
	 * 设置 HTTP 响应数据体的长度
	 * @param n {acl_int64} 数据体长度
	 */
#ifdef WIN32
	HttpServletResponse& setContentLength(__int64 n);
#else
	HttpServletResponse& setContentLength(long long int n);
#endif

	/**
	 * 设置与 HTTP 客户端保持联系长连接
	 * @param on {bool}
	 */
	HttpServletResponse& setKeepAlive(bool on);

	/**
	 * 设置 HTTP 响应数据体的 Content-Type 字段值，可字段值可以为：
	 * text/html 或 text/html; charset=utf8 格式
	 * @param value {const char*} 字段值
	 */
	HttpServletResponse& setContentType(const char* value);

	/**
	 * 设置 HTTP 响应数据体中字符集，当已经在 setContentType 设置
	 * 了字符集，则就不必再调用本函数设置字符集
	 * @param charset {const char*} 响应体数据的字符集
	 */
	HttpServletResponse& setCharacterEncoding(const char* charset);

	/**
	 * 设置 HTTP 响应头中的日期格式的字段
	 * @param name {const char*} HTTP 响应头中的字段名
	 * @param value {time_t} 时间值
	 */
	HttpServletResponse& setDateHeader(const char* name, time_t value);

	/**
	 * 设置 HTTP 响应头中的字符串格式字段
	 * @param name {const char*} HTTP 响应头中的字段名
	 * @param value {const char*} 字段值
	 */
	HttpServletResponse& setHeader(const char* name, const char* value);

	/**
	 * 设置 HTTP 响应头中的整数格式字段
	 * @param name {const char*} HTTP 响应头中的字段名
	 * @param value {int} 字段值
	 */
	HttpServletResponse& setHeader(const char* name, int value);

	/**
	 * 设置 HTTP 响应头中的状态码：1xx, 2xx, 3xx, 4xx, 5xx
	 * @param status {int} HTTP 响应状态码, 如：200
	 */
	HttpServletResponse& setStatus(int status);

	/**
	 * 设置为 CGI 模式，用户一般不需手工设置，因为 HttpServlet 类
	 * 会自动设置是否是 CGI 模式
	 * @param on {bool} 是否是 CGI 模式
	 */
	HttpServletResponse& setCgiMode(bool on);

	/**
	 * 设置 HTTP 响应头中的重定向 location 字段
	 * @param location {const char*} URL，非空
	 * @param status {int} HTTP 响应状态码，一般为 3xx 类
	 */
	HttpServletResponse& setRedirect(const char* location, int status = 302);

	/**
	 * 添加 cookie 对象，该对象必须是动态分配的，且用户自己不能
	 * 再显示释放该对象，因为内部会自动释放
	 * @param cookie {HttpCookie*}
	 */
	HttpServletResponse& addCookie(HttpCookie* cookie);

	/**
	 * 添加 cookie
	 * @param name {const char*} cookie 名
	 * @param value {const char*} cookie 值
	 * @param domain {const char*} cookie 存储域
	 * @param path {const char*} cookie 存储路径
	 * @param expires {time_t} cookie 过期时间间隔，当当前时间加
	 *  该值为 cookie 的过期时间截(秒)
	 */
	HttpServletResponse& addCookie(const char* name, const char* value,
		const char* domain = NULL, const char* path = NULL,
		time_t expires = 0);

	/**
	 * 将 url 进行 url 编码
	 * @param out {string&} 存储编码后的结果
	 * @param url {const char*} 未编码前原始的 url
	 */
	void encodeUrl(string& out, const char* url);

	/**
	 * 获得 HTTP 响应头
	 * @return {http_header&}
	 */
	http_header& getHttpHeader(void) const;

	/**
	 * 发送 HTTP 响应头，用户应该发送数据体前调用此函数将 HTTP
	 * 响应头发送给客户端
	 */
	bool sendHeader(void);

	/**
	 * 获得 HTTP 响应对象的输出流对象，用户在调用 sendHeader 发送
	 * 完 HTTP 响应头后，通过该输出流来发送 HTTP 数据体
	 * @return {ostream&}
	 */
	ostream& getOutputStream(void) const;
private:
	socket_stream& stream_;
	http_header* header_;
	char  charset_[32];
	char  content_type_[32];
};

}  // namespace acl
