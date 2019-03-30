#include <ros/ros.h>
#include <uwds/uwds.h>

typedef boost::shared_ptr<ros::NodeHandle> NodeHandlePtr;

using namespace std;
using namespace std_msgs;
using namespace uwds;

class ProviderExample
{
public:
  ProviderExample(NodeHandlePtr nh, NodeHandlePtr pnh, string filename)
  {
    ctx_ = boost::make_shared<UnderworldsProxy>(nh, pnh, "provider_example", PROVIDER);
    if(ctx_->worlds()["robot/env"].pushSceneFrom3DFile(filename))
      ROS_INFO("Successfully load file !");
  }
protected:
  UnderworldsProxyPtr ctx_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "provider_example");
  NodeHandlePtr nh = boost::make_shared<ros::NodeHandle>();
  NodeHandlePtr pnh = boost::make_shared<ros::NodeHandle>("~");
  string filename;
  nh->param<string>("filename", filename, "");
  ProviderExample provider = ProviderExample(nh, pnh, filename);
  ros::spin();
}
