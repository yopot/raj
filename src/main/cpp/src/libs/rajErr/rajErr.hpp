#ifndef RAJ_RAJ_ERROR
#define RAJ_RAJ_ERROR

#include <string>

class rajErr
{
 public:
 bool isAndroid = false;
 rajErr(std::string msg):msg(msg){}
 std::string msg;
};

#endif