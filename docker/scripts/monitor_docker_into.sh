#!/usr/bin/env bash
#启动一个docker容器；
xhost +local:root 1>/dev/null 2>&1
docker exec \
    -u root \
    -it server_monitor \
    /bin/bash
xhost -local:root 1>/dev/null 2>&1
