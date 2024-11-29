#pragma once
#include <string>
#include "monitor_info.pb.h"
#include "monitor_inter.h"

namespace monitor {

class MonitorInter {
   public:
    MonitorInter() {}
    virtual ~MonitorInter() {}
    virtual void updateOnce(monitor::proto::MonitorInfo* monitor_info) = 0;
    virtual void stop() = 0;
};

};  // namespace monitor