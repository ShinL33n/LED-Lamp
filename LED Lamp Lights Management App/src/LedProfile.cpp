#include <stdint.h>

class LedProfile
{
    private:       

        uint8_t _red, _green, _blue;        // Red, green and blue values ranged from 0 (0%) to 255 (100%) of the color given
        uint8_t _brightness;                // Brightness of leds given in percentage ranged from 0 to 100
        uint8_t _startHour, _endHour;       // The time lamp is suposed to work defined in hour and minutes of the hour
        uint8_t _startMinutes, _endMinutes; 
        // uint8_t _whiteHue;                   // White color temperature the light is suposed to be

    public:

        uint8_t getRed() const          { return _red; }
        uint8_t getGreen() const        { return _green; }
        uint8_t getBlue() const         { return _blue; }
        uint8_t getBrightness() const   { return _brightness; }
        uint8_t getStartHour() const    { return _startHour; }
        uint8_t getEndtHour() const     { return _endHour; }
        uint8_t getStarMinutes() const  { return _startMinutes; }
        uint8_t getEndMinutes() const   { return _endMinutes; }


        void setRGB(uint8_t red, uint8_t green, uint8_t blue){
            _red = red;
            _green = green;
            _blue = blue;
        }

        void setBrightness(uint8_t brightness){
            _brightness = brightness;
        }

        void setWorkHours(uint8_t startHour, uint8_t endHour, uint8_t startMinutes, uint8_t endMinutes){
            _startHour = startHour;
            _endHour = endHour;
            _startMinutes = startMinutes;
            _endMinutes = endMinutes;
        }

        // void setWhiteHue(){

        // }
};