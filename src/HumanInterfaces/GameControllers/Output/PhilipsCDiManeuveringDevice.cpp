#include "PhilipsCDiManeuveringDevice.hpp"

bool PhilipsCDiManeuveringDevice::SendState(ControllerState controller_state)
{
    if (!gpio_get_level(cts_pin))
    {
        WaitForCTS();
        if (id_already_sent)
            vTaskDelay(1);
        else
            vTaskDelay(100);
        uart_write_bytes(uart_port, new char[1]{ 'J' }, 1); // device ID
    }

    unsigned char data_byte1 = 0b1000000, data_byte2 = 0b0000000, data_byte3 = 0b0000000;

    // translate controller state to maneauvering devices data bytes
    if (controller_state.action1)
        data_byte1 |= 0b0100000;
    if (controller_state.action2)
        data_byte1 |= 0b0010000;

    if (!controller_state.n_sticks || force_dpad_input)
    {
        // no analog sticks available (or we are asked to ignore them), use dpad values
        if (controller_state.dpad_right)
        {
            data_byte1 |= 0b0000011;
            data_byte2 |= 0b0111111;
        }
        else if (controller_state.dpad_left)
        {
            data_byte1 |= 0b0000001;
            data_byte2 |= 0b0111111;
        }

        if (controller_state.dpad_up)
        {
            data_byte1 |= 0b0001100;
            data_byte3 |= 0b0111111;
        }
        else if (controller_state.dpad_down)
        {
            data_byte1 |= 0b0000100;
            data_byte3 |= 0b0111111;
        }
    }
    else
    {
        // TODO: use analog stick data from first stick
    }

    // only transmit if anything has changed
    if (gpio_get_level(cts_pin) && (data_byte1 != old_byte1 || data_byte2 != old_byte2 || data_byte3 != old_byte3))
    {
        uart_write_bytes(uart_port, new char[3] { data_byte1, data_byte2, data_byte3 }, 3);

        old_byte1 = data_byte1;
        old_byte2 = data_byte2;
        old_byte3 = data_byte3;
    }

    return true;
}

void PhilipsCDiManeuveringDevice::WaitForCTS()
{
    while (!gpio_get_level(cts_pin))
    {
        vTaskDelay(1);
    }
}