
#include<iostream>
#include<queue>
#include<vector>
#include<condition_variable>
#include<stop_watch.h>
#include<gtest/gtest.h>
#include<test_locked_based_queue.h>
#include<test_lock_free_queue.h>


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

