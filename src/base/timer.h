#include <iostream>
#include <sys/time.h>
#include <time.h>

namespace base
{
    class timer
    {
    public:
        timer();
        unsigned long int slice() const { return Slice; }
        unsigned long int current_time() const;
        unsigned long int frames_missed() const { return Frames_missed; }
        void sleep(unsigned long int msecs) const;

        void set_slice(unsigned long int sl);

        void update();

    private:
        unsigned long int convert_timeval (const struct timeval & tv) const
        { return ((tv.tv_sec - initial_time.tv_sec) * 1000 + (tv.tv_usec - initial_time.tv_usec) / 1000); }
        struct timeval initial_time;

        unsigned long int Slice;
        unsigned long int Lasttime;
        unsigned long int Frames_missed;
    };
}
