#!/usr/bin/perl

use Crypt::CBC;

my $cipher = Crypt::CBC->new(
    -key         => 'EF23456879ABCD01',
    -keysize     => 16,
    -cipher      => 'Crypt::Rijndael',
    -iv          => '0000000000000000',
    -header      => 'none',
);

sub encrypt {
  local($txt) = @_;
  return $cipher->encrypt_hex($txt);
}

sub decrypt {
  local($txt) = @_;
  return $cipher->decrypt_hex($txt);
}

1;
