#include "base/timer.h"

namespace base
{
    timer::timer() : Slice(100), Lasttime(0), Frames_missed(0)
    {
        gettimeofday(&initial_time, NULL);
    }

    void timer::set_slice(unsigned long int sl)
    {
        Slice = sl;
    }

    unsigned long int timer::current_time() const
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return convert_timeval(tv);
    }

    void timer::sleep(unsigned long int msecs) const
    {
        unsigned char err;
        struct timespec req, rem;
        rem.tv_sec = msecs / 1000;
        rem.tv_nsec = (msecs % 1000) * 1000000;

        do
        {
            req = rem;
            err = nanosleep(&req, &rem);
        } while (err && (errno == EINTR));

        
    }

    void timer::update()
    {
        unsigned long int curtime = current_time();

        while (curtime - Lasttime < Slice)
        {
            //this->sleep(1);
            curtime = current_time();
        }
        
        Frames_missed = (curtime - Lasttime) / Slice - 1;
        Lasttime = curtime - (curtime - Lasttime) % Slice;
    }
}
