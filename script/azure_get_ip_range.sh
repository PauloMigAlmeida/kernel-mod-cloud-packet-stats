#!/bin/sh

# Azure: https://www.microsoft.com/en-us/download/details.aspx?id=56519

# I get the feeling that this URL will eventually change...so I'm making
# this a variable
JSON_URL="https://download.microsoft.com/download/7/1/D/71D86715-5596-4529-9B13-DA13A5DE5B63/ServiceTags_Public_20200112.json"

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

curl -s $JSON_URL | jq -r '.values | .[].properties.addressPrefixes[]'