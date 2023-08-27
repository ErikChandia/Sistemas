#ifndef __ERROR__
#define __ERROR__

#include <stdexcept>

void error(const std::string& message){
    throw std::runtime_error(message);
}
void error(const std::string& s, const std::string& s2){
	error(s + s2);
}

#endif