#include "to_json.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>

#ifndef NDEBUG
#define CHECK_PROP(p,name) do {if(p == NULL) { fprintf(stderr, "%s(): Object is missing property '%s'\n", __func__, name); return 1; }}while(0)

#define MUST(code) do{ int err; if(( err = code )) {fprintf(stderr, "%s(): code '%s' failed\n", __func__, #code); return 1;} }while(0)
#else
#define CHECK_PROP(p,name) do {if(p == NULL) {  return 1; }}while(0)
#define MUST(code) do{ int err; if(( err = code )) { return 1;} }while(0)
#endif

int json_to_exercise_set(json_object *jso, struct ExerciseSet *es){
    PRINT_OBJ(jso);
    json_object *jweight = json_object_object_get(jso, "weight");
    if(jweight == NULL){
        return 1;
    }

    es->weight = json_object_get_double(jweight);

    json_object *jreps = json_object_object_get(jso, "reps");
    if(jreps == NULL){
        return 1;
    }
    es->reps = json_object_get_int(jreps);

    return 0;
}


int json_to_exercise_info(json_object *jso, struct ExerciseInfo *ei){
    PRINT_OBJ(jso);

    json_object *jname = json_object_object_get(jso, "name");
    CHECK_PROP(jname, "name");
    ei->name = strdup(json_object_get_string(jname));

    json_object *jgroup = json_object_object_get(jso, "group");
    CHECK_PROP(jgroup, "group");
    ei->group = strdup(json_object_get_string(jgroup));
    return 0;
}

int json_to_workout_info(json_object *jso, struct WorkoutInfo *wi){
    PRINT_OBJ(jso);
    json_object *jdate = json_object_object_get(jso, "date");
    CHECK_PROP(jdate, "date");
    wi->date = strdup(json_object_get_string(jdate));
    json_object *jmain_group = json_object_object_get(jso, "main_group");
    CHECK_PROP(jmain_group, "main_group");
    wi->main_group = strdup(json_object_get_string(jmain_group));
    return 0;
}


int json_to_exercise(json_object *jso, struct Exercise *e){
    PRINT_OBJ(jso);
    MUST(json_to_exercise_info(json_object_object_get(jso, "info"), &e->info));

    json_object *json_sets = json_object_object_get(jso, "sets");
    CHECK_PROP(json_sets, "sets");

    int nb_sets = json_object_array_length(json_sets);
    e->sets = malloc(nb_sets * sizeof(*e->sets));
    e->cap_sets = nb_sets;
    e->nb_sets = nb_sets;
    struct ExerciseSet *es = e->sets;
    for(int i = 0; i < nb_sets; i++, es++){
         MUST(json_to_exercise_set(json_object_array_get_idx(json_sets, i), es));
    }
    return 0;
}

int json_to_workout(json_object *jso, struct Workout *w){
    PRINT_OBJ(jso);

    MUST(json_to_workout_info(json_object_object_get(jso, "info"), &w->info));
    json_object *json_exercises = json_object_object_get(jso, "exercises");
    CHECK_PROP(json_exercises, "exercises");

    int nb_ex = json_object_array_length(json_exercises);
    w->exercises = malloc(nb_ex * sizeof(*w->exercises));
    w->nb_exercises = nb_ex;
    w->cap_exercises = nb_ex;

    struct Exercise *e = w->exercises;
    for(int i = 0; i < nb_ex; i++, e++){
        MUST(json_to_exercise(json_object_array_get_idx(json_exercises, i), e));
    }
    return 0;
}

int json_to_workout_history(json_object *jso, struct WorkoutHistory *wh){

    json_object *jongoing = json_object_object_get(jso, "ongoing_workout");
    wh->has_ongoing_workout = (jongoing != NULL);
    if(jongoing){
        MUST(json_to_workout(jongoing, &wh->ongoing_workout));
    }

    json_object *jworkouts = json_object_object_get(jso, "workouts");
    int nb_workouts = json_object_array_length(jworkouts);
    wh->workouts = malloc(nb_workouts * sizeof(*wh->workouts));
    wh->nb_workouts = nb_workouts;
    wh->cap_workouts = nb_workouts;
    struct Workout *w = wh->workouts;
    for(int i = 0; i < nb_workouts; i++, w++){
        MUST(json_to_workout(json_object_array_get_idx(jworkouts, i), w));
    }
    return 0;
}

int load_workout_file(const char *filename, struct WorkoutHistory *wh){

    int retval = 1;
    int fd = open(filename, O_RDONLY);
    if(fd < 0){
        // fprintf(stderr, "Could not open file %s for reading");
        perror(__func__);
        retval = 1;
        goto out;
    }

    json_object *jso = json_object_from_fd(fd);
    if(jso == NULL){
        fprintf(stderr, "%s: Could not parse json file\n", __func__);
        retval = 1;
        goto out_close;
    }

    retval = json_to_workout_history(jso, wh);

out_close:
    close(fd);
out:
    return retval;
}


