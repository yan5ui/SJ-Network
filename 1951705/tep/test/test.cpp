#include <iostream>
#include <map>
#include <string>
#include <vector>
struct JSON {
    std::string raw_json;
    void clear() { raw_json.clear(); }
    JSON(std::string s = "") {
        for (char i : s) {
            if (i != ' ' && i != '\n' && i != '\t' && i != '\r') {
                raw_json.push_back(i);  //ȥ���س��ո���ַ�
            }
        }
    }
    friend std::ostream &operator<<(std::ostream &os, const JSON &a);  //���������
    JSON operator[](const std::string &key);                           //����
    JSON operator[](int index);                                        //����
    std::map<std::string, JSON> as_map();
    std::vector<JSON> as_vector();
    std::string as_string() { return raw_json.substr(1, raw_json.size() - 2); };  //��Ҫ��ȥ��˫����
    int as_int() { return atoi(raw_json.c_str()); };
    // float as_float();
    bool as_bool() {
        if (raw_json == "true")
            return true;
        else
            return false;
    };
};
#include <stack>
using namespace std;
std::map<std::string, JSON> JSON_to_map(JSON json) {
    string raw_json = json.raw_json;
    map<string, JSON> res;
    string key, value;
    stack<char> find_value_stack;
    bool find_key = 0, find_value = 0;
    for (size_t i = 1; i < raw_json.size(); ++i) {
        if (find_value == true) {
            if (find_value_stack.size() == 0 && raw_json[i] == '}') {
                res[key] = JSON(value);
                // cout << "key: " << key << endl << "value: " << value << endl;
                break;
            } else if (find_value_stack.size() == 0 && raw_json[i] == ',') {
                res[key] = JSON(value);
                // cout << "key: " << key << endl << "value: " << value << endl;
                value.clear();
                find_value = false;
            } else {
                if (raw_json[i] == '[' || raw_json[i] == '{') {
                    find_value_stack.push(raw_json[i]);
                } else if (raw_json[i] == '}' || raw_json[i] == ']') {
                    find_value_stack.pop();
                }
                value.push_back(raw_json[i]);
            }
        }
        if (raw_json[i] == '\"' && find_value == false) {
            if (find_key == true) {
                // key׼������,׼����value
                find_key = false;
                ++i;  //����ð��
                find_value = true;
            } else {  //׼����key
                find_key = true;
                key.clear();
            }
        } else if (find_key == true) {
            key.push_back(raw_json[i]);
        }
    }
    return res;
}
std::vector<JSON> JSON_to_vector(JSON json) {
    vector<JSON> res;
    string raw_json = json.raw_json;
    stack<char> find_item_stack;
    string target_json;
    for (size_t i = 1; i < raw_json.size(); ++i) {
        if (raw_json[i] == ',' && find_item_stack.size() == 0) {
            ++i;
            res.push_back(JSON(target_json));
            target_json.clear();
        }
        if (raw_json[i] == ']' && find_item_stack.size() == 0) {
            res.push_back(JSON(target_json));
            target_json.clear();
        }
        target_json.push_back(raw_json[i]);
        if (raw_json[i] == '[' || raw_json[i] == '{') {
            find_item_stack.push(raw_json[i]);
        } else if (raw_json[i] == '}' || raw_json[i] == ']') {
            find_item_stack.pop();
        }
    }
    return res;
}

std::map<std::string, JSON> JSON::as_map() { return JSON_to_map(*this); }
std::vector<JSON> JSON::as_vector() { return JSON_to_vector(*this); }
std::ostream &operator<<(std::ostream &os, const JSON &a) {
    os << a.raw_json;
    return os;
}
JSON JSON::operator[](const std::string &key) { return this->as_map()[key]; }
JSON JSON::operator[](int index) { return this->as_vector()[index]; }

int main() {
    string send_json =
        "{\n\
\"data\": [{\"md5\":1},{\"md5\":3}], \n\
\"message\": \"�����ʽ����\", \n\
\"statusCode\": 400, \n\
\"success\": false\n}\
";
    cout << send_json << endl;
    JSON tep(send_json);
    cout << tep << endl;
    std::map<string, JSON> tep_map = JSON_to_map(tep);
    cout << "message: " << tep_map["message"].as_string() << endl;
    cout << "size: " << JSON_to_vector(tep_map["data"]).size() << endl;
    cout << "md5(1): " << JSON_to_map(JSON_to_vector(tep_map["data"])[1])["md5"].as_int() << endl;
    cout << "md5(0): " << tep_map["data"][0]["md5"].as_int() << endl;
    cout << "md5(1): " << tep_map["data"][1]["md5"].as_int() << endl;
    return 0;
}