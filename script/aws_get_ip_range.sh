#!/bin/sh

# AWS: https://docs.aws.amazon.com/general/latest/gr/aws-ip-ranges.html

ERRORS_FOUND=()
if ! [ -x "$(command -v curl)" ]; then
	ERRORS_FOUND+=("curl is not installed and it's required.")
fi
if ! [ -x "$(command -v jq)" ]; then
	ERRORS_FOUND+=("jq is not installed and it's required.")
fi

if (( ${#ERRORS_FOUND[@]} > 0 )) ; then
	echo "Errors:"
	for (( i=0; i<${#ERRORS_FOUND[@]}; i++ ));
	do
	  echo -e "\t${ERRORS_FOUND[$i]}"
	done
	exit 1
fi

curl -s https://ip-ranges.amazonaws.com/ip-ranges.json | jq -r '.prefixes | .[].ip_prefix' 