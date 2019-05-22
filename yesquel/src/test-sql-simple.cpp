//
// test-sql-simple.cpp
//
// Issues a simple SQL query to Yesquel.
//

/*
  Original code: Copyright (c) 2014 Microsoft Corporation
  Modified code: Copyright (c) 2015-2016 VMware, Inc
  All rights reserved. 
  Written by Marcos K. Aguilera
  MIT License
  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:
  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <stdarg.h>
#include <values.h>
#include <stdint.h>
#include <string>
#include <random>
#include <iostream>


#include <chrono>
#include <sstream> 
#include <vector>
#include <thread> 
#include <future>
#include <stdlib.h>

#include "sqlite3.h"

void printres(int res)
{
  switch(res){
  case SQLITE_BUSY: printf(" busy\n"); break;
  case SQLITE_DONE: printf(" done\n"); break;
  case SQLITE_ROW: printf(" row\n"); break;
  case SQLITE_ERROR: printf(" error\n"); break;
  case SQLITE_MISUSE: printf(" misuse\n"); break;
  }
}

static const char alphanum[] =
"0123456789"
"!@#$%^&*"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

static const int stringLength = sizeof(alphanum) - 1;

char genRandom()
{
    return alphanum[rand() % stringLength];
}

std::string genString(int size)
{
  std::string Str;
  for(int i = 0; i < size; ++i)
  {
    Str += genRandom();
  }
  return Str;
}


int randNum(int min, int max) {
  return rand()% max % min;

}

void workload_uniform_read(sqlite3 *db, int REPS){

  int res, i, rand;
  sqlite3_stmt *stmt;
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(1,250); // distribution in range [1, 6]

  
  for (i=0; i < REPS; ++i){

    //1000000000000000000 to 1000000000000006000
    std::string rand = std::to_string(dist6(rng) + 1000000000000000000);


    std::string str = "select * from t4 where a='"+rand+"';";
    const char *cstr = str.c_str();
    //printf("%s\n", cstr);

    res=sqlite3_prepare(db, cstr, -1, &stmt, 0); assert(res==0);

    res=sqlite3_step(stmt);
    
    //printf("%s\n", sqlite3_column_text(stmt,1));

    res=sqlite3_finalize(stmt); assert(res==0);

  }
}

void workload_uniform_read_write(sqlite3 *db, int REPS){

  int res, i, randtoadd;
  sqlite3_stmt *stmt;
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(1,250); // distribution in range [1, 6]
  
  long int beginning = 100000000000;
  
  for (i=0; i < REPS; ++i){

    if((i%2)==0){
      randtoadd = rand() % 1000000 + 1; //generate random between 1 - 1000000
      beginning += randtoadd;
      std::string a = std::to_string(beginning);
      std::string b = genString(100).c_str();

      std::string str = "insert into t4 values ('"+a+"','"+b+"','"+b+"','"+b+"','"+b+"');";
      
      const char *cstr = str.c_str();

      //printf("%s\n", cstr);

      res=sqlite3_prepare(db, cstr, -1, &stmt, 0); assert(res==0);

      res=sqlite3_step(stmt);
      
      //printf("%s\n", sqlite3_column_text(stmt,1));

      res=sqlite3_finalize(stmt); assert(res==0);
    }

    else{
      //1000000000000000000 to 1000000000000006000
      std::string rand = std::to_string(dist6(rng) + 1000000000000000000);

      std::string str = "select * from t4 where a='"+rand+"';";
      const char *cstr = str.c_str();
      //printf("%s\n", cstr);

      res=sqlite3_prepare(db, cstr, -1, &stmt, 0); assert(res==0);

      res=sqlite3_step(stmt);
      
      //printf("%s\n", sqlite3_column_text(stmt,1));

      res=sqlite3_finalize(stmt); assert(res==0);
      
    }
  }
}

void workload_uniform_read_write_replace(sqlite3 *db, int REPS){

  int res, i, randtoadd;
  sqlite3_stmt *stmt;
  std::mt19937 rng;
  rng.seed(std::random_device()());
  std::uniform_int_distribution<std::mt19937::result_type> dist6(1,250); // distribution in range [1, 6]
  
  long int beginning = 1000000000000000000;
  
  for (i=0; i < REPS; ++i){

    if((i%2)==0){
      beginning += 1;
      std::string a = std::to_string(beginning);
      std::string b = genString(100).c_str();

      std::string str = "replace into t4 values ('"+a+"','"+b+"','"+b+"','"+b+"','"+b+"');";
      
      const char *cstr = str.c_str();

      //printf("%s\n", cstr);

      res=sqlite3_prepare(db, cstr, -1, &stmt, 0); assert(res==0);

      res=sqlite3_step(stmt);
      
      //printf("%s\n", sqlite3_column_text(stmt,1));

      res=sqlite3_finalize(stmt); assert(res==0);
    }

    else{
      //1000000000000000000 to 1000000000000006000
      std::string rand = std::to_string(dist6(rng) + 1000000000000000000);

      std::string str = "select * from t4 where a='"+rand+"';";
      const char *cstr = str.c_str();
      //printf("%s\n", cstr);

      res=sqlite3_prepare(db, cstr, -1, &stmt, 0); assert(res==0);

      res=sqlite3_step(stmt);
      
      //printf("%s\n", sqlite3_column_text(stmt,1));

      res=sqlite3_finalize(stmt); assert(res==0);
      
    }
  }
}

int workloadTime(sqlite3 *db, int REPS){
  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

  workload_uniform_read(db, REPS);

  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count();

  return duration;
}

int execute(int iterations, int REPS){
  //std::cout << "Starting" << std::endl;

  sqlite3 *db;
  int duration = 0, i, res;

  res=sqlite3_open("TEST", &db); assert(res==0);

  for (i=0; i < iterations; ++i){
    duration += workloadTime(db, REPS);
  }

  res=sqlite3_close(db); assert(res==0);

  //std::cout << "Ended" << std::endl;

  return duration/iterations;
}

int main(int argc, char **argv)
{
  int res,i;


  int REPS = 1000;
  int iterations = 3;
  int threads = 2;
  int average_time_threads=0;

  if (argc < 2) {
      // Tell the user how to run the program
      std::cerr << "Usage: <#clients> <#reps/client> " << std::endl;
      /* "Usage messages" are a conventional way of telling the user
       * how to run a program if they enter the command incorrectly.
       */
      return 1;
  }

  threads = strtol(argv[1], nullptr, 0);
  REPS = strtol(argv[2], nullptr, 0);

  printf("#Clients -> %d \n",threads);
  printf("#Reps/Client -> %d \n",REPS);


  std::vector<std::future<int>> thread_pool;

  for(int i = 0; i < threads; i++)
  {   
    thread_pool.push_back(std::async(std::launch::async,execute, iterations, REPS));
  }


  for(int i = 0; i < threads; i++)
  {   
    average_time_threads += thread_pool.at(i).get();
  }

  average_time_threads = average_time_threads/threads;

  //printf("reps -> %d \n",REPS);
  //printf("iterations -> %d \n",iterations);
  //printf("average duration -> %d \n",duration/iterations);
  
  printf("average duration (%d iterations) -> %d \n",iterations,average_time_threads);



  return 0;
}