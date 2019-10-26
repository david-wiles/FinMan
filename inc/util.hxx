#ifndef FINMAN_UTIL_H
#define FINMAN_UTIL_H

namespace util
{
    inline std::string now()
    {
        time_t rawtime;
        char buffer [80];

        time(&rawtime);
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&rawtime));
        puts(buffer);

        return std::string(buffer);
    }
}

#endif //FINMAN_UTIL_H
