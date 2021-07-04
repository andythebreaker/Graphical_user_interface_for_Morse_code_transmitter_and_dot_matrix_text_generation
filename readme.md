# M2 Installation and execution

## Installation 

There are already compiled files in the folder (it is not necessary to follow the steps and compile again)

Please move to the directory with the C file and "Makefile" file (that is, the directory at the same level as this readme file)

`make`

## Transfer to the target platform

`scp m2.ko em_up:`

## execution

ssh 進入upBoard 環境: `ssh em_up`

執行方式為: `sudo insmod m2.ko` 

結束方式為: `sudo rmmod m2.ko`
