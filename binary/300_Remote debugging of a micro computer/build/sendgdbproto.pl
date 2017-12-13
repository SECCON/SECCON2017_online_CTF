#!/usr/bin/perl

# $g
# $G<values>
# $m<addr>,<size>
# $M<addr>,<size>:<values>
# $s
# $c
# $D
# $k
#
# 0000000000000000 -> 0*,
# 00000000000000000000000000000000 -> 0*<
# 0000000000000000000000000000000000000000000000000000000000000000 -> 0*\

$| = 1;

while (<>) {
	chomp;

	if (!/^[a-zA-Z]+/) {
		next;
	}

	if ($_ eq "exit") {
		last;
	}

	$checksum = 0;
	for ($i = 0; $i < length($_); $i++) {
		$c = substr($_, $i, 1);
		$n = unpack("C", $c);
		$checksum = ($checksum + $n) & 0xff;
	}
	$s = '$' . $_ . '#' . sprintf("%02x", $checksum);
	print "$s+";
	print STDERR "\nSEND: $s+\nRECV: ";

	sleep(1);
}

sleep(3);
