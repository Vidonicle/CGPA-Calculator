/***********************************
 
  main.c
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

#define COURSE_WEIGHT_BUF 6 // numeric input + '\n' + '\0'
#define MENU_BUF_LEN 16 // 14 chars + '\n' + '\0'
#define SEPERATOR " ===================================\n"

int main(){
   char course_code_buf[COURSE_CODE_BUF_LEN];
   char course_weight_buf[COURSE_WEIGHT_BUF];
   float course_weight = 0.0f;
   char letter_grade_buf[LETTER_GRADE_BUF_LEN];
   char menu_buf[MENU_BUF_LEN];

   coursenode_t *courses = NULL;

   printf("\n =============WELCOME===============\n"
      "  -Welcome to the CGPA Calculator\n"
      "  -Please follow all instructions\n"
      "  -Currently only compatible with the system\n"
      "   used by Carleton University\n"
      "  -V1.0.0\n"
      " =========CGPA CALCULATOR===========\n");
   
   // Menu loop
   while (true) {
      printf("\n  1. Add a course\n"
         "  2. Display Courses and CGPA\n"
         "  3. Exit\n\n"
         "  Enter your selection (1-3): ");

      if(!fgets(menu_buf, sizeof(menu_buf), stdin)) break; // Get choice (1-3)

      if (!strchr(menu_buf, '\n')) {
         fprintf(stderr, SEPERATOR);
         fprintf(stderr, "  Input is too long! Flushing stdin...\n");
         flush_stdin();
         continue;

      }
      menu_buf[strcspn(menu_buf, "\n")] = '\0';

      char *m_endptr;
      long menu_choice = strtol(menu_buf, &m_endptr, 10);

      if (*m_endptr != '\0') {
         fprintf(stderr, SEPERATOR);
         fprintf(stderr, "\n  Invalid input. Please try again\n"
            "  Enter your selection (1-3): ");
         continue;

      }

      if (menu_choice < 1 || menu_choice > 3) {
         fprintf(stderr, SEPERATOR
            "\n  Invalid choice. Please try again.\n"
            "  Enter your selection (1-3): ");
         continue;

      }

      if (menu_choice == 1) {
         // Course Code - Prompt until a valid course code is entered
         while (true) {
            printf(SEPERATOR);
            printf("  Please enter your course code (Ex. SYSC2006): ");

            if(!fgets(course_code_buf, sizeof(course_code_buf), stdin)) break;

            if (!strchr(course_code_buf, '\n')) {
               fprintf(stderr, SEPERATOR
                  "\n  Input is too long! Flushing stdin...\n");
               flush_stdin();
               continue;

            }
            course_code_buf[strcspn(course_code_buf, "\n")] = '\0';

            // Check course code for valid input
            if(!validate_course_code(course_code_buf)) {
               fprintf(stderr, SEPERATOR
                  "  Invalid Course code\n");
               continue;

            }

            if(check_courses(courses, course_code_buf)) {
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

         if(!add_course(&courses, course_code_buf, course_weight, letter_grade_buf)) {
            // clear list with malloc failure and exit
            fprintf(stderr,
               "===================================\n"
               "Fatal error: out of memory while adding course\n"
               "===================================\n");

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
         display_grades(courses);

      } else if (menu_choice == 3) {
         printf("\n  Goodbye!\n\n");
         deconstruct(courses);
         return EXIT_SUCCESS;

      }

   }
   
}