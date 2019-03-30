#include <ros/ros.h>
#include <uwds/uwds.h>

typedef boost::shared_ptr<ros::NodeHandle> NodeHandlePtr;

using namespace std;
using namespace std_msgs;
using namespace uwds;

class FilterExample
{
public:
  FilterExample(NodeHandlePtr nh, NodeHandlePtr pnh, string regex)
  {
    regex_ = std::regex(regex);
    ctx_ = boost::make_shared<UnderworldsProxy>(nh, pnh, "filter_example", FILTER);
    if(ctx_->worlds()["robot/env"].connect(bind(&FilterExample::onChanges, this, _1, _2, _3)));
  }
protected:
  void onChanges(string world_name, Header header, Invalidations invalidations)
  {
    Changes changes;
    auto& scene = ctx_->worlds()["robot/env"].scene();
    for(const auto& node_id : invalidations.node_ids_updated)
    {
      if(regex_match(scene.nodes()[node_id].name, regex_))
      {
        changes.nodes_to_update.push_back(scene.nodes()[node_id]);
      }
    }
    ctx_->worlds()["robot/env_filtered"].update(changes);
  }
  regex regex_;
  UnderworldsProxyPtr ctx_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "filter_example");
  NodeHandlePtr nh = boost::make_shared<ros::NodeHandle>();
  NodeHandlePtr pnh = boost::make_shared<ros::NodeHandle>("~");
  string regex;
  nh->param<string>("regex", regex, "orange|red");
  FilterExample filter = FilterExample(nh, pnh, regex);
  ros::spin();
}
