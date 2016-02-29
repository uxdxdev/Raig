cd /home/pi/projects/raig
git pull origin develop
clean.sh
mkdir linux-build && cd linux-build
cmake ..
make
/home/pi/projects/raig/bin/raig &
