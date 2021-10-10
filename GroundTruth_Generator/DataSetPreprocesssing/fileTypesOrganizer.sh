#!/bin/bash                                                                                                                                                                                                 

# Run this script with the arugment of the directory contains all the data-set that contain different file types.
workigDirectory=$1

Recurse_save_extention (){

	for filename in "$1"/*; do
		if [ -d "$filename" ]; then
			echo "Check folder $filename .. \n"
			Recurse_save_extention "$filename"
		elif [[ -f "$filename" ]]; then
			echo "File $filename will be moved to its extention folder\n"
			base=${filename%.*}
			ext=${filename#$base.}
			mkdir -p "$workigDirectory/${ext}"
			mv "$filename" "$workigDirectory/${ext}"
		fi
	done

}

Recurse_save_extention "$workigDirectory"