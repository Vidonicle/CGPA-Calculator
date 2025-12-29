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

 // TODO FInish edit courses, formatting of menu is probably finished, check all input stacks

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

   printf( SEPERATOR1
      "\n  Would you like to load courses from file? (Y/n): ");
   fgets(menu_buf, sizeof(menu_buf), stdin);

   if (!strchr(menu_buf, '\n')) {
         fprintf(stderr, SEPERATOR1
            "\n  Input is too long! Flushing stdin...\n"
            SEPERATOR1);
         flush_stdin();
   }
   menu_buf[strcspn(menu_buf, "\n")] = '\0';

   menu_buf[0] = (char)tolower((unsigned char)menu_buf[0]);

   if (menu_buf[0] != 'n') {
      // File parsing loop
      do {

         printf(SEPERATOR1
            "\n  Please enter the name of your file (.txt): ");
         fgets(filename, sizeof(filename), stdin);

         if (!strchr(filename, '\n')) {
            fprintf(stderr, SEPERATOR1
               "\n  Input is too long! Flushing stdin...\n"
               SEPERATOR1);
            flush_stdin();
         }
         filename[strcspn(filename, "\n")] = '\0';
         
         const char *ext = strrchr(filename, '.');

         if (!ext || strcmp(ext, ".txt") != 0) {
            fprintf(stderr, SEPERATOR1
               "\n  Invalid file type\n"
               SEPERATOR1);
            break;  
         }

         FILE *fptr = fopen(filename, "r");\
         
         if (!fptr) {
            fprintf(stderr, SEPERATOR1
               "\n  File not found\n"
               SEPERATOR1);
            break;

         }

         if (!load_from_file(&courses, fptr)) {
            // clear list with malloc failure and exit
            fprintf(stderr, SEPERATOR1
               "  Fatal error: out of memory while adding course\n"
               SEPERATOR1);

               deconstruct(courses);
               return EXIT_FAILURE;
         } else {
            printf(SEPERATOR1
               "\n  Load from file successful!\n"
               SEPERATOR1);
               fclose(fptr);
         }

      } while (0);
   }
   
   // Menu loop
   while (true) {

      print_menu();

      if (!fgets(menu_buf, sizeof(menu_buf), stdin)) break; // Get choice (1-MENU_COUNT)

      if (!strchr(menu_buf, '\n')) {
         fprintf(stderr, SEPERATOR1
            "\n  Input is too long! Flushing stdin...\n"
            SEPERATOR1);
         flush_stdin();
         continue;
      }
      menu_buf[strcspn(menu_buf, "\n")] = '\0';

      char *m_endptr;
      long menu_choice = strtol(menu_buf, &m_endptr, 10);

      if (*m_endptr != '\0') {
         fprintf(stderr, SEPERATOR1
            "\n  Invalid choice. Please try again.\n"
            SEPERATOR1);
         continue;
      }

      if (menu_choice < 1 || menu_choice > MENU_COUNT) {
         fprintf(stderr, SEPERATOR1
            "\n  Invalid choice. Please try again.\n"
            SEPERATOR1);
         continue;
      }

      if (menu_choice == 1) {
         // Course Code
         do {

            printf(SEPERATOR1
               "  Please enter your course code (Ex. SYSC2006): ");

            if (!fgets(course_code_buf, sizeof(course_code_buf), stdin)) break;

            if (!strchr(course_code_buf, '\n')) {
               fprintf(stderr, SEPERATOR1
                  "\n  Input is too long! Flushing stdin...\n"
                  SEPERATOR1);
               flush_stdin();
               break;
            }
            course_code_buf[strcspn(course_code_buf, "\n")] = '\0';

            if (!validate_course_code(course_code_buf)) {
               fprintf(stderr, SEPERATOR1
                  "\n  Invalid Course code\n"
                  SEPERATOR1);
               break;
            }

            if (check_courses(courses, course_code_buf)) {
               fprintf(stderr, SEPERATOR1
                  "\n  Course already exists\n"
                  SEPERATOR1);
               break;
            }
         
         // Course Weight
            printf(SEPERATOR2
               "  Please enter your course weight (1.00, 0.50, 0.25): ");

            if (!fgets(course_weight_buf, sizeof course_weight_buf, stdin)) break;

            if (!strchr(course_weight_buf, '\n')) {
               fprintf(stderr, SEPERATOR1
                  "\n  Input is too long! Flushing stdin...\n"
                  SEPERATOR1);
               flush_stdin();
               break;
            }
            course_weight_buf[strcspn(course_weight_buf, "\n")] = '\0';

            char *cw_endptr;
            course_weight = strtof(course_weight_buf, &cw_endptr);

            if (*cw_endptr != '\0' || !(course_weight == 1.0f || course_weight == 0.5f || course_weight == 0.25f)) {
               fprintf(stderr, SEPERATOR1
                  "\n  Invalid input. Please try again\n"
                  SEPERATOR1);
               break;
            } 
            
         // Letter Grade
            printf(SEPERATOR2
               "  Please enter your letter grade ((A, B, C, D)+/- or F): ");

            if (!fgets(letter_grade_buf, sizeof letter_grade_buf, stdin)) break;

            if (!strchr(letter_grade_buf, '\n')) {
               fprintf(stderr, SEPERATOR1
                  "\n  Input is too long! Flushing stdin...\n"
                  SEPERATOR1);
               flush_stdin();
               break;
            }
            letter_grade_buf[strcspn(letter_grade_buf, "\n")] = '\0';
            letter_grade_buf[0] = (char)toupper((unsigned char)letter_grade_buf[0]);

            if (!validate_letter_grade(letter_grade_buf)) {
               fprintf(stderr, SEPERATOR1
                  "\n  Invalid Letter grade\n"
                  SEPERATOR1);
               break; 
            }

            if (!add_course(&courses, course_code_buf, course_weight, letter_grade_buf)) {
               // clear list with malloc failure and exit
               fprintf(stderr, SEPERATOR1
                  "  Fatal error! out of memory while adding course\n"
                  SEPERATOR1);

                  deconstruct(courses);
                  return EXIT_FAILURE;
            } else {
               printf(SEPERATOR2
                  "  Course Added!\n"
                  "  -Course Code: %s\n"
                  "  -Course Weight: %4.2f\n"
                  "  -Letter Grade: %s\n"
                  SEPERATOR2,
               course_code_buf, course_weight, letter_grade_buf);
            }
         } while (0);
      } else if (menu_choice == 2) {
         if (!courses) {
            fprintf(stderr, SEPERATOR1
               "\n  No courses exist\n"
               SEPERATOR1);
               continue;
         }

         printf(SEPERATOR1
            "  Please enter the course code for the course you want to delete (Ex. SYSC2006): ");

         fgets(course_code_buf, sizeof(course_code_buf), stdin);

         if (!strchr(course_code_buf, '\n')) {
               fprintf(stderr, SEPERATOR1
                  "\n  Input is too long! Flushing stdin...\n"
                  SEPERATOR1);
               flush_stdin();
         }
         course_code_buf[strcspn(course_code_buf, "\n")] = '\0';

         if (!validate_course_code(course_code_buf)) {
            fprintf(stderr, SEPERATOR1
               "\n  Invalid course code format\n"
               SEPERATOR1);
         } else {

            if (!check_courses(courses, course_code_buf)) {
               fprintf(stderr, SEPERATOR1
                  "\n  Course not found\n"
                  SEPERATOR1);
            } else {
               delete_course(&courses, course_code_buf);
               printf(SEPERATOR1
                  "\n  Course successfully deleted\n"
                  SEPERATOR1);
            }
         }
      } else if (menu_choice == 3) {
         do {
            // Get Old course code
            printf(SEPERATOR1
            "  Please enter the course code for the course you want to edit (Ex. SYSC2006): ");

            if (!fgets(course_code_buf, sizeof(course_code_buf), stdin))
            break;

            if (!strchr(course_code_buf, '\n')) {
            fprintf(stderr, SEPERATOR1
            "\n  Input is too long! Flushing stdin...\n"
            SEPERATOR1);
            flush_stdin();
            break;
            }

            course_code_buf[strcspn(course_code_buf, "\n")] = '\0';

            if (!validate_course_code(course_code_buf)) {
            fprintf(stderr, SEPERATOR1
            "\n  Invalid course code format\n"
            SEPERATOR1);
            break;
            }

            if (!check_courses(courses, course_code_buf)) {
            fprintf(stderr, SEPERATOR1
            "  Course not found\n"
            SEPERATOR1);
            break;
            }

            char course_code_old[COURSE_CODE_BUF_LEN];
            strcpy(course_code_old, course_code_buf);

            float old_weight = fetch_node(courses, course_code_buf)->course_weight;
            char old_grade[LETTER_GRADE_BUF_LEN];
            strcpy(old_grade, fetch_node(courses, course_code_buf)->letter_grade);

            // Get new course code
            printf(SEPERATOR1
               "  Enter new course code (leave blank for no changes): ");

            if (!fgets(course_code_buf, sizeof(course_code_buf), stdin))
            break;

            if (!strchr(course_code_buf, '\n')) {
               fprintf(stderr, SEPERATOR1
                  "\n  Input is too long! Flushing stdin...\n"
                  SEPERATOR1);
               flush_stdin();
               break;
            }
            course_code_buf[strcspn(course_code_buf, "\n")] = '\0';

            char course_code_new[COURSE_CODE_BUF_LEN];
            if (course_code_buf[0] == '\0') {
            strcpy(course_code_new, course_code_old);
            } else {
               if (!validate_course_code(course_code_buf)) {
                  fprintf(stderr, SEPERATOR1
                     "  Invalid new course code\n"
                     SEPERATOR1);
                  break;
               }
               strcpy(course_code_new, course_code_buf);
            }

            // Get course weight
            printf(SEPERATOR1
               "  Enter new course weight (leave blank for no changes): ");

            if (!fgets(course_weight_buf, sizeof(course_weight_buf), stdin))
            break;

            if (!strchr(course_weight_buf, '\n')) {
               fprintf(stderr, SEPERATOR1
                  "\n  Input is too long! Flushing stdin...\n"
                  SEPERATOR1);
               flush_stdin();
               break;
            }
            course_weight_buf[strcspn(course_weight_buf, "\n")] = '\0';

            float course_weight_new = old_weight;

            if (course_weight_buf[0] != '\0') {
               char *cwn_endptr;
               float tmp = strtof(course_weight_buf, &cwn_endptr);

               if (*cwn_endptr != '\0' || !(tmp == 1.0f || tmp == 0.5f || tmp == 0.25f)) {
                  fprintf(stderr, SEPERATOR1
                     "  Invalid course weight\n"
                     SEPERATOR1);
                  break;
               }

               course_weight_new = tmp;
            }

            printf(SEPERATOR1
            "  Enter new letter grade (leave blank for no changes): ");

            if (!fgets(letter_grade_buf, sizeof(letter_grade_buf), stdin)) break;

            if (!strchr(letter_grade_buf, '\n')) {
               fprintf(stderr, SEPERATOR1
                  "\n  Input is too long! Flushing stdin...\n"
                  SEPERATOR1);
               flush_stdin();
               break;
            }
            letter_grade_buf[strcspn(letter_grade_buf, "\n")] = '\0';

            char letter_grade_new[LETTER_GRADE_BUF_LEN];
            if (letter_grade_buf[0] == '\0') {
            strcpy(letter_grade_new, old_grade);
            } else {
               letter_grade_buf[0] = (char)toupper((unsigned char)letter_grade_buf[0]);

               if (!validate_letter_grade(letter_grade_buf)) {
                  fprintf(stderr, SEPERATOR1
                     "  Invalid letter grade\n"
                     SEPERATOR1);
                  break;
               }
               strcpy(letter_grade_new, letter_grade_buf);
            }

            // Finalize edit
            if (!edit_course(&courses, course_code_old, course_code_new, course_weight_new, letter_grade_new)) {
               fprintf(stderr, SEPERATOR1
                  "  Fatal error: out of memory\n"
                  SEPERATOR1);
                  deconstruct(courses);
                  return EXIT_FAILURE;
            }

            printf(SEPERATOR1
               "  Course successfully edited\n"
               SEPERATOR1);
         } while (0);
      } else if (menu_choice == MENU_DISPLAY) {
         display_grades(courses);
      } else if (menu_choice == MENU_COUNT) {
         printf("\n  Goodbye!\n");
         deconstruct(courses);
         return EXIT_SUCCESS;
      }
   }
}