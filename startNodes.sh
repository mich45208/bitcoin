#!/bin/bash 

mv node1 "/tmp/`date` node1"
mv node2 "/tmp/`date` node2"

mkdir node1 
mkdir node2 

./src/bitcoind -datadir=node1 -conf=../node1.conf &
./src/bitcoind -datadir=node2 -conf=../node2.conf &

if [ $? -ne 0 ]; then
    echo "Node loading Error."
    exit 1
fi

# ./src/bitcoind -datadir=/home/ittay/files/workspace/bitcoin-pouw/data1 -conf=../node1.conf &
# ./src/bitcoind -datadir=/home/ittay/files/workspace/bitcoin-pouw/data2 -conf=../node2.conf &
sleep 1
echo "Node1 and node 2 have started. loading private key for nodes..."
sleep 3
echo -n "..."
sleep 3
echo -n "..."
sleep 3
echo -n "..."
sleep 3
echo -n "..."
sleep 3
echo -n "..."

#Node 1
#Private Key
#L52Jh9XMMYbyU9mKCDNssADpUvGgP2FhKG5JUkaDfJpAnAcHtLLQ
#Address
#16NVusriGQXgWG21WzEvu25UNgVcFnP6ub
./src/bitcoin-cli -datadir=node1 -conf=../node1.conf importprivkey L52Jh9XMMYbyU9mKCDNssADpUvGgP2FhKG5JUkaDfJpAnAcHtLLQ

#Node 2
#Private key
#L12Xox1FpjaEgFPXEXpjrokd3H2DSEfJfr1GxA7JYS4EzzkoGcXN
#Address 
#1DYLcRkuTvnu7w1trrmnFJfwRNBHmjdgAP
./src/bitcoin-cli -datadir=node2 -conf=../node2.conf importprivkey L12Xox1FpjaEgFPXEXpjrokd3H2DSEfJfr1GxA7JYS4EzzkoGcXN

echo "Loading done."
