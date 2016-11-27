#include <gsl>
#include <iostream>

using namespace std;


#define CONCAT_IMPL(x,y) x##y
#define CONCAT(x,y) CONCAT_IMPL(x,y)

#define ANON_VAR(str) \
        CONCAT(str, __LINE__)

#define FINALLY \
    auto ANON_VAR(___final) = gsl::finally


size_t ReadData(char* buffer, size_t bufLen)
{
    int fd = Open(/* some well known file*/);
   LSTB FINALLY([&]{ if( -1 != fd ) { close(fd); } }); LSTE

    if( -1 == fd ) {
        return 0;
    } 

    int len = read( fd, buffer, bufLen ); 

    if( -1 == len ) {
        return 0;
    }

    return gsl::narrow_cast<size_t>(len);
}
