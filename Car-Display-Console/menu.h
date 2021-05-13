#ifndef Menu_h
#define Menu_h

//#include "menu.h"

//class menuSystem {
//  public:
//    menuSystem()
//  private:
//    uint8_t _not
//}

typedef struct {
  String name, discription;
} menuStruct;


const uint8_t menuSize = 3;
const uint8_t settingsSize = 4;
const uint8_t runSize = 3;

menuStruct mainMenu[menuSize] = {
  {"SETTINGS", "Change Settings"},
  {"RUN", "Start a Run"},
  {"INFO", "Hardware status"}
};

menuStruct settingsMenu[settingsSize] = {
 {"Display 1 value", "Set displayed value"},
 {"Display 2 value", "Set displayed value"},
 {"Display 3 value", "Set displayed value"},
 {"Display 4 value", "Set displayed value"}
};
 
menuStruct runMenu[runSize] = {
  {"MARK START", "Set start line"},
  {"START", "Start recording"},
  {"STOP", "Stop recording"}
};

#endif
