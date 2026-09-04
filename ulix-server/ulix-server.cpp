#include<iostream>
#include"strutil_lib.h"
#include<ctime>
#include<map>
#include<sstream>
#include<fstream>
#include<filesystem>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
using namespace strutil;
namespace fs = filesystem;
class ulix_request {
private:
	string char_data;
	string header_byte;
	map<string, string>header_key_value;
	string body_byte;
public:
	
	ulix_request(string inp_data) {
		char_data = inp_data;
		int save_ind = 0;
		for (int i = 0; i < char_data.size() - 1; i++) {
			if ((char_data[i] == 0x0D) and (char_data[i + 1] == 0x0A)) {
				save_ind = i + 2;
				break;
			}
			else {
				header_byte += char_data[i];
			}
		}
		for (int i = save_ind; i < char_data.size(); i++) {
			body_byte += char_data[i];
		}
		vector<string>header_pairs = split(header_byte, '|');
		for (int i = 0; i < header_pairs.size(); i++) {
			string header_pair = header_pairs[i];
			string key;
			string value;
			bool flag = false;
			for (int j = 0; j < header_pair.size(); j++) {
				if(header_pair[j] == '/'){
					flag = true;
				}
				else if (flag) {
					value += header_pair[j];
				}
				else {
					key += header_pair[j];
				}
			}
			header_key_value[key] = value;
		}
	}

	string get_method() {
		if (header_key_value.count("method")) return header_key_value["method"];	
		return "get";
	}

	string get_code() {
		if (header_key_value.count("code")) return header_key_value["code"];
		return "ascii";
	}
	
	string get_datatype() {
		if (header_key_value.count("datatype")) return header_key_value["datatype"];
		return "byte";
	}

	string get_special_info(string key) {
		if (header_key_value.count(key) and header_key_value[key] != "") return header_key_value[key];
		return "nill";
	}
	string get_body() {
		return body_byte;
	}
	string get_header() {
		return  header_byte;
	}
	string get_char_data() {
		return char_data;
	}
	
};
class ulix_response {
string output;
public:
	
	ulix_response(string status, string datatype, string code, string data) {
		string header = "";
		if (status == "ok") {
			header += ("ok|" + code + '|' + datatype);
			output = header + char(0x0D) + char(0x0A) + data;
		}
		else {
			header += (string("error|") + code + string("|text"));
			output = header + char(0x0D) + char(0x0A) + data;
		}

	}
	string get_output() {
		return output;
	}
};
class simple_handler{
private:
	int last_saved = 0;
public:
	bool save_file(const string& path, const string& content) {
		ofstream file(path, ios::binary);  
		if (!file.is_open()) {
			return false;
		}
		file.write(content.c_str(), content.size());
		file.close();
		return true;
	}
	string get_current_time() {
		time_t now = time(nullptr);
		char buffer[26];
		ctime_s(buffer, sizeof(buffer), &now);
		string time_str(buffer);
		if (!time_str.empty()) {
			time_str.pop_back();  
		}
		return time_str;
	}
	string read_file(const string& path) {
		ifstream file(path, ios::binary);
		if (!file.is_open()) {
			return "";
		}
		stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
	bool file_exists(const string &path) {
		return fs::exists(path) && fs::is_regular_file(path);
	}
	string handle_request(ulix_request req) {
		string method = req.get_method();
		string code = req.get_code();
		string datatype = req.get_datatype();
		if (method == "get") {
			if (datatype == "html") {
				if (code == "ascii") {
					string file_name =  req.get_body();
					string path = "../www" + file_name;
					if (file_exists(path)) {
						string file_content = read_file(path);
						ulix_response resp("ok", "html", "utf-8", file_content);
						return resp.get_output();
					}
					else {
						ulix_response resp("error", "text", "ascii", "not found");
						return resp.get_output();
					}
					
				}
			}
			else if (datatype == "text") {
				if (code == "ascii") {
					string com = req.get_body();
					if (com == "/time") {
						ulix_response resp("ok", "text", "ascii", get_current_time());
						return resp.get_output();
					}
				}
			}
		}
		else if (method == "post") {
			if (datatype == "html") {
				if (code == "utf-8") {
					if (req.get_special_info("password") == "1111") {
						string file_content = req.get_body();
						int num = last_saved + 1;
						string path = "../www/file_from_user_" + to_string(num)+ ".html";
						bool result = save_file(path, file_content);
						if (result) {
							last_saved++;
							string body = "file saved by name file_from_user_" + to_string(last_saved) + ".html";
							ulix_response resp("ok", "text", "ascii", body);
							return resp.get_output();
						}
						else {
							ulix_response resp("error", "text", "ascii", "cant save this file");
							return resp.get_output();
						}
					}
				}
			}
			else if (datatype == "text") {
				if (code == "ascii") {
					string text = req.get_body();
					cout << "text from user: " << text << '\n';
					ulix_response resp("ok", "text", "ascii", "delivered");
					return resp.get_output();
				}
			}
		}
		ulix_response dum("error", "text", "ascii", "uknown request");
		cout << req.get_char_data() << endl;

		cout << req.get_method() << ' ' << req.get_code() << ' ' << req.get_datatype() << endl;
		return dum.get_output();
		
	}
};
void start_server(int port) {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
	listen(server_socket, SOMAXCONN);

	cout << "Server start on port: " << port << endl;

	while (true) {
		SOCKET client = accept(server_socket, nullptr, nullptr);
		char buffer[4096] = { 0 };
		recv(client, buffer, sizeof(buffer), 0);

		ulix_request req(buffer);
		string response = simple_handler().handle_request(req);

		send(client, response.c_str(), response.size(), 0);
		closesocket(client);
	}

	closesocket(server_socket);
	WSACleanup();
}
int main(void) {
	start_server(8080);
	return 0;
}
