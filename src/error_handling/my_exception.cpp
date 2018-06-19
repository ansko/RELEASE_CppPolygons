#include "my_exception.hpp"


MyException::MyException(std::string description) {
    std::cout << description << std::endl;
}
