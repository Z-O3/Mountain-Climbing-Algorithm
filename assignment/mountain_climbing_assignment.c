#include "gradient.h"

// helper function to make sure jump is correctly aligned with whether the direction of x or y is negative or positive
int direction(int step) {
    if (step > 0) return 1;
    if (step < 0) return -1;
    return 0;
}

// helper function to check valid coordinates in a given view
int hasValidCoordinates(float view[VIEW_SIZE][VIEW_SIZE]) {
    int is_valid = 0;

    for (int i = 0; i < VIEW_SIZE; i++) {
        for (int j = 0; j < VIEW_SIZE; j++) {
            if (view[i][j] != -1) {
                is_valid = 1;
                break;
            }
        }
    }
    return is_valid;
}

// helper fucntion to check how many times a value occurs in a given view
int countSame(float view[VIEW_SIZE][VIEW_SIZE], float value) {
    int count = 0;

    for (int i = 0; i < VIEW_SIZE; i++) {
        for (int j = 0; j < VIEW_SIZE; j++) {
            if (view[i][j] == value) {
                count++;
            }
        }
    }
    return count;
}

path_point find_highest_point() {
    float view[VIEW_SIZE][VIEW_SIZE];
    float test_view[VIEW_SIZE][VIEW_SIZE];

    int x = 75;
    int y = 75;

    int max_jump = 1.5 * VIEW_RADIUS;
    int jump = max_jump;

    int prev_direction_x = 0;
    int prev_direction_y = 0;

    while (1) {
        generate_view(view, y, x);

        int best_i = 0;
        int best_j = 0;
        float best = -1;

        // finding max in view
        for (int i = 0; i < VIEW_SIZE; i++){
            for (int j = 0; j < VIEW_SIZE; j++){
                if (view[i][j] > best){
                    best = view[i][j];
                    best_i = i;
                    best_j = j;
                }
            }
        }

        // the earlier check
        int is_potential_peak = 1;

        if ((best_i > 1 && best_i < VIEW_SIZE - 2) && (best_j > 1 && best_j < VIEW_SIZE - 2)) {
            for (int test_i = -1; test_i <= 1; test_i++) {
                for (int test_j = -1; test_j <= 1; test_j++) {
                    if (test_i == 0 && test_j == 0) continue;

                    if (view[best_i + test_i][best_j + test_j] >= view[best_i][best_j]) {
                        is_potential_peak = 0;
                        break;
                    }
                }
                if (is_potential_peak == 0) break;
            }

            int count_same = countSame(view, view[best_i][best_j]);

            if (is_potential_peak == 1 && count_same == 1) {
                int potential_x = x + (best_j - VIEW_RADIUS);
                int potential_y = y + (best_i - VIEW_RADIUS);

                if (declare_peak(potential_x, potential_y)) {
                    // printf("At: %d, %d, peak val: %0.2f | ", x, y, view[best_i][best_j]);
                    path_point peak;
                    peak.x = potential_x;
                    peak.y = potential_y;
                    return peak;
                }
            }
        }

        // distance from center (VIEW_RADIUS)
        int x_step = best_j - VIEW_RADIUS;
        int y_step = best_i - VIEW_RADIUS;

        // new x and y 
        int new_x = x + x_step;
        int new_y = y + y_step;

        // If no movement (still at center) maybe possibly on plateau or potential peak
        if (new_x == x && new_y == y) {

            // likely not needed anymore due to earlier check
            // --------------------------------------------------------------------
            float current_val = view[VIEW_RADIUS][VIEW_RADIUS];
            int count_same = countSame(view, current_val);

            // only try declare_peak if only 1 instance of this value exists
            if (count_same == 1) {
                if (declare_peak(x, y)) {
                    // printf("At: %d, %d, peak val: %0.2f | ", x, y, current_val);
                    path_point peak;
                    peak.x = x;
                    peak.y = y;
                    return peak;
                }
            }
            // -------------------------------------------------------------------- 

            int rand_jump_lim = 2 * VIEW_SIZE;
            int jump_valid = 0;

            // attempting to continue forward with the decaying jump
            if ((prev_direction_x != 0 || prev_direction_y != 0) && jump > 0) {
                int test_x = x + (prev_direction_x * jump);
                int test_y = y + (prev_direction_y * jump);

                generate_view(test_view, test_y, test_x);

                if (hasValidCoordinates(test_view) == 1) {
                    new_x = test_x;
                    new_y = test_y;
                    jump_valid = 1;
                }
            }

            // resorting to random jumps again if directional jump fails
            if (jump_valid == 0) {
                do {
                    new_x = x + ((rand() % (2 * rand_jump_lim)) - rand_jump_lim) + (prev_direction_x * 5);
                    new_y = y + ((rand() % (2 * rand_jump_lim)) - rand_jump_lim) + (prev_direction_y * 5);

                    generate_view(test_view, new_y, new_x);

                } while (hasValidCoordinates(test_view) == 0);
            }
            jump = max_jump;
        } else {
            prev_direction_x = direction(x_step);
            prev_direction_y = direction(y_step);

            new_x += (prev_direction_x * jump);
            new_y += (prev_direction_y * jump);

            if (jump > 0) {
                jump = jump / 2;
            }

            if (jump < 1) {
                jump = 0;
            }
        }

        // movement
        x = new_x;
        y = new_y;
    }
}