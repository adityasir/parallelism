#ifndef TEST_LOCKED_BASED_QUEUE_H
#define TEST_LOCKED_BASED_QUEUE_H

#include <simple_socket.h>
#include <lock_based_queue.h>

#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/atomic.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


bool log_message = true;
boost::atomic_int server_msg_count(0);
boost::atomic_int consumer_msg_count(0);
threadsafe_queue<std::string> queue;
const int iterations = 1000000;


void server_thread(void)
{
	std::ofstream objFile;
	std::string file_name("producer.log");
	objFile.open (file_name);

	CSocket objSocket;
    for (int i = 0; i != iterations; ++i) {
    
        ++server_msg_count;
        std::string item = std::move(objSocket.GetNextMessage());
        queue.push(item);
        if(log_message){
            objFile <<server_msg_count <<" messsage "<<item<<std::endl;
        }

    }
  	objFile.close();
}

void consumer_thread(const int thread_no, const int iterations )
{
	std::ofstream objFile;
	std::string file_name("consumer.log");
	file_name+= "-";
	file_name+= std::to_string(thread_no);
	objFile.open (file_name);

    std::string strRecBuffer;
    for (int i = 0; i != iterations; ++i) {
    
        ++consumer_msg_count;
        queue.wait_and_pop(strRecBuffer);
        if(log_message){
            	objFile<<consumer_msg_count <<" messsage "<<strRecBuffer <<std::endl;
        }
    }
	objFile.close();
    
}


TEST(Test_threadsafe_queue, one_producer_one_consumer) 
{
    using namespace std;
    const int consumer_thread_count = 1;
    boost::thread_group producer_threads, consumer_threads;
    producer_threads.create_thread(server_thread);
      
    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(std::bind(consumer_thread, i, iterations));
      
    producer_threads.join_all();
    consumer_threads.join_all();

    //cout <<"Server_msg_count  " << server_msg_count << " objects.";
    //cout << std::endl <<"Consumer_msg_count " << consumer_msg_count << " objects.\n";
    ASSERT_EQ(server_msg_count , consumer_msg_count);
}

TEST(Test_threadsafe_queue, one_producer_four_consumer) 
{
    using namespace std;
	server_msg_count = 0;
	consumer_msg_count = 0;

    const int consumer_thread_count = 4;
    boost::thread_group producer_threads, consumer_threads;
    producer_threads.create_thread(server_thread);
      
    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(std::bind(consumer_thread, i, (iterations/consumer_thread_count)));
      
    producer_threads.join_all();
    consumer_threads.join_all();
      
    //cout <<"Server_msg_count  " << server_msg_count << " objects.";
    //cout << std::endl <<"Consumer_msg_count " << consumer_msg_count << " objects.\n";
    ASSERT_EQ(server_msg_count , consumer_msg_count);
}

TEST(Test_threadsafe_queue, one_producer_eight_consumer) 
{
    using namespace std;
	server_msg_count = 0;
	consumer_msg_count = 0;

    const int consumer_thread_count = 8;
    boost::thread_group producer_threads, consumer_threads;
    producer_threads.create_thread(server_thread);
      
    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(std::bind(consumer_thread, i, (iterations/consumer_thread_count)));
      
    producer_threads.join_all();
    consumer_threads.join_all();

    //cout <<"Server_msg_count  " << server_msg_count << " objects.";
    //cout << std::endl <<"Consumer_msg_count " << consumer_msg_count << " objects.\n";   

    ASSERT_EQ(server_msg_count , consumer_msg_count);
}
#endif //TEST_LOCKED_BASED_QUEUE_H