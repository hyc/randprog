#! /usr/bin/env python
# -*- coding: utf-8 -*-

"""
usage: python test.py $loop_count

deps:
	pip install moneropy requests
"""


import sys
import os
import moneropy.account
import requests
import json
import subprocess


randprog_path = './randprog'

def get_random_blockhashing_blob():
    address = moneropy.account.gen_new_wallet()[-1]	
    print 'address: ' + address
    blockhashing_blob = get_blockhashing_blob(address)
    return blockhashing_blob


def get_blockhashing_blob(address):
    data = {"method": "getblocktemplate", "params": {"reserve_size": 8, "wallet_address": "{}"}}
    data['params']['wallet_address'] = address
    x = requests.post('http://node.moneroworld.com:18089/json_rpc', json=data)
    result = x.json()['result']
    
    blockhashing_blob = result['blockhashing_blob']
    print 'blockhashing_blob: ' + blockhashing_blob
    return blockhashing_blob




if __name__ == "__main__":
    loop_count = 1
    if len(sys.argv) == 2:
        loop_count = int(sys.argv[1])
        pass

    assert loop_count >= 1, loop_count
    for x in xrange(0, loop_count):
        blockhashing_blob = get_random_blockhashing_blob()
        retcode = subprocess.call([randprog_path, str(blockhashing_blob)])
        print 'blockhashing_blob: ' + blockhashing_blob
        assert retcode == 0

        pass
    pass
