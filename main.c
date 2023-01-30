#include <stdio.h>
#include <json.h>
#include <string.h>
#include "workout.h"
#include "from_json.h"
#include "to_json.h"
#include "modifiers.h"


int main(int argc, char **argv){

    struct WorkoutHistory wh;
    load_workout_file("../test_workout_data.json", &wh);

    // Causes error since wh already has an ongoing_workout
    start_workout(&wh, "NOW", "back");

    end_workout(&wh);
    start_workout(&wh, "NOW", "back");
    struct Workout *w = &wh.ongoing_workout;
    begin_exercise(w, "ez_bar_row", "back");
    enter_set(w, 15, 10);

    print_workout_history(&wh);

    save_workout_to_file("output_file.json", &wh);

    return 0;
}
