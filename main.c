#include <stdio.h>
#include <json.h>
#include <string.h>
#include "workout.h"
#include "from_json.h"
#include "to_json.h"
#include "modifiers.h"


int main(int argc, char **argv){

    char *home = getenv("HOME");
    if(home == NULL){
        return 1;
    }
    char basename[] = ".workout_data.json";
    size_t len = strlen(home)+1+strlen(basename);
    char filename[len+1];
    snprintf(filename, len+1, "%s/%s", home, basename);

    struct WorkoutHistory wh;
    int err = load_workout_file(filename, &wh);
    if(err){
        fprintf(stderr, "ERROR loading workout file %s\n",filename);
        return 1;
    }
    print_workout_history(&wh);
    free_workout_history(&wh);
    return 0;

    // Causes error since wh already has an ongoing_workout
    if((err = start_workout(&wh, "NOW", "back"))){
        fprintf(stderr, "This error was expected\n");
    }

    if((err = end_workout(&wh))){
        fprintf(stderr, "ERROR: end_workout failed\n");
        return 1;
    }

    if((err = start_workout(&wh, "NOW", "back"))){
        fprintf(stderr, "ERROR: start_workout failed\n");
        return 1;
    }

    struct Workout *w = &wh.ongoing_workout;
    if((err = begin_exercise(w, "ez_bar_row", "back"))){
        fprintf(stderr, "ERROR: begin_exercise\n");
        return 1;
    }

    if((err = enter_set(w, 15, 10))){
        fprintf(stderr, "ERROR: enter_set\n");
        return 1;
    }

    print_workout_history(&wh);

    if((err = save_workout_to_file("output_file.json", &wh))){
        fprintf(stderr, "ERROR: save_workout_to_file\n");
        return 1;
    }

    return 0;
}
