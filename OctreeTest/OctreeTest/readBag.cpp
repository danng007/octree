//#include "readBag.h"
//
//
//
//readBag::readBag()
//{
//}
//
//
//readBag::~readBag()
//{
//}
//
//void readBag::readBagFile(string fileName) {
//	rosbag::Bag bag;
//	bag.open(fileName, rosbag::bagmode::Read);
//	std::vector<std::string> topics;
//	topics.push_back(std::string("/imu"));
//	//topics.push_back(std::string("numbers"));
//	rosbag::View view(bag, rosbag::TopicQuery(topics));
//	
//	foreach(rosbag::MessageInstance const m, view)
//	{
//		std::cout << m.instantiate<string>()<<endl;
//		/*std_msgs::Int32::ConstPtr s = m.instantiate<std_msgs::String>();
//		if (s != NULL)
//			std::cout << s->data << std::endl;
//		std_msgs::Int32::ConstPtr i = m.instantiate<std_msgs::Int32>();
//		if (i != NULL)
//			std::cout << i->data << std::endl;*/
//	}
//	bag.close();
//}