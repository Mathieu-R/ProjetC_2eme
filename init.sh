#!/bin/zsh

echo Lancement du programme de course...

gcc CourseF1.c ResultCourse.c -lm -lpthread -o main
./main
