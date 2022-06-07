# UNIX Time Converter

Jun 7 2022 - v1.1.0

UNIX Time Converter (unixtimeconv, unixtime) is a tool to convert Epoch timestamp to human readable format and vise-versa.

Usage:

	utime 1654627179000 -g         --->    2022/06/07.18:39:39
	utime 2022/06/07.18:39:39 -s   --->    1654616379000

Flags:
 -g : Display GMT time
 -s : DST on, summer time
 -w : DST off, winter time

By default DST is off and the local time is used (GMT off)

[Download package 📦](build-win32/utime-1.1.0.zip)

