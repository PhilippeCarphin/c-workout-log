# C Worktout log

# Build requirements

- Clone https://github.com/json-c/json-c, build and install to a directory
  of your choice.
- Run CMake with `-Djson-c_ROOT=<install prefix of json-c>`

I'm writing a simple workout log thing as a way to learn Rust and because I
eventually want to have it do something that the Duolingo app does that I
really like: It has the concept of a streak and it is surprisingly effective
at making me practice Korean every day.  At the time of this writing, I'm at
80 days and the thought of losing my streak is unbearable.

## Thoughts on C vs Rust

I first wrote the program in Rust because the goal was to use this task as
a way of learning rust.  Serializing and deserializing JSON is trivally easy
in Rust with the `serde` crate:  Writing out my structs took me about half
an hour and it took me another 15 minutes or so to test serializing and
deserializing them to a file. 

It took me the whole rest of the day to get simple stuff to work because I
didn't understand the ownership and reference system of rust.

- Serializing and deserializing JSON into structs
- Managing growing arrays For the JSON part, I used
  https://github.com/json-c/json-c
  as recommended by the stack overflow post
  https://stackoverflow.com/a/6674004/5795941


The comparison with Rust would be that in Rust, the parsing of JSON
is trivial with serde whereas with C, I had to write lots of functions
to convert between `json_object*` and the various structs and vice versa.

However, once I have my structs, it was a lot easier to play with them
in C because I already know C but since I'm a noob in Rust, it took me
a long time to figure out how to do things.

The `end_workout` function which "moves" the ongoing workout to the
end of the workout array is a good example.
In the original program, the presence of an ongoing workout is handled
using the `Option<Workout>`.
```Rust
     struct WorkoutHistory {
         workouts: Vec<Workout>,
         ongoing_workout: Option<Workout>
     }
```
When a workout is complete, we want to put the workout contained in the
option at the end of the workouts array and set the option to None.

At first I was trying
```Rust
impl WorkoutHistory {
    fn end_workout(&mut self){
        match &self.ongoing_workout {
            Some(w) => {
                self.workouts.push(w); // <<< This line is a compilation error
                self.ongoing_workout = None;
            },
            None => { println!("ERROR: end_workout called with no ongoing workout") }
        }
    }
}
```

The problem is that passing 'w' to the self.workouts.push() method takes
ownership of w and w is owned by the option.  After monkey programming
by adding and removing '&' in all kinds of places, I googled "Rust move
out of an option that is a field of a struct" and found what I was looking
for : https://stackoverflow.com/a/52031060/5795941

```Rust
impl WorkoutHistory {
    fn end_workout(&mut self){
        if let Some(cw) = self.ongoing_workout.take() {
            self.workouts.push(cw);
        } else {
            println!("WARNING: WorkoutHistory::end_workout called with no ongoing workout");
        }
    }
}
```
- In Rust, it took me a long time to find a way for the compiler to
  let me do it.  However once it compiled, it was correct.

- In C it was super easy to do but if more tedious and error prone.
  - Check the capacity of the `workouts` field and grow the array
    if necessary.
  - Copy the `ongoing_workout` field into an element of the `workouts` array
    which copies the `ongoing_workout->exersices` pointer.  Now it is simple
    to set `ongoing_workout->exercises = NULL`.  This transfers ownership
    of the array of exercises.
  - Do the same for `ongoing_workout->info.name` and `ongoing_workout->info.group`
  This is super simple since I know exactly what is going on.  However I forgot
  to set the `has_ongoing_workout` field to `0` which caused a segfault when
  attempting to print the workout history.

## Disclaimer

The error handling in this program is very defficient.  I just wanted to find
out how hard it was to parse JSON with C and getting a segfault does a good job
of telling me I did something wrong.
