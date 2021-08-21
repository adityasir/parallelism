#ifndef SIMPLE_SOCKET_H
#define SIMPLE_SOCKET_H


class CSocket
{
public:
	CSocket()
		:m_iCurrent(0)
	{
		m_messages.reserve(COUNT_TEST_MESSAGES);
		//std::cout<<"CSocket Start";
		for (size_t i = 0; i < COUNT_TEST_MESSAGES; ++i)
		{
			std::string strMessage = "Message " + std::to_string(i);
			m_messages.push_back(strMessage);
		}

	}

	std::string GetNextMessage()
	{
		std::string strCurrentValue = m_messages[m_iCurrent++];
		if (m_iCurrent == COUNT_TEST_MESSAGES)
		{
			m_iCurrent = 0;
		}
		return strCurrentValue;
	}

private:
	std::vector<std::string> m_messages;
	size_t m_iCurrent;
	static const size_t COUNT_TEST_MESSAGES = 1000;
};


#endif //SIMPLE_SOCKET_H