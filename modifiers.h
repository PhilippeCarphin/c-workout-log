#ifndef _MODIFIERS_H
#define _MODIFIERS_H
#include "workout.h"

void free_exercise(struct Exercise *e);
void free_workout(struct Workout *w);
void free_workout_history(struct WorkoutHistory *wh);
int end_workout(struct WorkoutHistory *wh);
int init_workout(struct Workout *w, char *date, char *main_group, int cap_ex);
int start_workout(struct WorkoutHistory *wh, char *date, char *main_group);
int print_workout(struct Workout *w, char *indent);
void print_workout_history(struct WorkoutHistory *wh);
int begin_exercise(struct Workout *w, char *name, char *group);
int exercise_add_set(struct Exercise *e, float weight, int reps);
int enter_set(struct Workout *w, float weight, int reps);
#endif
