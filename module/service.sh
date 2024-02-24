MODDIR=${0%/*}
chmod 777 /data/adb/modules/cpulog/cpulog
/data/adb/modules/cpulog/cpulog &
chmod 777 /data/adb/modules/cpulog/gpulog
/data/adb/modules/cpulog/gpulog &
