#!/bin/sh

DOX_PATH="./dox_comments/header_files"
WOLFSSL_PATH="../wolfssl"
WOLFCRYPT_PATH="$WOLFSSL_PATH/wolfcrypt"
OPENSSL_PATH="$WOLFSSL_PATH/openssl"
API_STRIP='/^ *WOLFSSL_API/ {
			:a
			/;/ {
				y/\n/ /
				s/^ *//
				s/ \{1,\}/ /g
				s/( /(/g
				p
				d
			}
			N
			b a
		}'

for h_file in "$DOX_PATH"/*
do
	h_file=$(basename "$h_file")
	sed -n "$API_STRIP" "$DOX_PATH/$h_file" > dox_api.txt

	if test -f "$WOLFSSL_PATH/$h_file"; then
		h_file_path="$WOLFSSL_PATH/$h_file"
	elif test -f "$WOLFCRYPT_PATH/$h_file"; then
		h_file_path="$WOLFCRYPT_PATH/$h_file"
	elif test -f "$OPENSSL_PATH/$h_file"; then
		h_file_path="$OPENSSL_PATH/$h_file"
	else
		continue
	fi

	sed -n "$API_STRIP" "$h_file_path" > wolf_api.txt

    sort dox_api.txt -o dox_api_sorted.txt
    sort wolf_api.txt -o wolf_api_sorted.txt
	common=$(comm -3 dox_api_sorted.txt wolf_api_sorted.txt)
	if test ! -z "$common"
	then
		echo file "$h_file"
		echo "$common"
	fi
done

rm -f dox_api.txt
rm -f dox_api_sorted.txt
rm -f wolf_api.txt
rm -f wolf_api_sorted.txt
