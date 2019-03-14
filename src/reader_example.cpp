#include <ros/ros.h>
#include <uwds/uwds.h>
#include <nodelet/nodelet.h>

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
    if(ctx_->worlds()["env"].connect(bind(&ReaderExample::onChanges, this, _1, _2, _3)));
      ROS_INFO("Ready to listen for changes !");
  }
protected:
  void onChanges(string world_name, Header header, Invalidations invalidations)
  {
    for(const auto id : invalidations.node_ids_updated)
      ROS_INFO("Received node <%s>", id.c_str());
    for(const auto id : invalidations.situation_ids_updated)
      ROS_INFO("Received situation <%s>", id.c_str());
    for(const auto id : invalidations.mesh_ids_updated)
      ROS_INFO("Received mesh <%s>", id.c_str());
  }
  UnderworldsProxyPtr ctx_;
};

typedef boost::shared_ptr<ReaderExample> ReaderExamplePtr;

class ReaderExampleNodelet : public nodelet::Nodelet
{
public:
  void onInit()
  {
    nh_ = boost::make_shared<ros::NodeHandle>(getMTNodeHandle());
    reader_example_ = boost::make_shared<ReaderExample>(nh_);
  }
protected:
  NodeHandlePtr nh_;
  ReaderExamplePtr reader_example_;
};

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(ReaderExampleNodelet, nodelet::Nodelet)

int main(int argc, char **argv)
{
  ros::init(argc, argv, "reader_example");
  NodeHandlePtr nh = boost::make_shared<ros::NodeHandle>();
  ReaderExample reader = ReaderExample(nh);
  ros::spin();
}
