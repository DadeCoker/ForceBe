#!/bin/bash

bos_path="../bos"

echo bos path : $bos_path

python -m pip install --upgrade pip

pip install kconfiglib

python b_config.py $bos_path

