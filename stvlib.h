/* Copyright (C) 2022 Ojacomarket.
   This file is not a part of the GNU C Library, but goes under GPL license.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/*
 *	ISO C99 Standard: 7.20 General utilities	<stvlib.h>
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define CITIZENS 10 // place here a number of citizens in your country (since everybody could attend ballot)
#define POLITICS 101 // place here a max number of politics, that can obtain mandates in your country

/**
* Check string to be applicable for STV voting system
* @param array [char[]]: element from a buffer (stored as a sequence of ASCII symbols)
* @param size [int]: how many ASCII symbols are found inside "array" sequence
* @param votes_given [int]: how many votes have been already made
* @return [-1]: if no input (Enter), [0]: if data is invalid, [1]: if data is valid
*/
int check_data_validity(const char *buffer, int size, int votes_given);

/**
 * Create two tables: one that will store how many people have chosen each candidate and another - points for each choice
 * @param buffer [char[][]]: 2D array that store all user input data
 * @param votes_per_candidate [int[]]: array, that will store amount of votes for each candidate
 * @param score_per_vote [int[]]: array, that will store amount of points for each candidate (each vote has different points)
 * @param votes [int]: how many votes been made by people in total
 * @param candidates [int]: how many ballot candidates we have
 */
void
create_vote_tables_sort_desc(char buffer[CITIZENS][POLITICS * 3], char candidate_results[POLITICS * 3],
                             int votes_per_candidate[POLITICS],
                             int score_per_vote[POLITICS],
                             int votes, int candidates);

/**
 * Parse data from a terminal input and calculate Droop quota
 * @param buffer [char[][]]: store user input (each input as single sequence of ASCII symbols)
 * @param total_candidates [int*]: store amount of voting candidates
 * @param mandates_given [int*]: store amount of mandates
 * @param total_votes [int*]: store amount of votes, that people made for candidates
 * @return [int]: Droop quota
 */
int parse_data_from_console(char buffer[CITIZENS][POLITICS * 3], int *total_candidates, int *mandates_given,
                            int *total_votes);

/**
 * Find maximum value inside an array of integers
 * @param array [int[]]: custom array of integers
 * @param size [int[]]: size of the array
 * @return [int]: index of maximum value found inside the array
 */
int find_max(const int *array, int size);

/**
 * Copy one integer array values into another integer array
 * @param src [int[]]: integer array, where we take values from (copy from)
 * @param dest [int[]]: integer array, where we put taken values (put into)
 * @param size [int]: size of both arrays should be the same
 */
void copy_int_array(const int *src, int *dest, int size);

/**
 * Sort two integer and one char arrays respectively in descending order
 * @param candidate_results [char[]]: array of candidate names (1 ASCII symbol per name)
 * @param array1 [int[]]: array, that represent votes per candidate
 * @param array2 [int[]]: array, that represent score per single vote
 * @param size [int]: size of integer array (both "array1" and "array2" should be equal in size)
 */
void parallel_sort(char candidate_results[POLITICS * 3], int *array1, int *array2, int size);

/**
 * Leave only those candidates, which passed Droop quota and mandates are yet available
 * @param votes_per_candidate [int[]]: integer array of votes per single candidate
 * @param mandates_given [int]: how many available mandates we have
 * @param candidates [int]: total amount of available for voting candidates
 * @param droop [int]: Droop quota
 */
void assign_mandates(int votes_per_candidate[POLITICS], int mandates_given, int candidates, int droop);

/**
 * Greet user with a message and program rules
 */
void greeting();

/**
 * Find duplicates inside a string (enhanced version of strchr() of <string.h> library)
 * @param array [char[]]: sequence of ASCII symbols
 * @param size [int]: how many ASCII symbols are inside that sequence
 * @return [0]: if duplicates were found, [1]: if no duplicates were found
 */
int find_duplicates_in_string(const char *array, int size);

/**
 * Search an ASCII symbol from one string inside another
 * @param search_from [char[]]: we take one by one ASCII symbols from this sequence
 * @param sample [char[]]: we try to find matching ASCII symbols from "search_from" and "sample"
 * @param size [int]: size of "search_from" string
 * @return [int]: how many characters were found
 */
int find_absence(const char *search_from, const char *sample, int size, int votes_per_ballot);

/**
 * Initialize array of strings with custom ASCII symbol
 * @param array_2D [char[][]]: array of strings that wish to be initialized with custom symbol
 * @param size [int]: size of "array_2D"
 * @param symbol [int]: ASCII symbol to fill any element of "array_2D"
 */
void init_2d_char_array(char array_2D[CITIZENS][POLITICS * 3], int size, int symbol);

/**
 * Initialize string with custom ASCII symbol
 * @param array [char[]]: string that wish to be initialized with custom symbol
 * @param size [int]: size of that string
 * @param symbol [int]: ASCII symbol to fill "array"
 */
void init_char_array(char *array, int size, int symbol);

/**
 * Fancy output into terminal as summary of STV voting
 * @param total_candidates [int]: total amount of candidates available
 * @param available_mandates [int]: how many mandates are given
 * @param votes_per_candidate [int[]]: array of votes per candidate
 * @param voting_results_candidates_hierarchy [char[]]: hierarchied candidates (ASCII symbols) as a result of voting
 */
void print_fancy(int total_candidates, int available_mandates, const int *votes_per_candidate,
                 const int *votes_per_candidate_final,
                 const int *score_per_vote, char *voting_results_candidates_hierarchy, int droop);