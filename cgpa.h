/***********************************
 
  cgpa.h
  Author: Arul Rao (Vidonicle)
  Date Created: 12/20/2025
  History: N/A
 
 ***********************************/

#ifndef CGPA_H
#define CGPA_H

#include <stdbool.h>

#define COURSE_CODE_BUF_LEN 10  // 8 chars + '\n' + '\0'
#define LETTER_GRADE_BUF_LEN 4  // 2 chars + '\n' + '\0'

typedef struct course {
   char course_code[COURSE_CODE_BUF_LEN];
   char letter_grade[LETTER_GRADE_BUF_LEN];
   float course_weight;
   float credits_earned;
   struct course *next;
} coursenode_t;

typedef struct {
   const char *grade;
   float value;
} grade_map_t;

bool add_course(coursenode_t **courses, const char *course_code, float course_weight, const char *letter_grade);

float earned_credits( float course_weight, const char *letter_grade);

void display_grades(coursenode_t *courses);

bool check_courses(coursenode_t *courses, const char *course_code);

bool validate_course_code(char *course_code);

bool validate_letter_grade (const char *letter_grade);

void deconstruct(coursenode_t *course);

void flush_stdin (void);

#endif