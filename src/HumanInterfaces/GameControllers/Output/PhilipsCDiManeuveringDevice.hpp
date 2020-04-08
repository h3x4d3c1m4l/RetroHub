#include "driver/uart.h"
#include "driver/gpio.h"
#include "GameControllerOutputBase.hpp"

class PhilipsCDiManeuveringDevice : public GameControllerOutputBase
{
public:
    PhilipsCDiManeuveringDevice(uart_port_t uart_port, gpio_num_t cts_pin, bool force_dpad_input = false, int baud_rate = 9600)
    {
        uart_config_t uart_config = {
            .baud_rate = baud_rate,
            .data_bits = UART_DATA_7_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_2,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .rx_flow_ctrl_thresh = 122,
            .use_ref_tick = false
        };
        ESP_ERROR_CHECK(uart_param_config(uart_port, &uart_config));
        this->uart_port = uart_port;
        ESP_ERROR_CHECK(uart_set_pin(uart_port, 17, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        ESP_ERROR_CHECK(uart_driver_install(uart_port, 129, 0, 0, NULL, 0));

        gpio_config_t cts_pin_config = {
            .pin_bit_mask = 1ULL<<cts_pin,
            .mode = GPIO_MODE_INPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_DISABLE,
        };
        gpio_config(&cts_pin_config);
        this->cts_pin = cts_pin;

        this->force_dpad_input = force_dpad_input;
    }
    bool SendState(ControllerState controller_state);

private:
    uart_port_t uart_port;
    gpio_num_t cts_pin;
    bool force_dpad_input;
    unsigned char old_byte1, old_byte2, old_byte3;
    bool id_already_sent;
    void WaitForCTS();
};