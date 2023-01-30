#include <json.h>
#include "workout.h"

json_object *workout_info_to_json(struct WorkoutInfo *wi);
json_object *exercise_info_to_json(struct ExerciseInfo *ei);
json_object *exercise_set_to_json(struct ExerciseSet *es);
json_object *exercise_to_json(struct Exercise *e);
json_object *workout_to_json(struct Workout *w);
json_object *workout_history_to_json(struct WorkoutHistory *wh);
int save_workout_to_file(const char *filename, struct WorkoutHistory *wh);
