// http_servlet.cpp : 定义控制台应用程序的入口点。
//
#include <assert.h>
#include <getopt.h>
#include "lib_acl.hpp"

using namespace acl;

//////////////////////////////////////////////////////////////////////////

class http_request_test
{
public:
	http_request_test(const char* server_addr, const char* file,
		const char* stype, const char* charset)
	{
		server_addr_= server_addr;
		file_ = file;
		stype_ = stype;
		charset_ = charset;
		to_charset_ = "gb2312";
	}

	~http_request_test() {}

	bool run(void)
	{
		string body;
		if (ifstream::load(file_, &body) == false)
		{
			logger_error("load %s error", file_.c_str());
			return false;
		}

		http_request req(server_addr_);

		// 添加 HTTP 请求头字段

		string ctype("text/");
		ctype << stype_ << "; charset=" << charset_;

		http_header& hdr = req.request_header();  // 请求头对象的引用
		hdr.set_url("/");
		hdr.set_content_type(ctype);

		// 发送 HTTP 请求数据
		if (req.request(body.c_str(), body.length()) == false)
		{
			logger_error("send http request to %s error",
				server_addr_.c_str());
			return false;
		}

		// 取出 HTTP 响应头的数据字段
		http_client* client = req.get_client();
		assert(client);
		const char* p = client->header_value("Content-Type");
		if (p == NULL || *p == 0)
		{
			logger_error("no Content-Type");
			return false;
		}

		// 分析 HTTP 响应头的数据类型
		http_ctype content_type;
		content_type.parse(p);

		// 响应头数据类型的子类型
		const char* stype = content_type.get_stype();

		bool ret;
		if (stype == NULL)
			ret = do_plain(req);
		else if (strcasecmp(stype, "xml") == 0)
			ret = do_xml(req);
		else if (strcasecmp(stype, "json") == 0)
			ret = do_json(req);
		else
			ret = do_plain(req);
		if (ret == true)
			printf("read ok!\r\n");
		return ret;
	}

private:
	// 处理 text/plain 类型数据
	bool do_plain(http_request& req)
	{
		string body;
		if (req.get_body(body, to_charset_) == false)
		{
			logger_error("get http body error");
			return false;
		}
		printf("body:\r\n(%s)\r\n", body.c_str());
		return true;
	}

	// 处理 text/xml 类型数据
	bool do_xml(http_request& req)
	{
		xml body;
		if (req.get_body(body, to_charset_) == false)
		{
			logger_error("get http body error");
			return false;
		}
		xml_node* node = body.first_node();
		while (node)
		{
			const char* tag = node->tag_name();
			const char* name = node->attr_value("name");
			const char* pass = node->attr_value("pass");
			printf(">>tag: %s, name: %s, pass: %s\r\n",
				tag ? tag : "null",
				name ? name : "null",
				pass ? pass : "null");
			node = body.next_node();
		}
		return true;
	}

	// 处理 text/json 类型数据
	bool do_json(http_request& req)
	{
		json body;
		if (req.get_body(body, to_charset_) == false)
		{
			logger_error("get http body error");
			return false;
		}

		json_node* node = body.first_node();
		while (node)
		{
			if (node->tag_name())
			{
				printf("tag: %s", node->tag_name());
				if (node->get_text())
					printf(", value: %s\r\n", node->get_text());
				else
					printf("\r\n");
			}
			node = body.next_node();
		}
		return true;
	}

private:
	string server_addr_;	// web 服务器地址
	string file_;		// 本地请求的数据文件
	string stype_;		// 请求数据的子数据类型
	string charset_;	// 本地请求数据文件的字符集
	string to_charset_;	// 将服务器响应数据转为本地字符集
};

//////////////////////////////////////////////////////////////////////////

static void usage(const char* procname)
{
	printf("usage: %s -h[help]\r\n", procname);
	printf("options:\r\n");
	printf("\t-f request file\r\n");
	printf("\t-t request stype[xml/json/plain]\r\n");
	printf("\t-c request file's charset[gb2312/utf-8]\r\n");
}

int main(int argc, char* argv[])
{
	int   ch;
	string server_addr("127.0.0.1:8888"), file("./xml.txt");
	string stype("xml"), charset("gb2312");

	while ((ch = getopt(argc, argv, "hs:f:t:c:")) > 0)
	{
		switch (ch)
		{
		case 'h':
			usage(argv[0]);
			return 0;
		case 'f':
			file = optarg;
			break;
		case 't':
			stype = optarg;
			break;
		case 'c':
			charset = optarg;
			break;
		default:
			usage(argv[0]);
			return 0;
		}
	}

	// 开始运行
	log::stdout_open(true);
	http_request_test test(server_addr, file, stype, charset);
	test.run();

	return 0;
}

