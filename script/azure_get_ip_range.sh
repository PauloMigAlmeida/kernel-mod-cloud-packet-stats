#!/bin/sh

# Azure: https://www.microsoft.com/en-us/download/details.aspx?id=56519

# I get the feeling that this URL will eventually change...so I'm making
# this a variable
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

curl -s $JSON_URL | jq -r '.values | .[].properties.addressPrefixes[]'
