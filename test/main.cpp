#include <iostream>
#include <stdio.h>
#include "bootloader_random.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "HumanInterfaces/GameControllers/Output/PhilipsCDiManeuveringDevice.hpp"

extern "C" {
    void app_main(void);
}

void app_main()
{
    bootloader_random_enable();
    PhilipsCDiManeuveringDevice x = PhilipsCDiManeuveringDevice(UART_NUM_2, GPIO_NUM_23, false, 9600);
    for (;;)
    {
        ControllerState cs = {};
        cs.dpad_left = true;
        if (esp_random() % 2 == 0)
            cs.dpad_up = true;
        else
            cs.dpad_down = true;
        x.SendState(cs);

        std::cout << 'C';
    }
}