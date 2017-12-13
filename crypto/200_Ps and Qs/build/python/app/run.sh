python code.py
openssl rsa -in ./data/priv1.pem -pubout > ./data/pub1.pub
openssl rsa -in ./data/priv2.pem -pubout > ./data/pub2.pub
echo $flag | openssl rsautl -encrypt -pubin -inkey ./data/pub1.pub > ./data/cipher
