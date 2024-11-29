#include "mem_monitor.h"
#include "utils/read_file.h"
using namespace std;

static constexpr float KBTOGB = 1000 * 1000;

void monitor::MemMonitor::updateOnce(
    monitor::proto::MonitorInfo* monitor_info) {
    ReadFile mem_file("/proc/meminfo");
    vector<string> mem_datas;
    struct MemInfo mem_info;

    while (mem_file.readLine(&mem_datas)) {
        if (mem_datas[0] == "MemTotal:") {
            mem_info.total = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "MemFree:") {
            mem_info.free = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "MemAvailable:") {
            mem_info.avail = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Buffers:") {
            mem_info.buffers = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Cached:") {
            mem_info.cached = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "SwapCached:") {
            mem_info.swap_cached = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Active:") {
            mem_info.active = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Inactive:") {
            mem_info.in_active = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Active(anon):") {
            mem_info.active_anon = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Inactive(anon):") {
            mem_info.inactive_anon = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Active(file):") {
            mem_info.active_file = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Inactive(file):") {
            mem_info.inactive_file = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Dirty:") {
            mem_info.dirty = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Writeback:") {
            mem_info.writeback = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "AnonPages:") {
            mem_info.anon_pages = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "Mapped:") {
            mem_info.mapped = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "KReclaimable:") {
            mem_info.kReclaimable = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "SReclaimable:") {
            mem_info.sReclaimable = stoll(mem_datas[1]);
        } else if (mem_datas[0] == "SUnreclaim:") {
            mem_info.sUnreclaim = stoll(mem_datas[1]);
        }
        mem_datas.clear();

        auto mem_detail = monitor_info->mutable_mem_info();

        mem_detail->set_used_percent((mem_info.total - mem_info.avail) * 1.0 /
                                     mem_info.total * 100.0);
        mem_detail->set_total(mem_info.total / KBTOGB);
        mem_detail->set_free(mem_info.free / KBTOGB);
        mem_detail->set_avail(mem_info.avail / KBTOGB);
        mem_detail->set_buffers(mem_info.buffers / KBTOGB);
        mem_detail->set_cached(mem_info.cached / KBTOGB);
        mem_detail->set_swap_cached(mem_info.swap_cached / KBTOGB);
        mem_detail->set_active(mem_info.active / KBTOGB);
        mem_detail->set_inactive(mem_info.in_active / KBTOGB);
        mem_detail->set_active_anon(mem_info.active_anon / KBTOGB);
        mem_detail->set_inactive_anon(mem_info.inactive_anon / KBTOGB);
        mem_detail->set_active_file(mem_info.active_file / KBTOGB);
        mem_detail->set_inactive_file(mem_info.inactive_file / KBTOGB);
        mem_detail->set_dirty(mem_info.dirty / KBTOGB);
        mem_detail->set_writeback(mem_info.writeback / KBTOGB);
        mem_detail->set_anon_pages(mem_info.anon_pages / KBTOGB);
        mem_detail->set_mapped(mem_info.mapped / KBTOGB);
        mem_detail->set_kreclaimable(mem_info.kReclaimable / KBTOGB);
        mem_detail->set_sreclaimable(mem_info.sReclaimable / KBTOGB);
        mem_detail->set_sunreclaim(mem_info.sUnreclaim / KBTOGB);
    }
}