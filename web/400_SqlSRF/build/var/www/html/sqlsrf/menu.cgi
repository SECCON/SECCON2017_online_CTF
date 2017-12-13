#!/usr/bin/perl

use CGI;
my $q = new CGI;

use CGI::Session;
my $s = CGI::Session->new(undef, $q->cookie('CGISESSID')||undef, {Directory=>'/tmp'});
$s->expire('+1M');

if(!defined $s->param('autheduser')) {
  print "Location: ./index.cgi\r\n\r\n";
  exit;
}

print $q->header(-charset=>'UTF-8', -cookie=>$q->cookie(-name=>'CGISESSID', -value=>$s->id)),
  $q->start_html(-lang=>'ja', -encoding=>'UTF-8', -title=>'SECCON 2017', -bgcolor=>'black');

my $user = $q->escapeHTML($s->param('autheduser'));
my $r = '';
my($args) = $q->param('args') =~ /^([a-zA-Z0-9\/\%\-\_\.\:\@ ]+)/;
my $dis = ' disabled';
$dis = '' if($user eq 'admin');;
if($q->param('cmd') eq 'netstat -tnl') {
  $r = &cmdexec('netstat -tnl');
} elsif($q->param('cmd') eq "wget --debug -O /dev/stdout 'http://" && $dis eq '') {
  $r = &cmdexec("/bin/timeout 15 /bin/wget --debug -O /dev/stdout 'http://$args' 2>&1");
}
my $ea = '2017.seccon.jp/';
$ea = $q->escapeHTML($args) if($args ne '');
$args = '2017.seccon.jp/' if($args eq '');

sub cmdexec {
  local($cmd) = @_;
  my $r = `$cmd`;
  $r =~ s/(\r\n|\r|\n)/|br|/g;
  $r = $q->escapeHTML($r);
  $r =~ s/\|br\|/\n/g;
  $r = '<pre style="background-color:white;color:black;border:1px solid silver;width:700px;padding:20px;text-align:left;">'.$r.'</pre>';
  return $r;
}

print <<"EOM";
<!-- The Kusomon by KeigoYAMAZAKI, 2017 -->
<div style="background:#000 url(./bg-header.jpg) 50% 50% no-repeat;position:fixed;width:100%;height:300px;top:0;">
</div>
<div style="position:relative;top:300px;color:white;background-color:black">
<center>
<h1>Menu</h1>
<hr>User: $user<hr>$r
<table border="1" cellpadding="20px">
<tr><td><form action="?" method="post">
1. <input type="submit" name="cmd" value="netstat -tnl"></form></td></tr>
<tr><td><form action="?" method="post">
2. <input type="submit" name="cmd" value="wget --debug -O /dev/stdout 'http://"$dis><input type="text" name="args" value="$args" size="80"$dis><input type="button" value="'"$dis></form><div style="color:red">&nbsp;* No.2 is only for "admin" user.</div></td></tr>
</table>
<hr>
</form>
</center>
</div>
</body>
</html>
EOM

1;
