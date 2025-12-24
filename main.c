/***********************************
 * main.c
 *
 * CGPA Calculator - program entry point and user interaction 
 *
 * Handles:
 * - menu loop
 * - user input
 * - program flow control
 *
 * Author: Arul Rao (Vidonicle)
 * License: MIT
 ***********************************/

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgpa.h"

#define MENU_BUF_LEN 64 // 62 chars + '\n' + '\0'
#define FILENAME_LEN MENU_BUF_LEN

int main() {
   char course_code_buf[COURSE_CODE_BUF_LEN];
   char course_weight_buf[COURSE_WEIGHT_BUF_LEN];
   float course_weight = 0.0f;
   char letter_grade_buf[LETTER_GRADE_BUF_LEN];
   char filename[FILENAME_LEN]; 
   char menu_buf[MENU_BUF_LEN];

   coursenode_t *courses = NULL;

   printf( "\n" SEPERATOR
      "\n  Would you like to load courses from file? (Y/n): ");
   fgets(menu_buf, sizeof(menu_buf), stdin);

   if (!strchr(menu_buf, '\n')) {
         fprintf(stderr, SEPERATOR
            "\n  Input is too long! Flushing stdin...\n");
         flush_stdin();
   }
   menu_buf[strcspn(menu_buf, "\n")] = '\0';

   menu_buf[0] = (char)tolower((unsigned char)menu_buf[0]);

   if (menu_buf[0] != 'n') {
      // File parsing loop
      while (true) {

         printf("\n" SEPERATOR
            "\n  Please enter the name of your file (.txt): ");
         fgets(filename, sizeof(filename), stdin);

         if (!strchr(filename, '\n')) {
            fprintf(stderr, "\n" SEPERATOR
               "\n  Input is too long! Flushing stdin...\n");
            flush_stdin();
         }
         filename[strcspn(filename, "\n")] = '\0';
         
         const char *ext = strrchr(filename, '.');

         if (!ext || strcmp(ext, ".txt") != 0) {
            fprintf(stderr, "\n" SEPERATOR
               "\n  Invalid file type\n");
            continue;  
         }

         FILE *fptr = fopen(filename, "r");\
         
         if (!fptr) {
            fprintf(stderr, "\n" SEPERATOR
               "\n  File not found\n");
            continue;

         }

         if (!load_from_file(&courses, fptr)) {
            // clear list with malloc failure and exit
            fprintf(stderr, SEPERATOR
               "  Fatal error: out of memory while adding course\n"
               SEPERATOR);

               deconstruct(courses);
               return EXIT_FAILURE;
         } else {
            printf("\n" SEPERATOR
               "\n  Load from file successful!\n");
            break;
         }

      }
   }
   
   // Menu loop
   while (true) {

      print_menu();

      if (!fgets(menu_buf, sizeof(menu_buf), stdin)) break; // Get choice (1-MENU_COUNT)

      if (!strchr(menu_buf, '\n')) {
         fprintf(stderr, SEPERATOR
            "\n  Input is too long! Flushing stdin...\n");
         flush_stdin();
         fprintf(stderr, "  Enter your selection (1-%d): ", MENU_COUNT);
         continue;
      }
      menu_buf[strcspn(menu_buf, "\n")] = '\0';

      char *m_endptr;
      long menu_choice = strtol(menu_buf, &m_endptr, 10);

      if (*m_endptr != '\0') {
         fprintf(stderr, SEPERATOR
            "\n  Invalid input. Please try again\n"
            "  Enter your selection (1-%d): ", MENU_COUNT);
         continue;
      }

      if (menu_choice < 1 || menu_choice > MENU_COUNT) {
         fprintf(stderr, SEPERATOR
            "\n  Invalid choice. Please try again.\n"
            "  Enter your selection (1-%d): ", MENU_COUNT);
         continue;
      }

      if (menu_choice == 1) {
         // Course Code - Prompt until a valid course code is entered
         while (true) {

            printf(SEPERATOR
               "  Please enter your course code (Ex. SYSC2006): ");

            if (!fgets(course_code_buf, sizeof(course_code_buf), stdin)) break;

            if (!strchr(course_code_buf, '\n')) {
               fprintf(stderr, SEPERATOR
                  "\n  Input is too long! Flushing stdin...\n");
               flush_stdin();
               continue;
            }
            course_code_buf[strcspn(course_code_buf, "\n")] = '\0';

            // Check course code for valid input
            if (!validate_course_code(course_code_buf)) {
               fprintf(stderr, SEPERATOR
                  "  Invalid Course code\n");
               continue;
            }

            if (check_courses(courses, course_code_buf)) {
               fprintf(stderr, SEPERATOR
                  "  Course already exists\n");
               continue;
            } else break;
         }

         // Course Weight - Prompt until a valid course weight is entered
         while (true) {

            printf(SEPERATOR
               "  Please enter your course weight (1.00, 0.50, 0.25): ");

            if (!fgets(course_weight_buf, sizeof course_weight_buf, stdin)) break;

            if (!strchr(course_weight_buf, '\n')) {
               fprintf(stderr, SEPERATOR
                  "\n  Input is too long! Flushing stdin...\n");
               flush_stdin();
               continue;
            }
            course_weight_buf[strcspn(course_weight_buf, "\n")] = '\0';

            char *cw_endptr;
            course_weight = strtof(course_weight_buf, &cw_endptr);

            // Check course weight for valid input
            if (*cw_endptr != '\0') {
               fprintf(stderr, SEPERATOR
                  "  Invalid input. Please try again\n");
               continue;
            } 
            
            if (!(course_weight == 1.0f || course_weight == 0.5f || course_weight == 0.25f)) {
               fprintf(stderr, SEPERATOR
                  "  Invalid input. Please try again\n");
                  continue;
            } else break;
         }

         // Letter Grade - Prompt until a valid letter grade is entered
         while (true) {

            printf(SEPERATOR
               "  Please enter your letter grade ((A, B, C, D)+/- or F): ");

            if (!fgets(letter_grade_buf, sizeof letter_grade_buf, stdin)) break;

            if (!strchr(letter_grade_buf, '\n')) {
               fprintf(stderr, SEPERATOR
                  "\n  Input is too long! Flushing stdin...\n");
               flush_stdin();
               continue;
            }
            letter_grade_buf[strcspn(letter_grade_buf, "\n")] = '\0';
            letter_grade_buf[0] = (char)toupper((unsigned char)letter_grade_buf[0]);

            // Check letter grade for valid input
            if (!validate_letter_grade(letter_grade_buf)) {
               fprintf(stderr, SEPERATOR
                  "Invalid Letter grade\n");
               continue; 
            } else break;
         }

         if (!add_course(&courses, course_code_buf, course_weight, letter_grade_buf)) {
            // clear list with malloc failure and exit
            fprintf(stderr, SEPERATOR
               "  Fatal error: out of memory while adding course\n"
               SEPERATOR);

               deconstruct(courses);
               return EXIT_FAILURE;

         } else {
            printf(SEPERATOR
               "  Course Added!\n"
               "  -Course Code: %s\n"
               "  -Course Weight: %4.2f\n"
               "  -Letter Grade: %s\n"
               SEPERATOR,
            course_code_buf, course_weight, letter_grade_buf);
         }
      } else if (menu_choice == 2) {
         if (!courses) {
            fprintf(stderr, "\n" SEPERATOR
               "  No courses exist\n"
               SEPERATOR);
               continue;
         }

         printf(SEPERATOR
            "\n  Please enter the course code for the course you want to delete (Ex. SYSC2006): ");

         fgets(course_code_buf, sizeof(course_code_buf), stdin);

         if (!strchr(course_code_buf, '\n')) {
               fprintf(stderr, "\n" SEPERATOR
                  "\n  Input is too long! Flushing stdin...\n");
               flush_stdin();
               continue;
         }
         course_code_buf[strcspn(course_code_buf, "\n")] = '\0';

         if (!validate_course_code(course_code_buf)) {
            fprintf(stderr, SEPERATOR
               "  Invalid course code format\n"
               SEPERATOR);
            continue;
         }


         if (!delete_course(&courses, course_code_buf)) {
            fprintf(stderr, "\n" SEPERATOR
               "  Course not found\n"
               SEPERATOR);
         } else {
            printf("\n" SEPERATOR
               "  Course successfully deleted\n"
               SEPERATOR);
         }
      } else if (menu_choice == MENU_DISPLAY) {
         display_grades(courses);
      } else if (menu_choice == MENU_COUNT) {
         printf("\n  Goodbye!\n\n");
         deconstruct(courses);
         return EXIT_SUCCESS;
      }
   }
}