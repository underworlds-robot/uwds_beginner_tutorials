#include <ros/ros.h>
#include <uwds/uwds.h>
#include <nodelet/nodelet.h>

typedef boost::shared_ptr<ros::NodeHandle> NodeHandlePtr;

using namespace std;
using namespace std_msgs;
using namespace uwds;

class ProviderExample
{
public:
  ProviderExample(NodeHandlePtr nh, string filename)
  {
    ctx_ = boost::make_shared<UnderworldsProxy>(nh, "provider_example", PROVIDER);
    if(ctx_->worlds()["env"].pushSceneFrom3DFile(filename))
      ROS_INFO("Successfully load file !");
  }
protected:
  UnderworldsProxyPtr ctx_;
};

typedef boost::shared_ptr<ProviderExample> ProviderExamplePtr;

class ProviderExampleNodelet : public nodelet::Nodelet
{
public:
  void onInit()
  {
    nh_ = boost::make_shared<ros::NodeHandle>(getMTNodeHandle());
    string filename;
    nh_->param<string>("filename", filename, "");
    provider_example_ = boost::make_shared<ProviderExample>(nh_, filename);
  }
protected:
  NodeHandlePtr nh_;
  ProviderExamplePtr provider_example_;
};

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(ProviderExampleNodelet, nodelet::Nodelet)

int main(int argc, char **argv)
{
  ros::init(argc, argv, "provider_example");
  NodeHandlePtr nh = boost::make_shared<ros::NodeHandle>();
  string filename;
  nh->param<string>("filename", filename, "");
  ProviderExample provider = ProviderExample(nh, filename);
  ros::spin();
}
