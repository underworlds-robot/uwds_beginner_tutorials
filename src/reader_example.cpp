#include <ros/ros.h>
#include <uwds/uwds.h>

typedef boost::shared_ptr<ros::NodeHandle> NodeHandlePtr;

using namespace std;
using namespace std_msgs;
using namespace uwds;

class ReaderExample
{
public:
  ReaderExample(NodeHandlePtr nh, NodeHandlePtr pnh)
  {
    ctx_ = boost::make_shared<UnderworldsProxy>(nh, pnh, "reader_example", READER);
    if(ctx_->worlds()["robot/env_filtered"].connect(bind(&ReaderExample::onChanges, this, _1, _2, _3)));
      ROS_INFO("Ready to listen for changes !");
  }
protected:
  void onChanges(string world_name, Header header, Invalidations invalidations)
  {
    for(const auto id : invalidations.node_ids_updated)
      ROS_INFO("Received node <(%s)%s>", ctx_->worlds()["robot/env_filtered"].scene().nodes()[id].name.c_str(), id.c_str());
    for(const auto id : invalidations.situation_ids_updated)
      ROS_INFO("Received situation <(%s)%s>", ctx_->worlds()["robot/env_filtered"].timeline().situations()[id].description.c_str(), id.c_str());
    for(const auto id : invalidations.mesh_ids_updated)
      ROS_INFO("Received mesh <%s>", ctx_->meshes()[id].id.c_str());
  }
  UnderworldsProxyPtr ctx_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "reader_example");
  NodeHandlePtr nh = boost::make_shared<ros::NodeHandle>();
  NodeHandlePtr pnh = boost::make_shared<ros::NodeHandle>("~");
  ReaderExample reader = ReaderExample(nh, pnh);
  ros::spin();
}
