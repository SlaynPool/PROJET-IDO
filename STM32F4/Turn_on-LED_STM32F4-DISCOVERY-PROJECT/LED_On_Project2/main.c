#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

int main()
{
  //Enable the GPIOD Clock
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
  
  // GPIOD Configuration
  STM_EVAL_LEDInit(LED6);
  
  // GPIOD-PIN-15 ON
  STM_EVAL_LEDOn(LED6);
  
  return 0;
}
