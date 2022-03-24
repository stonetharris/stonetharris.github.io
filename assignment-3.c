/**
 * Assignment #3
	Stone Harris, U41533031
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

// Limit constants
#define MAX_CITY_NAME_LEN 20
#define MAX_FLIGHTS_PER_CITY 5
#define MAX_DEFAULT_SCHEDULES 50

// Time definitions
#define TIME_MIN 0
#define TIME_MAX ((60 * 24) - 1)
#define TIME_NULL -1

/******************************************************************************
 * Structure and Type definitions                                             *
 ******************************************************************************/
typedef int time_t; // integers used for time values
typedef char city_t[MAX_CITY_NAME_LEN + 1];
; // null terminate fixed length city

// Structure to hold all the information for a single flight
//   A city's schedule has an array of these
struct flight
{
  int time;      // departure time of the flight
  int available; // number of seats currently available on the flight
  int capacity;  // maximum seat capacity of the flight
};

// Structure for an individual flight schedule
// The main data structure of the program is an Array of these structures
// Each structure will be placed on one of two linked lists:
//                free or active
// Initially the active list will be empty and all the schedules
// will be on the free list.  Adding a schedule is finding the first
// free schedule on the free list, removing it from the free list,
// setting its destination city and putting it on the active list
struct flight_schedule
{
  city_t destination;                          // destination city name
  struct flight flights[MAX_FLIGHTS_PER_CITY]; // array of flights to the city
  struct flight_schedule *next;                // link list next pointer
  struct flight_schedule *prev;                // link list prev pointer
};

/******************************************************************************
 * Global / External variables                                                *
 ******************************************************************************/

struct flight_schedule *flight_schedules_free = NULL;
struct flight_schedule *flight_schedules_active = NULL;

/******************************************************************************
 * Function Prototypes                                                        *
 ******************************************************************************/
// Misc utility io functions
int city_read(city_t city);
bool time_get(time_t *time_ptr);
bool flight_capacity_get(int *capacity_ptr);
void print_command_help(void);

// Core functions of the program
void flight_schedule_initialize(struct flight_schedule array[], int n);
struct flight_schedule *flight_schedule_find(city_t city);
struct flight_schedule *flight_schedule_allocate(void);
void flight_schedule_free(struct flight_schedule *fs);
void flight_schedule_add(city_t city);
void flight_schedule_listAll(void);
void flight_schedule_list(city_t city);
void flight_schedule_add_flight(city_t city);
void flight_schedule_remove_flight(city_t city);
void flight_schedule_schedule_seat(city_t city);
void flight_schedule_unschedule_seat(city_t city);
void flight_schedule_remove(city_t city);

void flight_schedule_sort_flights_by_time(struct flight_schedule *fs);
int flight_compare_time(const void *a, const void *b);

int main(int argc, char *argv[])
{
  long n = MAX_DEFAULT_SCHEDULES;
  char command;
  city_t city;

  if (argc > 1)
  {
    // If the program was passed an argument then try and convert the first
    // argument in the a number that will override the default max number
    // of schedule we will support
    char *end;
    n = strtol(argv[1], &end, 10); // CPAMA p 787
    if (n == 0)
    {
      printf("ERROR: Bad number of default max scedules specified.\n");
      exit(EXIT_FAILURE);
    }
  }

  // C99 lets us allocate an array of a fixed length as a local
  // variable.  Since we are doing this in main and a call to main will be
  // active for the entire time of the program's execution this
  // array will be alive for the entire time -- its memory and values
  // will be stable for the entire program execution.
  struct flight_schedule flight_schedules[n];

  // Initialize our global lists of free and active schedules using
  // the elements of the flight_schedules array
  flight_schedule_initialize(flight_schedules, n);

  assert(flight_schedules_free != NULL && flight_schedules_active == NULL);

  // Print the instruction in the beginning
  print_command_help();

  // Command processing loop
  while (scanf(" %c", &command) == 1)
  {
    switch (command)
    {
    case 'A':
      //  Add an active flight schedule for a new city eg "A Toronto\n"
      city_read(city);
      flight_schedule_add(city);

      break;
    case 'L':
      // List all active flight schedules eg. "L\n"
      flight_schedule_listAll();
      break;
    case 'l':
      // List the flights for a particular city eg. "l\n"
      city_read(city);
      flight_schedule_list(city);
      break;
    case 'a':
      // Adds a flight for a particular city "a Toronto\n
      //                                      360 100\n"
      city_read(city);
      flight_schedule_add_flight(city);
      break;
    case 'r':
      // Remove a flight for a particular city "r Toronto\n
      //                                        360\n"
      city_read(city);
      flight_schedule_remove_flight(city);
      break;
    case 's':
      // schedule a seat on a flight for a particular city "s Toronto\n
      //                                                    300\n"
      city_read(city);
      flight_schedule_schedule_seat(city);
      break;
    case 'u':
      // unschedule a seat on a flight for a particular city "u Toronto\n
      //                                                      360\n"
      city_read(city);
      flight_schedule_unschedule_seat(city);
      break;
    case 'R':
      // remove the schedule for a particular city "R Toronto\n"
      city_read(city);
      flight_schedule_remove(city);
      break;
    case 'h':
      print_command_help();
      break;
    case 'q':
      goto done;
    default:
      //printf("");
      printf("Bad command. Use h to see help.\n");
    }
  }
done:
  return EXIT_SUCCESS;
}

/**********************************************************************
 * city_read: Takes in and processes a given city following a command *
 *********************************************************************/
int city_read(city_t city)
{
  int ch, i = 0;

  // skip leading non letter characters
  while (true)
  {
    ch = getchar();
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
    {
      city[i++] = ch;
      break;
    }
  }
  while ((ch = getchar()) != '\n')
  {
    if (i < MAX_CITY_NAME_LEN)
    {
      city[i++] = ch;
    }
  }
  city[i] = '\0';
  return i;
}

/****************************************************************
 * Message functions so that your messages match what we expect *
 ****************************************************************/
void msg_city_bad(char *city)
{
  printf("No schedule for %s\n", city);
}

void msg_city_exists(char *city)
{
  printf("There is a schedule of %s already.\n", city);
}

void msg_schedule_no_free(void)
{
  printf("Sorry no more free schedules.\n");
}

void msg_city_flights(char *city)
{
  printf("The flights for %s are:", city);
}

void msg_flight_info(int time, int avail, int capacity)
{
  printf(" (%d, %d, %d)", time, avail, capacity);
}

void msg_city_max_flights_reached(char *city)
{
  printf("Sorry we cannot add more flights on this city.\n");
}

void msg_flight_bad_time(void)
{
  printf("Sorry there's no flight scheduled on this time.\n");
}

void msg_flight_no_seats(void)
{
  printf("Sorry there's no more seats available!\n");
}

void msg_flight_all_seats_empty(void)
{
  printf("All the seats on this flights are empty!\n");
}

void msg_time_bad()
{
  printf("Invalid time value\n");
}

void msg_capacity_bad()
{
  printf("Invalid capacity value\n");
}

void print_command_help()
{
  printf("Here are the possible commands:\n"
         "A <city name>     - Add an active empty flight schedule for\n"
         "                    <city name>\n"
         "L                 - List cities which have an active schedule\n"
         "l <city name>     - List the flights for <city name>\n"
         "a <city name>\n"
         "<time> <capacity> - Add a flight for <city name> @ <time> time\n"
         "                    with <capacity> seats\n"
         "r <city name>\n"
         "<time>            - Remove a flight form <city name> whose time is\n"
         "                    <time>\n"
         "s <city name>\n"
         "<time>            - Attempt to schedule seat on flight to \n"
         "                    <city name> at <time> or next closest time on\n"
         "                    which their is an available seat\n"
         "u <city name>\n"
         "<time>            - unschedule a seat from flight to <city name>\n"
         "                    at <time>\n"
         "R <city name>     - Remove schedule for <city name>\n"
         "h                 - print this help message\n"
         "q                 - quit\n");
}

/****************************************************************
 * Resets a flight schedule                                     *
 ****************************************************************/
void flight_schedule_reset(struct flight_schedule *fs)
{
  fs->destination[0] = 0;
  for (int i = 0; i < MAX_FLIGHTS_PER_CITY; i++)
  {
    fs->flights[i].time = TIME_NULL;
    fs->flights[i].available = 0;
    fs->flights[i].capacity = 0;
  }
  fs->next = NULL;
  fs->prev = NULL;
}

/******************************************************************
* Initializes the flight_schedule array that will hold any flight *
* schedules created by the user. This is called in main for you.  *
 *****************************************************************/

void flight_schedule_initialize(struct flight_schedule array[], int n)
{
  flight_schedules_active = NULL;
  flight_schedules_free = NULL;

  // takes care of empty array case
  if (n == 0)
    return;

  // Loop through the Array connecting them
  // as a linear doubly linked list
  array[0].prev = NULL;
  for (int i = 0; i < n - 1; i++)
  {
    flight_schedule_reset(&array[i]);
    array[i].next = &array[i + 1];
    array[i + 1].prev = &array[i];
  }
  // Takes care of last node.
  // Its prev pointer will be set in the loop,
  // we just have to set its next to NULL.
  array[n - 1].next = NULL;
  flight_schedules_free = &array[0];
}

/***********************************************************
 * time_get: read a time from the user
   Time in this program is a minute number 0-((24*60)-1)=1439
   -1 is used to indicate the NULL empty time 
   This function should read in a time value and check its 
   validity.  If it is not valid eg. not -1 or not 0-1439
   It should print "Invalid Time" and return false.
   othewise it should return the value in the integer pointed
   to by time_ptr.
 ***********************************************************/
bool time_get(int *time_ptr)
{
  if (scanf("%d", time_ptr) == 1)
  {
    return (TIME_NULL == *time_ptr ||
            (*time_ptr >= TIME_MIN && *time_ptr <= TIME_MAX));
  }
  msg_time_bad();
  return false;
}

/***********************************************************
 * flight_capacity_get: read the capacity of a flight from the user
   This function should read in a capacity value and check its 
   validity.  If it is not greater than 0, it should print 
   "Invalid capacity value" and return false. Othewise it should 
   return the value in the integer pointed to by cap_ptr.
 ***********************************************************/
bool flight_capacity_get(int *cap_ptr)
{
  if (scanf("%d", cap_ptr) == 1)
  {
    return *cap_ptr > 0;
  }
  msg_capacity_bad();
  return false;
}

void flight_schedule_sort_flights_by_time(struct flight_schedule *fs)
{
  qsort(fs->flights, MAX_FLIGHTS_PER_CITY, sizeof(struct flight),
        flight_compare_time);
}

int flight_compare_time(const void *a, const void *b)
{
  const struct flight *af = a;
  const struct flight *bf = b;

  return (af->time - bf->time);
}

// helper function for flight_schedule_add()
struct flight_schedule *flight_schedule_allocate(void){
  if (flight_schedules_free == NULL) {
    msg_schedule_no_free();
    return NULL;
  }
  //make a pointer fs to use
  struct flight_schedule *fs = flight_schedules_free;
  flight_schedules_free = flight_schedules_free->next;
  
  //if free is empty
  if (flight_schedules_free == NULL) {
    fs->next = flight_schedules_active;
    fs->prev = NULL;
    if (flight_schedules_active == NULL) {
      flight_schedules_active = fs;
    } else {
    flight_schedules_active->prev = fs;
    }
    flight_schedules_active = fs;
    return flight_schedules_active;
  }

  flight_schedules_free->prev = NULL;
  fs->next = flight_schedules_active;
  fs->prev = NULL;
  if (flight_schedules_active == NULL) {
    flight_schedules_active = fs;
  } else {
    flight_schedules_active->prev = fs;
  }
  
  flight_schedules_active = fs;
  return flight_schedules_active;
}

// used as a helper method for flight_schedule_remove()
void flight_schedule_free(struct flight_schedule *fs){
  //if free is empty
  if (flight_schedules_free == NULL) {
    //if it is an end
    if (fs->next == NULL || fs->prev == NULL) {
      if (fs->next == NULL && fs->prev == NULL) {
        flight_schedules_active = NULL;
        flight_schedule_reset(fs);
        flight_schedules_free = fs;
      } else if (fs->next == NULL) {
        fs->prev->next = NULL;
        flight_schedules_active = fs->prev;
        flight_schedule_reset(fs);
        flight_schedules_free = fs;
      } else {
        fs->next->prev = NULL;
        flight_schedule_reset(fs);
        flight_schedules_free = fs;
      }
    }
  } else {
    //same structure of cases, just when free is not empty
    if (fs->next == NULL || fs->prev == NULL) {
      if (fs->next == NULL && fs->prev == NULL) {
          flight_schedules_active = NULL;
          flight_schedule_reset(fs);
          fs->next = flight_schedules_free;
          fs->next->prev = fs;
          flight_schedules_free = fs;
      } else if (fs->next == NULL) {
          fs->prev->next = NULL;
          flight_schedules_active = fs->prev;
          fs->next = flight_schedules_free;
          fs->next->prev = fs;
          flight_schedules_free = fs;
          flight_schedule_reset(fs);
      } else {
        fs->next->prev = NULL;
        flight_schedule_reset(fs);
        fs->next = flight_schedules_free;
        fs->next->prev = fs;
        flight_schedules_free = fs;
      }
    }
  }
}

struct flight_schedule *flight_schedule_find(city_t city) {
  struct flight_schedule *active_iterator = flight_schedules_active;
  while (active_iterator != NULL) {
      //if the destination matches the input city
      if (strcmp(active_iterator->destination, city) == 0) {
      return active_iterator;
    }
    // iterate.
    active_iterator = active_iterator->next;
  }
  return active_iterator;
}

void flight_schedule_listAll(void) {
  struct flight_schedule *active_iterator = flight_schedules_active;
  while (active_iterator != NULL) {
    printf(active_iterator->destination);
    printf("\n");
    // iterate.
    active_iterator = active_iterator->next;
  }
}

void flight_schedule_list(city_t city) {
  struct flight_schedule *active_iterator = flight_schedules_active;
  while (active_iterator != NULL) {
    //if the destination matches the city
    if (strcmp(active_iterator->destination, city) == 0) {
      msg_city_flights(city);
      //iterate through all flights in flights[]
      for (int i = 0; i < MAX_FLIGHTS_PER_CITY; i++) {
        if (active_iterator->flights[i].time != -1){
          //print time, available and capacity of each flight
          msg_flight_info(active_iterator->flights[i].time, active_iterator->flights[i].available, active_iterator->flights[i].capacity);   
        }
      }
      printf("\n");
    }
    // iterate.
    active_iterator = active_iterator->next;
  }
}

void flight_schedule_add(city_t city) {
  //call flight_schedule_allocate() to grab a fs for the new city
  if (flight_schedules_free == NULL) {
    msg_schedule_no_free();
    return;
  } else if (flight_schedule_find(city) != NULL) {
    msg_city_exists(city);
    return;
  } else {
    struct flight_schedule *fs = flight_schedule_allocate();
    //assign the given city to the now active fs
    strcpy(fs->destination, city);
    return;
  }
}

void flight_schedule_remove(city_t city) {
  //use f_s_find() to locate where in the active list the fs that needs to be removed is
  if (flight_schedule_find(city) != NULL) {
    //if it exists:
    //pass that reference to f_s_free() to reset it, take it off the active list and move it to the free list
    flight_schedule_free(flight_schedule_find(city));
  } else {
    //if we get here then there isn't an existing schedule for the given city
    msg_city_bad(city);
  }
}

void flight_schedule_add_flight(city_t city) {
  //first check to see if there is already an existing fs for this city
  struct flight_schedule *fs = flight_schedule_find(city);
  //time and capacity to be used later on
  int time;
  int capacity;
  if (fs == NULL) {
    msg_city_bad(city);
  }
  //make sure both time and capacity are valid
  if (!time_get(&time) || !flight_capacity_get(&capacity)) {
    return;
  }
  //check to make sure there is enough room to add another flight before we go ahead and prep to add
  for (int i = 0; i < MAX_FLIGHTS_PER_CITY; i++) {
    if (fs->flights[i].time == -1) {
      break;
    }
    if (i == 4) {
      msg_city_max_flights_reached(city);
      return;
    }
  }

  //then make sure we put the new flight in the right place in the flights[] array
  for (int i = 0; i < MAX_FLIGHTS_PER_CITY; i++) {
    if (fs->flights[i].time == TIME_NULL) {
      //assign the taken in time and capacity to the new flight
      fs->flights[i].time = time;
      fs->flights[i].available = capacity;
      fs->flights[i].capacity = capacity;
      break;
    }
  }
}

void flight_schedule_remove_flight(city_t city) {
  struct flight_schedule *fs = flight_schedule_find(city);
  if (fs == NULL) {
    msg_city_bad(city);
   }
  //use scanf() to grab the time from the user
  int time_input;
  scanf("%d", &time_input);
  for (int i = 0; i < MAX_FLIGHTS_PER_CITY; i++) {
    //if the time of the existing flight matches
    if (time_input == fs->flights[i].time) {
      fs->flights[i].time = TIME_NULL;
      fs->flights[i].capacity = 0;
      fs->flights[i].available = 0;
      return;
    }
  }
  //if we get to here then the flight time that was requested to be removed was invalid
  msg_flight_bad_time();
}

void flight_schedule_schedule_seat(city_t city) {
  struct flight_schedule *fs = flight_schedule_find(city);
  if (fs == NULL) {
    msg_city_bad(city);  
  }
  //use scanf() to grab the time from the user
  int time_input;
  scanf("%d", &time_input);

  //find the appropriately timed flight
  int best_time_index;
  int smallest_time_before = time_input;
  for (int i = 0; i < MAX_FLIGHTS_PER_CITY; i++) {
    //if there is a flight that has the exact specified time
    if (time_input == fs->flights[i].time) {
      best_time_index = i;
    } else if (fs->flights[i].time - time_input > 0) {
      //if the flight is after the desired time
      //and the flight is closer to the time than previous flights were
      if (fs->flights[i].time - time_input < smallest_time_before) {
        smallest_time_before = fs->flights[i].time - time_input;
        best_time_index = i;
      }
    }
  }
  //actually schedule the seat
  if (fs->flights[best_time_index].available == 0) {
    msg_flight_no_seats();
  } else {
    fs->flights[best_time_index].available--;
  }
  return;
}

void flight_schedule_unschedule_seat(city_t city) {
  struct flight_schedule *fs = flight_schedule_find(city);
  if (fs == NULL) {
    msg_city_bad(city);
  }
  //use scanf() to grab the time from the user
  int time_input;
  scanf("%d", &time_input);
  
  for (int i = 0; i < MAX_FLIGHTS_PER_CITY; i++) {
  //if the time of the existing flight matches
    if (time_input == fs->flights[i].time) {
      //if all the seats are already available
      if (fs->flights[i].available == fs->flights[i].capacity) {
        msg_flight_all_seats_empty();
        return;
      } else {
      fs->flights[i].available++;
      return;
      }
    }
  }
  
  msg_flight_bad_time();
  return;
}