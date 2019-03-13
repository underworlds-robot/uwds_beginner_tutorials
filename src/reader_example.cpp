#include <ros/ros.h>
#include <uwds/uwds.h>

typedef boost::shared_ptr<ros::NodeHandle> NodeHandlePtr;

using namespace std;
using namespace std_msgs;
using namespace uwds;

class ReaderExample
{
public:
  ReaderExample(NodeHandlePtr nh)
  {
    ctx_ = boost::make_shared<UnderworldsProxy>(nh, "reader_example", READER);
    ctx_->worlds()["env"].connect(bind(&ReaderExample::changesCallback, this, _1, _2, _3));
  }
protected:
  void changesCallback(string world_name, Header header, Invalidations invalidations)
  {
    for(const auto id : invalidations.node_ids_updated)
      ROS_INFO("Received node <%s> update", id.c_str());
    for(const auto id : invalidations.situation_ids_updated)
      ROS_INFO("Received situation <%s> update", id.c_str());
    for(const auto id : invalidations.mesh_ids_updated)
      ROS_INFO("Received mesh <%s> update", id.c_str());
  }

  UnderworldsProxyPtr ctx_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "reader_example");
  NodeHandlePtr nh = boost::make_shared<ros::NodeHandle>();
  ReaderExample reader = ReaderExample(nh);
  ros::spin();
}
