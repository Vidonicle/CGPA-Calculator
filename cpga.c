/***********************************
 
  cgpa.c
  Author: Arul Rao (Vidonicle)
  Date Created: 12/20/2025
  History: N/A

 ***********************************/

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgpa.h"

#define SEPERATOR " ===================================\n" // Seperator for UI elements

static const grade_map_t grade_map[] = {{"A+", 12.0f}, {"A", 11.0f}, {"A-", 10.0f}, 
                                        {"B+", 9.0f}, {"B", 8.0f}, {"B-", 7.0f}, 
                                        {"C+", 6.0f}, {"C", 5.0f}, {"C-", 4.0f},
                                        {"D+", 3.0f}, {"D", 2.0f}, {"D-", 1.0f}, 
                                        {"F", 0.0f}};
static const size_t GRADE_MAP_LEN = sizeof grade_map / sizeof grade_map[0];

// Add course in alphanumerical order
bool add_course(coursenode_t **courses, const char *course_code, float course_weight, const char *letter_grade)  {

    coursenode_t *new_node = malloc(sizeof(coursenode_t));
    if (!new_node) return false;

    strcpy(new_node->course_code, course_code);
    new_node->course_weight = course_weight;
    strcpy(new_node->letter_grade, letter_grade);
    new_node->credits_earned = earned_credits(course_weight, letter_grade);
    new_node->next = NULL;

    if (*courses == NULL){ 

        *courses = new_node;

        return true;
        
    }

    coursenode_t *prev = NULL;
    coursenode_t *curr = *courses;

    while (curr && strcmp(curr->course_code, course_code) < 0) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        new_node->next = *courses;
        *courses = new_node;

    } else {
        new_node->next = curr;
        prev->next = new_node;

    }

    return true;

}

// Calculate earned credits depending on course weight and letter grade
float earned_credits(float course_weight, const char *letter_grade) {

    for (size_t i = 0; i < GRADE_MAP_LEN; i++) {
        if(strcmp(letter_grade, grade_map[i].grade) == 0) return (grade_map[i].value * course_weight);

    }

    return 0.0f;

}

// Print grades and formats column sections
void display_grades(coursenode_t *courses) {

    float accum_credits = 0.0f;
    float accum_weight = 0.0f;

    printf(SEPERATOR
           "  Course Code  Course Weight  Letter Grade  Credits Earned\n");

    for (coursenode_t *curr = courses; curr; curr = curr->next) {
        int grade_w   = (curr->letter_grade[1] == '\0') ? 11 : 12;
        int credit_w  = (curr->letter_grade[1] == '\0') ? 17 : 16;

        printf("  %-12s %4.2f %*s %*.2f\n",
            curr->course_code,
            curr->course_weight,
            grade_w,  curr->letter_grade,
            credit_w, curr->credits_earned);

        accum_credits += curr->credits_earned;
        accum_weight += curr->course_weight;

    }

    printf("\n\n  Total Credits Earned: %4.2f"
           "\n  Total Credits Completed: %4.2f"
           "\n\n  Current CGPA: %4.2f\n"
           SEPERATOR,
           accum_credits,
           accum_weight,
           accum_weight > 0 ? accum_credits / accum_weight : 0.0f);

}

// Check for existing courses
bool check_courses(coursenode_t *courses, const char *course_code) {

    coursenode_t *curr = courses;

    while (curr) {
        if (strcmp(curr->course_code, course_code) == 0) return true;

        curr = curr->next;
    }

    return false;

}

// Check validation for course code
bool validate_course_code(char *course_code) {

    if (!(strlen(course_code) == 8)){
        return false;

    }

    for (size_t i = 0; i < 4; i++) {
        if (!isalpha(course_code[i])) return false;

        course_code[i] = (char)toupper((unsigned char)course_code[i]);

    }

    for (size_t i = 4; i < 8; i++) {
        if (!isdigit(course_code[i])) return false;

    }

    return true;

}

// Check validation for letter grade
bool validate_letter_grade (const char *letter_grade) {
    
    for (size_t i = 0; i < GRADE_MAP_LEN; i++) {
        if(strcmp(letter_grade, grade_map[i].grade) == 0) return true;

    }

    return false;

}

// Deconstruct and free list
void deconstruct(coursenode_t *courses) {

    while(courses){
        coursenode_t *next = courses->next;
        free(courses);
        courses = next;

    }

}

// Flush input buffer
void flush_stdin (void) {

    int c_flush;
    while ((c_flush = getchar()) != '\n' && c_flush != EOF);

}