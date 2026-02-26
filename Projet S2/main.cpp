#include <main.h>

uint8_t hp = 20;
uint8_t munition = 10;
uint16_t LedState = 0b001111111111;

void setup() {
  bitSet(LedState, 10); //led vert
  Serial.begin(115200);
  SetupLed();
  joystick_setup();
  bouton_update();
  setup_encoder(2, 3);
}

void loop() {
    
  if (bouton1_est_appuye())
  {
    if (munition > 0)
    {
      munition--;
      LedState = ShiftRight(LedState);
    }
    bitSet(LedState, 11); //led rouge
  }
  else
  {
    bitClear(LedState, 11); //led rouge
  }
  if ((munition == 0)&&(bouton2_est_appuye()))
  {
    munition = 10;
    LedState = (LedState & 0b110000000000) | 0b001111111111;
  }
  
  hp = Serial.read();
  Serial.print(munition);
  Serial.print(readJoystick_x());
  Serial.print(readJoystick_y());
  Serial.print(bouton1_est_appuye());
  Serial.print(bouton2_est_appuye());
  
  bouton_update();
  UpdateLed(LedState); 
}


uint16_t ShiftRight(uint16_t value)
{
    uint8_t msb = value & 0b11000000;     
    uint8_t lower = value & 0b00111111;   

    lower = lower >> 1;                   

    return msb | lower;                   
}

uint16_t ShiftLeft(uint16_t value)
{
    uint8_t msb = value & 0b11000000;     
    uint8_t lower = value & 0b00111111;   

    lower = lower << 1;                   

    return msb | lower;                   
}


