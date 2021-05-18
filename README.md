# UDP-Multicast-NetCat

Simple c++ implementation for a UDP Multicast netcat whose usage is similar to [linux nc](https://linux.die.net/man/1/nc) which doesn't support connection/listen for UDP Multicast. This program only implemented very basic and straightforward usage of linux nc. 



## Compile

This implementation utilizes the famous c++ boost library, to install boost on ubuntu:

```shell
sudo apt install libboost-dev
sudo apt install libboost-all-dev
```

Then compile using:

```shell
make
```

   

## Usage

```shell
mnc ([-l listen/server mode]) [destination multicast ip address] [port]
```

* -l flag: run this mnc session as server mode to listen to the given endpoint(ip addr+port) for udp packets, and mnc will then print the packets in stdout 
  * If this flag is not used, mnc would be in send/client mode which will take user's typed input in stdin and send it to the given endpoint.

#### Example

1. In one terminal, type:

   ```shell
   ./mnc 224.5.23.2 10020
   ```

   to open a broadcast session that detects your keyboard input and send it to the 224.5.23.2:10020 endpoint.

2. In another terminal, type:

   ```shell
   ./mnc -l 224.5.23.2 10020
   ```

   to listen & print the broadcast from the previous opened terminal.

3. Then simply type random stuff in terminal 1 and observe terminal 2 for the echo of what you typed. (since it's udp, the order of running the sender / listener doesn't matter)

