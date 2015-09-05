#include <StringListener.h>

class CmspDisplayLine : StringListener<19> {
private:
  char _lineTxt[17];
  LiquidCrystal* _lcdDisplay;
  uint8_t _line;
  
public:
  CmspDisplayLine(unsigned int address, LiquidCrystal* lcdDisplay, uint8_t line) : StringListener<19>(address) {
    _lineTxt[16] = 0;
    _lcdDisplay = lcdDisplay;
    _line = line;
  }

  virtual void onDcsBiosFrameSync() {
    if (isDirty()) {
      _lcdDisplay->setCursor(0,_line);
      for (int i=0; i<4; i++) {
        memcpy(&_lineTxt[i*4], &getString()[i*5], 4);
      }
      _lcdDisplay->print(_lineTxt);
      clearDirty();
    }
  }
};
