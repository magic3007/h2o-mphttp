#!/bin/bash
#
#  tc uses the following units when passed as a parameter.
#  kbps: Kilobytes per second
#  mbps: Megabytes per second
#  kbit: Kilobits per second
#  mbit: Megabits per second
#  bps: Bytes per second
#       Amounts of data can be specified in:
#       kb or k: Kilobytes
#       mb or m: Megabytes
#       mbit: Megabits
#       kbit: Kilobits
#  To get the byte figure from bits, divide the number by 8 bit
#

#
# Name of the traffic control command.
TC=tc

# The network interface we're planning on limiting bandwidth.
IF=H2veth0-1             # Interface

# Download limit (in mega bits)
DNLD=1mbit          # DOWNLOAD Limit

# Upload limit (in mega bits)
UPLD=1mbit          # UPLOAD Limit

# IP address of the machine we are controlling
IP=10.100.1.2     # Host IP

# Filter options for limiting the intended interface.
U32="$TC filter add dev $IF protocol ip parent 1:0 prio 1 u32"

start() {

	echo starting

# We'll use Hierarchical Token Bucket (HTB) to shape bandwidth.
# For detailed configuration options, please consult Linux man
# page.

$TC qdisc add dev $IF root handle 1: htb default 30
$TC class add dev $IF parent 1: classid 1:1 htb rate $DNLD
$TC class add dev $IF parent 1: classid 1:2 htb rate $UPLD
$U32 match ip dst $IP/24 flowid 1:1
$U32 match ip src $IP/24 flowid 1:2

# The first line creates the root qdisc, and the next two lines
# create two child qdisc that are to be used to shape download
# and upload bandwidth.
#
# The 4th and 5th line creates the filter to match the interface.
# The 'dst' IP address is used to limit download speed, and the
# 'src' IP address is used to limit upload speed.

}

stop() {

# Stop the bandwidth shaping.
$TC qdisc del dev $IF root


}

tc qdisc add dev H2veth0-1 parent 1:1 handle 10: tbf rate 256kbit buffer 1600 limit 3000
# stop
# start
#