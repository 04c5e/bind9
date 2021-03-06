#!/bin/sh -e
#
# Copyright (C) Internet Systems Consortium, Inc. ("ISC")
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# See the COPYRIGHT file distributed with this work for additional
# information regarding copyright ownership.

# shellcheck source=conf.sh
. "$SYSTEMTESTTOP/conf.sh"

echo_i "ns6/setup.sh"

setup() {
	zone="$1"
	echo_i "setting up zone: $zone"
	zonefile="${zone}.db"
	infile="${zone}.db.infile"
	echo "$zone" >> zones.2
}

private_type_record() {
	_zone=$1
	_algorithm=$2
	_keyfile=$3

	_id=$(keyfile_to_key_id "$_keyfile")

	printf "%s. 0 IN TYPE65534 %s 5 %02x%04x0000\n" "$_zone" "\\#" "$_algorithm" "$_id"
}


# Make lines shorter by storing key states in environment variables.
H="HIDDEN"
R="RUMOURED"
O="OMNIPRESENT"
U="UNRETENTIVE"

#
# The zones at algorithm-roll.kasp represent the various steps of a ZSK/KSK
# algorithm rollover.
#

# Step 1:
# Introduce the first key. This will immediately be active.
setup step1.algorithm-roll.kasp
KSK=$($KEYGEN -a RSASHA1 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK=$($KEYGEN -a RSASHA1 -L 3600 $zone 2> keygen.out.$zone.2)
TactN="now"
$SETTIME -s -P $TactN -A $TactN  -g $O -k $O $TactN -r $O $TactN -d $O $TactN "$KSK" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TactN -A $TactN  -g $O -k $O $TactN -z $O $TactN              "$ZSK" > settime.out.$zone.2 2>&1
cat template.db.in "${KSK}.key" "${ZSK}.key" > "$infile"
private_type_record $zone 5 "$KSK" >> "$infile"
private_type_record $zone 5 "$ZSK" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 2:
# After the publication interval has passed the DNSKEY is OMNIPRESENT.
setup step2.algorithm-roll.kasp
KSK1=$($KEYGEN -a RSASHA1 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK1=$($KEYGEN -a RSASHA1 -L 3600 $zone 2> keygen.out.$zone.2)
KSK2=$($KEYGEN -a ECDSAP256SHA256 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK2=$($KEYGEN -a ECDSAP256SHA256 -L 3600 $zone 2> keygen.out.$zone.2)
# The time passed since the new algorithm keys have been introduced is 3 hours.
TactN="now-3h"
TpubN1="now-3h"
TactN1="now+6h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -r $O $TactN  -d $O $TactN  "$KSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -z $O $TactN                "$ZSK1" > settime.out.$zone.2 2>&1
$SETTIME -s -P $TpubN1 -A $TpubN1        -g $O -k $R $TpubN1 -r $R $TpubN1 -d $H $TpubN1 "$KSK2" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $R $TpubN1 -z $R $TpubN1               "$ZSK2" > settime.out.$zone.2 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${KSK1}.state"
echo "Lifetime: 0" >> "${ZSK1}.state"
cat template.db.in "${KSK1}.key" "${ZSK1}.key" "${KSK2}.key" "${ZSK2}.key" > "$infile"
private_type_record $zone 5  "$KSK1" >> "$infile"
private_type_record $zone 5  "$ZSK1" >> "$infile"
private_type_record $zone 13 "$KSK2" >> "$infile"
private_type_record $zone 13 "$ZSK2" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 3:
# The zone signatures are also OMNIPRESENT.
setup step3.algorithm-roll.kasp
KSK1=$($KEYGEN -a RSASHA1 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK1=$($KEYGEN -a RSASHA1 -L 3600 $zone 2> keygen.out.$zone.2)
KSK2=$($KEYGEN -a ECDSAP256SHA256 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK2=$($KEYGEN -a ECDSAP256SHA256 -L 3600 $zone 2> keygen.out.$zone.2)
# The time passed since the new algorithm keys have been introduced is 9 hours.
TactN="now-9h"
TpubN1="now-9h"
TactN1="now"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -r $O $TactN  -d $O $TactN  "$KSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -z $O $TactN                "$ZSK1" > settime.out.$zone.2 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $O $TpubN1 -r $O $TpubN1 -d $H $TpubN1 "$KSK2" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $O $TpubN1 -z $R $TpubN1               "$ZSK2" > settime.out.$zone.2 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${KSK1}.state"
echo "Lifetime: 0" >> "${ZSK1}.state"
cat template.db.in "${KSK1}.key" "${ZSK1}.key" "${KSK2}.key" "${ZSK2}.key" > "$infile"
private_type_record $zone 5  "$KSK1" >> "$infile"
private_type_record $zone 5  "$ZSK1" >> "$infile"
private_type_record $zone 13 "$KSK2" >> "$infile"
private_type_record $zone 13 "$ZSK2" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 4:
# The DS is swapped and can become OMNIPRESENT.
setup step4.algorithm-roll.kasp
KSK1=$($KEYGEN -a RSASHA1 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK1=$($KEYGEN -a RSASHA1 -L 3600 $zone 2> keygen.out.$zone.2)
KSK2=$($KEYGEN -a ECDSAP256SHA256 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK2=$($KEYGEN -a ECDSAP256SHA256 -L 3600 $zone 2> keygen.out.$zone.2)
# The time passed since the DS has been swapped is 29 hours.
TactN="now-38h"
TpubN1="now-38h"
TactN1="now-29h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -r $O $TactN  -d $U $TactN1 "$KSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -z $O $TactN                "$ZSK1" > settime.out.$zone.2 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $O $TpubN1 -r $O $TpubN1 -d $R $TactN1 "$KSK2" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $O $TpubN1 -z $R $TpubN1               "$ZSK2" > settime.out.$zone.2 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${KSK1}.state"
echo "Lifetime: 0" >> "${ZSK1}.state"
cat template.db.in "${KSK1}.key" "${ZSK1}.key" "${KSK2}.key" "${ZSK2}.key" > "$infile"
private_type_record $zone 5  "$KSK1" >> "$infile"
private_type_record $zone 5  "$ZSK1" >> "$infile"
private_type_record $zone 13 "$KSK2" >> "$infile"
private_type_record $zone 13 "$ZSK2" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 5:
# The DNSKEY is removed long enough to be HIDDEN.
setup step5.algorithm-roll.kasp
KSK1=$($KEYGEN -a RSASHA1 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK1=$($KEYGEN -a RSASHA1 -L 3600 $zone 2> keygen.out.$zone.2)
KSK2=$($KEYGEN -a ECDSAP256SHA256 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK2=$($KEYGEN -a ECDSAP256SHA256 -L 3600 $zone 2> keygen.out.$zone.2)
# The time passed since the DNSKEY has been removed is 2 hours.
TactN="now-40h"
TpubN1="now-40h"
TactN1="now-31h"
TremN="now-2h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $U $TremN  -r $U $TremN  -d $H $TremN  "$KSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $U $TremN  -z $U $TremN                "$ZSK1" > settime.out.$zone.2 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $O $TpubN1 -r $O $TpubN1 -d $O $TremN  "$KSK2" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $O $TpubN1 -z $R $TpubN1               "$ZSK2" > settime.out.$zone.2 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${KSK1}.state"
echo "Lifetime: 0" >> "${ZSK1}.state"
cat template.db.in "${KSK1}.key" "${ZSK1}.key" "${KSK2}.key" "${ZSK2}.key" > "$infile"
private_type_record $zone 5  "$KSK1" >> "$infile"
private_type_record $zone 5  "$ZSK1" >> "$infile"
private_type_record $zone 13 "$KSK2" >> "$infile"
private_type_record $zone 13 "$ZSK2" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 6:
# The RRSIGs have been removed long enough to be HIDDEN.
setup step6.algorithm-roll.kasp
KSK1=$($KEYGEN -a RSASHA1 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK1=$($KEYGEN -a RSASHA1 -L 3600 $zone 2> keygen.out.$zone.2)
KSK2=$($KEYGEN -a ECDSAP256SHA256 -f KSK -L 3600 $zone 2> keygen.out.$zone.1)
ZSK2=$($KEYGEN -a ECDSAP256SHA256 -L 3600 $zone 2> keygen.out.$zone.2)
# Additional time passed: 7h.
TactN="now-47h"
TpubN1="now-47h"
TactN1="now-38h"
TremN="now-9h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $U $TremN  -r $U $TremN  -d $H $TremN  "$KSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $U $TremN  -z $U $TremN                "$ZSK1" > settime.out.$zone.2 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $O $TpubN1 -r $O $TpubN1 -d $O $TremN  "$KSK2" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TactN1        -g $O -k $O $TpubN1 -z $R $TpubN1               "$ZSK2" > settime.out.$zone.2 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${KSK1}.state"
echo "Lifetime: 0" >> "${ZSK1}.state"
cat template.db.in "${KSK1}.key" "${ZSK1}.key" "${KSK2}.key" "${ZSK2}.key" > "$infile"
private_type_record $zone 5  "$KSK1" >> "$infile"
private_type_record $zone 5  "$ZSK1" >> "$infile"
private_type_record $zone 13 "$KSK2" >> "$infile"
private_type_record $zone 13 "$ZSK2" >> "$infile"
$SIGNER -S -x -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

#
# The zones at csk-algorithm-roll.kasp represent the various steps of a CSK
# algorithm rollover.
#

# Step 1:
# Introduce the first key. This will immediately be active.
setup step1.csk-algorithm-roll.kasp
echo "$zone" >> zones
CSK=$($KEYGEN -k csk-algoroll -l policies/csk1.conf $zone 2> keygen.out.$zone.1)
TactN="now"
$SETTIME -s -P $TactN -A $TactN  -g $O -k $O $TactN -r $O $TactN -z $O $TactN -d $O $TactN "$CSK" > settime.out.$zone.1 2>&1
cat template.db.in "${CSK}.key" > "$infile"
private_type_record $zone 5 "$CSK" >> "$infile"
$SIGNER -S -x -z -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 2:
# After the publication interval has passed the DNSKEY is OMNIPRESENT.
setup step2.csk-algorithm-roll.kasp
CSK1=$($KEYGEN -k csk-algoroll -l policies/csk1.conf $zone 2> keygen.out.$zone.1)
CSK2=$($KEYGEN -k csk-algoroll -l policies/csk2.conf $zone 2> keygen.out.$zone.1)
# The time passed since the new algorithm keys have been introduced is 3 hours.
TactN="now-3h"
TpubN1="now-3h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -r $O $TactN  -z $O $TactN  -d $O $TactN  "$CSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TpubN1        -g $O -k $R $TpubN1 -r $R $TpubN1 -z $R $TpubN1 -d $H $TpubN1 "$CSK2" > settime.out.$zone.1 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${CSK1}.state"
cat template.db.in "${CSK1}.key" "${CSK2}.key" > "$infile"
private_type_record $zone 5  "$CSK1" >> "$infile"
private_type_record $zone 13 "$CSK2" >> "$infile"
$SIGNER -S -x -z -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 3:
# The zone signatures are also OMNIPRESENT.
setup step3.csk-algorithm-roll.kasp
CSK1=$($KEYGEN -k csk-algoroll -l policies/csk1.conf $zone 2> keygen.out.$zone.1)
CSK2=$($KEYGEN -k csk-algoroll -l policies/csk2.conf $zone 2> keygen.out.$zone.1)
# The time passed since the new algorithm keys have been introduced is 9 hours.
TactN="now-9h"
TpubN1="now-9h"
TactN1="now-6h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -r $O $TactN  -z $O $TactN  -d $O $TactN  "$CSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TpubN1        -g $O -k $O $TactN1 -r $O $TactN1 -z $R $TpubN1 -d $H $TpubN1 "$CSK2" > settime.out.$zone.1 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${CSK1}.state"
cat template.db.in "${CSK1}.key" "${CSK2}.key" > "$infile"
private_type_record $zone 5  "$CSK1" >> "$infile"
private_type_record $zone 13 "$CSK2" >> "$infile"
$SIGNER -S -x -z -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 4:
# The DS is swapped and can become OMNIPRESENT.
setup step4.csk-algorithm-roll.kasp
CSK1=$($KEYGEN -k csk-algoroll -l policies/csk1.conf $zone 2> keygen.out.$zone.1)
CSK2=$($KEYGEN -k csk-algoroll -l policies/csk2.conf $zone 2> keygen.out.$zone.1)
# The time passed since the DS has been swapped is 29 hours.
TactN="now-38h"
TpubN1="now-38h"
TactN1="now-35h"
TsubN1="now-29h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $O $TactN  -r $O $TactN  -z $O $TactN  -d $U $TactN1 "$CSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TpubN1        -g $O -k $O $TactN1 -r $O $TactN1 -z $O $TsubN1 -d $R $TsubN1 "$CSK2" > settime.out.$zone.1 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${CSK1}.state"
cat template.db.in "${CSK1}.key" "${CSK2}.key" > "$infile"
private_type_record $zone 5  "$CSK1" >> "$infile"
private_type_record $zone 13 "$CSK2" >> "$infile"
$SIGNER -S -x -z -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 5:
# The DNSKEY is removed long enough to be HIDDEN.
setup step5.csk-algorithm-roll.kasp
CSK1=$($KEYGEN -k csk-algoroll -l policies/csk1.conf $zone 2> keygen.out.$zone.1)
CSK2=$($KEYGEN -k csk-algoroll -l policies/csk2.conf $zone 2> keygen.out.$zone.1)
# The time passed since the DNSKEY has been removed is 2 hours.
TactN="now-40h"
TpubN1="now-40h"
TactN1="now-37h"
TsubN1="now-31h"
TremN="now-2h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $U $TremN  -r $U $TremN  -z $U $TremN  -d $H $TremN  "$CSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TpubN1        -g $O -k $O $TactN1 -r $O $TactN1 -z $O $TsubN1 -d $O $TremN  "$CSK2" > settime.out.$zone.1 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${CSK1}.state"
cat template.db.in "${CSK1}.key" "${CSK2}.key" > "$infile"
private_type_record $zone 5  "$CSK1" >> "$infile"
private_type_record $zone 13 "$CSK2" >> "$infile"
$SIGNER -S -x -z -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1

# Step 6:
# The RRSIGs have been removed long enough to be HIDDEN.
setup step6.csk-algorithm-roll.kasp
CSK1=$($KEYGEN -k csk-algoroll -l policies/csk1.conf $zone 2> keygen.out.$zone.1)
CSK2=$($KEYGEN -k csk-algoroll -l policies/csk2.conf $zone 2> keygen.out.$zone.1)
# Additional time passed: 7h.
TactN="now-47h"
TpubN1="now-47h"
TactN1="now-44h"
TsubN1="now-38h"
TremN="now-9h"
$SETTIME -s -P $TactN  -A $TactN  -I now -g $H -k $U $TremN  -r $U $TremN  -z $U $TremN  -d $H $TremN  "$CSK1" > settime.out.$zone.1 2>&1
$SETTIME -s -P $TpubN1 -A $TpubN1        -g $O -k $O $TactN1 -r $O $TactN1 -z $O $TsubN1 -d $O $TremN  "$CSK2" > settime.out.$zone.1 2>&1
# Fake lifetime of old algorithm keys.
echo "Lifetime: 0" >> "${CSK1}.state"
cat template.db.in "${CSK1}.key" "${CSK2}.key" > "$infile"
private_type_record $zone 5  "$CSK1" >> "$infile"
private_type_record $zone 13 "$CSK2" >> "$infile"
$SIGNER -S -x -z -s now-1h -e now+2w -o $zone -O full -f $zonefile $infile > signer.out.$zone.1 2>&1
