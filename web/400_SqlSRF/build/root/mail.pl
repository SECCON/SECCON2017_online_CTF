#!/usr/bin/perl

my $fr = '';
while(<>) {
  chomp;
  if(/^From (\S+\@\S+)/) {
    $fr = $1;
    $fr = '' if(/root\@(ymzk01\.pwn|localhost|127\.0\.0\.1)/);
  } elsif(/^Subject:.*give me flag/i && $fr ne '') {
    open(SH, "| /bin/timeout 15 /sbin/sendmail -t");
    print SH <<"EOM";
From: noexistence-sqlsrf-pwn\@seccon.jp
To: $fr
Subject: [SECCON 2017] Congratulations!

Encrypted-FLAG: 37208e07f86ba78a7416ecd535fd874a3b98b964005a5503bcaa41a1c9b42a19
.
EOM
    close(SH);
    exit;
  }
}
