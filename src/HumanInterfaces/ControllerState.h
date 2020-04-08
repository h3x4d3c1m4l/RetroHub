struct ControllerState {
    bool action1 = false, action2 = false, action3 = false, action4 = false;

    int stick1_x = 0, stick1_y = 0;
    int n_sticks = 0;

    bool dpad_left = false, dpad_up = false, dpad_right = false, dpad_down = false;
};