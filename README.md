# kernel-mod-cloud-packet-stats
A module for the Linux kernel that counts how many packets to each cloud provider your machine has sent

## Article: (High-level):
https://deepdives.medium.com/how-much-traffic-from-your-machine-goes-to-the-cloud-63792e670b28

## Installation
### Dependencies:
```
# dnf install gcc make jq curl dig
```
### Build:
```
$ git clone git@github.com:PauloMigAlmeida/kernel-mod-cloud-packet-stats.git --depth=1
$ cd kernel-mod-cloud-packet-stats
$ make modules
```
### Install:
```
# insmod kernel_cloud_packet_stats.ko
```
### Verify:
```
$ dmesg -k
```
You should see something akin to
```[ 2923.180487] kernel_cloud_packet_stats: module loaded from 0x00000000b710fd65```

### View packet counters
```
$ tail /sys/cloud_packet_stats/*

# Results:
==> /sys/cloud_packet_stats/aws <==
1256

==> /sys/cloud_packet_stats/azure <==
575

==> /sys/cloud_packet_stats/gcp <==
472
```
Here goes a list of websites that you can use to force counters to go up:

|  Cloud Provider| URL |
|---|---|
| AWS | https://www.cloudping.info/  |
| GCP | http://www.gcping.com/  |
| Azure | https://cloudpingtest.com/azure  |

### Reset counters
```
$ sudo -s
# echo reset > /sys/cloud_packet_stats/aws
```
### Uninstall
```
# rmmod kernel_cloud_packet_stats
```

### Clean
```
$ make clean
```
