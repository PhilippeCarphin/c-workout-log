#include "workout.h"
#include <json.h>

int json_to_exercise_set(json_object *jso, struct ExerciseSet *es);
int json_to_exercise_info(json_object *jso, struct ExerciseInfo *ei);
int json_to_workout_info(json_object *jso, struct WorkoutInfo *wi);
int json_to_exercise(json_object *jso, struct Exercise *e);
int json_to_workout(json_object *jso, struct Workout *w);
int json_to_workout_history(json_object *jso, struct WorkoutHistory *wh);
int load_workout_file(const char *filename, struct WorkoutHistory *wh);
