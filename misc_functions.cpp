#include <string>
#include <iostream>
#include <sstream>

long strtolong(const std::string& str) {
	std::istringstream stream (str);
	long i = 0;
	stream >> i;
	return i;
}

int64_t strtolonglong(const std::string& str) {
	std::istringstream stream (str);
	int64_t i = 0;
	stream >> i;
	return i;
}


std::string inttostr(const int i) {
	std::string str;
	std::stringstream out;
	out << i;
	str = out.str();
	return str;
}

static int hex_decode(char v)
{
        if (v >= '0' && v <= '9')
                return v - '0';
        if (v >= 'A' && v <= 'F')
                return v - 'A' + 10;
        if (v >= 'a' && v <= 'f')
                return v - 'a' + 10;
        return -1;
};

unsigned long long hex2dec_c(const char *s)
{
    unsigned long long n = 0;
    int i, len = strlen(s);
    for(i=0; i < len; i++)
    {
	n = n * 16 + hex_decode(s[i]);
    }
    return n;
}

unsigned long long hex2dec(const std::string &in) 
{
    return hex2dec_c(in.c_str());
}

std::string hex_decode(const std::string &in) {
	std::string out;
	out.reserve(20);
	unsigned int in_length = in.length();
	for (unsigned int i = 0; i < in_length; i++) {
		unsigned char x = '0';
		if (in[i] == '%' && (i + 2) < in_length) {
			i++;
			if (in[i] >= 'a' && in[i] <= 'f') {
				x = static_cast<unsigned char>((in[i]-87) << 4);
			} else if (in[i] >= 'A' && in[i] <= 'F') {
				x = static_cast<unsigned char>((in[i]-55) << 4);
			} else if (in[i] >= '0' && in[i] <= '9') {
				x = static_cast<unsigned char>((in[i]-48) << 4);
			}

			i++;
			if (in[i] >= 'a' && in[i] <= 'f') {
				x += static_cast<unsigned char>(in[i]-87);
			} else if (in[i] >= 'A' && in[i] <= 'F') {
				x += static_cast<unsigned char>(in[i]-55);
			} else if (in[i] >= '0' && in[i] <= '9') {
				x += static_cast<unsigned char>(in[i]-48);
			}
		} else {
			x = in[i];
		}
		out.push_back(x);
	}
	return out;
}

std::string bintohex(const std::string &in) {
	std::string out;
	out.reserve(40);
	size_t length = in.length();
	for (unsigned int i = 0; i < length; i++) {
		unsigned char x = (unsigned char)in[i] >> 4;
		if (x > 9) {
			x += 'a' - 10;
		} else {
			x += '0';
		}
		out.push_back(x);
		x = in[i] & 0xF;
		if (x > 9) {
			x += 'a' - 10;
		} else {
			x += '0';
		}
		out.push_back(x);
	}
	return out;
}
