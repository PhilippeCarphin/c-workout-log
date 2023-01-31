#include <stdlib.h>
#include "workout.h"
#include "modifiers.h"
#include <stdio.h>
#include <string.h>

void free_workout(struct Workout *w){
    free(w->info.date);
    free(w->info.main_group);
    struct Exercise *e = w->exercises;
    for(int i = 0; i < w->nb_exercises; i++, e++){
        free_exercise(e);
    }
    free(w->exercises);
    w->nb_exercises = 0;
    w->cap_exercises = 0;
}

void free_workout_history(struct WorkoutHistory *wh){
    if(wh->has_ongoing_workout){
        free_workout(&wh->ongoing_workout);
    }
    struct Workout *w = wh->workouts;
    for(int i = 0; i < wh->nb_workouts; i++, w++){
        free_workout(w);
    }
    wh->nb_workouts = 0;
    wh->cap_workouts = 0;
}

int end_workout(struct WorkoutHistory *wh){
    if( wh->cap_workouts <= wh->nb_workouts){
        // Grow the array
        wh->cap_workouts += 10;
        wh->workouts = realloc(wh->workouts, (wh->cap_workouts) * sizeof(*wh->workouts));
    }
    /*
     * ongoing_workout is *moved* into wh->workouts[wh->nb_workouts++]:
     * The pointers wh->ongoing_workout.{info.date,info.main_group,exercise}
     * are copied by the following assignment.
     */
    wh->workouts[wh->nb_workouts++] = wh->ongoing_workout;

    wh->ongoing_workout.info.date = NULL;
    wh->ongoing_workout.info.main_group = NULL;
    wh->ongoing_workout.exercises = NULL;
    wh->ongoing_workout.nb_exercises = 0;
    wh->ongoing_workout.cap_exercises = 0;
    wh->has_ongoing_workout = 0;

    return 0;
}

int init_workout(struct Workout *w, char *date, char *main_group, int cap_ex){
    w->info.date = strdup(date);
    w->info.main_group = strdup(main_group);
    w->exercises = malloc(cap_ex * sizeof(*w->exercises));
    w->nb_exercises = 0;
    w->cap_exercises = cap_ex;
    return 0;
}

int start_workout(struct WorkoutHistory *wh, char *date, char *main_group){
    if(wh->has_ongoing_workout){
        fprintf(stderr, "ERROR there is already an ongoing workout\n");
        return 1;
    }
    init_workout(&wh->ongoing_workout, date, main_group, 10);
    wh->has_ongoing_workout = 1;
    return 0;
}

void free_exercise(struct Exercise *e){
    free(e->info.name);
    free(e->info.group);
    free(e->sets);
    e->nb_sets=0;
    e->cap_sets=0;
}

int print_workout(struct Workout *w, char *indent){
    for(int e = 0; e < w->nb_exercises; e++){
        struct Exercise exc = w->exercises[e];
        fprintf(stderr, "%sExercise: %s\n%s%s", indent, exc.info.name, indent, indent);
        for(int s = 0; s < w->exercises[e].nb_sets; s++){
            struct ExerciseSet set = exc.sets[s];
            fprintf(stderr, "%.1fx%d; ", set.weight, set.reps);
        }
        fprintf(stderr, "\n");
    }
    return 0;
}

void print_workout_history(struct WorkoutHistory *wh){
    if(wh->has_ongoing_workout){
        fprintf(stderr, "Ongoing workout\n");
        print_workout(&wh->ongoing_workout, "    ");
    }

    for(int i = 0; i < wh->nb_workouts; i++){
        struct Workout w = wh->workouts[i];
        fprintf(stderr, "Workout done on %s, main_group: %s\n", w.info.date, w.info.main_group);
        print_workout(&w, "    ");
    }
}

int begin_exercise(struct Workout *w, char *name, char *group){
    if(w->nb_exercises == w->cap_exercises){
        // GROW ARRAY
        w->cap_exercises += 10;
        w->exercises = realloc(w->exercises, w->cap_exercises);
    }
    struct Exercise *e = &w->exercises[w->nb_exercises++];
    e->info.name = strdup(name);
    e->info.group = strdup(group);
    e->nb_sets = 0;
    e->cap_sets = 10;
    e->sets = malloc(e->cap_sets * sizeof(*e->sets));
    return 0;
}

int exercise_add_set(struct Exercise *e, float weight, int reps){
    if(e->cap_sets == e->nb_sets){
        e->cap_sets += 10;
        e->sets = realloc(e->sets, e->cap_sets);
    }
    struct ExerciseSet es = {.weight=weight, .reps=reps};
    e->sets[e->nb_sets++] = es;
    return 0;
}

int enter_set(struct Workout *w, float weight, int reps){
    if(w->nb_exercises == 0){
        fprintf(stderr, "%s(): ERROR: no current exercise\n", __func__);
        return 1;
    }
    struct Exercise *current_exercise = &w->exercises[w->nb_exercises-1];
    exercise_add_set(current_exercise, weight, reps);
    return 0;
}
