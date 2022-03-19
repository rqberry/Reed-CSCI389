#include <iostream> // for std::cout
#include <thread>   // for std::thread
#include <vector>   // for std::vector
#include <unistd.h> // for sleep()
#include <cstring>  // for strlen()

#include "barriers.cc" // barrier implementations

/* sleep_test_barrier
   sleep one thread for a while and have it set a value (-1) to sleep_test_flag
   then have another thread set sleep_test_flag (1) upon leaving,
   sleep_test_flag should always be set to the second thread's value (1)
*/
std::atomic<int> sleep_test_flag = -1;  // correctness checker

void sleep_test_barrier(int32_t tid, const int32_t P, void (*barrier)(int32_t, int32_t)) {
  if (tid == 0) {
    sleep(1);
    sleep_test_flag = 0;
  }
  barrier(tid,P);
  if (tid == P-1) sleep_test_flag = 1;
}

/* test_centralized_barrier
   functions the same as the centralized_barrier but tests for
   correctness, the value of correct should be equal to P-1
*/
std::atomic<int> leak_test_mark = 0;  // each thread should see 0 before barrier
std::atomic<int> leak_test_flag = 1;  // correctness flag should be (1) upon completion

void leak_test_barrier(int32_t tid, const int32_t P, void (*barrier)(int32_t, int32_t)) {
  if (leak_test_mark != 0) leak_test_flag = 0;
  barrier(tid,P);
  leak_test_mark = 1;
}

/* sense_centralized_barrier
   test if consecutive barrier calls run without stalling
*/
std::atomic<int> sense_test_flag = -1;  // correctness flag should be (1) upon completion

void sense_test_barrier(int32_t tid, const int32_t P, void (*barrier)(int32_t, int32_t)) {
  for (int i  = 0; i < 15; ++i) barrier(tid, P);
  sense_test_flag = 1;
}

/* time_test
   time the execution of each barrier over 100 iterations and report to the
   timespec structs listed below
*/
struct timespec c_start, c_end, d_start, d_end, m_start, m_end;

void time_test(int32_t tid, const int32_t P, void (*central)(int32_t, int32_t),
                                             void (*dissem)(int32_t, int32_t),
                                             void (*mcs)(int32_t, int32_t)) {
  // tids chosen to mimic longest potential runtime
  if (tid == 0) clock_gettime(CLOCK_MONOTONIC, &c_start);
  for (int i = 0; i < 100; ++i) central(tid, P);
  if (tid == 0) clock_gettime(CLOCK_MONOTONIC, &c_end);

  if (tid == P-1) clock_gettime(CLOCK_MONOTONIC, &d_start);
  for (int i = 0; i < 100; ++i) dissem(tid, P);
  if (tid == P-1) clock_gettime(CLOCK_MONOTONIC, &d_end);
  // see notes on mcs issues

  if (tid == P-1) clock_gettime(CLOCK_MONOTONIC, &m_start);
  // for (int i = 0; i < 100; ++i) mcs(tid, P);
  if (tid == P-1) clock_gettime(CLOCK_MONOTONIC, &m_end);
}

// run all tests in sequence
void all_tests(int32_t tid, const int32_t P, void (*barrier)(int32_t, int32_t)) {
  leak_test_barrier(tid, P, barrier);
  sleep_test_barrier(tid, P, barrier);
  sense_test_barrier(tid, P, barrier);
}

// print if incorrect arguments are entered in the command line
void print_arguments() {
  std::cout << "Incorrect arguments: ";
  std::cout << "\n\t expected 2 or more: ./<exec> <flags>";
  std::cout << "\n\t valid testing flags: \"-c\" centralized barrier test";
  std::cout << "\n\t                      \"-d\" dissemination barrier test";
  std::cout << "\n\t                      \"-m\" MCS barrier test";
  std::cout << "\n\t                      \"-t\" all barrier time comparison\n";
}

// get user arguments and run tests accordingly
int main(int argc, char *argv[]) {

  // set test parameters
  char run_barrier_test = '-';
  bool run_time_test = false;
  if (argc > 1) {
    for (int32_t i = 1; i < argc; ++i) {
      // assert flag is valid
      if (strlen(argv[i]) != 2) {
        std::cout << "Invalid flag.\n";
        print_arguments();
      } else {
        if (argv[i][1] == 'd') run_barrier_test = 'd';
        else if (argv[i][1] == 'm') run_barrier_test = 'm';
        else if (argv[i][1] == 'c') run_barrier_test = 'c';
        else if (argv[i][1] == 't') run_time_test = true;
      }
    }
  }

  // allocate vector for threads
  std::vector<std::thread> v;

  // spawn threads of the appropriate barrier
  if (!run_time_test) {
    if (run_barrier_test == 'd') {
      std::cout << "DISSEMINATION BARRIER TEST\n";
      for (int32_t tid = 0; tid < P; ++tid) {
        v.emplace_back(all_tests, tid, P, &dissemination_barrier);
      }
    } else if (run_barrier_test == 'm') {
      std::cout << "MCS BARRIER TEST\n";
      for (int32_t tid = 0; tid < P; ++tid) {
        v.emplace_back(all_tests, tid, P, &mcs_barrier);
      }
    } else if (run_barrier_test == 'c') {
      std::cout << "CENTRALIZED BARRIER TEST\n";
      for (int32_t tid = 0; tid < P; ++tid) {
        v.emplace_back(all_tests, tid, P, &centralized_barrier);
      }
    }
    // join all threads
    for (auto& t : v) {
      t.join();
    }

    // report tests
    if (sense_test_flag == 1) {
      	std::cout << "TEST <SENSE> - PASSED - flag: " << sense_test_flag << " barriers running in sequence with proper sense and parity" << std::endl;
    }

    if (sleep_test_flag == 1) {
      std::cout << "TEST <SLEEP> - PASSED - flag: " << sleep_test_flag << " pre barrier write completed before post barrier write" << std::endl;
    } else if (sleep_test_flag == 0 ){
      std::cout << "TEST <SLEEP> - FAILED - flag: " << sleep_test_flag << " pre barrier write completed after post barrier write" << std::endl;
    }

    if (leak_test_mark > 0) {
      if (leak_test_flag == 1) {
        std::cout << "TEST <LEAKS> - PASSED - flag: " << leak_test_flag << " no threads leaking from the barrier" << std::endl;
      } else if (leak_test_flag == 0 ){
        std::cout << "TEST <LEAKS> - FAILED - flag: " << leak_test_flag << " some threads leaking from the barrier" << std::endl;
      }
    }
  }

  // run timing test
  if (run_time_test) {
    std::cout << "TIMING BARRIERS\n";
    for (int32_t tid = 0; tid < P; ++tid)
      v.emplace_back(time_test, tid, P, &centralized_barrier,
                                        &dissemination_barrier,
                                        &mcs_barrier);
    // join all threads
    for (auto& t : v) {
      t.join();
    }
    // compute the elapsed and mean times in nanoseconds
    double c_start_time = (double)(c_start.tv_sec * 1e9) + (double)c_start.tv_nsec;
    double c_end_time = (double)(c_end.tv_sec * 1e9) + (double)c_end.tv_nsec;
    double c_elapsed_time = (c_end_time - c_start_time) / 100.0;
    double d_start_time = (double)(d_start.tv_sec * 1e9) + (double)d_start.tv_nsec;
    double d_end_time = (double)(d_end.tv_sec * 1e9) + (double)d_end.tv_nsec;
    double d_elapsed_time = (d_end_time - d_start_time) / 100.0;
    double m_start_time = (double)(m_start.tv_sec * 1e9) + (double)m_start.tv_nsec;
    double m_end_time = (double)(m_end.tv_sec * 1e9) + (double)m_end.tv_nsec;
    double m_elapsed_time = (m_end_time - m_start_time) / 100.0;

    // report times
    if (argv[1][1] == 't') {
      std::cout << "barrier         time (ns) \n";
      std::cout << "centralized, " << c_elapsed_time << ", " << P << "\n";
      std::cout << "dissemination, " << d_elapsed_time << ", " << P << "\n";
      std::cout << "MCS, " << m_elapsed_time << ", " << P << "\n";
    }
  }
  return 0;
}
