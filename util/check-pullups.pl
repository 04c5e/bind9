#!/usr/bin/perl
#
# Copyright (C) 2004  Internet Systems Consortium, Inc. ("ISC")
# Copyright (C) 2001-2003  Internet Software Consortium.
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
# REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
# AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
# INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
# LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
# OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.

# $Id: check-pullups.pl,v 1.4.2.2.8.3 2004/03/12 10:31:27 marka Exp $

# Given two CHANGES files, list [bug] entries present in the
# first one but not in the second one.
#

use FileHandle;

# $/ = "";

# Read the CHANGES file $fn and return a hash of change
# texts and categories indexed by change number.

sub readfile {
	my ($fn) = @_;
	my $fh = new FileHandle($fn, "r")
	    or die "open: $fn: $!";
	
	my $changes = { };

	my ($changeid, $category);

	$changeid = "none";
	$category = "none";

	while (<$fh>) {
		if (m/^\s*(\d+)\.\s+\[(\w+)\]/) {
			$changeid = $1;
			$category = $2;
			# print "*** $1 $2\n";
		} elsif (m/---.* released ---/) {
			$changeid = "none";
			$category = "none";
			next;
		} elsif (m/^# /) {
			$changeid = "none";
			$category = "none";
			next;
		}
		if ($changeid eq "none") {
			next;
		}
		$changes->{$changeid}->{text} .= $_;
		$changes->{$changeid}->{category} = $category;
	}

	return $changes;
}

@ARGV == 2 || @ARGV == 3 or die "usage: $0 changes-file-1 changes-file-2\n";

my $c1 = readfile($ARGV[0]);
my $c2 = readfile($ARGV[1]);
if (@ARGV == 3) {
	$c3 = readfile($ARGV[2]);
} else {
	my $c3 = { };
}

foreach my $c (sort {$a <=> $b} keys %$c1) {
	my $category = $c1->{$c}->{category};
	if (($category ne "placeholder") &&
	    !exists($c2->{$c}) && !exists($c3->{$c})) {
		print $c1->{$c}->{text};
	}
}
