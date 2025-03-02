//include header files for API

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct interval { // struct for time intervals

    float time;
    float speed;

};

struct program {

    struct interval* program;
    int num_intervals;

};

struct program* fetch_intervals() {

    //creating new prog
    struct program* example_program = malloc(sizeof(struct program));

    //setting placeholder values, change with bluetooth connection data
    // example_program->num_intervals = 3;
    // example_program->program = malloc(sizeof(struct interval)*example_program->num_intervals);

    // for (int i = 0; i < example_program->num_intervals / 2; i++) {

    //     example_program->program[i].time = i;
    //     example_program->program[i].speed = i*5;

    // }

    example_program->num_intervals = 6;
    example_program->program = malloc(sizeof(struct interval)*example_program->num_intervals);

    for (int i = 0; i < example_program->num_intervals / 2; i++) {

        example_program->program[i].time = i;
        example_program->program[i].speed = i*5;

    }

    example_program->program[3].time = 4;
    example_program->program[3].speed = 9;

    example_program->program[4].time = 6;
    example_program->program[4].speed = 20;

    example_program->program[5].time = 7;
    example_program->program[5].speed = 5;

    return example_program;


}

int main() {

    //retreive values from bluetooth
    struct program *example_program = fetch_intervals();

    for (int i = 0; i < example_program->num_intervals; i++){

        printf("time: %f\nspeed: %f\n", example_program->program[i].time, example_program->program[i].speed);

    }

    clock_t start;
    float end_val = 0;


    start = clock();
    end_val = (float)example_program->program[example_program->num_intervals - 1].time;
    int curr_interval = 0;
    float elapsed = 0.0;
    float split = 0.0;
    float change = 0.0;

    while(elapsed < end_val) {

        while(elapsed < example_program->program[curr_interval].time) {

            elapsed = ((float)(clock() - start)/1000);
            split = (elapsed - example_program->program[curr_interval - 1].time) / (example_program->program[curr_interval].time - example_program->program[curr_interval - 1].time);
            change = example_program->program[curr_interval].speed - example_program->program[curr_interval - 1].speed;
            
            printf("%f: %f\n", elapsed, (split * change) + example_program->program[curr_interval - 1].speed);


        }

        curr_interval++;

    }


    free(example_program->program);
    free(example_program);

}
