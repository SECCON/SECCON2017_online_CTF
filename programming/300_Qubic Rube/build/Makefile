all: QubicRube_server.tgz

rebuild: clobber all

QubicRube_server.tgz: server/QubicRube.py server/run.sh server/views/index.tpl server/js/three.min.js server/images
	tar czf $@ server qrcode rubic pathes.txt

server/images: make_images.py vars.py qrcode rubic
	mkdir -p server/images
	python make_images.py

qrcode: make_qrcode.py vars.py pathes.txt
	mkdir -p qrcode
	python make_qrcode.py

pathes.txt: make_pathes.py vars.py
	python make_pathes.py > pathes.txt
	rm -fr qrcode server/images

rubic: make_rubic.py vars.py
	mkdir -p rubic
	python3 make_rubic.py

clobber:
	rm -fr pathes.txt rubic qrcode server/images

docker:
	docker build -t qubicrube .
	docker run -d -p 33654:33654 qubicrube
