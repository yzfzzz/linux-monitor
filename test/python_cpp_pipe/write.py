#-- coding:GBK --
import json
import os
import time
from pynvml import *

# 有名管道名称
pipe_path = "py_cpp_pipe.fifo"
nvmlInit()
# 如果管道不存在，则创建
if not os.path.exists(pipe_path):
    os.mkfifo(pipe_path)

pipe = os.open(pipe_path, os.O_WRONLY)
bytesToMB = 1024*1024
while(True):

    deviceCount = nvmlDeviceGetCount()
    data = []
    for i in range(deviceCount):
        handle = nvmlDeviceGetHandleByIndex(i)
        memory_info = nvmlDeviceGetMemoryInfo(handle)
        utilization = nvmlDeviceGetUtilizationRates(handle)
        # 要发送的数据
        # 读取的数据是 字节数，我们要转为MB
        # ! 12288 = 12884901888/(1024*1024)
        data.append({
            "ID": "GPU"+str(i),
            "DeviceName": nvmlDeviceGetName(handle).decode("utf-8"),
            "MemoryTotal": memory_info.total/bytesToMB,
            "MemoryFree": memory_info.free/bytesToMB,
            "MemoryUsed": memory_info.used/bytesToMB,
            "MemoryUtilization": utilization.memory,
            "GPUUtilization": utilization.gpu,
            "Temperature": nvmlDeviceGetTemperature(handle,NVML_TEMPERATURE_GPU),
            "FanSpeed": nvmlDeviceGetFanSpeed(handle),
            "PowerStstus": nvmlDeviceGetPowerState(handle)
        })
    # 将数据封装为 JSON 格式字符串
    json_data = json.dumps(data)
    print(json_data)
    max_attempts = 3
    attempt_count = 0
    while attempt_count < max_attempts:
        try:
            # 写入数据
            os.write(pipe, json_data.encode("utf-8"))
            # 睡眠 3 秒
            time.sleep(3)
            break
        except BrokenPipeError as e:
            print("Accidentally disconnected!", e)
            attempt_count += 1
            time.sleep(2*attempt_count)
    if attempt_count == max_attempts:
           print("Timeout! Program ending!")
           os._exit(0)




