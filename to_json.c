#include <json.h>
#include "workout.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

json_object *workout_info_to_json(struct WorkoutInfo *wi)
{
    json_object *date = json_object_new_string(wi->date);
    json_object *main_group = json_object_new_string(wi->main_group);

    json_object *jwi = json_object_new_object();
    json_object_object_add(jwi, "date", date);
    json_object_object_add(jwi, "main_group", main_group);
    return jwi;
}

json_object *exercise_info_to_json(struct ExerciseInfo *ei)
{
    json_object *name = json_object_new_string(ei->name);
    json_object *group = json_object_new_string(ei->group);

    json_object *jei = json_object_new_object();
    json_object_object_add(jei, "name", name);
    json_object_object_add(jei, "group", group);
    return jei;
}

json_object *exercise_set_to_json(struct ExerciseSet *es)
{
    json_object *weight = json_object_new_double(es->weight);
    json_object *reps = json_object_new_int(es->weight);

    json_object *jes = json_object_new_object();
    json_object_object_add(jes, "weight", weight);
    json_object_object_add(jes, "reps", reps);
    return jes;
}

json_object *exercise_to_json(struct Exercise *e)
{
    json_object *je = json_object_new_object();
    json_object *info = exercise_info_to_json(&e->info);
    json_object_object_add(je, "info", info);

    json_object *jes = json_object_new_array();
    struct ExerciseSet *es = e->sets;
    for(int i = 0; i < e->nb_sets; i++, es++){
        json_object_array_add(jes, exercise_set_to_json(es));
    }
    json_object_object_add(je, "sets", jes);
    return je;
}


json_object *workout_to_json(struct Workout *w)
{
    json_object *jw = json_object_new_object();
    json_object_object_add(jw, "info", workout_info_to_json(&w->info));

    json_object *je = json_object_new_array();
    struct Exercise *e = w->exercises;
    for(int i = 0; i < w->nb_exercises; i++, e++){
        json_object_array_add(je, exercise_to_json(e));
    }
    json_object_object_add(jw, "exercises", je);
    return jw;
}

json_object *workout_history_to_json(struct WorkoutHistory *wh)
{
    json_object *jwh = json_object_new_object();
    if(wh->has_ongoing_workout){
        json_object_object_add(jwh, "ongoing_workout", workout_to_json(&wh->ongoing_workout));
    } else {
        json_object_object_add(jwh, "ongoing_workout", json_object_new_null());
    }

    struct Workout *w = wh->workouts;
    json_object *jw = json_object_new_array();
    for(int i = 0; i < wh->nb_workouts; i++, w++){
        json_object_array_add(jw, workout_to_json(w));
    }
    json_object_object_add(jwh, "workouts", jw);
    return jwh;
}

int save_workout_to_file(const char *filename, struct WorkoutHistory *wh)
{
    json_object *jwh = workout_history_to_json(wh);
    int fd = open(filename, O_CREAT|O_WRONLY|O_TRUNC, 0644);
    json_object_to_fd(fd, jwh, JSON_C_TO_STRING_PRETTY);
    close(fd);
    return 0;
}

