#!/usr/bin/expect 
set timeout 30
spawn sudo umount /mnt/win/
expect "password for lhb:"
send "lhbzyy\n"
interact

spawn sudo umount /mnt/win_tmp/
expect "password for lhb:"
send "lhbzyy\n"
interact

spawn sudo umount /mnt/itarge/
expect "password for lhb:"
send "lhbzyy\n"
interact


spawn sudo mount -t smbfs -o username="android",passwd="lhbzyy",gid="1000",uid="1000" //192.168.3.61/y /mnt/win/
expect "password for lhb:"
send "lhbzyy\n"
interact

#spawn sudo mount //192.168.3.61/y /mnt/win_tmp/ -o username=android
#expect "password for lhb:"
#send "lhbzyy\n"
#expect "Password:"
#send "lhbzyy\n"
#interact

#spawn sudo mount -t smbfs -o username="android",passwd="lhbzyy",gid="1000",uid="1000" //192.168.3.61/ /mnt/win_tmp/
#spawn sudo mount //192.168.3.61/e /mnt/win/ -o username=android
#expect "password for lhb:"
#send "lhbzyy\n"
#expect "Password:"
#send "lhbzyy\n"
#interact

spawn sudo mount -t smbfs -o username="android",passwd="lhbzyy",gid="1000",uid="1000" //192.168.3.61/itarge2 /mnt/itarge/
expect "password for lhb:"
send "lhbzyy\n"
interact
#spawn sudo mount //192.168.3.61/itarge2 /mnt/itarge/ -o username=android
#expect "password for lhb:"
#send "lhbzyy\n"
#expect "Password:"
#send "lhbzyy\n"
#interact

#echo $PASSWD > /tmp/pass;sudo -S -i</tmp/pass 2>/dev/null;sudo -S umount /mnt/win/;

#sudo mount //192.168.3.61/e /mnt/win/ -o username=android<<EOF
#lhbzyy
#EOF

#sudo mount //192.168.3.61/itarge2 /mnt/itarge/ -o username=android<<EOF
#lhbzyy
#EOF


