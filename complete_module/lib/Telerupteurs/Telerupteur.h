#ifndef Telerupteur_h
#define Telerupteur_h

class Telerupteur {
    public: 
        Telerupteur(int pin);

        void enable();

        void disable();

        void impulse(int millis = 200);

    private:
        int _pin;
};

#endif