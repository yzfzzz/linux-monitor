#pragma once
#include <string>
#include "monitor_inter.h"
#include "monitor_info.grpc.pb.h"

namespace monitor{
/*  抽象类
    - 无法实例化对象
    - 子类必须重写抽象类中的纯虚函数，否则也属于抽象类
*/
class MonitorInter{

public:
    MonitorInter(){}
    virtual ~MonitorInter(){}
    // 纯虚函数
    virtual void UpdateOnce(monitor::proto::MonitorInfo* monitor_info) = 0;
    virtual void Stop() = 0;

};

};