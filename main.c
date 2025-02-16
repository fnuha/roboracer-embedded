//include header files for API

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct interval { // struct for time intervals

    int time;
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
    example_program->num_intervals = 3;
    example_program->program = malloc(sizeof(struct interval)*example_program->num_intervals);

    for (int i = 0; i < example_program->num_intervals; i++) {

        example_program->program[i].time = i;
        example_program->program[i].speed = i*5;

    }

    return example_program;


}

int main() {

    //retreive values from bluetooth
    struct program *example_program = fetch_intervals();

    for (int i = 0; i < example_program->num_intervals; i++){

        printf("time: %f\nspeed: %f\n", example_program->program[i].time, example_program->program[i].speed);

    }

    clock_t start;
    int end_val = 0;


    start = clock();
    end_val = example_program->program[example_program->num_intervals - 1].time;

    while((clock() - start) <= end_val) {

        printf("curr speed: %f\n",


    }


    printf("done lol\n");

    free(example_program->program);
    free(example_program);

}
