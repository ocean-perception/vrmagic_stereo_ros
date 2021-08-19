#!/bin/bash

check_acquisition () {
	## Parameters ##
	WATCHMAN_FILE_PATH=/media/driftcam/sdcard/watchman/path.txt
	RESTARTS_LOG_PATH=/media/driftcam/sdcard/log/watchman_restarts.txt

	if test -f "$WATCHMAN_FILE_PATH"; then
		echo "$WATCHMAN_FILE_PATH exists -> we are recording data"
	else
		echo "Mission path $WATCHMAN_FILE_PATH does not exist. Standing by."
		return
	fi

	{ IFS= read -r DIVE_DIR && IFS= read -r CAM_A && IFS= read -r CAM_B; } < $WATCHMAN_FILE_PATH
	if [ -z ${CAM_B} ]; then
		echo "Error: Watchman file does not contain camera names. Update seaxerock4control."
		return
	fi

	IMAGE_DIR=$DIVE_DIR

	if [ ! -d $IMAGE_DIR ]; then
		echo "Directory $IMAGE_DIR does not yet exist. Creating it..."
		mkdir $IMAGE_DIR
		echo "...done."
	fi

	COUNT_A=`find $IMAGE_DIR$CAM_A'/'${CAM_A}'_'* -type f | wc -l`
	COUNT_B=`find $IMAGE_DIR$CAM_B'/'${CAM_B}'_'* -type f | wc -l`

	if [ ! -f $IMAGE_DIR$CAM_A'-count.txt' ]; then
		echo "First image count: $CAM_A: $COUNT_A, $CAM_B: $COUNT_B"
		echo "Image counter textfiles don't exist yet. Creating them..."
		echo $COUNT_A > $IMAGE_DIR$CAM_A'-count.txt'
		echo $COUNT_B > $IMAGE_DIR$CAM_B'-count.txt'
		echo "...done."
		return
	fi

	COUNT_A_PREV=$(cat $IMAGE_DIR$CAM_A'-count.txt')
	COUNT_B_PREV=$(cat $IMAGE_DIR$CAM_B'-count.txt')
	echo "Previous image count $CAM_A: $COUNT_A_PREV, $CAM_B: $COUNT_B_PREV"
	echo "Current  image count $CAM_A: $COUNT_A, $CAM_B: $COUNT_B"

	if (( $COUNT_A <= $COUNT_A_PREV )) || (( $COUNT_B <= $COUNT_B_PREV )); then
		echo "Image count is NOT increasing. Rebooting in 10s..."
		mkdir -p $(dirname RESTARTS_LOG_PATH)
		echo $(date '+%Y%m%d_%H%M%S') >> $RESTARTS_LOG_PATH
		sleep 10
		sudo reboot
	else
		echo "Image count IS increasing. Carry on."
		echo $COUNT_A > $IMAGE_DIR$CAM_A'-count.txt'
		echo $COUNT_B > $IMAGE_DIR$CAM_B'-count.txt'
	fi
}

echo Infinite watchman starting...
sleep 120
while true; do
	check_acquisition
	sleep 90
done
