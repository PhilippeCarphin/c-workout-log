#ifndef _WORKOUT_H
#define _WORKOUT_H

#ifndef NDEBUG
#   define PRINT_OBJ(o) do{fprintf(stderr, "%s() handling object '%s'\n", __func__, json_object_to_json_string(o));}while(0)
#else
#   define PRINT_OBJ(o) do{}while(0)
#endif

struct WorkoutInfo {
    char *date;
    char *main_group;
};

struct Workout {
    struct WorkoutInfo info;
    struct Exercise *exercises;
    int nb_exercises;
    int cap_exercises;
};

struct WorkoutHistory {
    struct Workout *workouts;
    struct Workout ongoing_workout;
    int nb_workouts;
    int cap_workouts;
    int has_ongoing_workout;
};

struct ExerciseInfo {
    char *name;
    char *group;
};

struct Exercise {
    struct ExerciseInfo info;
    struct ExerciseSet *sets;
    int nb_sets;
    int cap_sets;
};

struct ExerciseSet {
    float weight;
    int reps;
};

struct WorkoutManager {
    struct WorkoutHistory *wh;
};

#endif
