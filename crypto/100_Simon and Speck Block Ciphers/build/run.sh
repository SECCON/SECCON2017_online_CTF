docker-compose build
flag=$(cat ../flag.txt) docker-compose up

mv ./data/* ../files/
