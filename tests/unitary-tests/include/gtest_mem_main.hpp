/**
 * @file
 * @copyright (c) 2013 Stephan Brenner
 * @license   This project is released under the MIT License.
 *
 * This file implements a main() function for Google Test that runs all tests
 * and detects memory leaks.
 */

#include <iostream>
#include <malloc.h>
#include <gtest/gtest.h>

#pragma GCC diagnostic ignored "-Wwrite-strings" //disable warning: deprecated conversion from string constant to ‘char*’

#ifndef UNUSED
#define UNUSED(expr) do { (void)(expr); } while (0)
#endif

using testing::GTEST_FLAG(repeat);

namespace testing
{
    namespace internal
    {
        extern void ColoredPrintf(GTestColor color, const char* fmt, ...);
    }

#define PRINTF(...)  do { testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, "[          ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_GREEN, __VA_ARGS__); } while(0)

#define WARNING(...) do { testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, "[ WARNING  ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_YELLOW, __VA_ARGS__); } while(0)
#define ERROR(...) do { testing::internal::ColoredPrintf(testing::internal::COLOR_RED, "[  ERROR   ] "); testing::internal::ColoredPrintf(testing::internal::COLOR_RED, __VA_ARGS__); } while(0)

  class MemoryLeakDetector : public EmptyTestEventListener
  {
#ifndef TEST_MEM_LEAK
  public:
      void OnTestProgramStart(const UnitTest&){
          PRINTF("Memory Check disabled due to undefined TEST_MEM_LEAK\n");
      }
#else
  public:
      virtual void OnTestProgramStart(const UnitTest&){
          if (isInsideValgrind()){
              enabled = false;
              PRINTF("Memory Check disabled due to valgrind\n");
          } else {
              enabled = true;
          }
          last_iteration = GTEST_FLAG(repeat)-1;
      }

      virtual void OnTestIterationStart(const UnitTest& unit_test,
                                        int iteration){
          this->iteration = iteration;
      }

      virtual void OnTestStart(const TestInfo&)
      {
          /* Get current memory usage */
          memState_ = mallinfo();
      }
      
      virtual void OnTestEnd(const TestInfo& test_info){
          if (!enabled){
              return;
          }

          if(test_info.result()->Passed())
              {
                  int memleak = memleaks();
                  if (memleak> 0){
                      if(GTEST_FLAG(repeat)==1){ //gtest_repeat is not defined or set to 1
                          ERROR("Memory leak of %d byte(s) detected. Exec with --gtest_repeat=2 to avoid detect errors on initialization\n", memleak);
                          ADD_FAILURE() << "Memory leak";
                      }else{
                          if (iteration==0){
                              WARNING("Seems to have memory leak of %d bytes. I'll set as failed if it has memory lost in the next iteration\n", memleak);
                          }else if(iteration>0){
                              ERROR("Memory leak of %d byte(s) detected\n", memleak);
                              ADD_FAILURE() << "Memory leak";
                          }
                      }
                  }
              }
      }

  private:
      int memleaks(){
          struct mallinfo stateNow;
          stateNow = mallinfo();
          return mallinfoDiff(stateNow, memState_);
      }
      int mallinfoDiff(struct mallinfo a,struct mallinfo b){
          return a.uordblks - b.uordblks;;
      }
      int isInsideValgrind(){
          char *str = getenv("_");
          if( str && strstr(str, "valgrind")!=NULL ) return 1;
          return 0;
      }

  private:
      int iteration;
      int last_iteration;
      bool enabled;
      struct mallinfo memState_;
#endif // TEST_MEM_LEAK
  };
}
