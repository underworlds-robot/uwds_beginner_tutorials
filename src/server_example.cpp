#include <ros/ros.h>
#include <uwds/uwds.h>

typedef boost::shared_ptr<ros::NodeHandle> NodeHandlePtr;

using namespace std;
using namespace std_msgs;
using namespace uwds;

class ServerExample
{
public:
  ServerExample(NodeHandlePtr nh)
  {
    ctx_ = boost::make_shared<Underworlds>(nh);
  }
protected:
  UnderworldsPtr ctx_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "server_example");
  NodeHandlePtr nh = boost::make_shared<ros::NodeHandle>();
  ServerExample server = ServerExample(nh);
  ros::spin();
}
