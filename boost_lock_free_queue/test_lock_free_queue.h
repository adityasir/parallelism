#ifndef TEST_LOCKED_FREE_QUEUE_H
#define TEST_LOCKED_FREE_QUEUE_H

#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/atomic.hpp>

#include <iostream>
#include <fstream>

boost::lockfree::queue<char*> bl_queue(128);
struct test_data{
    test_data():server_msg_count(0),consumer_msg_count(0){}
    boost::atomic_int server_msg_count;
    boost::atomic_int consumer_msg_count;
    const int iterations = 1000000;
};

test_data data;

void producer(void)
{
	std::ofstream objFile;
	std::string file_name("producer.log");
	objFile.open (file_name);

	CSocket objSocket;
    for (int i = 0; i != data.iterations; ++i) {
      	{
    		++data.server_msg_count;
    		char *item = const_cast<char*>(objSocket.GetNextMessage().c_str());
    		while (!bl_queue.push(item));
    		if(log_message){
    			objFile <<data.server_msg_count <<" messsage "<<item<<std::endl;
    		}

    	}
    }

	objFile.close();
}

boost::atomic<bool> done (false);
void consumer(int thread_no)
{

	std::ofstream objFile;
	std::string file_name("consumer.log");
	file_name+= "-";
	file_name+= std::to_string(thread_no);
	objFile.open (file_name);

    char* strRecBuffer;
    while (!done) {
        while (bl_queue.pop(strRecBuffer))
        {
        	++data.consumer_msg_count;
            if(log_message){
            	objFile<<data.consumer_msg_count <<" messsage "<<strRecBuffer <<std::endl;
            }
        }
    }

    while (bl_queue.pop(strRecBuffer)){
        ++data.consumer_msg_count;
    }


	objFile.close();
}




TEST(Test_boost_lockfree_queue, one_producer_one_consumer) 
{
    using namespace std;
    data.server_msg_count = 0;
	data.consumer_msg_count = 0;
    const int consumer_thread_count = 1;
    boost::thread_group producer_threads, consumer_threads;
    producer_threads.create_thread(producer);
      
    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(std::bind(consumer, i));
      
    producer_threads.join_all();
    done = true;
    consumer_threads.join_all();
      
    cout << std::endl <<"Server_msg_count  " << server_msg_count << " objects.";
    cout << std::endl <<"Consumer_msg_count " << consumer_msg_count << " objects.\n";
    ASSERT_EQ(data.server_msg_count , data.consumer_msg_count);
}

/*
TEST(Test_boost_lockfree_queue, one_producer_four_consumer) 
{
    using namespace std;
    
	data.server_msg_count = 0;
	data.consumer_msg_count = 0;

    const int consumer_thread_count = 4;
    boost::thread_group producer_threads, consumer_threads;
    producer_threads.create_thread(producer);
      
    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(std::bind(consumer, i));
      
    producer_threads.join_all();
    done = true;
    
    consumer_threads.join_all();
      
    cout << std::endl <<"Server_msg_count  " << data.server_msg_count << " objects." << endl;
    cout << std::endl <<"Consumer_msg_count " << data.consumer_msg_count << " objects." << endl;
    ASSERT_EQ(data.server_msg_count , data.consumer_msg_count);
}
*/
/*
TEST(Test_boost_lockfree_queue, one_producer_eight_consumer) 
{
    using namespace std;
	data.server_msg_count = 0;
	data.consumer_msg_count = 0;

    const int consumer_thread_count = 8;
    boost::thread_group producer_threads, consumer_threads;
    producer_threads.create_thread(producer);
      
    for (int i = 0; i != consumer_thread_count; ++i)
        consumer_threads.create_thread(std::bind(consumer, i));
      
    producer_threads.join_all();
    done = true;
    consumer_threads.join_all();
      
    cout << std::endl <<"Server_msg_count  " << data.server_msg_count << " objects." << endl;
    cout << std::endl <<"Consumer_msg_count " << data.consumer_msg_count << " objects." << endl;
    ASSERT_EQ(data.server_msg_count , data.consumer_msg_count);
}
*/
#endif //TEST_LOCKED_FREE_QUEUE_H