#include <stdio.h>
#include <string.h>

#include "cmsis_os2.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "drivers/st7789.h"
#include "types/sensortype.h"

void startdisplaytask(void *argument) {
    ST7789_Init();
    ST7789_Init();
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);  // 开背光
    ST7789_Fill_Color(BLACK);

    sensordata_t *data ;
    char buf [64];

    uint16_t x = 0;
    uint16_t last_y_temp = 230;
    uint16_t last_y_volt = 230;
    uint16_t last_y_curr = 230;
    uint16_t last_y_vib = 230;

    for (;;) {
        osMessageQueueGet(displayqueueHandle, &data, NULL, osWaitForever);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);  // 每次都拉高背光
        // 第一行：温度和电压
        sprintf(buf, "T:%.1fC V:%.2fV", data->temperature, data->voltage);
        ST7789_WriteString(0, 10, buf,Font_11x18, WHITE, BLACK);
        sprintf(buf, "G:%.2fg I:%.1fma", data->vibration, data->current);
        ST7789_WriteString(0, 40, buf, Font_11x18, WHITE, BLACK);

        // 曲线区域：y=80（最上）~ y=230（最下），高度150像素
        // 温度：15℃→y=230，50℃→y=80
        float temp = data->temperature;
        if (temp < 15) temp = 15;
        if (temp > 50) temp = 50;
        uint16_t y_temp = 230 - (temp - 15) * (150.0/35);

        // 电压：0V→y=230，3.3V→y=80
        float volt = data->voltage;
        if (volt < 0) volt = 0;
        if (volt > 3.3) volt = 3.3;
        uint16_t y_volt = 230 - volt * (150.0/3.3);

        // 电流：0mA→y=230，80mA→y=80
        float curr = data->current;
        if (curr < 0) curr = 0;
        if (curr > 80) curr = 80;
        uint16_t y_curr = 230 - curr * (150.0/80);

        // 振动：0g→y=230，2g→y=80
        float vib = data->vibration;
        if (vib < 0) vib = 0;
        if (vib > 2) vib = 2;
        uint16_t y_vib = 230 - vib * (150.0/2);

        // 画四条线
        ST7789_DrawLine(x, last_y_temp, x+1, y_temp, RED);
        ST7789_DrawLine(x, last_y_volt, x+1, y_volt, GREEN);
        ST7789_DrawLine(x, last_y_curr, x+1, y_curr, BLUE);
        ST7789_DrawLine(x, last_y_vib, x+1, y_vib, YELLOW);

        // 更新上一个点
        last_y_temp = y_temp;
        last_y_volt = y_volt;
        last_y_curr = y_curr;
        last_y_vib = y_vib;
        x++;

        if (x >= 240) {
            x = 0;
            ST7789_Fill_Color(BLACK);

        }
        vPortFree(data);

    }
}