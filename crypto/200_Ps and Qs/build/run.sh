docker-compose build
flag=$(cat ../flag.txt) docker-compose up
cp ./data/pub1.pub ../files
cp ./data/pub2.pub ../files
cp ./data/cipher ../files
