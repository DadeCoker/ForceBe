#!/bin/bash

bos_path="../bos"

echo bos path : $bos_path

pip install kconfiglib
pip install psutil

python b_config.py $bos_path

