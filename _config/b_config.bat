@echo off

set bos_path=../bos

echo bos path : %bos_path%

:checkenv
echo ###############################################################
echo Check environment variables
echo ############################################################### 

echo %path% | findstr /I "python"

if %ERRORLEVEL% == 0 (
	echo ###############################################################
	echo Existing python environment
	echo ###############################################################
) else (
	echo ###############################################################
	echo No python environment
	echo ###############################################################
	pause
	goto checkenv
)

python -m pip install --upgrade pip

pip install kconfiglib
pip install windows-curses

python b_config.py %bos_path%






