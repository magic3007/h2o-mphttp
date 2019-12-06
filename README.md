# CompNet.Lab3: Multipath HTTP/2

Due: Dec 9 23:59:59

[TOC]

## Introduction

In the third lab, you are required to develop an HTTP/2-based downloader, which fetches HTTP objects over multiple network paths. The idea behind this lab originates from the paper [_MP-H2: A Client-only Multipath Solution for HTTP/2_]() (Nikravesh et al.). Before starting working on the lab, you are supposed to carefully read the paper for a better understanding of how the downloader functions. 

For simplicity, we won't implement the downloader on an Android device, as described in the paper. Instead, the downloader will be implemented on the top of an HTTP/2 library in the C programming language. Also, to make things more comfortable, we omit the DNS lookup here, and the IP addresses of the servers are given.

While the paper focuses on two paths, it is possible to apply this design to three or more paths. In the lab, we are going to implement the downloader in a more scalable way. Your downloader should be able to leverage three paths.

Your downloader will be run in an emulated environment with exactly three CDNs and one client. In order to emulate a network environment with two or more CDN servers, we will take advantage of [Linux namespaces](http://man7.org/linux/man-pages/man7/namespaces.7.html) to set up a virtual network on a Linux machine. Three CDN servers serve the same content. Your target in this lab is to reduce the downloading time for a given file. We will measure the downloading time of your implementation under varied network conditions.

After finishing this lab, you are expected to:

- Get a taste of how the multipath solution boosts downloading performance.
- Be familiar with new features in HTTP/2, including stream multiplexing, flow control, and application-layer PING.
- Be capable of adding new features to an existing production-level library.

## Handout Instructions

This section will instruct you to set up the development environment on a Linux machine. We will evaluate your programs in the same environment.

### Download the HTTP/2 Library

Our lab is based on a mature HTTP/2 Library, named [H2O](https://github.com/h2o/h2o). After building the library on your Linux machine, you can:

- Follow the [Wiki](https://github.com/h2o/h2o/wiki) to run the static content server. When setting up the SSL (443 port), you can create a self-signed certificate and disable certificate verification on the client.
- Develop your multipath downloader with the help of the library. See [Hints on Implementation]() for details.

### Emulate Network Condition

Here we introduce two tools: NetEm and Mahimahi.

[NetEm](http://man7.org/linux/man-pages/man8/tc-netem.8.html) is an enhancement of the Linux traffic control facilities that allow adding delay, packet loss, duplication and more other characteristics to packets outgoing from a selected network interface. 

[Mahimahi](http://mahimahi.mit.edu) is a set of lightweight tools that can emulate network conditions. Refer to the paper [_Mahimahi: Accurate Record-and-Replay for HTTP_]() (Netravali et al.) for a better understanding of how it works. You can find some network condition traces [here](https://github.com/ravinet/mahimahi/tree/master/traces).

### Create a Virtual Network

Again, [vnetUtils]() is our good friend. You may find its usage in the handout of lab2. 

In lab3, you can create a virtual network by running `makeVNet` with the following configuration (note that the first and the last two blank lines are left intentionally):

```

3
0 1 10.100.1
0 2 10.100.2
0 3 10.100.3


```

By now, in addition to the default namespace, you have already set up three virtual network namespaces. This can be verified by running the command `ip netns list`. You can run a content server in each network namespace, and your downloader in the default namespace.

The aforementioned configuration is just an example. When testing your downloader, you can create virtual networks with any topologies. 

## Tasks

### Programming Task (30%)

Your downloader should work after receiving these arguments:

- Three servers' IP addresses `<host#1> <host#2> <host#3>`.
- `-t <path>`: the **absolute** file path on the CDN server. In other words, you can send a request `https://<host#i><path>` to download the file. **You oughtn't to verify the server certificates.**
- `-o <file>`: save the download to `<file>`. 

These are possible inputs (your downloader is `h2o-mphttp`):

```
$ ./h2o-mphttp 10.100.1.2 10.100.2.2 10.100.3.2 -o 2M.bin -t /2M_file
$ ./h2o-mphttp 10.100.1.2 -t /dir1/dir2/hello.flv 10.100.2.2 10.100.3.2 -o ./hello.flv
$ ./h2o-mphttp -o /tmp/hello.flv 10.100.3.2 10.100.1.2 10.100.2.2 -t /dir1/dir2/hello.flv
```

We recommend [getopt()](http://man7.org/linux/man-pages/man3/getopt.3.html) here, a function that parses the command-line arguments.

#### Test and Evaluation

The grading of the final hand-in will be based on the download time your downloader takes on the given traces. Each trace contains three bandwidth and delay profiles used to specify the network condition (imposed by Mahimahi), one for each CDN server. Final evaluation traces are kept secret.

### Writing Task (30%)

Describe your implementation:

- How do you estimate the bandwidth and the delay of a path?
- How do you assign jobs to the three paths?
- What features (pipelining, eliminating tail byes, etc.) do you implement? And how do you implement them? You are encouraged to write down other design aspects of your implementation.

In addition to the questions above, Comments and ideas on the multipath HTTP solution are also welcomed!

### Drawing Task (40%)

You are required to draw some figures to prove that your downloader does work: 

- under static network conditions
- under varying bandwidth conditions (you can use the [traces](https://github.com/ravinet/mahimahi/tree/master/traces) provided by Mahimahi)
- with a high-bandwidth path and two low-bandwidth paths
- with a short-delay path and two long-delay paths

You are encouraged to submit other figures that you find interesting.

Here are two powerful plotting tools. You can choose other tools at your convenience.

- [Gnuplot](http://www.gnuplot.info): a portable command-line driven graphing utility for Linux, OS/2, MS Windows, OSX, VMS, and many other platforms. 
- [Matplotlib](https://matplotlib.org/index.html): a Python 2D plotting library which produces publication quality figures in a variety of hardcopy formats and interactive environments across platforms.

Here is an example using Gnuplot (Note that the example only uses two paths, but your implementation should use three paths):

```
$ cat plot.gnu
set terminal png transparent enhanced font "arial,10" fontscale 1.0 size 600, 400 
set output 'gnuplot.png'
set style increment default
set style data lines
set xlabel 'time elapsed (ms)'
set ylabel 'byte range (Kb)'
plot [0:1600][0:2200] "1.dat" title '1' with points, "2.dat" title '2' with points 
$ head 1.dat 
0 0
64 16
77 32
91 48
102 63
117 79
130 95
145 111
159 127
172 143
$ gnuplot -c plot.gnu 
```

Result:

![output copy](assets/gnuplot.png)

## Hints on Implementation

### RFCs are Important

- [RFC 2616](https://httpwg.org/specs/rfc7233.html): Hypertext Transfer Protocol (HTTP/1.1): Range Requests
- [RFC 2818](https://httpwg.org/specs/rfc2818.html): HTTP Over TLS
- [RFC 7540](https://httpwg.org/specs/rfc7540.html): Hypertext Transfer Protocol Version 2 (HTTP/2)
- [RFC 7639](https://httpwg.org/specs/rfc7639.html): The ALPN HTTP Header Field 

In this lab, you don't have to follow the RFC strictly, but your downloader should be able to communicate with the H2O static content server.

### Where to Start

In H2O, you may find these codes useful:

- `/src/httpclient.c`: A good starting point. Try to build and run it. Use `-k` option to disable certificate verification. You can build the downloader on the top of this file.
- `/lib/common/httpclient.c`: How to connect to a server
- `/lib/common/http2client.c`: HTTP/2 functionalities
- `/lib/common/socket/evloop.c.h`: Event-driven programming model
- `/deps/klib/khash.h`: A standalone and lightweight C library implementing generic [hash table](https://en.wikipedia.org/wiki/Hash_table) with open addressing. See [here](http://attractivechaos.github.io/klib/#About) for details.

## Handin Instructions

In this lab, you should submit a directory named `lab3` containing the following items in an archive named `lab3-[your student ID]-[your name].tar`:

- `src/`: Source code of your programs ([Programming Tasks]()) 
- `bin/h2o-mphttp`: Your downloader in binary executable format, which can run on Ubuntu.([Programming Tasks]()) 
- `report.pdf`: A single document that answers the questions in [Writing Tasks]()
- `plot/`: Figures showing that your downloader does work. ([Drawing Tasks]())

Please send an email with title `lab3-[your student ID]-[your name]` to [kenuo.xu@pku.edu.cn](mailto:kenuo.xu@pku.edu.cn). Missing the deadlines incurs a penalty.
