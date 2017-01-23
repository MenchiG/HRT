#include "includes.h"

void MMA_set_mode(INT8U x)
{
	mma_mode = x;
	if(x == MMA_RANGE105)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
	else if(x == MMA_RANGE6)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}
}

INT8U MMA_get_mode(void)
{
	return mma_mode;
}


