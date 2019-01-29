#include "common.h"

IWDG_HandleTypeDef hiwdg;



/* IWDG init function */
extern void MX_IWDG_Init(void)
{

  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
  hiwdg.Init.Window = 0xEA6;   //0xFFF
  hiwdg.Init.Reload = 0xEA6;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

extern void ReStart(void)
{
	hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Window = 1;   
  hiwdg.Init.Reload = 1;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
}

extern void IWDG_Refresh(void)
{
	HAL_IWDG_Refresh(&hiwdg);
	//printf("iwdg ");
}
