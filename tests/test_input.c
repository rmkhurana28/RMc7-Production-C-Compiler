#include <stdio.h>

// Test 04: Super nested control structures with arrays

int main() {
    int grid[20];
    double values[10];
    bool flags[5];
    
    int i;
    int j;
    int k;
    
    // Initialize arrays
    for (i = 0; i < 20; i++) {
        grid[i] = i;
    }
    
    for (j = 0; j < 10; j++) {
        values[j] = j * 1.5;
    }
    
    for (k = 0; k < 5; k++) {
        flags[k] = true;
    }
    
    // Triple nested loops
    int sum = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            for (k = 0; k < 2; k++) {
                int idx = i + j + k;
                if (idx < 20) {
                    grid[idx] = grid[idx] + 1;
                    sum = sum + grid[idx];
                }
            }
        }
    }
    
    // Nested if-else with while loops
    if (sum > 50) {
        int counter = 0;
        while (counter < 10) {
            if (values[counter] > 5.0) {
                values[counter] = values[counter] - 1.0;
            } else {
                values[counter] = values[counter] + 2.0;
            }
            counter++;
        }
    } else {
        for (i = 0; i < 5; i++) {
            if (flags[i]) {
                grid[i] = grid[i] * 2;
            } else {
                grid[i] = grid[i] / 2;
            }
        }
    }
    
    // Complex nested conditions
    for (i = 0; i < 20; i++) {
        if (grid[i] > 10) {
            if (grid[i] < 30) {
                grid[i] = grid[i] + 5;
            } else {
                grid[i] = grid[i] - 5;
            }
        } else {
            if (i > 5) {
                grid[i] = grid[i] * 2;
            } else {
                grid[i] = grid[i] + 10;
            }
        }
    }
    
    // Nested while with for
    int outer = 0;
    while (outer < 5) {
        for (i = 0; i < 4; i++) {
            if (i < 5) {
                flags[i] = !flags[i];
            }
        }
        outer++;
    }
}