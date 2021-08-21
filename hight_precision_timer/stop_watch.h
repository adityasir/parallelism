#ifndef CSTOPWATCH_H
#define CSTOPWATCH_H
#include<chrono>
#include<iostream>
#include<gtest/gtest.h>



class CStopWatch{
    public:
    CStopWatch():start(std::chrono::high_resolution_clock::now()) {
        
    }
    ~CStopWatch(){
        stop = std::chrono::high_resolution_clock::now();
            
      std::cout << "Time taken by program is : " <<  std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() <<"\n"; 
    }
private:
std::chrono::system_clock::time_point start;
std::chrono::system_clock::time_point stop;
};

TEST(CStopWatch, simple_object_test) {
    CStopWatch object;
    ASSERT_EQ(1+1, 2);
}
#endif //CSTOPWATCH_H