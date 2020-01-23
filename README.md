# kernel-mod-cloud-packet-stats
A module for the Linux kernel that counts how many packets to each cloud provider you machine has sent

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
