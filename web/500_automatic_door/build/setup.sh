#!/bin/sh
touch /var/www/html/0b503d0caf712352fc200bc5332c4f95/sandbox/.htaccess
chattr +i /var/www/html/0b503d0caf712352fc200bc5332c4f95/sandbox/.htaccess
chgrp www-data /var/www/html/0b503d0caf712352fc200bc5332c4f95/sandbox
chmod 730 /var/www/html/0b503d0caf712352fc200bc5332c4f95/sandbox
# apm...
# disabled_functions exec,passthru,popen,shell_exec,system
