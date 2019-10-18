#ifndef FINMAN_UTIL_H
#define FINMAN_UTIL_H

namespace util
{
    inline std::string now()
    {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer [80];
        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer,80,"%Y-%m-%d %H:%M:%S",timeinfo);
        std::puts(buffer);

        return std::string(buffer);
    }
}

#endif //FINMAN_UTIL_H
