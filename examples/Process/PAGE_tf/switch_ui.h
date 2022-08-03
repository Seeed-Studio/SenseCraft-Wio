#ifndef Morse_h
#define Morse_h

class Switch_UI
{
  public:
    Morse(int pin);
    void dot();
    void dash();
  private:
    int _pin;
};

#endif