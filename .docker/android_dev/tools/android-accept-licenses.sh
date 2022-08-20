#!/usr/bin/expect -f

set timeout 1800
set cmd [lindex $argv 0]
set licenses [lindex $argv 1]

spawn {*}$cmd
expect {
  "(y/N)" {
        exp_send "y\r"
        exp_continue
  }
  eof
}
