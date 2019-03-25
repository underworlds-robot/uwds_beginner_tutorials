#include <ros/ros.h>
#include <uwds/uwds.h>
#include <nodelet/nodelet.h>

typedef boost::shared_ptr<ros::NodeHandle> NodeHandlePtr;

using namespace std;
using namespace std_msgs;
using namespace uwds;

class FilterExample
{
public:
  FilterExample(NodeHandlePtr nh, NodeHandlePtr pnh, string query)
  {
    query_ = query;
    ctx_ = boost::make_shared<UnderworldsProxy>(nh, pnh, "filter_example", FILTER);
    if(ctx_->worlds()["env"].connect(bind(&FilterExample::onChanges, this, _1, _2, _3)));
  }
protected:
  void onChanges(string world_name, Header header, Invalidations invalidations)
  {
    Changes changes;
    vector<string> result = ctx_->worlds()["env"][query_];

    for(const auto invalidation : invalidations.node_ids_updated)
    {
      for(const auto id : result)
      {
        if(invalidation == id)
          changes.nodes_to_update.push_back(ctx_->worlds()["env"].scene().nodes()[id]);
      }
    }

    for(const auto invalidation : invalidations.situation_ids_updated)
    {
      for(const auto id : result)
      {
        if(invalidation == id)
          changes.situations_to_update.push_back(ctx_->worlds()["env"].timeline().situations()[id]);
      }
    }
    ctx_->worlds()["filtered"].update(changes);
  }
  string query_;
  UnderworldsProxyPtr ctx_;
};

typedef boost::shared_ptr<FilterExample> FilterExamplePtr;

class FilterExampleNodelet : public nodelet::Nodelet
{
public:
  void onInit()
  {
    nh_ = boost::make_shared<ros::NodeHandle>(getMTNodeHandle());
    pnh_ = boost::make_shared<ros::NodeHandle>(getMTPrivateNodeHandle());
    string query;
    nh_->param<string>("query", query, "");
    filter_example_ = boost::make_shared<FilterExample>(nh_, pnh_, query);
  }
protected:
  NodeHandlePtr nh_;
  NodeHandlePtr pnh_;
  FilterExamplePtr filter_example_;
};

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(FilterExampleNodelet, nodelet::Nodelet)


int main(int argc, char **argv)
{
  ros::init(argc, argv, "filter_example");
  NodeHandlePtr nh = boost::make_shared<ros::NodeHandle>();
  NodeHandlePtr pnh = boost::make_shared<ros::NodeHandle>("~");
  string query;
  nh->param<string>("query", query, "");
  FilterExample filter = FilterExample(nh, pnh, query);
  ros::spin();
}
