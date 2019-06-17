#ifndef Clock_h
#define Clock_h

class Clock {
    public:
        Clock(int* time);
        void setTime(int* time);
        long getTime();

    private:
        void process();

        int _hCount;
        int _mCount;
        int _sCount;
};

#endif