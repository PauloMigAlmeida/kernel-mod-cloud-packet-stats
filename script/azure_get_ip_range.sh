#!/bin/sh

# Azure: https://www.microsoft.com/en-us/download/details.aspx?id=56519

# Sneaky way to find the direct download URL for the IP address ranges
JSON_URL="$(curl -s 'https://www.microsoft.com/en-us/download/confirmation.aspx?id=56519' | grep 'downloadretry' | grep -oP '(?<=\")https.*?(?=\")')"

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

#
# Filter out IPv6 ranges
#
# TODO: Support IPv6
#
only_ipv4() {
	grep -E '^[0-9]+\.[0-9]+\.[0-9]+\.[0-9]+'
}

curl -s $JSON_URL | jq -r '.values | .[].properties.addressPrefixes[]' | only_ipv4
